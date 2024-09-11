#include "transferpak/TransferPakDataCopier.h"
#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakSaveManager.h"

#include <cstring>

ITransferPakDataCopySource::~ITransferPakDataCopySource()
{
}

ITransferPakDataCopyDestination::~ITransferPakDataCopyDestination()
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


bool TransferPakRomReaderCopySource::readyForTransfer() const
{
    return true;
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
        return bytesToRead;
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

bool TransferPakSaveManagerCopySource::readyForTransfer() const
{
    return true;
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
        return bytesToRead;
    }
    return 0;
}

TransferPakFileCopySource::TransferPakFileCopySource(const char* filePath)
    : inputFile_(nullptr)
    , bytesRead_(0)
{
    inputFile_ = fopen(filePath, "r");
}

TransferPakFileCopySource::~TransferPakFileCopySource()
{
    if(inputFile_)
    {
        fclose(inputFile_);
        inputFile_ = nullptr;
    }
}

bool TransferPakFileCopySource::readyForTransfer() const
{
    return (inputFile_ != nullptr);
}

uint16_t TransferPakFileCopySource::getCurrentBankIndex() const
{
    return 1;
}

uint32_t TransferPakFileCopySource::getNumberOfBytesRead() const
{
    return bytesRead_;
}

uint32_t TransferPakFileCopySource::read(uint8_t* buffer, uint32_t bytesToRead)
{
    uint32_t ret = static_cast<uint32_t>(fread(buffer, sizeof(char), bytesToRead, inputFile_));
    bytesRead_ += ret;
    return ret;
}

TransferPakNullCopySource::TransferPakNullCopySource()
    : bytesRead_(0)
{
}

TransferPakNullCopySource::~TransferPakNullCopySource()
{
}

bool TransferPakNullCopySource::readyForTransfer() const
{
    return true;
}

uint16_t TransferPakNullCopySource::getCurrentBankIndex() const
{
    return 1;
}

uint32_t TransferPakNullCopySource::getNumberOfBytesRead() const
{
    return bytesRead_;
}

uint32_t TransferPakNullCopySource::read(uint8_t *buffer, uint32_t bytesToRead)
{
    memset(buffer, 0, bytesToRead);
    return bytesToRead;
}

TransferPakSaveManagerDestination::TransferPakSaveManagerDestination(TransferPakSaveManager& saveManager)
    : saveManager_(saveManager)
    , bytesWritten_(0)
{
}

TransferPakSaveManagerDestination::~TransferPakSaveManagerDestination()
{
    close();
}

bool TransferPakSaveManagerDestination::readyForTransfer() const
{
    return true;
}

uint16_t TransferPakSaveManagerDestination::getCurrentBankIndex() const
{
    return saveManager_.getCurrentBankIndex();
}

uint32_t TransferPakSaveManagerDestination::getNumberOfBytesWritten() const
{
    return bytesWritten_;
}

uint32_t TransferPakSaveManagerDestination::write(uint8_t* buffer, uint32_t bytesToWrite)
{
    saveManager_.write(buffer, bytesToWrite);
    bytesWritten_ += bytesToWrite;
    return bytesToWrite;
}

void TransferPakSaveManagerDestination::close()
{
    // dummy
}

TransferPakFileCopyDestination::TransferPakFileCopyDestination(const char* pathOnSDCard)
    : outputFile_(nullptr)
    , bytesWritten_(0)
{
    outputFile_ = fopen(pathOnSDCard, "w");
}

TransferPakFileCopyDestination::~TransferPakFileCopyDestination()
{
    close();
}

bool TransferPakFileCopyDestination::readyForTransfer() const
{
    return (outputFile_ != nullptr);
}

uint16_t TransferPakFileCopyDestination::getCurrentBankIndex() const
{
    return 1;
}

uint32_t TransferPakFileCopyDestination::getNumberOfBytesWritten() const
{
    return bytesWritten_;
}

uint32_t TransferPakFileCopyDestination::write(uint8_t* buffer, uint32_t bytesToWrite)
{
    const uint32_t ret = static_cast<uint32_t>(fwrite(buffer, sizeof(char), bytesToWrite, outputFile_));
    bytesWritten_ += ret;
    return ret;
}

void TransferPakFileCopyDestination::close()
{
    if(outputFile_)
    {
        fclose(outputFile_);
        outputFile_ = nullptr;
    }
}

TransferPakDataCopier::TransferPakDataCopier(ITransferPakDataCopySource& source, ITransferPakDataCopyDestination& destination)
    : source_(source)
    , destination_(destination)
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

        // now write the bytes to the destination
        bytesRemaining -= destination_.write(buffer, bytesToRead);
    }
    return numBytesToCopy - bytesRemaining;
}