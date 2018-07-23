#include "Encoder.h"
#include <bcm2835.h>
#include "spdlog/spdlog.h"

namespace HWID
{
    Encoder::Encoder(string name, uint8_t switchPin, uint8_t clockPin, uint8_t dataPin)
        : Switch(name, switchPin),
          mClockPin(clockPin),
          mDataPin(dataPin),
          mSwitchValue(0)
    {
        auto log = spdlog::get("Encoder");
        if (log == nullptr)
        {
            log = spdlog::stdout_color_mt("Encoder");
        }
        log->info
        (
            "\"{}\" is constructing with sw({}) clk({}) data({})",
            name, switchPin, clockPin, dataPin
        );
    }

    Encoder::~Encoder()
    {

    }

    void Encoder::setUp()
    {
        Switch::setUp();

        auto log = spdlog::get("Encoder");
        // Clock Pin
        log->info("\"{}\" is setting up clock pin: {}",mName, mClockPin);
        bcm2835_gpio_fsel(mClockPin, BCM2835_GPIO_FSEL_INPT);
        // Data Pin
        log->info("\"{}\" is setting up data pin: {}",mName, mDataPin);
        bcm2835_gpio_fsel(mDataPin, BCM2835_GPIO_FSEL_INPT);
    }

    void Encoder::pollingThread()
    {
        auto log = spdlog::get("Encoder");
        log->info("Running polling thread for {}",mName);

        uint8_t clockLast = bcm2835_gpio_lev(mClockPin);
        uint8_t switchLast = bcm2835_gpio_lev(mSwitchPin);

        mPolling = true;

        while (mPolling)
        {
            uint8_t clock = bcm2835_gpio_lev(mClockPin);
            uint8_t data = bcm2835_gpio_lev(mDataPin);
            uint8_t switchData = bcm2835_gpio_lev(mSwitchPin);
            bool change = false;

            if (switchData != switchLast)
            {
                change = true;
                mSwitchValue = switchData;
                //log->info("{}'s switch has changed to {}",getName(),switchData);
            }
            switchLast = switchData;

            if (clock != clockLast)
            {
                if (data != clock)
                {
                    mCurrentValue--;
                }
                else
                {
                    mCurrentValue++;
                }
                change = true;
                //log->info("{}'s value has changed to {}",getName(),getCurrentValue());
            }
            clockLast = clock;

            if(change)
            {
                json data;
                data[JSON_NAME] = getName();
                data[JSON_VALUE] = mCurrentValue;
                data[JSON_ENCODER_SWITCH] = mSwitchValue;
                notifyAll(data);
            }

            // wait a bit
            this_thread::sleep_for(5ms);
        }

        log->info("Polling thread finished for {}",getName());
    }

    void Encoder::startPollingThread()
    {
        auto log = spdlog::get("Encoder");
        log->info("Creating polling thread for {}",getName());
        mPollingThread = make_shared<thread>(&Encoder::pollingThread, this);
    }

    void Encoder::stopPollingThread()
    {
        auto log = spdlog::get("Encoder");
        log->info("Stopping polling thread for {}",getName());

        setPolling(false);
        if (mPollingThread != nullptr)
        {
            mPollingThread->join();
        }
    }

    const string Encoder::JSON_ENCODER_SWITCH = "encoder_switch";
}
