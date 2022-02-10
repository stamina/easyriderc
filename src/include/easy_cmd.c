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
void cmd_reset() {
#if EASY_DEBUG
  debug_write("CMD_RESET\n");
#endif
}

void cmd_ri_on() {
#if EASY_DEBUG
  debug_write("CMD_RI_ON\n");
#endif
  set_substate(ST_RI);
  write_relay(RELAY_PIN_RIF, RELAY_ON);
  write_relay(RELAY_PIN_RIB, RELAY_ON);
  write_relay(RELAY_PIN_PIT_INDICATOR, RELAY_ON);
  set_indicator_toggle(INDICATOR_TOGGLE_ON);
  // TODO: set toggle ticks/buzzer length according settings
  set_buzzer_sound(BUZZ_SOUND_INDICATOR_HALFSECOND);
  set_10ms_tick_slot(INDICATOR_10MS_SLOT, 50);
}

void cmd_ri_off() {
#if EASY_DEBUG
  debug_write("CMD_RI_OFF\n");
#endif
  remove_substate(ST_RI);
  write_relay(RELAY_PIN_RIF, RELAY_OFF);
  write_relay(RELAY_PIN_RIB, RELAY_OFF);
  write_relay(RELAY_PIN_PIT_INDICATOR, RELAY_OFF);
  set_indicator_toggle(INDICATOR_TOGGLE_OFF);
}

void cmd_ri_toggle() {
#if EASY_DEBUG
  debug_write("CMD_RI_TOGGLE\n");
#endif
  write_relay(RELAY_PIN_RIF, RELAY_TOGGLE);
  write_relay(RELAY_PIN_RIB, RELAY_TOGGLE);
  write_relay(RELAY_PIN_PIT_INDICATOR, RELAY_TOGGLE);
  set_10ms_tick_slot(INDICATOR_10MS_SLOT, 50);
  if (get_indicator_toggle() == INDICATOR_TOGGLE_ON) {
    set_indicator_toggle(INDICATOR_TOGGLE_OFF);
  } else {
    set_indicator_toggle(INDICATOR_TOGGLE_ON);
    // TODO: set buzzer length according settings
    set_buzzer_sound(BUZZ_SOUND_INDICATOR_HALFSECOND);
  }
}

void cmd_li_on() {
#if EASY_DEBUG
  debug_write("CMD_LI_ON\n");
#endif
  set_substate(ST_LI);
  write_relay(RELAY_PIN_LIF, RELAY_ON);
  write_relay(RELAY_PIN_LIB, RELAY_ON);
  write_relay(RELAY_PIN_PIT_INDICATOR, RELAY_ON);
  set_indicator_toggle(INDICATOR_TOGGLE_ON);
  // TODO: set toggle ticks/buzzer length according settings
  set_buzzer_sound(BUZZ_SOUND_INDICATOR_HALFSECOND);
  set_10ms_tick_slot(INDICATOR_10MS_SLOT, 50);
}

void cmd_li_off() {
#if EASY_DEBUG
  debug_write("CMD_LI_OFF\n");
#endif
  remove_substate(ST_LI);
  write_relay(RELAY_PIN_LIF, RELAY_OFF);
  write_relay(RELAY_PIN_LIB, RELAY_OFF);
  write_relay(RELAY_PIN_PIT_INDICATOR, RELAY_OFF);
  set_indicator_toggle(INDICATOR_TOGGLE_OFF);
}

void cmd_li_toggle() {
#if EASY_DEBUG
  debug_write("CMD_LI_TOGGLE\n");
#endif
  write_relay(RELAY_PIN_LIF, RELAY_TOGGLE);
  write_relay(RELAY_PIN_LIB, RELAY_TOGGLE);
  write_relay(RELAY_PIN_PIT_INDICATOR, RELAY_TOGGLE);
  set_10ms_tick_slot(INDICATOR_10MS_SLOT, 50);
  if (get_indicator_toggle() == INDICATOR_TOGGLE_ON) {
    set_indicator_toggle(INDICATOR_TOGGLE_OFF);
  } else {
    set_indicator_toggle(INDICATOR_TOGGLE_ON);
    // TODO: set buzzer length according settings
    set_buzzer_sound(BUZZ_SOUND_INDICATOR_HALFSECOND);
  }
}

void cmd_warning_on() {
#if EASY_DEBUG
  debug_write("CMD_WARNING_ON\n");
#endif
  // disable possible right/left indicators
  remove_substate(ST_RI);
  remove_substate(ST_LI);
  set_substate(ST_WARNING);
  write_relay(RELAY_PIN_LIF, RELAY_ON);
  write_relay(RELAY_PIN_LIB, RELAY_ON);
  write_relay(RELAY_PIN_RIF, RELAY_ON);
  write_relay(RELAY_PIN_RIB, RELAY_ON);
  write_relay(RELAY_PIN_PIT_INDICATOR, RELAY_ON);
  set_indicator_toggle(INDICATOR_TOGGLE_ON);
  // TODO: set toggle ticks/buzzer length according settings
  set_buzzer_sound(BUZZ_SOUND_INDICATOR_HALFSECOND);
  set_10ms_tick_slot(INDICATOR_10MS_SLOT, 50);
}

