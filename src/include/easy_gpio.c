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
#include "easy_spi.h"
#include "easy_state.h"

// portA
#define S_XTRA3 1
#define S_XTRA2 2
#define S_XTRA1 4
#define S_ALARM 8
#define S_WARNING 16
#define S_IGNITION 32
#define S_LIGHT 64
#define S_PILOT 128

// portB
#define S_G1 1
#define S_G2 2
#define S_G3 4
#define S_G4 8
#define S_BRAKE 16
#define S_LI 32
#define S_RI 64
#define S_CLAXON 128

#define SENSE_DEBOUNCE_CHECKS 5
#define RELAYS_DEBOUNCE_CHECKS 5

uint8_t g_senses_checks;
uint8_t g_relays_checks;
uint8_t g_senses_debounce_pending;
uint8_t g_relays_debounce_pending;
uint8_t g_cur_sense_gpio_a;
uint8_t g_cur_sense_gpio_b;
uint8_t g_cur_sense_capture_a;
uint8_t g_cur_sense_capture_b;

// proto's
void dispatch_senses(void);
void check_neutral(void);

void gpio_setup() {
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);
  // status leds
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
  // interrupt pin SENSES PB1 and interrupt pin RELAYS PB15
  // NOTE: since the RELAYS I/O expander is set only outputs, we're not
  // using PB15 atm (XTRA4 and XTRA5 are not used and can be configured
  // as inputs later on)
  gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO1 | GPIO15);
}

void dispatch_senses() {
  // NOTE: S_XTRA1-3 are not used atm, these pins are for future senses
  // pin == 1, means sense is OFF, pin == 0, means sense is ON (pulled down)
  (g_cur_sense_gpio_a & S_ALARM) ? set_event(EV_ALARM_SET_OFF)
                                 : set_event(EV_ALARM_SET_ON);
  (g_cur_sense_gpio_a & S_WARNING) ? set_event(EV_WARNING_OFF)
                                   : set_event(EV_WARNING_ON);
  (g_cur_sense_gpio_a & S_IGNITION) ? set_event(EV_IGN_OFF)
                                    : set_event(EV_IGN_ON);
  (g_cur_sense_gpio_a & S_LIGHT) ? set_event(EV_LIGHT_OFF)
                                 : set_event(EV_LIGHT_ON);
  (g_cur_sense_gpio_a & S_PILOT) ? set_event(EV_PILOT_OFF)
                                 : set_event(EV_PILOT_ON);
  (g_cur_sense_gpio_b & S_G1) ? set_event(EV_GEAR1_OFF)
                              : set_event(EV_GEAR1_ON);
  (g_cur_sense_gpio_b & S_G2) ? set_event(EV_GEAR2_OFF)
                              : set_event(EV_GEAR2_ON);
  (g_cur_sense_gpio_b & S_G3) ? set_event(EV_GEAR3_OFF)
                              : set_event(EV_GEAR3_ON);
  (g_cur_sense_gpio_b & S_G4) ? set_event(EV_GEAR4_OFF)
                              : set_event(EV_GEAR4_ON);
  (g_cur_sense_gpio_b & S_BRAKE) ? set_event(EV_BRAKE_OFF)
                                 : set_event(EV_BRAKE_ON);
  (g_cur_sense_gpio_b & S_LI) ? set_event(EV_LI_OFF) : set_event(EV_LI_ON);
  (g_cur_sense_gpio_b & S_RI) ? set_event(EV_RI_OFF) : set_event(EV_RI_ON);
  (g_cur_sense_gpio_b & S_CLAXON) ? set_event(EV_CLAXON_OFF)
                                  : set_event(EV_CLAXON_ON);
}

void process_senses() {
  // if PB1 == HIGH (incoming sense change interrupt)
  // read both INT capture ports
  // start 5ms timer
  // read both GPIO ports 5 times in a row with 5ms delta's
  // compare them to the INT capture ports
  if (gpio_get(GPIOB, GPIO1)) {
    g_senses_debounce_pending = 1;
    g_senses_checks = SENSE_DEBOUNCE_CHECKS;
    g_cur_sense_capture_a = read_sense_capture_a();
    g_cur_sense_capture_b = read_sense_capture_b();
  }
  if (g_senses_debounce_pending) {
    if (g_senses_checks) {                        // debouncing checks
      if (!get_1ms_tick_slot(SENSES_1MS_SLOT)) {  // 5ms passed
        g_cur_sense_gpio_a = read_sense_port_a();
        g_cur_sense_gpio_b = read_sense_port_b();
        // current GPIO not equal anymore -> noise bounce
        // bailout with no senses events dispatched
        if ((g_cur_sense_gpio_a != g_cur_sense_capture_a) ||
            (g_cur_sense_gpio_b != g_cur_sense_capture_b)) {
          g_senses_debounce_pending = 0;
        } else {
          g_senses_checks--;
          if (g_senses_checks) {
            set_1ms_tick_slot(SENSES_1MS_SLOT, 5);
          }
        }
      }
    } else {  // debouncing checks done, dispatch events
      g_senses_debounce_pending = 0;
      dispatch_senses();
    }
  }
  // check for neutral, which doesn't have its own sense wire, but should be
  // activated when the gear1-4 substates are all off.
  check_neutral();
}

void check_neutral() {
  if (get_neutral_pending()) {
    if (!get_100ms_tick_slot(NEUTRAL_100MS_SLOT)) {
      // only set neutral, when no other gears are on
      if (!(check_substate(ST_GEAR1) || check_substate(ST_GEAR2) ||
            check_substate(ST_GEAR3) || check_substate(ST_GEAR4))) {
        set_event(EV_NEUTRAL_ON);
      }
      set_neutral_pending(0);
    }
  }
}

void process_relays() {}

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
