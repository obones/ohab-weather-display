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
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <PCF8563.h>

#include "lang.h"

#include "screenDimensions.h"
#include "fonts/opensans8.h"
#include "fonts/opensans8b.h"
#include "fonts/opensans12b.h"
#include "fonts/opensans18.h"
#include "fonts/opensans24b.h"
#include "fonts/opensans26b.h"
#include "drawPrimitives.h"
#include "fontManagement.h"
#include "timeManagement.h"
#include "constants.h"
#include "partialUpdate.h"

String Time_str = "--:--:--";
String Date_str = "-- --- ----";
int wifi_signal;

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

uint8_t StartWiFi() 
{
    Serial.println("\r\nConnecting to: " + String(ssid));
    WiFi.disconnect();
    WiFi.mode(WIFI_STA); // switch off AP
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) 
    {
        Serial.printf("STA: Failed!\n");
        WiFi.disconnect(false);
        delay(500);
        WiFi.begin(ssid, password);
    }

    if (WiFi.status() == WL_CONNECTED) 
    {
        wifi_signal = WiFi.RSSI(); // Get Wifi Signal strength now, because the WiFi will be turned off to save power!
        Serial.println("WiFi connected at: " + WiFi.localIP().toString());
    }
    else
    { 
        Serial.println("WiFi connection *** FAILED ***");
    }

    return WiFi.status();
}

void StopWiFi() 
{
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    Serial.println("WiFi switched Off");
}

enum getStateResult {Success, WifiIssue, TimeIssue, ServerIssue};

getStateResult getLatestStateFromOpenHAB(bool SynchronizeWithNTP)
{
    if (StartWiFi() != WL_CONNECTED)
        return WifiIssue;

    if (SynchronizeWithNTP && !TimeManagement::SynchronizeWithNTP())
        return TimeIssue;

    Time_str = TimeManagement::GetFormattedTime(PCF_TIMEFORMAT_HH_MM_SS);

    WiFiClient wifiClient;   // wifi client object
    wifiClient.stop(); // close connection before sending a new request
    HTTPClient http;
    String uri = String("/rest/items/") + OpenHABItemName; //http://server:8080/rest/items/ope

    http.begin(wifiClient, OpenHABServerName, OpenHABServerPort, uri); //http.begin(uri,test_root_ca); //HTTPS example connection
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) 
    {
        wifiClient.stop();
        http.end();
        return Success;
    }
    else
    {
        String errorString = http.errorToString(httpCode);
        setFont(OpenSans8);
        drawString(150, 10, uri, LEFT);
        drawString(150, 10, errorString, LEFT);

        Serial.printf("connection failed, error: %s", errorString.c_str());
        wifiClient.stop();
        http.end();
        return ServerIssue;
    }
    http.end();
    
    return Success;
}

void DoFullUpdate(bool SynchronizeWithNTP)
{
    getStateResult stateResult = getLatestStateFromOpenHAB(SynchronizeWithNTP);
    StopWiFi();

    epd_poweron();      // Switch on EPD display
    epd_clear();        // Clear the screen

    String issueText = "";
    switch (stateResult)
    {
        case Success:
            DrawPartialUpdateElements();
            DisplayWindSection(137, 150, 240, 20, 100);
            setFont(OpenSans18);
            drawString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, Time_str, LEFT);
            break;
        case WifiIssue:
            issueText = "Failed to connect to WiFi!";
            break;
        case TimeIssue:
            issueText = "Failed to retrieve time!";
            break;
        case ServerIssue:
            setFont(OpenSans8);
            drawString(10, 10, OpenHABServerName, LEFT);
            drawString(10, 20, String(OpenHABServerPort), LEFT);
            drawString(10, 30, OpenHABItemName, LEFT);
            issueText = "Failed to communicate\nwith OpenHAB server!";
            break;
    }

    if (!issueText.isEmpty())
    {
        setFont(OpenSans26B);
        drawString(10, SCREEN_HEIGHT / 2, issueText, LEFT);
    }

    edp_update();       // Update the display to show the information
    epd_poweroff_all(); // Switch off all power to EPD
}
