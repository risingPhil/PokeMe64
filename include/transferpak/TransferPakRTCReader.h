#ifndef _TRANSFERPAKRTCREADER_H
#define _TRANSFERPAKRTCREADER_H

#include "RTCReader.h"

class TransferPakManager;

/**
 * @brief This class implements the IRTCReader interface specifically for the N64 Transfer Pak with libdragon
 */
class TransferPakRTCReader : public BaseRTCReader
{
public:
    TransferPakRTCReader(TransferPakManager& tpakManager);
    virtual ~TransferPakRTCReader();

    /**
     * This function will grab the current values of the RTC chip and store them internally
     *
     * Use the getXYZ() functions to actually retrieve this data
     */
    void latch() override;
protected:
private:
    TransferPakManager& tpakManager_;
};

#endif