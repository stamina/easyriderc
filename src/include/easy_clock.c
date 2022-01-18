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
#include "easy_clock.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>

#define MAX_TICK_SLOTS 16

// software ticks
volatile uint32_t g_1ms_ticker;
volatile uint32_t g_10ms_ticker;
volatile uint32_t g_100ms_ticker;
volatile uint32_t g_1s_ticker;

// software downcounter tick arrays
volatile uint32_t g_1ms_tick_slot[MAX_TICK_SLOTS];
volatile uint32_t g_10ms_tick_slot[MAX_TICK_SLOTS];
volatile uint32_t g_100ms_tick_slot[MAX_TICK_SLOTS];
volatile uint32_t g_1s_tick_slot[MAX_TICK_SLOTS];

// proto's
void init_systick(void);
void process_tick_slots(volatile uint32_t slot[]);

// setup Reset and Clock Control RCC
// using 2 external crystals for main and RTC
// SYSCLK = 180MHz
// 180Mhz: Power/HCLK to AHB bus, core, memory and DMA,
// Cortex system timer(Systick), FCLK, APB2 timer clocks
// 90MHz: APB1 timer clocks, APB2 peripheral clocks
// 45MHz: APB1 peripheral clocks
void clock_setup() {
  rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);
  init_systick();
}

// The System Tick timer is part of the ARM Cortex core. It is a 24 bit down
// counter that can be configured with an automatical reload value.
// Count down from 180k to 0 at 180mhz -> 1ms ticker
void init_systick() {
  systick_set_reload(180000);
  systick_clear();
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_counter_enable();
  systick_interrupt_enable();
}

// software downcounter triggers
void process_tick_slots(volatile uint32_t slot[]) {
  for (uint8_t i = 0; i < MAX_TICK_SLOTS; i++) {
    if (slot[i]) slot[i]--;
  }
}

// ISR handler triggered every 1ms
void sys_tick_handler(void) {
  g_1ms_ticker++;
  process_tick_slots(g_1ms_tick_slot);
  // 10ms
  if (g_1ms_ticker % 10 == 0) {
    g_10ms_ticker++;
    process_tick_slots(g_10ms_tick_slot);
  }
  // 100ms
  if (g_1ms_ticker % 100 == 0) {
    g_100ms_ticker++;
    process_tick_slots(g_100ms_tick_slot);
  }
  // 1s
  if (g_1ms_ticker % 1000 == 0) {
    g_1s_ticker++;
    process_tick_slots(g_1s_tick_slot);
  }
}

inline uint32_t get_1ms_ticker() { return g_1ms_ticker; }
inline uint32_t get_10ms_ticker() { return g_10ms_ticker; }
inline uint32_t get_100ms_ticker() { return g_100ms_ticker; }
inline uint32_t get_1s_ticker() { return g_1s_ticker; }

void clear_1ms_ticker() { g_1ms_ticker = 0; }
void clear_10ms_ticker() { g_10ms_ticker = 0; }
void clear_100ms_ticker() { g_100ms_ticker = 0; }
void clear_1s_ticker() { g_1s_ticker = 0; }

void set_1ms_tick_slot(t_1ms_tick_slot slot, uint32_t val) {
  g_1ms_tick_slot[slot] = val;
}

void set_10ms_tick_slot(t_10ms_tick_slot slot, uint32_t val) {
  g_10ms_tick_slot[slot] = val;
}

void set_100ms_tick_slot(t_100ms_tick_slot slot, uint32_t val) {
  g_100ms_tick_slot[slot] = val;
}

void set_1s_tick_slot(t_1s_tick_slot slot, uint32_t val) {
  g_1s_tick_slot[slot] = val;
}

uint32_t get_1ms_tick_slot(t_1ms_tick_slot slot) {
  return g_1ms_tick_slot[slot];
}
uint32_t get_10ms_tick_slot(t_10ms_tick_slot slot) {
  return g_10ms_tick_slot[slot];
}

uint32_t get_100ms_tick_slot(t_100ms_tick_slot slot) {
  return g_100ms_tick_slot[slot];
}
uint32_t get_1s_tick_slot(t_1s_tick_slot slot) { return g_1s_tick_slot[slot]; }

