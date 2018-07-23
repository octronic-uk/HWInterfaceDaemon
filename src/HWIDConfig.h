/*
 * HWIDConfig.h
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

#ifdef __APPLE__
    #include <json/json.hpp>
#else
    #include <json.hpp>
#endif

#include <string>

using namespace nlohmann;
using namespace std;

namespace HWID
{
    class HWIDConfig
    {
    public:
        HWIDConfig(string configPath);

        bool load();
        json getSwitches();
        json getEncoders();

    protected:
        string mPath;
        json mJsonObject;

    public:
        const static string JSON_SWITCHES;
        const static string JSON_ENCODERS;
        const static string JSON_NAME;
        const static string JSON_CLOCK_PIN;
        const static string JSON_DATA_PIN;
        const static string JSON_SWITCH_PIN;
    };
}
