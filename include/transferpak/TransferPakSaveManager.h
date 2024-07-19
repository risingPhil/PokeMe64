#ifndef _TRANSFERPAKSAVEMANAGER_H
#define _TRANSFERPAKSAVEMANAGER_H

#include "SaveManager.h"

class TransferPakManager;

class TransferPakSaveManager : public BaseSaveManager
{
public:
    TransferPakSaveManager(TransferPakManager& pakManager);
    virtual ~TransferPakSaveManager();

    /**
     * @brief This function reads a byte, returns it and advances the internal pointer by 1 byte
     * 
     * @return uint8_t 
     */
    bool readByte(uint8_t& outByte) override;
    void writeByte(uint8_t byte) override;

    /**
     * @brief This function reads multiple bytes into the specified output buffer
     * 
     * @return whether the operation was successful
     */
    bool read(uint8_t* outBuffer, uint32_t bytesToRead) override;
    void write(const uint8_t* buffer, uint32_t bytesToWrite) override;

    /**
     * @brief This function reads the current byte without advancing the internal pointer by 1 byte
     * 
     * @return uint8_t 
     */
    uint8_t peek() override;

    /**
     * @brief This function advances the internal pointer by the specified numBytes.
     * This can be considered a relative seek
     * 
     */
    bool advance(uint32_t numBytes = 1) override;
    bool rewind(uint32_t numBytes = 1) override;

    /**
     * @brief This function seeks to the specified absolute save file/buffer offset
     * 
     * @param absoluteOffset 
     */
    bool seek(uint32_t absoluteOffset) override;

    /**
     * @brief Returns the index of the current bank
     */
    uint8_t getCurrentBankIndex() const override;
protected:
private:
    TransferPakManager& pakManager_;
    uint32_t sramOffset_;
};

#endif