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
#include <time.h>
#include <esp_adc_cal.h>
#include <HardwareSerial.h>
#include "screenDimensions.h"
#include "timeManagement.h"
#include "drawPrimitives.h"

#define AREA_WIDTH 50
#define AREA_HEIGHT 50
#define BATT_PIN   (14)

Rect_t partialUpdateArea = 
    {
        .x = SCREEN_WIDTH - AREA_WIDTH,
        .y = 0,
        .width = AREA_WIDTH,
        .height = AREA_HEIGHT
    };

void edp_partial_update() 
{
    epd_draw_grayscale_image(partialUpdateArea, FrameBuffer); // Update the screen
}

void refreshBattery()
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
    float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);

    drawString(partialUpdateArea.x, partialUpdateArea.y + partialUpdateArea.height / 2, String(battery_voltage), RIGHT);
}

void refreshTime()
{
    struct tm timeInfo;
    timeInfo.tm_hour = CurrentHour;
    timeInfo.tm_min = CurrentMin;
    timeInfo.tm_sec = CurrentSec;

    char updateTime[30];
    strftime(updateTime, sizeof(updateTime), "%H:%M:%S", &timeInfo);
    drawString(partialUpdateArea.x, partialUpdateArea.y, updateTime, RIGHT);
}

void DoPartialUpdate()
{
    epd_poweron();      // Switch on EPD display
    epd_clear_area(partialUpdateArea);

    refreshBattery();
    refreshTime();

    edp_partial_update();       // Update the display to show the information
    epd_poweroff_all(); // Switch off all power to EPD
}

