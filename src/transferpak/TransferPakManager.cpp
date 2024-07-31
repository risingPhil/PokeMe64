#include "transferpak/TransferPakManager.h"

#include <algorithm>
#include <unistd.h>

/** @brief Transfer Pak address for cartridge data */
#define TPAK_ADDRESS_DATA   0xC000

static const uint16_t sramBankStartGBAddress = 0xA000;

TransferPakManager::TransferPakManager()
    : port_(JOYPAD_PORT_1)
    , isPoweredOn_(false)
    , currentSRAMBank_(0)
    , readBufferBankOffset_(0xFFFF)
    , writeBufferSRAMBankOffset_(0xFFFF)
    , readBuffer_()
    , writeBuffer_()
{
}

TransferPakManager::~TransferPakManager()
{
}

joypad_port_t TransferPakManager::getPort() const
{
    return port_;
}

void TransferPakManager::setPort(joypad_port_t port)
{
    if(isPoweredOn_)
    {
        setPower(false);
    }
    port_ = port;
}

bool TransferPakManager::hasTransferPak()
{
    if(!joypad_is_connected(port_))
    {
        debugf("[TransferPakManager]: joypad not connected %d\r\n", (int)port_);
        return false;
    }

    const joypad_accessory_type_t type = joypad_get_accessory_type(port_);
    debugf("[TransferPakManager]: accessory type %d\r\n", (int)type);
    return (type == JOYPAD_ACCESSORY_TYPE_TRANSFER_PAK);
}

bool TransferPakManager::isPoweredOn() const
{
    return isPoweredOn_;
}

bool TransferPakManager::setPower(bool on)
{
    uint8_t status;
    int ret;

    if(on)
    {
        ret = tpak_init(static_cast<int>(port_));
        if(ret)
        {
            debugf("[TransferPakManager]: %s: tpak_init got error %d\r\n", __FUNCTION__, ret);
        }

        status = getStatus();
        while(!(status | TPAK_STATUS_READY))
        {
            debugf("[TransferPakManager]: %s: ERROR: transfer pak not ready yet. Current status is %hu\r\n", __FUNCTION__, status);
            status = getStatus();
        }
        isPoweredOn_ = true;
    }
    else
    {
        ret = tpak_set_access(port_, false);
        if(ret)
        {
            debugf("[TransferPakManager]: %s: tpak_set_access got error %d\r\n", __FUNCTION__, ret);
        }

        ret = tpak_set_power(port_, false);
        if(ret)
        {
            debugf("[TransferPakManager]: %s: tpak_set_access got error %d\r\n", __FUNCTION__, ret);
        }
        isPoweredOn_ = false;
    }

    return (!ret);
}

uint8_t TransferPakManager::getStatus()
{
    return tpak_get_status(static_cast<int>(port_));
}

bool TransferPakManager::validateGbHeader()
{
    gameboy_cartridge_header header;
    uint8_t status = getStatus();
    int ret;
    bool retBool;

    while(!(status | TPAK_STATUS_READY))
    {
        debugf("[TransferPakManager]: ERROR: transfer pak not ready yet. Current status is %hu\r\n", status);
        status = getStatus();
    }

    if(status == TPAK_STATUS_REMOVED)
    {
        debugf("[TransferPakManager]: ERROR: transfer pak has STATUS_REMOVED\r\n");
        return false;
    }

    ret = tpak_get_cartridge_header(static_cast<int>(port_), &header);
    if(ret)
    {
        debugf("[TransferPakManager]: ERROR: tpak_get_cartridge_header got error %d\r\n", ret);
        return false;
    }
    retBool = tpak_check_header(&header);

    if(!retBool)
    {
        debugf("[TransferPakManager]: ERROR: tpak_check_header returned false!\r\n");
    }

    return retBool;
}

void TransferPakManager::switchGBROMBank(uint8_t bankIndex)
{
    uint8_t data[TPAK_BLOCK_SIZE];

//  debugf("[TransferPakManager]: %s(%hu)\r\n", __FUNCTION__, bankIndex);

    memset(data, bankIndex, TPAK_BLOCK_SIZE);
    tpak_write(port_, 0x2000, data, TPAK_BLOCK_SIZE);

    // invalidate read buffer
    readBufferBankOffset_ = 0xFFFF;
}

