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
#ifndef EASY_PRODIGY_H_INCLUDED
#define EASY_PRODIGY_H_INCLUDED

#include "sound/notes.h"
#include "stdio.h"

const uint16_t g_music_prodigy[] = {
    70, 16,  e7, 16,  d7x, 16, e7,  16, d7x, 16,      e7,  16, d7x, 16, e7,
    16, g7,  16, a7,  16,  g7, 16,  a7, 16,  g7,      16,  a7, 16,  g7, 16,
    e7, 16,  e7, 16,  e7,  16, d7x, 16, e7,  16,      d7x, 16, e7,  16, d7x,
    16, e7,  16, g7,  16,  a7, 16,  g7, 16,  a7,      16,  g7, 16,  a7, 16,
    g7, 16,  e7, 16,  e7,  16, e7,  16, d7x, 16,      e7,  16, d7x, 16, e7,
    16, d7x, 16, e7,  16,  g7, 16,  a7, 16,  g7,      16,  a7, 16,  g7, 16,
    a7, 16,  g7, 16,  e7,  16, e7,  16, e7,  16,      d7x, 16, e7,  16, d7x,
    16, e7,  16, d7x, 16,  e7, 16,  g7, 16,  a7,      16,  g7, 16,  a7, 16,
    g7, 16,  a7, 16,  g7,  16, e7,  16, e7,  BUZZ_END};
#endif
