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
#include <mbedtls/base64.h>

#include "lang.h"

#include "screenDimensions.h"
#include "fonts/opensans6.h"
#include "fonts/opensans8.h"
#include "fonts/opensans8b.h"
#include "fonts/opensans10.h"
#include "fonts/opensans12.h"
#include "fonts/opensans12b.h"
#include "fonts/opensans14.h"
#include "fonts/opensans14b.h"
#include "fonts/opensans18.h"
#include "fonts/opensans16.h"
#include "fonts/opensans24b.h"
#include "fonts/opensans26b.h"
#include "fonts/opensans32.h"
#include "fonts/weathericons48.h"
#include "fonts/weathericons64.h"
#include "drawPrimitives.h"
#include "fontManagement.h"
#include "timeManagement.h"
#include "constants.h"
#include "partialUpdate.h"
#include "ohab_weather_generated.h"

String Time_str = "--:--:--";
String Date_str = "-- --- ----";
uint8_t* currentState = nullptr;
int wifi_signal;

void edp_update() 
{
  epd_draw_grayscale_image(epd_full_screen(), FrameBuffer); // Update the screen
}

String WindDegToOrdinalDirection(float windDirection) 
{
    if (windDirection >= 348.75 || windDirection < 11.25)  return Lang::TXT_N;
    if (windDirection >=  11.25 && windDirection < 33.75)  return Lang::TXT_NNE;
    if (windDirection >=  33.75 && windDirection < 56.25)  return Lang::TXT_NE;
    if (windDirection >=  56.25 && windDirection < 78.75)  return Lang::TXT_ENE;
    if (windDirection >=  78.75 && windDirection < 101.25) return Lang::TXT_E;
    if (windDirection >= 101.25 && windDirection < 123.75) return Lang::TXT_ESE;
    if (windDirection >= 123.75 && windDirection < 146.25) return Lang::TXT_SE;
    if (windDirection >= 146.25 && windDirection < 168.75) return Lang::TXT_SSE;
    if (windDirection >= 168.75 && windDirection < 191.25) return Lang::TXT_S;
    if (windDirection >= 191.25 && windDirection < 213.75) return Lang::TXT_SSW;
    if (windDirection >= 213.75 && windDirection < 236.25) return Lang::TXT_SW;
    if (windDirection >= 236.25 && windDirection < 258.75) return Lang::TXT_WSW;
    if (windDirection >= 258.75 && windDirection < 281.25) return Lang::TXT_W;
    if (windDirection >= 281.25 && windDirection < 303.75) return Lang::TXT_WNW;
    if (windDirection >= 303.75 && windDirection < 326.25) return Lang::TXT_NW;
    if (windDirection >= 326.25 && windDirection < 348.75) return Lang::TXT_NNW;
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
        if (a == 45)  drawString(dxo + x + 15, dyo + y - 18, Lang::TXT_NE, CENTER);
        if (a == 135) drawString(dxo + x + 20, dyo + y - 2,  Lang::TXT_SE, CENTER);
        if (a == 225) drawString(dxo + x - 20, dyo + y - 2,  Lang::TXT_SW, CENTER);
        if (a == 315) drawString(dxo + x - 15, dyo + y - 18, Lang::TXT_NW, CENTER);
        dxi = dxo * 0.9;
        dyi = dyo * 0.9;
        drawLine(dxo + x, dyo + y, dxi + x, dyi + y, Black);
        dxo = dxo * 0.7;
        dyo = dyo * 0.7;
        dxi = dxo * 0.9;
        dyi = dyo * 0.9;
        drawLine(dxo + x, dyo + y, dxi + x, dyi + y, Black);
    }
    drawString(x, y - compassRadius - 20,     Lang::TXT_N, CENTER);
    drawString(x, y + compassRadius + 10,     Lang::TXT_S, CENTER);
    drawString(x - compassRadius - 15, y - 5, Lang::TXT_W, CENTER);
    drawString(x + compassRadius + 10, y - 5, Lang::TXT_E, CENTER);
    drawString(x + 3, y + 50, String(angle, 0) + "°", CENTER);
    setFont(OpenSans12B);
    drawString(x, y - 50, WindDegToOrdinalDirection(angle), CENTER);
    setFont(OpenSans24B);
    drawString(x, y - 18, String(windSpeed, 0), CENTER);
    setFont(OpenSans12B);
    drawString(x, y + 25, "km/h", CENTER);
}