void TransferPakManager::setRAMEnabled(bool enabled)
{
    uint8_t data[TPAK_BLOCK_SIZE];

//  debugf("[TransferPakManager]: %s(%d)\r\n", __FUNCTION__, enabled);

    const uint8_t valueToWrite = (enabled) ? 0xA : 0x0;
    memset(data, valueToWrite, TPAK_BLOCK_SIZE);

    tpak_write(port_, 0x0, data, TPAK_BLOCK_SIZE);

    uint8_t status = getStatus();

    while(!(status | TPAK_STATUS_READY))
    {
        debugf("[TransferPakManager]: %s: ERROR: transfer pak not ready yet. Current status is %hu\r\n", __FUNCTION__, status);
        status = getStatus();
    }
}

void TransferPakManager::switchGBSRAMBank(uint8_t bankIndex)
{
    uint8_t data[TPAK_BLOCK_SIZE];

    if(bankIndex == currentSRAMBank_)
    {
        return;
    }
//    debugf("[TransferPakManager]: %s(%hu)\r\n", __FUNCTION__, bankIndex);
    // make sure to finish any writes in the write buffer before switching
    finishWrites();
    
    memset(data, bankIndex, TPAK_BLOCK_SIZE);
    tpak_write(port_, 0x4000, data, TPAK_BLOCK_SIZE);

    currentSRAMBank_ = bankIndex;
    // invalidate read and write buffer
    readBufferBankOffset_ = 0xFFFF;
    writeBufferSRAMBankOffset_ = 0xFFFF;
}

void TransferPakManager::read(uint16_t gbAddress, uint8_t* data, uint16_t size)
{
//  debugf("[TransferPakManager]: %s(0x%x, %p, %u)\r\n", __FUNCTION__, gbAddress, data, size);

    uint16_t bytesRemaining = size;
    uint8_t* cur = data;

    // we need to do 32-byte aligned reads
    uint16_t readBufOffset = gbAddress % TPAK_BLOCK_SIZE;
    uint16_t alignedGbAddress = gbAddress - readBufOffset;
    uint16_t currentReadSize;

    // first of all determine if we already have a filled readBuffer around this address
    if(readBufferBankOffset_ == 0xFFFF || (alignedGbAddress < readBufferBankOffset_) || (alignedGbAddress >= readBufferBankOffset_ + TPAK_BLOCK_SIZE))
    {
        // the readBuffer doesn't contain the data we're looking for
        // so read some
        readBufferBankOffset_ = alignedGbAddress;
        // we need to read into the readBuffer first
//      debugf("[TransferPakManager]: %s -> tpak_read(%d, 0x%x, %p, %u)\r\n", __FUNCTION__, port_, readBufferBankOffset_, readBuffer_, TPAK_BLOCK_SIZE);
        tpak_read(port_, readBufferBankOffset_, readBuffer_, TPAK_BLOCK_SIZE);
    }


    while(bytesRemaining > 0)
    {
        // copy from the read buffer
        currentReadSize = std::min<uint16_t>(bytesRemaining, TPAK_BLOCK_SIZE - readBufOffset);
        memcpy(cur, readBuffer_ + readBufOffset, currentReadSize);

        bytesRemaining -= currentReadSize;
        cur += currentReadSize;
        
        // check if we still need more bytes after this. If so, we've reached the end of our read buffer
        // and will need to read more from the transfer pak
        if(bytesRemaining > 0)
        {
            // we have reached the end of our readBuffer_
            // we need to read more from the transfer pak
            readBufferBankOffset_ += TPAK_BLOCK_SIZE;
            readBufOffset = 0;
//          debugf("[TransferPakManager]: %s -> tpak_read(%d, 0x%x, %p, %u)\r\n", __FUNCTION__, port_, readBufferBankOffset_, readBuffer_, TPAK_BLOCK_SIZE);
            tpak_read(port_, readBufferBankOffset_, readBuffer_, TPAK_BLOCK_SIZE);
        }
    }
}

