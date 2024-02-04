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
#include <math.h>
#include <Arduino.h>

#include "lang.h"

#include "screenDimensions.h"
#include "fonts/opensans8b.h"
#include "fonts/opensans12b.h"
#include "fonts/opensans24b.h"
#include "drawPrimitives.h"
#include "fontManagement.h"

void edp_update() 
{
  epd_draw_grayscale_image(epd_full_screen(), FrameBuffer); // Update the screen
}

String WindDegToOrdinalDirection(float windDirection) 
{
    if (windDirection >= 348.75 || windDirection < 11.25)  return TXT_N;
    if (windDirection >=  11.25 && windDirection < 33.75)  return TXT_NNE;
    if (windDirection >=  33.75 && windDirection < 56.25)  return TXT_NE;
    if (windDirection >=  56.25 && windDirection < 78.75)  return TXT_ENE;
    if (windDirection >=  78.75 && windDirection < 101.25) return TXT_E;
    if (windDirection >= 101.25 && windDirection < 123.75) return TXT_ESE;
    if (windDirection >= 123.75 && windDirection < 146.25) return TXT_SE;
    if (windDirection >= 146.25 && windDirection < 168.75) return TXT_SSE;
    if (windDirection >= 168.75 && windDirection < 191.25) return TXT_S;
    if (windDirection >= 191.25 && windDirection < 213.75) return TXT_SSW;
    if (windDirection >= 213.75 && windDirection < 236.25) return TXT_SW;
    if (windDirection >= 236.25 && windDirection < 258.75) return TXT_WSW;
    if (windDirection >= 258.75 && windDirection < 281.25) return TXT_W;
    if (windDirection >= 281.25 && windDirection < 303.75) return TXT_WNW;
    if (windDirection >= 303.75 && windDirection < 326.25) return TXT_NW;
    if (windDirection >= 326.25 && windDirection < 348.75) return TXT_NNW;
    return "?";
}

void DisplayWindSection(int x, int y, float angle, float windSpeed, int compassRadius) 
{
    arrow(x, y, compassRadius - 22, angle, 18, 33); // Show wind direction on outer circle of width and length
    setFont(OpenSans8B);
    int dxo, dyo, dxi, dyi;
    drawCircle(x, y, compassRadius, Black);       // Draw compass circle
    drawCircle(x, y, compassRadius + 1, Black);   // Draw compass circle
    drawCircle(x, y, compassRadius * 0.7, Black); // Draw compass inner circle
    for (float a = 0; a < 360; a = a + 22.5) {
        dxo = compassRadius * cos((a - 90) * PI / 180);
        dyo = compassRadius * sin((a - 90) * PI / 180);
        if (a == 45)  drawString(dxo + x + 15, dyo + y - 18, TXT_NE, CENTER);
        if (a == 135) drawString(dxo + x + 20, dyo + y - 2,  TXT_SE, CENTER);
        if (a == 225) drawString(dxo + x - 20, dyo + y - 2,  TXT_SW, CENTER);
        if (a == 315) drawString(dxo + x - 15, dyo + y - 18, TXT_NW, CENTER);
        dxi = dxo * 0.9;
        dyi = dyo * 0.9;
        drawLine(dxo + x, dyo + y, dxi + x, dyi + y, Black);
        dxo = dxo * 0.7;
        dyo = dyo * 0.7;
        dxi = dxo * 0.9;
        dyi = dyo * 0.9;
        drawLine(dxo + x, dyo + y, dxi + x, dyi + y, Black);
    }
    drawString(x, y - compassRadius - 20,     TXT_N, CENTER);
    drawString(x, y + compassRadius + 10,     TXT_S, CENTER);
    drawString(x - compassRadius - 15, y - 5, TXT_W, CENTER);
    drawString(x + compassRadius + 10, y - 5, TXT_E, CENTER);
    drawString(x + 3, y + 50, String(angle, 0) + "°", CENTER);
    setFont(OpenSans12B);
    drawString(x, y - 50, WindDegToOrdinalDirection(angle), CENTER);
    setFont(OpenSans24B);
    drawString(x + 3, y - 18, String(windSpeed, 1), CENTER);
    setFont(OpenSans12B);
    drawString(x, y + 25, "km/h", CENTER);
}

void DoFullUpdate()
{
    epd_poweron();      // Switch on EPD display
    epd_clear();        // Clear the screen

    DisplayWindSection(137, 150, 240, 20, 100);

    edp_update();       // Update the display to show the information
    epd_poweroff_all(); // Switch off all power to EPD
}
