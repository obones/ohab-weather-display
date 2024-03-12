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
#include <esp_attr.h>
#include <PCF8563.h>
#include "timeManagement.h"
#include "pins.h"
#include "constants.h"

#define PCF8563_TIMER_1_60_HZ (0b11)

PCF8563_Class rtc;

bool TimeManagement::Setup()
{
    Wire.begin(SDA_PIN, SCL_PIN);
    return rtc.begin() != 0;
}

void TimeManagement::StartTimer()
{
    rtc.setTimer(1, PCF8563_TIMER_1_60_HZ, true);
    rtc.enableTimer();
}

bool TimeManagement::SynchronizeWithNTP()
{
    // reset the current time to force complete time read
    struct tm timeInfo;
    struct timeval val;

    timeInfo.tm_hour = 0;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;
    timeInfo.tm_year = 1970;
    timeInfo.tm_mon = 0;
    timeInfo.tm_mday = 1;
    val.tv_sec = mktime(&timeInfo);
    val.tv_usec = 0;
    settimeofday(&val, NULL);

    // start sync with NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer, "time.nist.gov"); //(gmtOffset_sec, daylightOffset_sec, ntpServer)

    setenv("TZ", Timezone, 1);  //setenv()adds the "TZ" variable to the environment with a value TimeZone, only used if set to 1, 0 means no change
    tzset(); // Set the TZ environment variable

    while (!getLocalTime(&timeInfo, 5000)) // Wait for 5-sec for time to synchronize
    {
        Serial.println("Failed to obtain time");
        return false;
    }

    StoreTime();

    return true;
}

void TimeManagement::StoreTime()
{
    rtc.syncToRtc();
}

void TimeManagement::RetrieveTime()
{
    rtc.syncToSystem();
}

const char* TimeManagement::GetFormattedTime()
{
    return GetFormattedTime(PCF_TIMEFORMAT_HH_MM);
}

const char* TimeManagement::GetFormattedTime(uint8_t style)
{
    return rtc.formatDateTime(style);
}
