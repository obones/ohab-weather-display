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
#include "fonts/opensans8.h"
#include "fonts/opensans16.h"
#include "fonts/opensanslight48.h"

Rect_t partialUpdateArea = 
    {
        .x = PARTIAL_AREA_X,
        .y = PARTIAL_AREA_Y,
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

    if (batteryVoltage > 1 )  // Only display if there is a valid reading
    { 
        Serial.println("\nVoltage = " + String(batteryVoltage));
        float percentage = BatteryManagement::GetBatteryPercentage(batteryVoltage);

        const int symbolWidth = 40;
        const int symbolHeight = 20;
        const int tipWidth = 4;
        const int tipHeight = 7;
        const int x = partialUpdateArea.x + partialUpdateArea.width - PARTIAL_AREA_MARGIN - (symbolWidth + tipWidth);
        const int y = partialUpdateArea.y + 90;//partialUpdateArea.height - symbolHeight - 1;

        drawRect(x, y, symbolWidth, symbolHeight, Black);
        fillRect(x + symbolWidth, y + (symbolHeight - tipHeight) / 2, tipWidth, tipHeight, Black);
        fillRect(x + 2, y + 3, (symbolWidth - 4) * percentage / 100.0, symbolHeight - 6, Black);

        setFont(OpenSans8);
        drawString(x - 10, y + symbolHeight / 2 - 6, String(percentage, 0) + "%  " + String(batteryVoltage, 2) + "v", RIGHT);
    }

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