void TransferPakManager::readSRAM(uint16_t SRAMBankOffset, uint8_t* data, uint16_t size)
{
//  debugf("[TransferPakManager]: %s(0x%hx, %p, %hu)\r\n", __FUNCTION__, SRAMBankOffset, data, size);

    // make sure to finish any writes before reading. Otherwise we might be reading outdated SRAM data
    // after all: we might have pending changes into our writebuffer and therefore we must be sure these are applied first.
    finishWrites();

    read(sramBankStartGBAddress + SRAMBankOffset, data, size);
}

void TransferPakManager::writeSRAM(uint16_t SRAMBankOffset, const uint8_t* data, uint16_t size)
{
    uint16_t bytesRemaining = size;
    uint16_t writeBufferOffset = SRAMBankOffset % TPAK_BLOCK_SIZE;
    const uint16_t alignedSRAMBankOffset = SRAMBankOffset - writeBufferOffset;
    uint16_t currentWriteSize;
    const uint8_t* cur = data;

//  debugf("[TransferPakManager]: %s(0x%hx, %p, %hu)\r\n", __FUNCTION__, SRAMBankOffset, data, size);

    // check if our current SRAM location is outside of the write buffer. If it is, we need to actually start writing the pending changes in the writeBuffer_ before continueing
    if((alignedSRAMBankOffset < writeBufferSRAMBankOffset_) || (alignedSRAMBankOffset >= (writeBufferSRAMBankOffset_ + TPAK_BLOCK_SIZE)))
    {
        // the new write is outside the boundaries of the previous write block
        // actually write the writeBuffer_ to SRAM now
        finishWrites();
    }

    if(writeBufferSRAMBankOffset_ == 0xFFFF)
    {
        // we don't have any pending writes
        // before doing a write to the write buffer, we need to read the current data first
//      debugf("[TransferPakManager]: initial read write buffer offset 0x%02hx\r\n", writeBufferSRAMBankOffset_);
        readSRAM(alignedSRAMBankOffset, writeBuffer_, TPAK_BLOCK_SIZE);
        // don't modify writeBufferSRAMBankOffset_ before calling readSRAM, otherwise finishWrites() will write whatever
        // is in the writeBuffer because writeBufferSRAMBankOffset_ wouldn't be 0xFFFF anymore
        writeBufferSRAMBankOffset_ = alignedSRAMBankOffset;
    }

    while(bytesRemaining > 0)
    {
        currentWriteSize = std::min<uint16_t>(bytesRemaining, TPAK_BLOCK_SIZE - writeBufferOffset);
        memcpy(writeBuffer_ + writeBufferOffset, cur, currentWriteSize);

        cur += currentWriteSize;
        bytesRemaining -= currentWriteSize;
        
        // check if we still have more data to write.
        // if so, the current writeBuffer end has been reached and we need to actually write it and continue
        if(bytesRemaining > 0)
        {
            // read the current data of the new block first, because we must write in blocks of 32 bytes
            // this will also call finishWrites()
            const uint16_t newOffset = writeBufferSRAMBankOffset_ + TPAK_BLOCK_SIZE;
//          debugf("[TransferPakManager]: read new write buffer offset 0x%hx\r\n", newOffset);
            readSRAM(newOffset, writeBuffer_, TPAK_BLOCK_SIZE);
            // don't modify writeBufferSRAMBankOffset before readSRAM, otherwise finishWrites() in readSRAM would
            // write to the wrong offset
            writeBufferSRAMBankOffset_ = newOffset;
            writeBufferOffset = 0;
        }
    }
}

void TransferPakManager::finishWrites()
{
    if(writeBufferSRAMBankOffset_ == 0xFFFF)
    {
        // no pending writes
        return;
    }
//  debugf("[TransferPakManager]: %s writeBufferSRAMOffset 0x%hx, buffer %p, blocksize %hu\r\n", __FUNCTION__, writeBufferSRAMBankOffset_, writeBuffer_, TPAK_BLOCK_SIZE);

    tpak_write(port_, sramBankStartGBAddress + writeBufferSRAMBankOffset_, writeBuffer_, TPAK_BLOCK_SIZE);

    // mark no pending writes
    writeBufferSRAMBankOffset_ = 0xFFFF;
    // also invalidate read buffer
    readBufferBankOffset_ = 0xFFFF;
}