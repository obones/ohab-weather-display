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

#include "partialUpdate.h"
#include "fullUpdate.h"
#include "screenDimensions.h"
#include "timeManagement.h"

const long SleepDuration   = 1; //60; // Sleep time in minutes, aligned to the nearest minute boundary, so if 30 will always update at 00 or 30 past the hour
const int  WakeupHour      = 8;  // Don't wakeup until after 07:00 to save battery power
const int  SleepHour       = 23; // Sleep after 23:00 to save battery power
const long Delta           = 30; // ESP32 rtc speed compensation, prevents display at xx:59:yy and then xx:00:yy (one minute later) to save power

long StartTime       = 0;

RTC_DATA_ATTR int EventCnt = 0;

uint8_t *FrameBuffer;

void BeginSleep() 
{
    epd_poweroff_all();
    //UpdateLocalTime();
    long SleepTimer = (SleepDuration * 60 - ((CurrentMin % SleepDuration) * 60 + CurrentSec)) + Delta; //Some ESP32 have a RTC that is too fast to maintain accurate time, so add an offset
    esp_sleep_enable_timer_wakeup(SleepTimer * 1000000LL); // in Secs, 1000000LL converts to Secs as unit = 1uSec
    Serial.println("Awake for : " + String((millis() - StartTime) / 1000.0, 3) + "-secs");
    Serial.println("Entering " + String(SleepTimer) + " (secs) of sleep time");
    Serial.println("Starting deep-sleep period...");
    esp_deep_sleep_start();  // Sleep for e.g. 30 minutes
}

void loop() 
{
    // Nothing to do here because we go to deep sleep directly
}

void InitializeSystem() 
{
    StartTime = millis();
    Serial.begin(115200);
    while (!Serial);
    
    Serial.println(String(__FILE__) + "\nStarting...");
    epd_init();
    
    FrameBuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
    if (!FrameBuffer) Serial.println("Memory alloc failed!");
    memset(FrameBuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
}

void setup() 
{
    InitializeSystem();
    
    /*bool WakeUp = false;                
    if (WakeupHour > SleepHour)
        WakeUp = (CurrentHour >= WakeupHour || CurrentHour <= SleepHour); 
    else                             
        WakeUp = (CurrentHour >= WakeupHour && CurrentHour <= SleepHour);                              

    if (WakeUp) */
    {
        if (EventCnt % 5 == 0)
        DoFullUpdate();
        else
        DoPartialUpdate();
    }

    EventCnt++;
    
    BeginSleep();
}


