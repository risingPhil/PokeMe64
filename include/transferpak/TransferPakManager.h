#ifndef _TRANSFERPAKMANAGER_H
#define _TRANSFERPAKMANAGER_H

#include <libdragon.h>

#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif

enum class TransferPakMode
{
    ROM,
    RAM
};

/** @brief Transfer Pak command block size (32 bytes) */
#define TPAK_BLOCK_SIZE  0x20

/**
 * @brief This class manages the N64 transfer pak
 * Both SRAM and ROM access are implemented in the same class here
 * because the transfer pak itself has some shenanigans:
 * - it has its own banking mechanism independent of the cartridge banks -> It can only access 16 KB of the gameboy address space at a time. However, libdragon
 *   takes care of most of the heavy lifting.
 * - It can't do single byte read/writes. Everything must be done in multiples of 32 bytes.
 * - switching transfer pak banks influences whether you are reading/writing the right addresses for ROM or SRAM.
 * 
 * So you can't implement ROM and SRAM reading in separate classes.
 * TransferPakManager manages and keeps track of all this and abstracts this complexity.
 */
class TransferPakManager
{
public:
    TransferPakManager();
    ~TransferPakManager();

    joypad_port_t getPort() const;
    void setPort(joypad_port_t port);

    bool hasTransferPak();
    bool isPoweredOn() const;
    bool setPower(bool on);
    uint8_t getStatus();

    bool readCartridgeHeader(gameboy_cartridge_header& cartridgeHeader);

    /**
     * @brief This function switches the Gameboy ROM bank index
     * WARNING: it switches to transfer pak bank 0
     */
    void switchGBROMBank(uint8_t bankIndex);

    /**
     * @brief This function enables/disables gameboy RAM/RTC access.
     * WARNING: it switches to transfer pak bank 0
     */
    void setRAMEnabled(bool enabled);

    /**
     * @brief This function switches the Gameboy RAM bank index
     * WARNING: it switches to transfer pak bank 1
     */
    void switchGBSRAMBank(uint8_t bankIndex);

    /**
     * For MBC1 controllers, this will effect the ROM or RAM modes, depending on what is written in $6000-$7FFF. 
     * If the mode is 0 (default), the RAM bank will be locked to 0, but the extended banks (bankindex > 0x1F)
     * can be accessed in the 0x0000 - 0x3FFF range (thereby replacing bank 0) by modifying the 0x4000-0x5FFF registers.
     * However, in this mode, the RAM bank is locked to bank 0
     *
     * If the mode is 1, the RAM bank can be switched, but the extended rom banks (bankindex > 0x1F) can't be switched to.
     * In this mode 0x0000 - 0x3FFF is locked to rom bank 0.
     * Sources:
     *  - https://retrocomputing.stackexchange.com/questions/11732/how-does-the-gameboys-memory-bank-switching-work
     *  - https://gbdev.io/pandocs/MBC1.html
     *
     * For our use case though, we don't need mode 0.
     */
    void switchMBC1BankingMode(uint8_t mode);

    /**
     * @brief This function reads data from the specified gameboy address
     */
    void read(uint16_t gbAddress, uint8_t* data, uint16_t size);

    /**
     * @brief This function reads data from the specified SRAM bank offset
     */
    void readSRAM(uint16_t SRAMBankOffset, uint8_t *data, uint16_t size);

    /**
     * @brief This function writes the given data to the given SRAMBankOffset
     * The write is cached and won't be written until a new write outside the current 32 byte block
     * is triggered or finishWrites() is called by the user.
     * 
     * WARNING: Don't forget to call finishWrites() when you're done writing! Otherwise corruption will occur
     * due to the cached writes
     */
    void writeSRAM(uint16_t SRAMBankOffset, const uint8_t *data, uint16_t size);

    /**
     * @brief This function writes the current writeBuffer immediately
     */
    void finishWrites();
protected:
private:
    joypad_port_t port_;
    bool isPoweredOn_;
    uint8_t currentSRAMBank_;
    uint16_t readBufferBankOffset_;
    uint16_t writeBufferSRAMBankOffset_;
    uint8_t readBuffer_[TPAK_BLOCK_SIZE];
    uint8_t writeBuffer_[TPAK_BLOCK_SIZE];
};

#endif