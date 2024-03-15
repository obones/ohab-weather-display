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
#include "epd_driver.h"         // https://github.com/Xinyuan-LilyGO/LilyGo-EPD47

#define SCREEN_WIDTH   EPD_WIDTH
#define SCREEN_HEIGHT  EPD_HEIGHT

#define PARTIAL_AREA_WIDTH 300
#define PARTIAL_AREA_HEIGHT 130
#define PARTIAL_AREA_MARGIN 10

#define PARTIAL_AREA_X (SCREEN_WIDTH - PARTIAL_AREA_WIDTH)
#define PARTIAL_AREA_Y PARTIAL_AREA_MARGIN

