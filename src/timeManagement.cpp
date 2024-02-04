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
#include <esp_attr.h>
#include "timeManagement.h"

RTC_DATA_ATTR int CurrentHour = 0;
RTC_DATA_ATTR int CurrentMin = 0;
RTC_DATA_ATTR int CurrentSec = 0;
