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
#include <HardwareSerial.h>
#include "partialUpdate.h"
#include "screenDimensions.h"
#include "timeManagement.h"
#include "batteryManagement.h"
#include "drawPrimitives.h"
#include "fontManagement.h"
#include "pins.h"
#include "fonts/opensans16.h"
#include "fonts/opensanslight48.h"

#define PARTIAL_AREA_WIDTH 300
#define PARTIAL_AREA_HEIGHT 150
#define PARTIAL_AREA_MARGIN 10

Rect_t partialUpdateArea = 
    {
        .x = SCREEN_WIDTH - PARTIAL_AREA_WIDTH,
        .y = PARTIAL_AREA_MARGIN,
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
    float batteryVoltage = BatteryManagement::GetBatteryVoltage();

    setFont(OpenSans16);
    drawString(partialUpdateArea.x + partialUpdateArea.width - PARTIAL_AREA_MARGIN, partialUpdateArea.y + partialUpdateArea.height - 40, String(batteryVoltage), RIGHT);

    esp_reset_reason_t resetReason = esp_reset_reason();
    drawString(partialUpdateArea.x, partialUpdateArea.y + partialUpdateArea.height - 40, String(resetReason), LEFT);
}

void refreshTime()
{
    const char* updateTime = TimeManagement::GetFormattedTime();

    setFont(OpenSansLight48);
    drawString(partialUpdateArea.x + partialUpdateArea.width - PARTIAL_AREA_MARGIN, partialUpdateArea.y, updateTime, RIGHT);
}

void DoPartialUpdate()
{
    epd_poweron();      // Switch on EPD display
    epd_clear_area_cycles(partialUpdateArea, 1, 50);

    DrawPartialUpdateElements();

    edp_partial_update();       // Update the display to show the information
    epd_poweroff_all(); // Switch off all power to EPD
}

void DrawPartialUpdateElements()
{
    refreshBattery();
    refreshTime();
}

