/*
 * HWIDConfig.cpp
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
#include "HWIDConfig.h"
#include <fstream>
#include "spdlog/spdlog.h"

using namespace std;
using namespace nlohmann;

namespace HWID
{
    HWIDConfig::HWIDConfig(string configPath)
        : mPath(configPath)
    {
        auto log = spdlog::get("HWIDConfig");
        if (log == nullptr)
        {
            log = spdlog::stdout_color_mt("HWIDConfig");
        }
        log->info("Constructing with file config {}", configPath);
    }

    bool HWIDConfig::load()
    {
        auto log = spdlog::get("HWIDConfig");
        try
        {
            ifstream configFileInputStream(mPath);
            configFileInputStream >> mJsonObject;

            if (mJsonObject.is_object())
            {
                log->info("Parsed config {}", mJsonObject.dump());
                return true;
            }
        }
        catch (json::parse_error ex)
        {
            log->error("Exception while loading config file {}", ex.what());
        }

        log->error("Loading configuration failed");

        return false;
    }

    json HWIDConfig::getSwitches()
    {
        if (!mJsonObject.is_null())
        {
            json switchesArray = mJsonObject[JSON_SWITCHES];
            if (switchesArray.is_array())
            {
                return switchesArray;
            }
        }
        return json::array();
    }

    json HWIDConfig::getEncoders()
    {
        if (!mJsonObject.is_null())
        {
            json encodersArray = mJsonObject[JSON_ENCODERS];
            if (encodersArray.is_array())
            {
                return encodersArray;
            }
        }
        return json::array();
    }


    const string HWIDConfig::JSON_SWITCHES = "switches";
    const string HWIDConfig::JSON_ENCODERS = "encoders";
    const string HWIDConfig::JSON_NAME = "name";
    const string HWIDConfig::JSON_CLOCK_PIN = "clock_pin";
    const string HWIDConfig::JSON_DATA_PIN = "data_pin";
    const string HWIDConfig::JSON_SWITCH_PIN = "switch_pin";
}
