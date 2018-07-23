#pragma once

#include "InputDevice.h"

namespace HWID
{
    class Switch : public InputDevice
    {

    public:
        Switch(string name = "", uint8_t switchPin = 0);
        ~Switch() override;

        uint8_t getSwitchPin() const;
        void setSwitchPin(uint8_t switchPin);

        void setUp() override;

        void pollingThread() override;

    protected:
        uint8_t mSwitchPin;

        // InputDevice interface
    public:
        void startPollingThread() override;
        void stopPollingThread() override;
    };

}
