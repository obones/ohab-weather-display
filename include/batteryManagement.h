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

namespace BatteryManagement
{
    float GetBatteryVoltage();

    float GetBatteryPercentage();
    float GetBatteryPercentage(float batteryVoltage);

    bool IsLowBattery();
    bool IsLowBattery(float batteryVoltage);
}