void cmd_warning_off() {
#if EASY_DEBUG
  debug_write("CMD_WARNING_OFF\n");
#endif
  remove_substate(ST_WARNING);
  write_relay(RELAY_PIN_LIF, RELAY_OFF);
  write_relay(RELAY_PIN_LIB, RELAY_OFF);
  write_relay(RELAY_PIN_RIF, RELAY_OFF);
  write_relay(RELAY_PIN_RIB, RELAY_OFF);
  write_relay(RELAY_PIN_PIT_INDICATOR, RELAY_OFF);
  set_indicator_toggle(INDICATOR_TOGGLE_OFF);
}

void cmd_warning_toggle() {
#if EASY_DEBUG
  debug_write("CMD_WARNING_TOGGLE\n");
#endif
  write_relay(RELAY_PIN_LIF, RELAY_TOGGLE);
  write_relay(RELAY_PIN_LIB, RELAY_TOGGLE);
  write_relay(RELAY_PIN_RIF, RELAY_TOGGLE);
  write_relay(RELAY_PIN_RIB, RELAY_TOGGLE);
  write_relay(RELAY_PIN_PIT_INDICATOR, RELAY_TOGGLE);
  set_10ms_tick_slot(INDICATOR_10MS_SLOT, 50);
  if (get_indicator_toggle() == INDICATOR_TOGGLE_ON) {
    set_indicator_toggle(INDICATOR_TOGGLE_OFF);
  } else {
    set_indicator_toggle(INDICATOR_TOGGLE_ON);
    // TODO: set buzzer length according settings
    set_buzzer_sound(BUZZ_SOUND_INDICATOR_HALFSECOND);
  }
}

void cmd_hazard_on() {
#if EASY_DEBUG
  debug_write("CMD_HAZARD_ON\n");
#endif
}
void cmd_hazard_off() {
#if EASY_DEBUG
  debug_write("CMD_HAZARD_OFF\n");
#endif
}
void cmd_hazard_toggle() {
#if EASY_DEBUG
  debug_write("CMD_HAZARD_TOGGLE\n");
#endif
}

void cmd_claxon_on() {
#if EASY_DEBUG
  debug_write("CMD_CLAXON_ON\n");
#endif
  set_substate(ST_CLAXON);
  write_relay(RELAY_PIN_CLAXON, RELAY_ON);
}

void cmd_claxon_off() {
#if EASY_DEBUG
  debug_write("CMD_CLAXON_OFF\n");
#endif
  remove_substate(ST_CLAXON);
  write_relay(RELAY_PIN_CLAXON, RELAY_OFF);
}

void cmd_brake_on() {
#if EASY_DEBUG
  debug_write("CMD_BRAKE_ON\n");
#endif
  set_substate(ST_BRAKE);
  write_relay(RELAY_PIN_BRAKE, RELAY_ON);
}

void cmd_brake_off() {
#if EASY_DEBUG
  debug_write("CMD_BRAKE_OFF\n");
#endif
  remove_substate(ST_BRAKE);
  write_relay(RELAY_PIN_BRAKE, RELAY_OFF);
}

void cmd_pilot_on() {
#if EASY_DEBUG
  debug_write("CMD_PILOT_ON\n");
#endif
  set_substate(ST_PILOT);
  write_relay(RELAY_PIN_PILOT, RELAY_ON);
}

void cmd_pilot_off() {
#if EASY_DEBUG
  debug_write("CMD_PILOT_OFF\n");
#endif
  remove_substate(ST_PILOT);
  write_relay(RELAY_PIN_PILOT, RELAY_OFF);
}

void cmd_light_on() {
#if EASY_DEBUG
  debug_write("CMD_LIGHT_ON\n");
#endif
  set_substate(ST_LIGHT);
  write_relay(RELAY_PIN_LIGHT, RELAY_ON);
}

void cmd_light_off() {
#if EASY_DEBUG
  debug_write("CMD_LIGHT_OFF\n");
#endif
  remove_substate(ST_LIGHT);
  write_relay(RELAY_PIN_LIGHT, RELAY_OFF);
}

void cmd_neutral_on() {
#if EASY_DEBUG
  debug_write("CMD_NEUTRAL_ON\n");
#endif
  set_substate(ST_NEUTRAL);
  write_relay(RELAY_PIN_PIT_STATUS, RELAY_ON);
}

void cmd_neutral_off() {
#if EASY_DEBUG
  debug_write("CMD_NEUTRAL_OFF\n");
#endif
  remove_substate(ST_NEUTRAL);
  write_relay(RELAY_PIN_PIT_STATUS, RELAY_OFF);
}

