#include "Switch.h"
#include "spdlog/spdlog.h"
#include <bcm2835.h>

namespace HWID
{

    Switch::Switch(string name, uint8_t switchPin)
        : InputDevice(name),
          mSwitchPin(switchPin)
    {
        auto log = spdlog::get("Switch");
        if (!log)
        {
            log = spdlog::stdout_color_mt("Switch");
        }
        log->info("\"{}\" is constructing with pin {}",name, switchPin);

    }

    void Switch::setUp()
    {
        auto log = spdlog::get("Switch");
        log->info("Setting up {} on pin {}", mName, mSwitchPin);
        bcm2835_gpio_fsel(mSwitchPin, BCM2835_GPIO_FSEL_INPT);
    }

    void Switch::pollingThread()
    {
        auto log = spdlog::get("Switch");
        log->info("Running polling thread for {}",getName());
        mPolling = true;
        while (mPolling)
        {
            uint8_t value = bcm2835_gpio_lev(mSwitchPin);
            if (value != mCurrentValue)
            {
                mCurrentValue = value;
                json data;
                data[JSON_NAME] = getName();
                data[JSON_VALUE] = mCurrentValue;
                notifyAll(data);
            }
            this_thread::sleep_for(1s);
        }
        log->info("Finished polling thread for {}",getName());
    }

    void Switch::startPollingThread()
    {
        mPollingThread = make_shared<thread>(&Switch::pollingThread, this);
    }

    void Switch::stopPollingThread()
    {
        setPolling(false);
        if (mPollingThread != nullptr)
        {
           mPollingThread->join();
        }
    }

    Switch::~Switch() {}

    uint8_t Switch::getSwitchPin() const
    {
        return mSwitchPin;
    }

    void Switch::setSwitchPin(uint8_t switchPin)
    {
        mSwitchPin = switchPin;
    }

}
