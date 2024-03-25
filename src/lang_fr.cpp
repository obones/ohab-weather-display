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
#include "lang.h"

#ifdef USE_LANG_FR
#include <WString.h>

//Wind
namespace Lang
{
    const String TXT_WIND_SPEED_DIRECTION = "Vitesse Vent/Direction";
    const String TXT_N   = "N";
    const String TXT_NNE = "NNE";
    const String TXT_NE  = "NE";
    const String TXT_ENE = "ENE";
    const String TXT_E   = "E";
    const String TXT_ESE = "ESE";
    const String TXT_SE  = "SE";
    const String TXT_SSE = "SSE";
    const String TXT_S   = "S";
    const String TXT_SSW = "SSO";
    const String TXT_SW  = "SO";
    const String TXT_WSW = "OSO";
    const String TXT_W   = "O";
    const String TXT_WNW = "ONO";
    const String TXT_NW  = "NO";
    const String TXT_NNW = "NNO";

    //Day of the week
    const char* weekday_a[] = { "Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam" };
    const char* weekday_A[] = { "Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi" };

    //Month
    const char* month_b[] = { "Jan", "Fév", "Mar", "Avr", "Mai", "Jun", "Jul", "Aou", "Sep", "Oct", "Nov", "Déc" };
    const char* month_B[] = { "Janvier", "Février", "Mars", "Avril", "Mai", "Juin", "Juillet", "Août", "Septpembre", "Octobre", "Novembre", "Décembre" };

    // with printf replacements, 1 is string for day of week, 2 is day in month as a number, 3 is month name
    const char* FormattedDateFormat = "%1$s %2$d %3$s";

    // system status
    namespace Status
    {
        const char* LowBattery = "Batterie faible";
        const char* FailedWiFi = "Echec de connexion au WiFi !";
        const char* FailedTime = "Echec de récupération de l'heure !";
        const char* FailedOpenHAB = "Echec de communication\navec le serveur OpenHAB !";
        const char* RTCError = "Echec de configuration RTC";
    }
}

#endif