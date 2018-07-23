/*
 * InputDevice.cpp
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

#include "InputDevice.h"
#include <bcm2835.h>
#include "spdlog/spdlog.h"

namespace HWID
{
    InputDevice::InputDevice(string name)
        : mCurrentValue(0),
        mName(name)
    {
        auto log = spdlog::get("InputDevice");
        if (log == nullptr)
        {
            log = spdlog::stdout_color_mt("InputDevice");
        }
    }

    InputDevice::~InputDevice()
    {

    }

    string InputDevice::getName() const
    {
        return mName;
    }

    void InputDevice::setName(const string& name)
    {
        mName = name;
    }

    void InputDevice::addListener(InputDeviceListener* listener)
    {
        auto log = spdlog::get("InputDevice");
        log->info("Registered listener for {}",getName());
        mListenerHandles.push_back(listener);
    }

    void InputDevice::removeListener(InputDeviceListener* listener)
    {
       /*
       auto itr = find(begin(mListenerHandles),end(mListenerHandles),listener);
       if (itr != end(mListenerHandles))
       {
           mListenerHandles.erase(itr);
       }
       */
    }

    void InputDevice::notifyAll(json data)
    {
       for (InputDeviceListener* listenerHandle : mListenerHandles)
       {
           listenerHandle->onChange(data);
       }
    }

    bool InputDevice::getPolling() const
    {
        return mPolling;
    }

    void InputDevice::setPolling(volatile bool polling)
    {
        mPolling = polling;
    }

    uint8_t InputDevice::getCurrentValue() const
    {
        return mCurrentValue;
    }

    void InputDevice::setCurrentValue(const uint8_t& currentValue)
    {
        mCurrentValue = currentValue;
    }

    const map<uint8_t,uint8_t> InputDevice::GPIO_TO_BCM
    {
        { 6, RPI_BPLUS_GPIO_J8_31},
        { 13, RPI_BPLUS_GPIO_J8_33},
        { 17, RPI_BPLUS_GPIO_J8_11},
        { 19, RPI_BPLUS_GPIO_J8_35},
        { 22, RPI_BPLUS_GPIO_J8_15},
        { 26, RPI_BPLUS_GPIO_J8_37},
        { 27, RPI_BPLUS_GPIO_J8_13}
    };

    const string InputDevice::JSON_VALUE = "value";
    const string InputDevice::JSON_NAME = "name";

    InputDeviceListener::~InputDeviceListener() {}
}
