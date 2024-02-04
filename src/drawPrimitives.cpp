/**
 * Copyright (c) 2024 Olivier Sannier
 *
 * See the NOTICE file(s) distributed with this work for additional
 * information.
 * 
 * Inspired by the great work of markbirss: https://github.com/markbirss/LilyGo-EPD-4-7-OWM-Weather-Display
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. 
 * If a copy of the MPL was not distributed with this file, 
 * you can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-License-Identifier: MPL-2.0
 */
#include <math.h>
#include <Arduino.h>

#include "screenDimensions.h"
#include "drawPrimitives.h"
#include "fontManagement.h"

void drawString(int x, int y, String text, alignment align) 
{
    char * data  = const_cast<char*>(text.c_str());
    int  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
    int w, h;
    int xx = x, yy = y;
    get_text_bounds(&currentFont, data, &xx, &yy, &x1, &y1, &w, &h, NULL);
    if (align == RIGHT)  x = x - w;
    if (align == CENTER) x = x - w / 2;
    int cursor_y = y + h;
    write_string(&currentFont, data, &x, &cursor_y, FrameBuffer);
}

void arrow(int x, int y, int arrowSize, float arrowAngle, int pointerWidth, int pointerLength) 
{
    float dx = (arrowSize - 10) * cos((arrowAngle - 90) * PI / 180) + x; // calculate X position
    float dy = (arrowSize - 10) * sin((arrowAngle - 90) * PI / 180) + y; // calculate Y position
    float x1 = 0;         float y1 = pointerLength;
    float x2 = pointerWidth / 2;  float y2 = pointerWidth / 2;
    float x3 = -pointerWidth / 2; float y3 = pointerWidth / 2;
    float angle = arrowAngle * PI / 180 - 135;
    float xx1 = x1 * cos(angle) - y1 * sin(angle) + dx;
    float yy1 = y1 * cos(angle) + x1 * sin(angle) + dy;
    float xx2 = x2 * cos(angle) - y2 * sin(angle) + dx;
    float yy2 = y2 * cos(angle) + x2 * sin(angle) + dy;
    float xx3 = x3 * cos(angle) - y3 * sin(angle) + dx;
    float yy3 = y3 * cos(angle) + x3 * sin(angle) + dy;
    fillTriangle(xx1, yy1, xx3, yy3, xx2, yy2, Black);
}

void fillCircle(int x, int y, int r, uint8_t color) 
{
    epd_fill_circle(x, y, r, color, FrameBuffer);
}

void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color) 
{
    epd_draw_hline(x0, y0, length, color, FrameBuffer);
}

void drawFastVLine(int16_t x0, int16_t y0, int length, uint16_t color) 
{
    epd_draw_vline(x0, y0, length, color, FrameBuffer);
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) 
{
    epd_write_line(x0, y0, x1, y1, color, FrameBuffer);
}

void drawCircle(int x0, int y0, int r, uint8_t color) 
{
    epd_draw_circle(x0, y0, r, color, FrameBuffer);
}

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
    epd_draw_rect(x, y, w, h, color, FrameBuffer);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
    epd_fill_rect(x, y, w, h, color, FrameBuffer);
}

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) 
{
    epd_fill_triangle(x0, y0, x1, y1, x2, y2, color, FrameBuffer);
}

void drawPixel(int x, int y, uint8_t color) 
{
    epd_draw_pixel(x, y, color, FrameBuffer);
}

void DrawSegment(int x, int y, int o1, int o2, int o3, int o4, int o11, int o12, int o13, int o14) 
{
    drawLine(x + o1,  y + o2,  x + o3,  y + o4,  Black);
    drawLine(x + o11, y + o12, x + o13, y + o14, Black);
}

