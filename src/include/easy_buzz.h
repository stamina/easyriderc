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
#ifndef EASY_BUZZ_H_INCLUDED
#define EASY_BUZZ_H_INCLUDED

#include <libopencm3/cm3/assert.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#define BUZZ_SOUND_POPCORN 1
#define BUZZ_SOUND_FROGGER 2
#define BUZZ_SOUND_FURELISE 3
#define BUZZ_SOUND_LARRY 4
#define BUZZ_SOUND_PIPI 5
#define BUZZ_SOUND_ALARM 6
#define BUZZ_SOUND_INDICATOR_HALFSECOND 7
#define BUZZ_SOUND_INDICATOR_QUARTERSECOND 8

void init_buzzer(void);
void set_buzzer_sound(uint8_t sound_song_idx);
void process_buzzer(void);

#endif
