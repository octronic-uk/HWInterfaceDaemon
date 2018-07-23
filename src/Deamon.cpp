#include <memory>
#include <thread>
#include <chrono>

#include <bcm2835.h>

#include "spdlog/spdlog.h"

#include "Daemon.h"
#include "Switch.h"
#include "Encoder.h"

using namespace spdlog;
using namespace std::literals::chrono_literals;

namespace HWID
{
    Daemon::Daemon(string configFilePath)
        :
          mConfigFilePath(configFilePath),
          mIsRunning(false)
    {
        auto log = spdlog::get("Daemon");
        if (log == nullptr)
        {
            log = spdlog::stdout_color_mt("Daemon");
        }
        log->info("Constructing");
        openGPIO();
    }

    void Daemon::openGPIO()
    {
        auto log = spdlog::get("Daemon");
#ifdef __APPLE__
        bcm2835_set_debug(1);
#endif
        mConfiguration = make_shared<HWIDConfig>(mConfigFilePath);
        bcm2835_init();
        log->info("Initialised BCM_2835");
        mConfiguration->load();
        setUpDevices();
    }

    Daemon::~Daemon()
    {

    }

    void Daemon::closeGPIO()
    {
        auto log = spdlog::get("Daemon");
        bcm2835_close();
        log->info("Closed BCM_2835");
    }

    void Daemon::setUpDevices()
    {
        auto log = spdlog::get("Daemon");
        log->info("Setting Up Devices");
        setUpSwitches();
        setUpEncoders();
    }

    void Daemon::setUpSwitches()
    {
        auto log = spdlog::get("Daemon");
        log->info("Setting Up Switches");
        for (auto switchJson : mConfiguration->getSwitches())
        {
            auto next = createSwitch(switchJson);
            next->setUp();
        }
    }

    void Daemon::setUpEncoders()
    {
        auto log = spdlog::get("Daemon");
        log->info("Setting Up Encoders");
        for (auto encoderJson : mConfiguration->getEncoders())
        {
            auto next = createEncoder(encoderJson);
            next->setUp();
        }
    }

    int Daemon::run()
    {
        auto log = spdlog::get("Daemon");
        log->info("Daemon is running");

        mIsRunning = true;

        startPollingThreads();

        while (mIsRunning)
        {
            this_thread::sleep_for(1s);
        }

        stopPollingThreads();
        closeGPIO();

        log->info("Daemon has stopped running");
        return 0;
    }

    void Daemon::startPollingThreads()
    {
        for (shared_ptr<InputDevice> id : mDevices)
        {
            id->startPollingThread();
        }
    }

    void Daemon::stopPollingThreads()
    {
        for (shared_ptr<InputDevice> t : mDevices)
        {
            t->stopPollingThread();
        }
    }

    void Daemon::stop()
    {
        auto log = spdlog::get("Daemon");
        log->info("Daemon is stopping");
        mIsRunning = false;
    }

    Switch* Daemon::createSwitch(json data)
    {
        auto log = spdlog::get("Daemon");
        string name = data[HWIDConfig::JSON_NAME];
        uint8_t switchPin = data[HWIDConfig::JSON_SWITCH_PIN];

        auto switchObj = make_shared<Switch>(name,InputDevice::GPIO_TO_BCM.at(switchPin));

        log->info("Adding switch {}",switchObj->getName());

        mDevices.push_back(switchObj);
        return switchObj.get();
    }

    Encoder* Daemon::createEncoder(json data)
    {
        auto log = spdlog::get("Daemon");

        string name = data[HWIDConfig::JSON_NAME];
        uint8_t switchPin = data[HWIDConfig::JSON_SWITCH_PIN];
        uint8_t clockPin = data[HWIDConfig::JSON_CLOCK_PIN];
        uint8_t dataPin = data[HWIDConfig::JSON_DATA_PIN];

        auto encoder = make_shared<Encoder>
                (
                    name,
                    InputDevice::GPIO_TO_BCM.at(switchPin),
                    InputDevice::GPIO_TO_BCM.at(clockPin),
                    InputDevice::GPIO_TO_BCM.at(dataPin)
                );

        log->info("Adding encoder {}", encoder->getName());

        mDevices.push_back(encoder);
        return encoder.get();
    }

    void Daemon::registerListener(InputDeviceListener* listenerHandle)
    {
        for (shared_ptr<InputDevice> device : mDevices)
        {
            device->addListener(listenerHandle);
        }
    }

    // LogListener

    LogListener::LogListener()
        : InputDeviceListener ()
    {

    }

    LogListener::~LogListener()
    {

    }

    void LogListener::onChange(json data)
    {
        auto log = spdlog::get("LogListener");
        if (log == nullptr)
        {
            log = spdlog::stdout_color_mt("LogListener");
        }
        log->info("Heard device change from {} ", data.dump());
    }
}
