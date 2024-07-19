#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakManager.h"

static uint16_t GB_BANK_SIZE = 0x4000;

static uint16_t calculateBytesLeftInCurrentBank(uint32_t currentRomOffset)
{
    const uint16_t bankOffset = static_cast<uint16_t>(currentRomOffset % GB_BANK_SIZE);
    return GB_BANK_SIZE - bankOffset;
}

// gameboy bank 0 is always mapped to 0x0-0x4000 of the gameboy address space
// gameboy bank 1 is switchable and is mapped to 0x4000-0x8000 of the gameboy address space
// so all rom banks beyond bank 0 need to be accessed in the bank 1 address space
static uint16_t calculateGBAddressForRomOffset(uint32_t romOffset)
{
    if(romOffset < GB_BANK_SIZE)
    {
        return static_cast<uint16_t>(romOffset);
    }
    else
    {
        return static_cast<uint16_t>(GB_BANK_SIZE + (romOffset % GB_BANK_SIZE));
    }
}

TransferPakRomReader::TransferPakRomReader(TransferPakManager& pakManager)
    : pakManager_(pakManager)
    , currentRomOffset_(0)
{
}

TransferPakRomReader::~TransferPakRomReader()
{
}

bool TransferPakRomReader::readByte(uint8_t& outByte)
{
    return read(&outByte, 1);
}

bool TransferPakRomReader::read(uint8_t* outBuffer, uint32_t bytesToRead)
{
    uint32_t bytesRemaining = bytesToRead;
    uint16_t bytesLeftInCurrentBank;
    uint16_t currentRead;

    while(bytesRemaining > 0)
    {
        bytesLeftInCurrentBank = calculateBytesLeftInCurrentBank(currentRomOffset_);
        currentRead = (bytesRemaining > bytesLeftInCurrentBank) ? bytesLeftInCurrentBank : static_cast<uint16_t>(bytesRemaining);

        pakManager_.read(calculateGBAddressForRomOffset(currentRomOffset_), outBuffer, currentRead);
        outBuffer += currentRead;
        bytesRemaining -= currentRead;
        
        advance(currentRead);
    }
    return true;
}

uint8_t TransferPakRomReader::peek()
{
    uint8_t buffer[1];
    pakManager_.read(calculateGBAddressForRomOffset(currentRomOffset_), buffer, 1);
    return buffer[0];
}

bool TransferPakRomReader::advance(uint32_t numBytes)
{
    return seek(currentRomOffset_ + numBytes);
}

bool TransferPakRomReader::seek(uint32_t absoluteOffset)
{
    const uint8_t previousBankIndex = getCurrentBankIndex();
    uint8_t newBankIndex;

    currentRomOffset_ = absoluteOffset;
    newBankIndex = getCurrentBankIndex();

    if(previousBankIndex != newBankIndex)
    {
        pakManager_.switchGBROMBank(newBankIndex);
    }
    return true;
}

bool TransferPakRomReader::searchFor(const uint8_t* needle, uint32_t needleLength)
{
    // NOT IMPLEMENTED
    return false;
}

uint8_t TransferPakRomReader::getCurrentBankIndex() const
{
    return static_cast<uint8_t>(currentRomOffset_ / GB_BANK_SIZE);
}