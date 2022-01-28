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
#include "easy_spi.h"
#include "easy_state.h"
#include "stdio.h"
#include "string.h"

// proto's
void process_status_leds(void);
void process_toggles(void);

// TODO: watchdog run out?
void cmd_reset() {}

void cmd_ri_on() {
  set_substate(ST_RI);
  write_relay(RELAY_PIN_RIF, RELAY_ON);
  write_relay(RELAY_PIN_RIB, RELAY_ON);
  set_indicator_toggle(INDICATOR_TOGGLE_ON);
  // TODO: set toggle ticks (0.5/0.25s according settings)
  // set buzzer beep according settings
  set_10ms_tick_slot(INDICATOR_10MS_SLOT, 50);
}

void cmd_ri_off() {
  remove_substate(ST_RI);
  write_relay(RELAY_PIN_RIF, RELAY_OFF);
  write_relay(RELAY_PIN_RIB, RELAY_OFF);
  set_indicator_toggle(INDICATOR_TOGGLE_OFF);
}

void cmd_ri_toggle() {
  write_relay(RELAY_PIN_RIF, RELAY_TOGGLE);
  write_relay(RELAY_PIN_RIB, RELAY_TOGGLE);
  set_10ms_tick_slot(INDICATOR_10MS_SLOT, 50);
  if (get_indicator_toggle() == INDICATOR_TOGGLE_ON) {
    // TODO: buzzer beep
  }
}

void cmd_li_on() {
  set_substate(ST_LI);
  write_relay(RELAY_PIN_LIF, RELAY_ON);
  write_relay(RELAY_PIN_LIB, RELAY_ON);
  set_indicator_toggle(INDICATOR_TOGGLE_ON);
  // TODO: set toggle ticks (0.5/0.25s according settings)
  // set buzzer beep according settings
}

void cmd_li_off() {
  remove_substate(ST_LI);
  write_relay(RELAY_PIN_LIF, RELAY_OFF);
  write_relay(RELAY_PIN_LIB, RELAY_OFF);
  set_indicator_toggle(INDICATOR_TOGGLE_OFF);
}

void cmd_li_toggle() {
  write_relay(RELAY_PIN_LIF, RELAY_TOGGLE);
  write_relay(RELAY_PIN_LIB, RELAY_TOGGLE);
  set_10ms_tick_slot(INDICATOR_10MS_SLOT, 50);
  if (get_indicator_toggle() == INDICATOR_TOGGLE_ON) {
    // TODO: buzzer beep
  }
}

void cmd_warning_on() {}
void cmd_warning_off() {}
void cmd_warning_toggle() {}

void cmd_hazard_on() {}
void cmd_hazard_off() {}
void cmd_hazard_toggle() {}

void cmd_claxon_on() {}

void cmd_claxon_off() {}

void cmd_brake_on() {}

void cmd_brake_off() {}

void cmd_pilot_on() {}

void cmd_pilot_off() {}

void cmd_light_on() {}

void cmd_light_off() {}

void cmd_alarm_set_on() {}
void cmd_alarm_set_off() {}
void cmd_neutral_on() {}
void cmd_neutral_off() {}
void cmd_alarm_trigger() {}

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
  set_buzzer_sound(BUZZ_SOUND_INDICATOR_HALFSECOND);
  set_buzzer_sound(BUZZ_SOUND_INDICATOR_QUARTERSECOND);
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

void cmd_g1_on() {
  set_event(EV_NEUTRAL_OFF);
  set_substate(ST_GEAR1);
}

void cmd_g1_off() {
  remove_substate(ST_GEAR1);
  // start neutral check, in case no other gear goes on
  set_neutral_pending(1);
  set_100ms_tick_slot(NEUTRAL_100MS_SLOT, 10);  // check after 1sec
}

void cmd_g2_on() {
  set_event(EV_NEUTRAL_OFF);
  set_substate(ST_GEAR2);
}

void cmd_g2_off() {
  remove_substate(ST_GEAR2);
  // start neutral check, in case no other gear goes on
  set_neutral_pending(1);
  set_100ms_tick_slot(NEUTRAL_100MS_SLOT, 10);  // check after 1sec
}
void cmd_g3_on() {
  set_event(EV_NEUTRAL_OFF);
  set_substate(ST_GEAR3);
}

void cmd_g3_off() {
  remove_substate(ST_GEAR3);
  // start neutral check, in case no other gear goes on
  set_neutral_pending(1);
  set_100ms_tick_slot(NEUTRAL_100MS_SLOT, 10);  // check after 1sec
}
void cmd_g4_on() {
  set_event(EV_NEUTRAL_OFF);
  set_substate(ST_GEAR1);
}

void cmd_g4_off() {
  remove_substate(ST_GEAR4);
  // start neutral check, in case no other gear goes on
  set_neutral_pending(1);
  set_100ms_tick_slot(NEUTRAL_100MS_SLOT, 10);  // check after 1sec
}

void process_status_leds() {
  // TODO: move to gpio, replace parameter with error bitmask byte
  set_status_led1(get_1s_ticker());
  set_status_led2(get_1s_ticker());
}

void process_toggles() {
  if (check_substate(ST_RI)) {
    if (!get_10ms_tick_slot(INDICATOR_10MS_SLOT)) {
      set_event(EV_RI_TOGGLE);
    }
  }
}

void process_cmds() {
  process_senses();
  process_buzzer();
  process_status_leds();
  process_toggles();
  // 5t/s ADC readouts
  if (!get_100ms_tick_slot(ADC_100MS_SLOT)) {
    start_adc_conversion();
#if EASY_DEBUG
    char buf[255];
    char buf2[255];
    snprintf(buf, 255, "accu/current/bat: %d %d %d\n", get_adc_vaccu(),
             get_adc_acurrent(), get_adc_vbat());
    debug_write(buf);
    snprintf(buf2, 255, "SENSE PORT A: %d\n", read_sense_port_a());
    debug_write(buf2);
#endif
    // repeat conversion every 200ms
    set_100ms_tick_slot(ADC_100MS_SLOT, 2);
  }
}

