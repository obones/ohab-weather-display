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
#include <Arduino.h>            // In-built
#include <esp_task_wdt.h>       // In-built
#include <HardwareSerial.h>
#include "freertos/FreeRTOS.h"  // In-built
#include "freertos/task.h"      // In-built
#include "esp_adc_cal.h"        // In-built
#include "soc/rtc_cntl_reg.h"

#include <PCF8563.h>

#include "partialUpdate.h"
#include "fullUpdate.h"
#include "screenDimensions.h"
#include "drawPrimitives.h"
#include "timeManagement.h"
#include "batteryManagement.h"
#include "lang.h"
#include "fontManagement.h"
#include "fonts/opensans18.h"
#include "fonts/opensans26b.h"

long StartTime       = 0;

void BeginSleep(bool allowWakeUp) 
{
    Serial.println("Awake for : " + String((millis() - StartTime) / 1000.0, 3) + "-secs");
    Serial.println("Starting deep-sleep period...");
    if (allowWakeUp)
    {
        esp_err_t resultCode = esp_sleep_enable_ext0_wakeup(GPIO_NUM_9, 0);
        if (resultCode != ESP_OK)
        {
            setFont(OpenSans26B);
            drawString(10, SCREEN_HEIGHT / 2, String("EXT0 issue: ") + String(resultCode), LEFT);
        }
    }

    epd_poweroff_all();

    esp_deep_sleep_start();  // Sleep until we have received a GPIO interrupt
}

void loop() 
{
    // Nothing to do here because we go to deep sleep directly
}

void InitializeSystem() 
{
    // disable brownout, it triggers too easily when waking up from "GPIO" deep sleep
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

    StartTime = millis();
    Serial.begin(115200);
    while (!Serial);
    
    Serial.println(String(__FILE__) + "\nStarting...");
    epd_init();
    
    FrameBuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
    if (!FrameBuffer) Serial.println("Full Screen Memory alloc failed!");
    memset(FrameBuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);

    if (!TimeManagement::Setup())
    {
        setFont(OpenSans26B);
        drawString(10, SCREEN_HEIGHT / 2, "Cannot setup RTC", LEFT);
    }
}

void setup() 
{
    InitializeSystem();

    bool lowBattery = BatteryManagement::IsLowBattery();
    if (lowBattery)
    {
        epd_clear();

        const int bodyWidth = 200;
        const int rectLeft = (SCREEN_WIDTH - bodyWidth) / 2;
        const int rectHeight = 50;
        const int bodyTop = (SCREEN_HEIGHT - rectHeight) / 2;
        const int lineMargin = 20;
        const int tipWidth = 10;
        const int tipHeight = 20;
        const int lineStartLeft = (SCREEN_WIDTH - rectHeight) / 2 - lineMargin;
        const int lineStartTop = (SCREEN_HEIGHT + rectHeight) / 2 + lineMargin;
        const int lineEndLeft = lineStartLeft + rectHeight + 2 * lineMargin;
        const int lineEndTop = lineStartTop - (rectHeight + 2 * lineMargin);

        drawRect(rectLeft, bodyTop, bodyWidth, rectHeight, Black);
        drawRect(rectLeft + 1, bodyTop + 1, bodyWidth - 2, rectHeight - 2, Black);

        drawRect(rectLeft + bodyWidth - 1, bodyTop + (rectHeight - tipHeight) / 2, tipWidth, tipHeight, Black);
        drawRect(rectLeft + bodyWidth - 1, bodyTop + (rectHeight - tipHeight) / 2 + 1, tipWidth - 2, tipHeight - 2, Black);

        drawLine(lineStartLeft, lineStartTop, lineEndLeft, lineEndTop, Black);
        drawLine(lineStartLeft - 1, lineStartTop, lineEndLeft - 1, lineEndTop, Black);
        drawLine(lineStartLeft + 1, lineStartTop, lineEndLeft + 1, lineEndTop, Black);

        setFont(OpenSans26B);
        drawString(SCREEN_WIDTH / 2, 50, Lang::Status::LowBattery, CENTER);

        setFont(OpenSans18);
        drawString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, TimeManagement::GetFormattedTime(PCF_TIMEFORMAT_YYYY_MM_DD_HH_MM_SS), LEFT);

        epd_draw_grayscale_image(epd_full_screen(), FrameBuffer); // Update the screen
    }
    else
    {
        esp_reset_reason_t resetReason = esp_reset_reason();
        if (resetReason == ESP_RST_DEEPSLEEP)
        {
            TimeManagement::RetrieveTime();

            time_t now;
            struct tm  info;
            time(&now);
            localtime_r(&now, &info);

            if (info.tm_min % 5 == 0)
            {
                DoFullUpdate((info.tm_hour == 4) && (info.tm_min < 2));
            }
            else
            {
                DoPartialUpdate();
            }
        }
        else
        {
            TimeManagement::StartTimer();
            DoFullUpdate(true);
        }
    }

    BeginSleep(!lowBattery);
}


