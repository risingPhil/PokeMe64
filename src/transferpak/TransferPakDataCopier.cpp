#include "transferpak/TransferPakDataCopier.h"
#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakSaveManager.h"

ITransferPakDataCopySource::~ITransferPakDataCopySource()
{
}

TransferPakRomReaderCopySource::TransferPakRomReaderCopySource(TransferPakRomReader& romReader)
    : romReader_(romReader)
    , bytesRead_(0)
{
}

TransferPakRomReaderCopySource::~TransferPakRomReaderCopySource()
{
}

uint16_t TransferPakRomReaderCopySource::getCurrentBankIndex() const
{
    return romReader_.getCurrentBankIndex();
}

uint32_t TransferPakRomReaderCopySource::getNumberOfBytesRead() const
{
    return bytesRead_;
}

uint32_t TransferPakRomReaderCopySource::read(uint8_t* buffer, uint32_t bytesToRead)
{
    if(romReader_.read(buffer, bytesToRead))
    {
        bytesRead_ += bytesToRead;
        return bytesRead_;
    }
    return 0;
}

TransferPakSaveManagerCopySource::TransferPakSaveManagerCopySource(TransferPakSaveManager& saveManager)
    : saveManager_(saveManager)
    , bytesRead_(0)
{
}

TransferPakSaveManagerCopySource::~TransferPakSaveManagerCopySource()
{
}

uint16_t TransferPakSaveManagerCopySource::getCurrentBankIndex() const
{
    return saveManager_.getCurrentBankIndex();
}

uint32_t TransferPakSaveManagerCopySource::getNumberOfBytesRead() const
{
    return bytesRead_;
}

uint32_t TransferPakSaveManagerCopySource::read(uint8_t* buffer, uint32_t bytesToRead)
{
    if(saveManager_.read(buffer, bytesToRead))
    {
        bytesRead_ += bytesToRead;
        return bytesRead_;
    }
    return 0;
}

TransferPakDataCopier::TransferPakDataCopier(ITransferPakDataCopySource& source, FILE* outputFile)
    : source_(source)
    , outputFile_(outputFile)
{
}

TransferPakDataCopier::~TransferPakDataCopier()
{
}

uint16_t TransferPakDataCopier::getCurrentBankIndex() const
{
    return source_.getCurrentBankIndex();
}

uint32_t TransferPakDataCopier::getNumberOfBytesRead() const
{
    return source_.getNumberOfBytesRead();
}

size_t TransferPakDataCopier::copyChunk(uint32_t numBytesToCopy)
{
    constexpr uint16_t bufferSize = 256;
    uint8_t buffer[bufferSize];
    uint32_t bytesRemaining = numBytesToCopy;
    uint32_t bytesToRead;

    while(bytesRemaining > 0)
    {
        bytesToRead = (bufferSize < bytesRemaining) ? bufferSize : bytesRemaining;

        if(!source_.read(buffer, bytesToRead))
        {
            // no bytes read. Abort
            break;
        }

        // now write the bytes to file
        fwrite(buffer, sizeof(char), bytesToRead, outputFile_);
        bytesRemaining -= bytesToRead;
    }
    return numBytesToCopy - bytesRemaining;
}