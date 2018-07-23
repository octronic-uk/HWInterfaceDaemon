/*
 * InputDevice.h
 *
 * Created: 20 2018 by Ashley
 *
 * Copyright 2018 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <map>

#include "json.hpp"

using namespace std;
using namespace nlohmann;

namespace HWID
{

    class InputDeviceListener
    {
    public:
        virtual ~InputDeviceListener();
        virtual void onChange(json value) = 0;
    };

    class InputDevice
    {
    public:
        InputDevice(string name = "");
        virtual ~InputDevice();

        string getName() const;
        void setName(const string& name);

        virtual void setUp() = 0;
        virtual void pollingThread() = 0;
        virtual void startPollingThread() = 0;
        virtual void stopPollingThread() = 0;

        void addListener(InputDeviceListener*);
        void removeListener(InputDeviceListener*);
        void notifyAll(json value);

        const static map<uint8_t,uint8_t> GPIO_TO_BCM;

        bool getPolling() const;
        void setPolling(volatile bool polling);

        uint8_t getCurrentValue() const;
        void setCurrentValue(const uint8_t& currentValue);

        const static string JSON_VALUE;
        const static string JSON_NAME;

    protected:
        volatile bool mPolling;
        uint8_t mCurrentValue;
        string mName;
        shared_ptr<thread> mPollingThread;
        vector<InputDeviceListener*> mListenerHandles;
    };
}