const char* GetWeatherIcon(int conditionCode, bool isDay)
{
    // UCS2 to UTF8 conversion done with https://r12a.github.io/app-conversion/
    switch (conditionCode)
    {
        case 0:
            return (isDay) ? "\xEF\x80\x8D" : "\xEF\x80\xAE"; // wi-day-sunny, wi-night-clear
        case 1:
            return (isDay) ? "\xEF\x80\x8C" : "\xEF\x81\xBE"; // wi-day-sunny-overcast, wi-night-alt-cloudy-high
        case 2:
            return (isDay) ? "\xEF\x80\x82" : "\xEF\x82\x86"; // wi-day-cloudy, wi-night-alt-cloudy
        case 3:
            return "\xEF\x81\x81"; // wi-cloud
        case 45:
        case 48:
            return "\xEF\x80\x94"; // wi-fog 
        case 51 ... 99:
            return "\xEF\x80\x93"; // wi-cloudy
        default:
            return "\xEF\x81\xB5"; // wi-alien
    }
}

typedef struct 
{
    int32_t codePoint;
    int yOffset;
    const String precipitation;
    int precipitationXOffset;
    int precipitationYOffset;
}
WeatherGlyphDetails;

const WeatherGlyphDetails GetWeatherGlyph(int conditionCode, bool isDay)
{
    switch (conditionCode)
    {
        case 0:   // clear sky
            return (isDay) ? WeatherGlyphDetails{0xf00d, 5} : WeatherGlyphDetails{0xf02e, 15};  // wi-day-sunny, wi-night-clear
        case 1:   // mainly clear
            return (isDay) ? WeatherGlyphDetails{0xf00c, 5} : WeatherGlyphDetails{0xf07e, 5}; // wi-day-sunny-overcast, wi-night-alt-cloudy-high
        case 2:   // partly clear
            return (isDay) ? WeatherGlyphDetails{0xf002, 5} : WeatherGlyphDetails{0xf086, 5}; // wi-day-cloudy, wi-night-alt-cloudy
        case 3:   // overcast
            return {0xf041, 15}; // wi-cloud
        case 45:  // fog
            return {0xf014}; // wi-fog 
        case 48:  // depositing rime fog
            return {0xf014, 0, "* * *"}; // wi-fog + rime
        case 51:  // Light drizzle
            return {0x0f013, 10, "."}; // wi-cloudy
        case 53:  // Moderate drizzle
            return {0x0f013, 10, ". ."}; // wi-cloudy
        case 55:  // Dense drizzle
            return {0x0f013, 10, ". . ."}; // wi-cloudy
        case 56:  // Light freezing drizzle
            return {0x0f013, 10, ". ¤"}; // wi-cloudy
        case 57:  // Dense freezing drizzle
            return {0x0f013, 10, ". ¤ ."}; // wi-cloudy
        case 61:  // Slight rain
            return {0x0f013, 10, "/"}; // wi-cloudy
        case 63:  // Moderate rain
            return {0x0f013, 10, "/ /"}; // wi-cloudy
        case 65:  // Heavy rain
            return {0x0f013, 10, "/ / /"}; // wi-cloudy
        case 66:  // Light freezing rain
            return {0x0f013, 10, "/ ¤"}; // wi-cloudy
        case 67:  // Heavy freezing rain
            return {0x0f013, 10, "/ ¤ /"}; // wi-cloudy
        case 71:  // Slight snow fall
            return {0x0f013, 10, "*"}; // wi-cloudy
        case 73:  // Moderate snow fall
            return {0x0f013, 10, "* *", 0, -8}; // wi-cloudy
        case 75:  // Heavy snow fall
            return {0x0f013, 10, "* * *", 0, -8}; // wi-cloudy
        case 77:  // Snow grains
            return {0x0f013, 10, "° °"}; // wi-cloudy
        case 80:  // Slight rain showers
            return (isDay) ? 
                WeatherGlyphDetails{0xf002, 5, ":", -10, -5} : // wi-day-cloudy 
                WeatherGlyphDetails{0xf086, 5, ":", -10, -5};  // wi-night-alt-cloudy
        case 81:  // Moderate rain showers
            return (isDay) ? 
                WeatherGlyphDetails{0xf002, 5, ":  :", -10, -5} : // wi-day-cloudy
                WeatherGlyphDetails{0xf086, 5, ":  :", -10, -5};  // wi-night-alt-cloudy
        case 82:  // Heavy rain showers
            return (isDay) ? 
                WeatherGlyphDetails{0xf002, 5, ":  :  :", -10, -5} : // wi-day-cloudy
                WeatherGlyphDetails{0xf086, 5, ":  :  :", -10, -5};  // wi-night-alt-cloudy
        case 85:  // Slight snow showers
            return (isDay) ? 
                WeatherGlyphDetails{0xf002, 5, "*", -15} : // wi-day-cloudy
                WeatherGlyphDetails{0xf086, 5, "*", -15};  // wi-night-alt-cloudy
        case 86:  // Heavy snow showers
            return (isDay) ? 
                WeatherGlyphDetails{0xf002, 5, "* * *", -15, -8} : // wi-day-cloudy
                WeatherGlyphDetails{0xf086, 5, "* * *", -15, -8};  // wi-night-alt-cloudy
        case 95:  // Slight or moderate thunderstorm
            return {0x0f016, 10}; // wi-lightning
        case 96:  // Slight hail thunderstorm
            return {0x0f016, 5, "ø"}; // wi-lightning
        case 99:  // Heavy hail thunderstorm
            return {0x0f016, 5, "ø  ø  ø"}; // wi-lightning
        default:
            return {0xf075, 2, String(conditionCode)}; // wi-alien
    }
}

