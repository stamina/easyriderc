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
#ifndef EASY_FROGGER_H_INCLUDED
#define EASY_FROGGER_H_INCLUDED

#include "sound/notes.h"
#include "stdio.h"

const uint16_t g_music_frogger[] = {
    120,        8,  d5,         8,       e5,         8,  f5x,        8,
    g5,         8,  a5,         8,       BUZZ_PAUSE, 8,  f5x,        8,
    BUZZ_PAUSE, 8,  d5,         8,       e5,         8,  f5x,        8,
    e5,         8,  d5,         8,       BUZZ_PAUSE, 8,  d5,         8,
    BUZZ_PAUSE, 8,  d5,         8,       e5,         8,  f5x,        8,
    g5,         8,  a5,         8,       BUZZ_PAUSE, 8,  f5x,        8,
    BUZZ_PAUSE, 8,  a5,         8,       g5,         8,  f5x,        8,
    e5,         8,  d5,         8,       BUZZ_PAUSE, 8,  d5,         8,
    BUZZ_PAUSE, 8,  d5,         8,       e5,         8,  f5x,        8,
    g5,         8,  a5,         8,       BUZZ_PAUSE, 8,  f5x,        8,
    BUZZ_PAUSE, 8,  d5,         8,       e5,         8,  f5x,        8,
    e5,         8,  d5,         8,       BUZZ_PAUSE, 8,  d5,         8,
    BUZZ_PAUSE, 8,  d5,         8,       e5,         8,  f5x,        8,
    g5,         8,  a5,         8,       BUZZ_PAUSE, 8,  f5x,        8,
    BUZZ_PAUSE, 8,  a5,         8,       g5,         8,  f5x,        8,
    e5,         8,  d5,         2,       BUZZ_PAUSE, 8,  f5x,        8,
    d5,         8,  a4,         8,       d5,         8,  f5x,        8,
    d5,         8,  a4,         8,       d5,         8,  g5,         8,
    g5,         8,  f5x,        8,       f5x,        8,  e5,         2,
    BUZZ_PAUSE, 8,  g5,         8,       g5,         8,  f5x,        8,
    f5x,        8,  e5,         8,       e5,         8,  b5,         8,
    b5,         8,  a5,         8,       g5,         8,  f5x,        8,
    e5,         8,  d5,         2,       BUZZ_PAUSE, 8,  f5x,        8,
    d5,         8,  a4,         8,       d5,         8,  f5x,        8,
    d5,         8,  a4,         8,       d5,         8,  g5,         8,
    g5,         8,  f5x,        8,       f5x,        8,  e5,         2,
    BUZZ_PAUSE, 8,  g5,         8,       g5,         8,  f5x,        8,
    f5x,        8,  e5,         8,       e5,         8,  b5,         8,
    b5,         8,  a5,         8,       g5,         8,  f5x,        8,
    e5,         8,  d5,         2,       BUZZ_PAUSE, 8,  d6,         8,
    d6,         16, b5,         16,      a5,         8,  BUZZ_PAUSE, 8,
    d6,         8,  d6,         16,      b5,         16, a5,         8,
    BUZZ_PAUSE, 8,  f5x,        8,       g5,         16, g5x,        16,
    a5,         8,  BUZZ_PAUSE, 8,       f5x,        8,  f5x,        16,
    e5,         16, d5,         8,       BUZZ_PAUSE, 8,  d6,         8,
    d6,         16, b5,         16,      a5,         8,  BUZZ_PAUSE, 8,
    d6,         8,  d6,         16,      b5,         16, a5,         8,
    BUZZ_PAUSE, 8,  f5x,        8,       g5,         16, g5x,        16,
    a5,         8,  BUZZ_PAUSE, 8,       f5x,        8,  f5x,        16,
    e5,         16, d5,         8,       BUZZ_PAUSE, 8,  f5x,        8,
    d5,         8,  a4,         8,       d5,         8,  f5x,        8,
    d5,         8,  a4,         8,       d5,         8,  g5,         8,
    g5,         8,  f5x,        8,       f5x,        8,  e5,         2,
    BUZZ_PAUSE, 8,  g5,         8,       g5,         8,  f5x,        8,
    f5x,        8,  e5,         8,       e5,         8,  b5,         8,
    b5,         8,  a5,         8,       g5,         8,  f5x,        8,
    e5,         1,  d5,         BUZZ_END};

#endif
