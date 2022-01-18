/* Copyright (C) 2021 Bas Brugman
 * http://www.visionnaire.nl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef EASY_LARRY_H_INCLUDED
#define EASY_LARRY_H_INCLUDED

#include "sound/notes.h"
#include "stdio.h"

const uint16_t g_music_larry[] = {
    120,        5,   e5,         40,  BUZZ_PAUSE, 5,   f5,         40,
    BUZZ_PAUSE, 7,   f5x,        17,  BUZZ_PAUSE, 7,   g5,         19,
    a5,         128, BUZZ_PAUSE, 10,  e5,         19,  BUZZ_PAUSE, 17,
    g5,         6,   BUZZ_PAUSE, 5,   a5,         109, BUZZ_PAUSE, 24,
    e5,         40,  BUZZ_PAUSE, 9,   g5,         40,  BUZZ_PAUSE, 23,
    a5,         153, BUZZ_PAUSE, 17,  g5,         11,  BUZZ_PAUSE, 2,
    c6,         14,  BUZZ_PAUSE, 9,   a5,         128, BUZZ_PAUSE, 16,
    c6,         128, BUZZ_PAUSE, 9,   g5x,        38,  BUZZ_PAUSE, 15,
    a5,         7,   BUZZ_PAUSE, 5,   c6,         24,  BUZZ_PAUSE, 20,
    g5x,        59,  BUZZ_PAUSE, 9,   a5,         64,  BUZZ_PAUSE, 19,
    c6,         109, BUZZ_PAUSE, 15,  d6,         17,  BUZZ_PAUSE, 4,
    d6x,        64,  d6,         384, BUZZ_PAUSE, 20,  c6x,        384,
    BUZZ_PAUSE, 7,   c6,         12,  BUZZ_PAUSE, 9,   e6,         9,
    BUZZ_PAUSE, 10,  e6,         9,   BUZZ_PAUSE, 8,   e6,         10,
    BUZZ_PAUSE, 9,   e6,         10,  BUZZ_PAUSE, 8,   e6,         19,
    d6x,        96,  BUZZ_PAUSE, 8,   d6,         51,  BUZZ_PAUSE, 3,
    c6x,        69,  BUZZ_PAUSE, 8,   a5,         13,  BUZZ_PAUSE, 8,
    d6x,        59,  BUZZ_PAUSE, 16,  e6,         8,   d6x,        128,
    BUZZ_PAUSE, 16,  e6,         85,  BUZZ_PAUSE, 9,   c6,         40,
    BUZZ_PAUSE, 13,  d6,         8,   BUZZ_PAUSE, 14,  c6,         4,
    BUZZ_PAUSE, 5,   e5,         40,  BUZZ_PAUSE, 5,   f5,         40,
    BUZZ_PAUSE, 7,   f5x,        17,  BUZZ_PAUSE, 7,   g5,         20,
    a5,         109, BUZZ_PAUSE, 10,  e5,         19,  BUZZ_PAUSE, 17,
    g5,         6,   BUZZ_PAUSE, 5,   a5,         128, BUZZ_PAUSE, 23,
    e5,         40,  BUZZ_PAUSE, 9,   g5,         38,  BUZZ_PAUSE, 24,
    a5,         128, BUZZ_PAUSE, 17,  g5,         12,  BUZZ_PAUSE, 2,
    c6,         14,  BUZZ_PAUSE, 9,   a5,         128, BUZZ_PAUSE, 16,
    c6,         153, BUZZ_PAUSE, 9,   g5x,        40,  BUZZ_PAUSE, 14,
    a5,         7,   BUZZ_PAUSE, 6,   c6,         23,  BUZZ_PAUSE, 20,
    g5x,        59,  BUZZ_PAUSE, 9,   a5,         59,  BUZZ_PAUSE, 19,
    c6,         96,  BUZZ_PAUSE, 15,  d6,         16,  BUZZ_PAUSE, 4,
    d6x,        76,  d6,         256, BUZZ_PAUSE, 20,  c6x,        7,
    c6,         12,  BUZZ_PAUSE, 9,   e6,         9,   BUZZ_PAUSE, 9,
    e6,         9,   BUZZ_PAUSE, 8,   e6,         10,  BUZZ_PAUSE, 9,
    e6,         10,  BUZZ_PAUSE, 7,   e6,         19,  d6x,        85,
    BUZZ_PAUSE, 8,   d6,         54,  BUZZ_PAUSE, 3,   c6x,        64,
    BUZZ_PAUSE, 8,   a5,         13,  BUZZ_PAUSE, 7,   d6x,        59,
    BUZZ_PAUSE, 16,  e6,         8,   d6x,        128, BUZZ_PAUSE, 16,
    e6,         76,  BUZZ_PAUSE, 9,   c6,         40,  BUZZ_PAUSE, 13,
    d6,         8,   BUZZ_PAUSE, 14,  c6,         1,   BUZZ_PAUSE, BUZZ_END};

#endif
