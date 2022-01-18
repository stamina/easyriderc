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
#ifndef EASY_PIPI_H_INCLUDED
#define EASY_PIPI_H_INCLUDED

#include "sound/notes.h"
#include "stdio.h"

const uint16_t g_music_pipi[] = {
    120,        12, c5,  26,      BUZZ_PAUSE, 12, f5,  24,
    BUZZ_PAUSE, 12, a5,  24,      BUZZ_PAUSE, 12, f5,  24,
    BUZZ_PAUSE, 6,  g5,  12,      BUZZ_PAUSE, 24, a5x, 48,
    BUZZ_PAUSE, 24, a5,  48,      BUZZ_PAUSE, 24, g5,  48,
    BUZZ_PAUSE, 24, f5,  48,      BUZZ_PAUSE, 12, e5,  24,
    BUZZ_PAUSE, 12, g5,  24,      BUZZ_PAUSE, 12, c5,  24,
    BUZZ_PAUSE, 12, e5,  24,      BUZZ_PAUSE, 6,  f5,  12,
    BUZZ_PAUSE, 6,  a5,  12,      BUZZ_PAUSE, 12, c5,  24,
    BUZZ_PAUSE, 12, f5,  24,      BUZZ_PAUSE, 12, a5,  24,
    BUZZ_PAUSE, 12, f5,  24,      BUZZ_PAUSE, 6,  g5,  12,
    BUZZ_PAUSE, 24, a5x, 48,      BUZZ_PAUSE, 24, a5,  48,
    BUZZ_PAUSE, 24, g5,  48,      BUZZ_PAUSE, 24, f5,  48,
    BUZZ_PAUSE, 12, e5,  24,      BUZZ_PAUSE, 12, g5,  24,
    BUZZ_PAUSE, 12, c5,  24,      BUZZ_PAUSE, 12, e5,  24,
    BUZZ_PAUSE, 3,  f5,  6,       BUZZ_PAUSE, 12, a5,  24,
    BUZZ_PAUSE, 24, a5,  48,      BUZZ_PAUSE, 24, a5,  48,
    BUZZ_PAUSE, 12, a5,  24,      BUZZ_PAUSE, 12, a5,  24,
    BUZZ_PAUSE, 6,  a5x, 12,      BUZZ_PAUSE, 12, a5x, 24,
    BUZZ_PAUSE, 24, a5x, 48,      BUZZ_PAUSE, 24, a5,  48,
    BUZZ_PAUSE, 12, g5,  24,      BUZZ_PAUSE, 12, g5,  24,
    BUZZ_PAUSE, 12, g5,  24,      BUZZ_PAUSE, 12, f5,  24,
    BUZZ_PAUSE, 12, e5,  24,      BUZZ_PAUSE, 12, f5,  24,
    BUZZ_PAUSE, 6,  g5,  12,      BUZZ_PAUSE, 12, a5,  24,
    BUZZ_PAUSE, 24, a5,  48,      BUZZ_PAUSE, 24, a5,  48,
    BUZZ_PAUSE, 12, a5,  24,      BUZZ_PAUSE, 12, a5,  24,
    BUZZ_PAUSE, 6,  a5x, 12,      BUZZ_PAUSE, 12, a5x, 24,
    BUZZ_PAUSE, 24, a5x, 48,      BUZZ_PAUSE, 24, a5,  48,
    BUZZ_PAUSE, 12, g5,  24,      BUZZ_PAUSE, 12, g5,  24,
    BUZZ_PAUSE, 12, f5,  24,      BUZZ_PAUSE, 12, e5,  24,
    BUZZ_PAUSE, 3,  f5,  BUZZ_END};
#endif
