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
#include "screenDimensions.h"
#include "timeManagement.h"
#include "drawPrimitives.h"

#define AREA_WIDTH 50
#define AREA_HEIGHT 50

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

void DoPartialUpdate()
{
    epd_poweron();      // Switch on EPD display
    epd_clear_area(partialUpdateArea);

    struct tm timeInfo;
    timeInfo.tm_hour = CurrentHour;
    timeInfo.tm_min = CurrentMin;
    timeInfo.tm_sec = CurrentSec;

    char updateTime[30];
    strftime(updateTime, sizeof(updateTime), "%H:%M:%S", &timeInfo);  // Creates: '@ 14:05:49'   and change from 30 to 8 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    drawString(partialUpdateArea.x, partialUpdateArea.y + partialUpdateArea.height / 2, updateTime, RIGHT);

    edp_partial_update();       // Update the display to show the information
    epd_poweroff_all(); // Switch off all power to EPD
}