void cmd_alarm_set_on() {
#if EASY_DEBUG
  debug_write("CMD_ALARM_SET_ON\n");
#endif
}

void cmd_alarm_set_off() {
#if EASY_DEBUG
  debug_write("CMD_ALARM_SET_OFF\n");
#endif
}

void cmd_alarm_trigger() {
#if EASY_DEBUG
  debug_write("CMD_ALARM_TRIGGER\n");
#endif
}

void cmd_ign_on() {
#if EASY_DEBUG
  debug_write("CMD_IGN_ON\n");
#endif
  // TODO: init some stuff
  // all relays on, play on sound
  // set ST_ACTIVE_SETTLE substate
  // cast EV_ACTIVE_SETTLE_ON
  // set tickslot timer for settle to countdown
  set_superstate(ST_ACTIVE);
  set_buzzer_sound(BUZZ_SOUND_POPCORN);
  set_buzzer_sound(BUZZ_SOUND_ALARM);
  set_buzzer_sound(BUZZ_SOUND_FROGGER);
  set_buzzer_sound(BUZZ_SOUND_FURELISE);
  set_buzzer_sound(BUZZ_SOUND_LARRY);
  set_buzzer_sound(BUZZ_SOUND_PIPI);
  set_buzzer_sound(BUZZ_SOUND_PRODIGY);
  set_buzzer_sound(BUZZ_SOUND_INDICATOR_HALFSECOND);
  set_buzzer_sound(BUZZ_SOUND_INDICATOR_QUARTERSECOND);
}

void cmd_ign_off() {
#if EASY_DEBUG
  debug_write("CMD_IGN_OFF\n");
#endif
  set_superstate(ST_IDLE);
  // TODO: re-init some stuff
  // all relays off, play off sound
  // TODO: check if ST_ALARM_SET is active,
  // then go to ALARM mode and settle instead
}

void cmd_g1_on() {
#if EASY_DEBUG
  debug_write("CMD_G1_ON\n");
#endif
  set_event(EV_NEUTRAL_OFF);
  set_substate(ST_GEAR1);
}

void cmd_g1_off() {
#if EASY_DEBUG
  debug_write("CMD_G1_OFF\n");
#endif
  remove_substate(ST_GEAR1);
  // start neutral check, in case no other gear goes on
  set_neutral_pending(1);
  set_100ms_tick_slot(NEUTRAL_100MS_SLOT, 10);  // check after 1sec
}

void cmd_g2_on() {
#if EASY_DEBUG
  debug_write("CMD_G2_ON\n");
#endif
  set_event(EV_NEUTRAL_OFF);
  set_substate(ST_GEAR2);
}

void cmd_g2_off() {
#if EASY_DEBUG
  debug_write("CMD_G2_OFF\n");
#endif
  remove_substate(ST_GEAR2);
  // start neutral check, in case no other gear goes on
  set_neutral_pending(1);
  set_100ms_tick_slot(NEUTRAL_100MS_SLOT, 10);  // check after 1sec
}

void cmd_g3_on() {
#if EASY_DEBUG
  debug_write("CMD_G3_ON\n");
#endif
  set_event(EV_NEUTRAL_OFF);
  set_substate(ST_GEAR3);
}

void cmd_g3_off() {
#if EASY_DEBUG
  debug_write("CMD_G3_OFF\n");
#endif
  remove_substate(ST_GEAR3);
  // start neutral check, in case no other gear goes on
  set_neutral_pending(1);
  set_100ms_tick_slot(NEUTRAL_100MS_SLOT, 10);  // check after 1sec
}

void cmd_g4_on() {
#if EASY_DEBUG
  debug_write("CMD_G4_ON\n");
#endif
  set_event(EV_NEUTRAL_OFF);
  set_substate(ST_GEAR1);
}

void cmd_g4_off() {
#if EASY_DEBUG
  debug_write("CMD_G4_OFF\n");
#endif
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
  if (check_substate(ST_LI)) {
    if (!get_10ms_tick_slot(INDICATOR_10MS_SLOT)) {
      set_event(EV_LI_TOGGLE);
    }
  }
  if (check_substate(ST_WARNING)) {
    if (!get_10ms_tick_slot(INDICATOR_10MS_SLOT)) {
      set_event(EV_WARNING_TOGGLE);
    }
  }
}

void process_cmds() {
  process_senses();
  process_toggles();
  process_buzzer();
  // TODO: only check status when ST_ACTIVE
  process_status_leds();
  // 5t/s ADC readouts
  if (!get_100ms_tick_slot(ADC_100MS_SLOT)) {
    start_adc_conversion();
#if EASY_DEBUG_OFF
    char buf[255];
    snprintf(buf, 255, "accu/current/bat: %d %d %d\n", get_adc_vaccu(),
             get_adc_acurrent(), get_adc_vbat());
    debug_write(buf);
#endif
    // repeat conversion every 200ms
    set_100ms_tick_slot(ADC_100MS_SLOT, 2);
  }
}

