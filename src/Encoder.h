#pragma once

#include "Switch.h"

namespace HWID
{
    class Encoder : public Switch
    {
    public:
        Encoder(string name = "", uint8_t switchPin = 0, uint8_t clockPin = 0, uint8_t dataPin = 0);
        ~Encoder() override;

        void setUp() override;

        void pollingThread() override;

        const static string JSON_ENCODER_SWITCH;

    protected:
        uint8_t mClockPin;
        uint8_t mDataPin;
        uint8_t mSwitchValue;

    public:
        void startPollingThread() override;
        void stopPollingThread() override;
    };

}
