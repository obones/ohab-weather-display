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
#include "partialUpdate.h"
#include "screenDimensions.h"
#include "timeManagement.h"
#include "drawPrimitives.h"

#define BATT_PIN   (14)

Rect_t partialUpdateArea = 
    {
        .x = SCREEN_WIDTH / 2, //SCREEN_WIDTH - PARTIAL_AREA_WIDTH - 10,
        .y = SCREEN_HEIGHT / 2, //10,
        .width = PARTIAL_AREA_WIDTH,
        .height = PARTIAL_AREA_HEIGHT
    };

uint8_t localFrameBuffer[PARTIAL_AREA_WIDTH * PARTIAL_AREA_HEIGHT / 2];

void edp_partial_update() 
{
    for (int row = 0; row < PARTIAL_AREA_HEIGHT; row++)
    {
        memcpy(
            &localFrameBuffer[row * PARTIAL_AREA_WIDTH / 2], 
            &FrameBuffer[(partialUpdateArea.y + row) * EPD_WIDTH / 2 + partialUpdateArea.x / 2], 
            PARTIAL_AREA_WIDTH / 2
        );
    }

    epd_draw_grayscale_image(partialUpdateArea, localFrameBuffer); // Update the screen
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

    drawString(partialUpdateArea.x, partialUpdateArea.y + partialUpdateArea.height / 2, String(battery_voltage), LEFT);
}

void refreshTime()
{
    struct tm timeInfo;
    timeInfo.tm_hour = CurrentHour;
    timeInfo.tm_min = CurrentMin;
    timeInfo.tm_sec = CurrentSec;

    char updateTime[30];
    strftime(updateTime, sizeof(updateTime), "%H:%M:%S", &timeInfo);
    drawString(partialUpdateArea.x, partialUpdateArea.y, updateTime, LEFT);
}

void DoPartialUpdate()
{
    epd_poweron();      // Switch on EPD display
    //epd_clear_area(partialUpdateArea);
    epd_clear_area_cycles(partialUpdateArea, 2, 50);

    drawLine(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Black);
    drawLine(SCREEN_WIDTH / 2 + 10, 0, SCREEN_WIDTH / 2 + 10, SCREEN_HEIGHT, Black);
    //drawLine(0, 0, PARTIAL_AREA_WIDTH - 2, PARTIAL_AREA_HEIGHT - 2, Black);

    refreshBattery();
    refreshTime();

    edp_partial_update();       // Update the display to show the information
    epd_poweroff_all(); // Switch off all power to EPD
}