// moonDay 0 is the day after the full moon
const String GetMoonIcon(int moonDay) 
{
    return String("\xEF\x83") + char(0xAB + moonDay);
}

void DrawWeatherIcon(const GFXfont &glyphFont, const GFXfont &precipitationFont, int x, int y, int conditionCode, bool isDay, bool centerVertically)
{
    WeatherGlyphDetails details = GetWeatherGlyph(conditionCode, isDay);

    float scale = glyphFont.advance_y / WeatherIcons48.advance_y;
    int yOffset = ceil(details.yOffset * scale);

    GFXglyph* glyph;
    get_glyph(&glyphFont, details.codePoint, &glyph);

    int32_t glyphX = x - glyph->width / 2;
    int32_t glyphY = (centerVertically) ? y + glyph->height / 2 : y + glyph->height + yOffset;
    FontProperties props = font_properties_default();
    draw_char(&glyphFont, FrameBuffer, &glyphX, glyphY, SCREEN_WIDTH / 2, SCREEN_HEIGHT, details.codePoint, &props);

    const int precipitationXOffset = ceil(details.precipitationXOffset * scale); 
    const int precipitationYOffset = ceil((15 + details.precipitationYOffset) * scale) + ((centerVertically) ? yOffset : 0); 
    setFont(precipitationFont);
    drawString(x + precipitationXOffset, glyphY + precipitationYOffset, details.precipitation, CENTER);
}

void DisplayTodayForecast(int x, int y, int conditionCode, float maxTemp, float minTemp, float precipitationSum)
{
    const int textShiftX = 120;
    const int textShiftY = 40;
    const int textSpacing = 45;

    DrawWeatherIcon(WeatherIcons64, OpenSans24B, x, y + textShiftY + textSpacing, conditionCode, true, true);

    setFont(OpenSans16);
    drawString(x - textShiftX, y + textShiftY, String(maxTemp, 0) + "°", RIGHT);
    drawString(x - textShiftX, y + textShiftY + textSpacing, String(minTemp, 0) + "°", RIGHT);
    drawString(x - textShiftX - 12, y + textShiftY + textSpacing * 2, String(precipitationSum, 0), RIGHT);
    setFont(OpenSans8);
    drawString(x - textShiftX - 7, y + textShiftY + textSpacing * 2 + 15, "mm", LEFT);
}

