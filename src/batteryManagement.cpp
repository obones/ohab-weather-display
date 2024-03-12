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
#include <esp_adc_cal.h>
#include <HardwareSerial.h>

#include "batteryManagement.h"
#include "pins.h"

namespace BatteryManagement
{
    float latestBatteryVoltage = 0;
    const float minimumBatteryVoltage = 2.99;

    float GetBatteryVoltage()
    {
        if (latestBatteryVoltage == 0)
        {
            esp_adc_cal_characteristics_t adc_chars;
            esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
                ADC_UNIT_2,
                ADC_ATTEN_DB_11,
                ADC_WIDTH_BIT_12,
                1100,
                &adc_chars
            );

            int vref = 1100;
            if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) 
            {
                Serial.printf("eFuse Vref: %umV\r\n", adc_chars.vref);
                vref = adc_chars.vref;
            }

            uint16_t v = analogRead(BATT_PIN);
            latestBatteryVoltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
        }

        return latestBatteryVoltage;
    }

    float GetBatteryPercentage()
    {
        return GetBatteryPercentage(GetBatteryVoltage());
    }

    float GetBatteryPercentage(float batteryVoltage)
    {
        float result = 2836.9625 * pow(batteryVoltage, 4) - 43987.4889 * pow(batteryVoltage, 3) + 255233.8134 * pow(batteryVoltage, 2) - 656689.7123 * batteryVoltage + 632041.7303;
        if (batteryVoltage >= 4.40) return 100;
        if (IsLowBattery(batteryVoltage)) return 0;
        
        return result;
    }

    bool IsLowBattery()
    {
        return IsLowBattery(GetBatteryVoltage());
    }

    bool IsLowBattery(float batteryVoltage)
    {
        return batteryVoltage < minimumBatteryVoltage;
    }
}
