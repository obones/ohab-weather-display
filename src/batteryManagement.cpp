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
        // 4PL symmetrical sigmoidal formula found with https://mycurvefit.com/ and those data points:
        //   4.42              100.5       
        //   4                  91.1       
        //   3.9                80
        //   3.75               50.09      
        //   2.99                0         
        //
        float result = 100.8605 + (-0.03662109 - 100.8605) / (1 + pow(batteryVoltage / 3.751438, 34.70826));
        if (result > 100) return 100;
        if ((result < 0) || IsLowBattery(batteryVoltage)) return 0;
        
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
