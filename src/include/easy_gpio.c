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
#include "easy_gpio.h"

#include "easy_buzz.h"
#include "easy_clock.h"
#include "easy_state.h"

void gpio_setup() {
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOC);
  // led pin nucleo board TODO: remove this on production
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);
  gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO5);
  gpio_set(GPIOA, GPIO5);
  // status leds
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
}

void set_status_led1(uint32_t blinkspd) {
  if (blinkspd) {
    ;
    ;
  }
  // slow blink
  if (!get_1s_tick_slot(LED1_1S_SLOT)) {
    gpio_toggle(GPIOC, GPIO12);
    set_1s_tick_slot(LED1_1S_SLOT, 1);
  }
  // fast blink
  /*if (!get_100ms_tick_slot(LED1_100MS_SLOT)) {*/
  /*gpio_toggle(GPIOA, GPIO5);*/
  /*set_100ms_tick_slot(LED1_100MS_SLOT, 1);*/
  /*}*/
}

void set_status_led2(uint32_t blinkspd) {
  if (blinkspd) {
    ;
    ;
  }
  // slow blink
  if (!get_1s_tick_slot(LED2_1S_SLOT)) {
    gpio_toggle(GPIOC, GPIO7);
    set_1s_tick_slot(LED2_1S_SLOT, 1);
  }
}
