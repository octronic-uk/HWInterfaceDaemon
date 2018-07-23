#pragma once

#include <vector>
#include <memory>
#include <thread>

#include "HWIDConfig.h"
#include "InputDevice.h"

using namespace std;

namespace HWID
{
    class Switch;
    class Encoder;

    class Daemon
    {
    public:
        Daemon(string configFilePath);
        ~Daemon();

        void setUpDevices();
        void setUpSwitches();
        void setUpEncoders();
        void addInputDevice(InputDevice* device);
        void removeInputDevice(InputDevice* device);
        void clearInputDevices();
        Switch* createSwitch(json data);
        Encoder* createEncoder(json data);
        void registerListener(InputDeviceListener* listenerHandle);
        int run();
        void stop();

    protected:
        void startPollingThreads();
        void stopPollingThreads();

        void openGPIO();
        void closeGPIO();


    private:
        string mConfigFilePath;
        volatile bool mIsRunning;
        vector<shared_ptr<InputDevice>> mDevices;
        shared_ptr<HWIDConfig> mConfiguration;
    };

    class LogListener : public InputDeviceListener
    {
    public:
        LogListener();
        ~LogListener() override;
        void onChange(json value) override;
    };
}
