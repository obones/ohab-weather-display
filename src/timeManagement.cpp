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

PCF8563_Class rtc;

bool TimeManagement::Setup()
{
    Wire.begin(SDA_PIN, SCL_PIN);
    return rtc.begin() != 0;
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
    return rtc.formatDateTime(PCF_TIMEFORMAT_HH_MM);
}

