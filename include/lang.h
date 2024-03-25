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
#define USE_LANG_EN

#ifdef USE_LANG_FR
#undef USE_LANG_EN
#endif

#include <WString.h>

namespace Lang
{
    //Wind
    extern const String TXT_WIND_SPEED_DIRECTION;
    extern const String TXT_N;
    extern const String TXT_NNE;
    extern const String TXT_NE;
    extern const String TXT_ENE;
    extern const String TXT_E;
    extern const String TXT_ESE;
    extern const String TXT_SE;
    extern const String TXT_SSE;
    extern const String TXT_S;
    extern const String TXT_SSW;
    extern const String TXT_SW;
    extern const String TXT_WSW;
    extern const String TXT_W;
    extern const String TXT_WNW;
    extern const String TXT_NW;
    extern const String TXT_NNW;

    //Day of the week
    extern const char* weekday_a[];
    extern const char* weekday_A[];

    //Month
    extern const char* month_b[];
    extern const char* month_B[];

    // with printf replacements, 1 is string for day of week, 2 is day in month as a number, 3 is month name
    extern const char* FormattedDateFormat;

    // system status
    namespace Status
    {
        extern const char* LowBattery;
        extern const char* FailedWiFi;
        extern const char* FailedTime;
        extern const char* FailedOpenHAB;
        extern const char* RTCError;
    }
}