void DisplayNextDaysForecast(int x, int y, int dayOfWeek, int conditionCode, float maxTemp, float minTemp)
{
    const int textShiftX = 60;
    const int textShiftY = 150;

    setFont(OpenSans14);
    int weekDayNameShiftY = 30;
    String WeekDayName = Lang::weekday_A[dayOfWeek];
    if (WeekDayName.indexOf("J") >= 0)
       weekDayNameShiftY += 5; 
    drawString(x, y - weekDayNameShiftY, WeekDayName, CENTER);

    DrawWeatherIcon(WeatherIcons48, OpenSans14B, x, y, conditionCode, true, false);

    setFont(OpenSans14);
    drawString(x - textShiftX, y + textShiftY, String(minTemp, 0) + "°", LEFT);
    drawString(x + textShiftX, y + textShiftY, String(maxTemp, 0) + "°", RIGHT);
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
    String uri = String("/rest/items/") + OpenHABItemName + "/state"; //http://server:8080/rest/items/ope

    http.begin(wifiClient, OpenHABServerName, OpenHABServerPort, uri); //http.begin(uri,test_root_ca); //HTTPS example connection
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) 
    {
        String base64State = http.getString();

        free(currentState);
        size_t bufferLength = base64State.length() * 2;
        size_t finalLength;
        currentState = reinterpret_cast<uint8_t*>(malloc(bufferLength));
        mbedtls_base64_decode(currentState, bufferLength, &finalLength, reinterpret_cast<const unsigned char *>(base64State.c_str()), base64State.length());

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

void DrawFullUpdateElements()
{
    DrawPartialUpdateElements();

    const ohab_weather::Weather* weather = ohab_weather::GetWeather(currentState);
    const ohab_weather::CurrentWeather* current = weather->current();

    // current wind information
    DisplayWindSection(137, 150, current->windDirection(), current->windSpeed(), 100);

    // current outdoor temperature
    setFont(OpenSans32);
    drawString(SCREEN_WIDTH - PARTIAL_AREA_MARGIN, PARTIAL_AREA_Y + PARTIAL_AREA_HEIGHT, String(current->outdoorTemperature(), 1) + "°", RIGHT);

    // current full string date
    setFont(OpenSans16);
    drawString(SCREEN_WIDTH / 2, 20, TimeManagement::GetFormattedDate(), CENTER);

    // forecast for today
    const int todayConditionCode = 99;
    DisplayTodayForecast(SCREEN_WIDTH / 2, 50, todayConditionCode, 15.2, -5.3, 2.9);

    // forecast for next days
    const int forecastDays = 6;
    const int daysMargin = 85;
    const int dayOfWeek = TimeManagement::getDayOfWeek();
    const int conditions[] = {80, 81, 82, 85, 86};
    for (int day = 1; day < forecastDays; day++)
        DisplayNextDaysForecast(
            daysMargin + (day - 1) * ((SCREEN_WIDTH - daysMargin * 2) / (forecastDays - 2)), 
            SCREEN_HEIGHT - 200, 
            (dayOfWeek + day) % 7, 
            conditions[day-1],//day+(day/4)*(44+2*(day%4)),
            5*day, 
            -1*day
        );
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
            DrawFullUpdateElements();
            break;
        case WifiIssue:
            issueText = "Failed to connect to WiFi!";
            break;
        case TimeIssue:
            issueText = "Failed to retrieve time!";
            break;
        case ServerIssue:
            setFont(OpenSans12);
            drawString(10, 10, String(OpenHABServerName) + "\n" + String(OpenHABServerPort) + "\n" + OpenHABItemName, LEFT);
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
