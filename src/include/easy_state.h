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
#ifndef EASY_STATE_H_INCLUDED
#define EASY_STATE_H_INCLUDED

#include <libopencm3/cm3/assert.h>

// super states
// NOTE: super states are mutually exclusive
#define ST_IDLE 1    // default state after initial power on
#define ST_ACTIVE 2  // ignition turned on
#define ST_ALARM 4   // alarm mode turned on

// sub states
// NOTE: sub states are not mutually exclusive: they can be combined
#define ST_RI 1           // right indicator on
#define ST_LI 2           // left indicator on
#define ST_CLAXON 4       // claxon on
#define ST_BRAKE 8        // brake on
#define ST_LIGHT 16       // main light on
#define ST_WARNING 32     // warning lights on (4 indicators)
#define ST_PILOT 64       // pilot front light on
#define ST_ALARM_SET 128  // alarm switch toggled on
#define ST_ALARM_SETTLE \
  256                   // alarm preparation -> store IMU values/sound trigger
#define ST_NEUTRAL 512  // in neutral, no gear selected
#define ST_GEAR1 1024   // in gear 1
#define ST_GEAR2 2048   // in gear 2
#define ST_GEAR3 4096   // in gear 3
#define ST_GEAR4 8192   // in gear 4
#define ST_HAZARD \
  16384  // in hazard mode (sudden IMU changes: breaking hard/crash)
#define ST_DBG 32768              // in debug mode, listens to USART cmds
#define ST_BLE 65536              // in Bluetooth mode
#define ST_GPS 131072             // in GPS mode
#define ST_IMU 262144             // in IMU mode
#define ST_FLASH 524288           // in Flash logging mode
#define ST_ACTIVE_SETTLE 1048576  // startup preparation -> sound/light triggers
#define ST_BUZZ 2097152           // buzzer sound data available

// all possible events that can be fired
#define EV_RESET 1
#define EV_RI_ON 2
#define EV_LI_ON 3
#define EV_RI_OFF 4
#define EV_LI_OFF 5
#define EV_CLAXON_ON 6
#define EV_CLAXON_OFF 7
#define EV_BRAKE_ON 8
#define EV_BRAKE_OFF 9
#define EV_PILOT_ON 10
#define EV_PILOT_OFF 11
#define EV_LIGHT_ON 12
#define EV_LIGHT_OFF 13
#define EV_ALARM_SET_ON 14
#define EV_ALARM_SET_OFF 15
#define EV_IGN_ON 16
#define EV_IGN_OFF 17
#define EV_WARNING_ON 18
#define EV_WARNING_OFF 19
#define EV_NEUTRAL_ON 20
#define EV_NEUTRAL_OFF 21
#define EV_GEAR1_ON 22
#define EV_GEAR1_OFF 23
#define EV_GEAR2_ON 24
#define EV_GEAR2_OFF 25
#define EV_GEAR3_ON 26
#define EV_GEAR3_OFF 27
#define EV_GEAR4_ON 28
#define EV_GEAR4_OFF 29
#define EV_WARNING_TOGGLE 30
#define EV_RI_TOGGLE 31
#define EV_LI_TOGGLE 32
#define EV_HAZARD_ON 33
#define EV_HAZARD_OFF 34
#define EV_HAZARD_TOGGLE 35
#define EV_ALARM_TRIGGER 36
#define EV_ACTIVE_SETTLE_ON 37
#define EV_ACTIVE_SETTLE_OFF 38

// led statuses
#define LED1_ST_CONT_ON 1
#define LED1_ST_CONT_OFF 2
#define LED1_ST_CONT_FAST 3
#define LED1_ST_CONT_SLOW 4
#define LED1_ST_INTERVAL_2_FAST 5
#define LED1_ST_INTERVAL_2_SLOW 6
#define LED1_ST_INTERVAL_3_FAST 7
#define LED1_ST_INTERVAL_3_SLOW 8
#define LED2_ST_CONT_ON 9
#define LED2_ST_CONT_OFF 10
#define LED2_ST_CONT_FAST 11
#define LED2_ST_CONT_SLOW 12
#define LED2_ST_INTERVAL_2_FAST 13
#define LED2_ST_INTERVAL_2_SLOW 14
#define LED2_ST_INTERVAL_3_FAST 15
#define LED2_ST_INTERVAL_3_SLOW 16

#define INDICATOR_TOGGLE_ON 1
#define INDICATOR_TOGGLE_OFF 0

void init_state(void);
void process_events(void);
void set_event(uint8_t ev);
void flush_events(void);
void set_superstate(uint8_t st);
void set_substate(uint32_t st);
uint8_t check_superstate(uint8_t st);
uint32_t check_substate(uint32_t st);
void remove_substate(uint32_t st);
void set_neutral_pending(uint8_t flag);
uint8_t get_neutral_pending(void);
void set_indicator_toggle(uint8_t flag);
uint8_t get_indicator_toggle(void);

#endif
