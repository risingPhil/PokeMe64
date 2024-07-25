#include "transferpak/TransferPakSaveManager.h"
#include "transferpak/TransferPakManager.h"

static uint16_t GB_SRAM_BANK_SIZE = 0x2000;

static uint16_t getSRAMBankOffset(uint32_t absoluteSRAMOffset)
{
    return static_cast<uint16_t>(absoluteSRAMOffset % GB_SRAM_BANK_SIZE);
}

static uint16_t calculateBytesLeftInCurrentBank(uint32_t SRAMOffset)
{
    const uint16_t bankOffset = getSRAMBankOffset(SRAMOffset);
    return GB_SRAM_BANK_SIZE - bankOffset;
}

TransferPakSaveManager::TransferPakSaveManager(TransferPakManager& pakManager)
    : pakManager_(pakManager)
    , sramOffset_(0)
{
    // reset back to sram bank 0
    pakManager_.switchGBSRAMBank(0);
}

TransferPakSaveManager::~TransferPakSaveManager()
{
}

bool TransferPakSaveManager::readByte(uint8_t& outByte)
{
    return read(&outByte, 1);
}

void TransferPakSaveManager::writeByte(uint8_t byte)
{
    write(&byte, 1);
}

void TransferPakSaveManager::write(const uint8_t* buffer, uint32_t bytesToWrite)
{
    uint32_t bytesRemaining = bytesToWrite;
    uint16_t bytesLeftInCurrentBank;
    uint16_t currentWrite;
    uint16_t bankOffset;

    while(bytesRemaining > 0)
    {
        bytesLeftInCurrentBank = calculateBytesLeftInCurrentBank(sramOffset_);
        currentWrite = (bytesRemaining > bytesLeftInCurrentBank) ? bytesLeftInCurrentBank : static_cast<uint16_t>(bytesRemaining);
        bankOffset = getSRAMBankOffset(sramOffset_);

        pakManager_.writeSRAM(bankOffset, buffer, currentWrite);
        buffer += currentWrite;
        bytesRemaining -= currentWrite;
        
        advance(currentWrite);
    }
}

bool TransferPakSaveManager::read(uint8_t* outBuffer, uint32_t bytesToRead)
{
    uint32_t bytesRemaining = bytesToRead;
    uint16_t bytesLeftInCurrentBank;
    uint16_t currentRead;
    uint16_t bankOffset;

    while(bytesRemaining > 0)
    {
        bytesLeftInCurrentBank = calculateBytesLeftInCurrentBank(sramOffset_);
        currentRead = (bytesRemaining > bytesLeftInCurrentBank) ? bytesLeftInCurrentBank : static_cast<uint16_t>(bytesRemaining);
        bankOffset = getSRAMBankOffset(sramOffset_);

        pakManager_.readSRAM(bankOffset, outBuffer, currentRead);
        outBuffer += currentRead;
        bytesRemaining -= currentRead;
        
        advance(currentRead);
    }
    return true;
}

uint8_t TransferPakSaveManager::peek()
{
    uint8_t buffer[1];
    const uint16_t bankOffset = getSRAMBankOffset(sramOffset_);
    pakManager_.readSRAM(bankOffset, buffer, 1);
    return buffer[0];
}

bool TransferPakSaveManager::advance(uint32_t numBytes)
{
    return seek(sramOffset_ + numBytes);
}

bool TransferPakSaveManager::rewind(uint32_t numBytes)
{
    return seek(sramOffset_ - numBytes);
}

bool TransferPakSaveManager::seek(uint32_t absoluteOffset)
{
    const uint8_t previousBankIndex = getCurrentBankIndex();
    uint8_t newBankIndex;

    sramOffset_ = absoluteOffset;
    newBankIndex = getCurrentBankIndex();

//  debugf("[TransferPakSaveManager]: %s(%lx) -> previousBankIndex %hu, newBankIndex %hu\r\n", __FUNCTION__, absoluteOffset, previousBankIndex, newBankIndex);

    if(previousBankIndex != newBankIndex)
    {
        pakManager_.switchGBSRAMBank(newBankIndex);
    }
    return true;
}

uint8_t TransferPakSaveManager::getCurrentBankIndex() const
{
    return static_cast<uint8_t>(sramOffset_ / GB_SRAM_BANK_SIZE);
}