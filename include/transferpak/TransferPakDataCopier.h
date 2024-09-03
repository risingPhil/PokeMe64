#ifndef _TRANSFERPAKDATACOPIER_H
#define _TRANSFERPAKDATACOPIER_H

#include <cstdio>
#include <cstdint>

class TransferPakRomReader;
class TransferPakSaveManager;

/**
 * This interface is used to define a transfer pak datasource to copy from
 */
class ITransferPakDataCopySource
{
public:
virtual ~ITransferPakDataCopySource();

virtual uint16_t getCurrentBankIndex() const = 0;
virtual uint32_t getNumberOfBytesRead() const = 0;

virtual uint32_t read(uint8_t* buffer, uint32_t bytesToRead) = 0;
protected:
private:
};

/**
 * This class implements the ITransferPakDataCopySource interface with TransferPakRomReader
 */
class TransferPakRomReaderCopySource : public ITransferPakDataCopySource
{
public:
TransferPakRomReaderCopySource(TransferPakRomReader& romReader);
virtual ~TransferPakRomReaderCopySource();

uint16_t getCurrentBankIndex() const override;
uint32_t getNumberOfBytesRead() const override;

uint32_t read(uint8_t* buffer, uint32_t bytesToRead) override;
protected:
private:
    TransferPakRomReader& romReader_;
    uint32_t bytesRead_;
};

/**
 * This class implements the ITransferPakDataCopySource interface with TransferPakRomReader
 */
class TransferPakSaveManagerCopySource : public ITransferPakDataCopySource
{
public:
TransferPakSaveManagerCopySource(TransferPakSaveManager& saveManager);
virtual ~TransferPakSaveManagerCopySource();

uint16_t getCurrentBankIndex() const override;
uint32_t getNumberOfBytesRead() const override;

uint32_t read(uint8_t* buffer, uint32_t bytesToRead) override;
protected:
private:
    TransferPakSaveManager& saveManager_;
    uint32_t bytesRead_;
};

/**
 * This class directs the copy process from the source to the specified output file
 * 
 * It exists to abstract the source (rom/SRAM) and control the copy flow/speed and to 
 * allow us to give UI feedback on the copy process.
 * 
 * After all: I found that the transfer pak is able to read 32 bytes every 1,5-2milliseconds
 * We don't want the UI to remain frozen during that time.
 * (source: http://n64devkit.square7.ch/pro-man/pro26/26-07.htm)
 */
class TransferPakDataCopier
{
public:
    TransferPakDataCopier(ITransferPakDataCopySource& source, FILE* outputFile);
    ~TransferPakDataCopier();

    uint16_t getCurrentBankIndex() const;
    uint32_t getNumberOfBytesRead() const;
    size_t copyChunk(uint32_t numBytesToCopy);
protected:
private:
    ITransferPakDataCopySource& source_;
    FILE* outputFile_;
};

#endif