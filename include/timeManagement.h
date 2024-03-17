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
#pragma once

namespace TimeManagement
{
    bool Setup();
    void StartTimer();
    bool SynchronizeWithNTP();
    void StoreTime();
    void RetrieveTime();
    const char* GetFormattedTime();
    const char* GetFormattedTime(uint8_t style);
    const char* GetFormattedDate();
    int getDayOfWeek(); // Sunday is 0
}

