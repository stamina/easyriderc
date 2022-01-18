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
#ifndef EASY_CLOCK_H_INCLUDED
#define EASY_CLOCK_H_INCLUDED

#include <libopencm3/stm32/rcc.h>

typedef enum { LED1_1MS_SLOT, LED2_1MS_SLOT, BUZZ_1MS_SLOT } t_1ms_tick_slot;
typedef enum { LED1_10MS_SLOT, LED2_10MS_SLOT } t_10ms_tick_slot;
typedef enum {
  LED1_100MS_SLOT,
  ADC_100MS_SLOT,
  LED2_100MS_SLOT
} t_100ms_tick_slot;
typedef enum { LED1_1S_SLOT, LED2_1S_SLOT } t_1s_tick_slot;

void clock_setup(void);
uint32_t get_1ms_ticker(void);
uint32_t get_10ms_ticker(void);
uint32_t get_100ms_ticker(void);
uint32_t get_1s_ticker(void);

void clear_1ms_ticker(void);
void clear_10ms_ticker(void);
void clear_100ms_ticker(void);
void clear_1s_ticker(void);

void set_1ms_tick_slot(t_1ms_tick_slot slot, uint32_t val);
void set_10ms_tick_slot(t_10ms_tick_slot slot, uint32_t val);
void set_100ms_tick_slot(t_100ms_tick_slot slot, uint32_t val);
void set_1s_tick_slot(t_1s_tick_slot slot, uint32_t val);

uint32_t get_1ms_tick_slot(t_1ms_tick_slot slot);
uint32_t get_10ms_tick_slot(t_10ms_tick_slot slot);
uint32_t get_100ms_tick_slot(t_100ms_tick_slot slot);
uint32_t get_1s_tick_slot(t_1s_tick_slot slot);

#endif
