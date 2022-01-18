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
#include "easy_cmd.h"

#include "easy_adc.h"
#include "easy_buzz.h"
#include "easy_clock.h"
#include "easy_debug.h"
#include "easy_gpio.h"
#include "easy_state.h"
#include "stdio.h"
#include "string.h"

// proto's
void process_status_leds(void);

void cmd_ign_on() {
#if EASY_DEBUG
  debug_write("cmd_ign_on\n");
#endif
  set_superstate(ST_ACTIVE);
  set_buzzer_sound(BUZZ_SOUND_POPCORN);
  set_buzzer_sound(BUZZ_SOUND_ALARM);
  set_buzzer_sound(BUZZ_SOUND_FROGGER);
  set_buzzer_sound(BUZZ_SOUND_FURELISE);
  set_buzzer_sound(BUZZ_SOUND_LARRY);
  set_buzzer_sound(BUZZ_SOUND_PIPI);
  /*set_buzzer_sound(BUZZ_SOUND_INDICATOR_HALFSECOND);*/
  /*set_buzzer_sound(BUZZ_SOUND_INDICATOR_QUARTERSECOND);*/
}

void cmd_ign_off() {
#if EASY_DEBUG
  debug_write("cmd_ign_off\n");
#endif
  set_superstate(ST_IDLE);
  // TODO: re-init some stuff
  // like clearing all systicks
  // all relays off, play off sound
  // TODO: check if ST_ALARM_SET is active,
  // then go to ALARM mode and settle instead
}

void process_status_leds() {
  set_status_led1(get_1s_ticker());
  set_status_led2(get_1s_ticker());
}

void process_cmds() {
  process_buzzer();
  // TODO: status leds logic here...
  process_status_leds();

  // 5t/s ADC readouts
  if (!get_100ms_tick_slot(ADC_100MS_SLOT)) {
    start_adc_conversion();
#if EASY_DEBUG
    char buf[255];
    snprintf(buf, 255, "accu/current/bat: %d %d %d\n", get_adc_vaccu(),
             get_adc_acurrent(), get_adc_vbat());
    debug_write(buf);
#endif
    // repeat conversion every 200ms
    set_100ms_tick_slot(ADC_100MS_SLOT, 2);
  }
}

