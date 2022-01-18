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
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>

#include "easy_adc.h"
#include "easy_buzz.h"
#include "easy_clock.h"
#include "easy_cmd.h"
#include "easy_debug.h"
#include "easy_gpio.h"
#include "easy_state.h"
#include "stdio.h"
#include "string.h"

void initialize(void);

void initialize() {
  clock_setup();
  gpio_setup();
  debug_setup();
  init_state();
  init_dma2();
  init_adc();
  init_buzzer();
  flush_events();
  set_event(EV_IGN_ON);
  set_event(EV_IGN_OFF);
}

int main(void) {
  initialize();
  while (1) {
    process_events();
    process_cmds();
  }
  return 0;
}

