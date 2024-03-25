/**
 * Copyright (c) 2024 Olivier Sannier
 *
 * See the NOTICE file(s) distributed with this work for additional
 * information.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. 
 * If a copy of the MPL was not distributed with this file, 
 * you can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-License-Identifier: MPL-2.0
 */
#include "lang.h"

#ifdef USE_LANG_EN
#include <WString.h>

namespace Lang
{
    //Wind
    const String TXT_WIND_SPEED_DIRECTION = "Wind Speed/Direction";
    const String TXT_N   = "N";
    const String TXT_NNE = "NNE";
    const String TXT_NE  = "NE";
    const String TXT_ENE = "ENE";
    const String TXT_E   = "E";
    const String TXT_ESE = "ESE";
    const String TXT_SE  = "SE";
    const String TXT_SSE = "SSE";
    const String TXT_S   = "S";
    const String TXT_SSW = "SSW";
    const String TXT_SW  = "SW";
    const String TXT_WSW = "WSW";
    const String TXT_W   = "W";
    const String TXT_WNW = "WNW";
    const String TXT_NW  = "NW";
    const String TXT_NNW = "NNW";

    //Day of the week
    const char* weekday_a[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    const char* weekday_A[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

    //Month
    const char* month_b[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    const char* month_B[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

    // with printf replacements, 1 is string for day of week, 2 is day in month as a number, 3 is month name
    const char* FormattedDateFormat = "%1$s, %3$s %2$d";

    // system status
    namespace Status
    {
        const char* LowBattery = "Low battery";
        const char* FailedWiFi = "Failed to connect to WiFi!";
        const char* FailedTime = "Failed to retrieve time!";
        const char* FailedOpenHAB = "Failed to communicate\nwith OpenHAB server!";
        const char* RTCError = "Cannot setup RTC";
    }
}

#endif