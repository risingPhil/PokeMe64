#include "transferpak/TransferPakRTCReader.h"
#include "transferpak/TransferPakManager.h"

TransferPakRTCReader::TransferPakRTCReader(TransferPakManager& tpakManager)
    : tpakManager_(tpakManager)
{
}

TransferPakRTCReader::~TransferPakRTCReader()
{
}

void TransferPakRTCReader::latch()
{
    uint8_t data[TPAK_BLOCK_SIZE];
    const int port = tpakManager_.getPort();

    // enable SRAM and RTC first
    const bool previousRAMState = tpakManager_.isRAMEnabled();
    tpakManager_.setRAMEnabled(true);
    tpakManager_.finishWrites();

    // now latch the RTC data into the RTC registers
    // https://gbdev.io/pandocs/MBC3.html
    memset(data, 0, sizeof(data));
    tpak_write(port, 0x6000, data, TPAK_BLOCK_SIZE);
    memset(data, 1, sizeof(data));
    tpak_write(port, 0x6000, data, TPAK_BLOCK_SIZE);

    // RTC S (seconds)
    tpakManager_.switchGBSRAMBank(0x08);
    tpak_read(port, 0xA000, data, sizeof(data));
    seconds_ = data[0];

    // RTC M (minutes)
    tpakManager_.switchGBSRAMBank(0x09);
    tpak_read(port, 0xA000, data, sizeof(data));
    minutes_ = data[0];

    // RTC H (hours)
    tpakManager_.switchGBSRAMBank(0x0A);
    tpak_read(port, 0xA000, data, sizeof(data));
    hours_ = data[0];

    // RTC DH (days counter upper 8 bits)
    tpakManager_.switchGBSRAMBank(0x0C);
    tpak_read(port, 0xA000, data, sizeof(data));
    daysRaw_ = data[0];
    daysRaw_ <<= 8;

    // RTC DL (days counter lower 8 bits)
    tpakManager_.switchGBSRAMBank(0x0B);
    tpak_read(port, 0xA000, data, sizeof(data));
    daysRaw_ |= data[0];

    tpakManager_.setRAMEnabled(previousRAMState);
}
