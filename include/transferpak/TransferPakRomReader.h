#ifndef _TRANSFERPAKROMREADER_H
#define _TRANSFERPAKROMREADER_H

#include "RomReader.h"

class TransferPakManager;

class TransferPakRomReader : public BaseRomReader
{
public:
    TransferPakRomReader(TransferPakManager& pakManager);
    virtual ~TransferPakRomReader();

    /**
     * @brief This function reads a byte, returns it and advances the internal pointer by 1 byte
     * 
     * @return uint8_t 
     */
    bool readByte(uint8_t& outByte) override;

    /**
     * @brief This function reads multiple bytes into the specified output buffer
     * 
     * @return whether the operation was successful
     */
    bool read(uint8_t* outBuffer, uint32_t bytesToRead) override;

    /**
     * @brief This function reads the current byte without advancing the internal pointer by 1 byte
     * 
     * @return uint8_t 
     */
    uint8_t peek() override;

    /**
     * @brief This function advances the internal pointer by 1 byte
     * 
     */
    bool advance(uint32_t numBytes = 1) override;

    /**
     * @brief This function seeks to the specified absolute rom offset
     * 
     * @param absoluteOffset 
     */
    bool seek(uint32_t absoluteOffset) override;

    /**
     * @brief This function searches for a sequence of bytes (the needle) in the buffer starting from
     * the current internal position

     * @return true we found the sequence of bytes and we've seeked toward this point
     * @return false we didn't find the sequence of bytes anywhere
     */
    bool searchFor(const uint8_t* needle, uint32_t needleLength) override;

    uint8_t getCurrentBankIndex() const override;
protected:
private:
    TransferPakManager& pakManager_;
    uint32_t currentRomOffset_;
};

#endif