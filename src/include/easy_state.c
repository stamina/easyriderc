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
#include "easy_state.h"

#include "easy_cmd.h"
#include "easy_debug.h"
#include "stdbool.h"
#include "stdio.h"

#define EVENT_SIZE 256

#define TR_CNT            \
  (sizeof(g_transition) / \
   sizeof(*g_transition))  // number of transition table entries

// neutral pending flag, in case no gear1-4 is selected
uint8_t g_neutral_pending;
// on/off flag so the buzzer knows when to beep or not
uint8_t g_indicator_toggle;

// proto's
uint8_t event_rdy(void);
uint8_t get_event(void);
uint8_t check_superstate_mask(uint8_t);
uint8_t check_superstate_neithermask(uint8_t);
uint8_t check_superstate_eithermask(uint8_t);
uint32_t check_substate_mask(uint32_t);
uint32_t check_substate_neithermask(uint32_t);
uint32_t check_substate_eithermask(uint32_t);

// current states
typedef struct {
  uint8_t superstate;
  uint32_t substate;
  uint8_t cur_event;
} t_state_info;

// event handling
typedef struct {
  uint8_t event;                   // event id
  uint8_t superstate_mask;         // check for a single superstate to be active
  uint8_t superstate_neithermask;  // none of these superstates can be active
  uint8_t
      superstate_eithermask;  // either one of these superstates must be active
  uint32_t substate_mask;     // check for a single substate to be active
  uint32_t substate_neithermask;  // none of these substates can be active
  uint32_t substate_eithermask;  // either one of these substates must be active
  uint8_t (*check_mask)(void);  // optional custom guard function if none of the
                                // other guards suffice
  void (*event_handler)(void);  // cmd to handle the event
} t_transition;

const t_transition g_transition[] = {
    {EV_RESET, 0, 0, (ST_IDLE | ST_ALARM | ST_ACTIVE), 0, 0, 0, NULL,
     cmd_reset},

    {EV_RI_ON, ST_ACTIVE, 0, 0, 0, (ST_RI | ST_WARNING), 0, NULL, cmd_ri_on},
    {EV_RI_OFF, ST_ACTIVE, 0, 0, ST_RI, 0, 0, NULL, cmd_ri_off},
    {EV_RI_TOGGLE, ST_ACTIVE, 0, 0, ST_RI, 0, 0, NULL, cmd_ri_toggle},

    {EV_LI_ON, ST_ACTIVE, 0, 0, 0, (ST_LI | ST_WARNING), 0, NULL, cmd_li_on},
    {EV_LI_OFF, ST_ACTIVE, 0, 0, ST_LI, 0, 0, NULL, cmd_li_off},
    {EV_LI_TOGGLE, ST_ACTIVE, 0, 0, ST_LI, 0, 0, NULL, cmd_li_toggle},

    {EV_HAZARD_ON, ST_ACTIVE, 0, 0, 0, (ST_HAZARD), 0, NULL, cmd_hazard_on},
    {EV_HAZARD_OFF, ST_ACTIVE, 0, 0, ST_HAZARD, 0, 0, NULL, cmd_hazard_off},
    {EV_HAZARD_TOGGLE, ST_ACTIVE, 0, 0, ST_HAZARD, 0, 0, NULL,
     cmd_hazard_toggle},

    {EV_CLAXON_ON, ST_ACTIVE, 0, 0, 0, (ST_CLAXON), 0, NULL, cmd_claxon_on},
    {EV_CLAXON_OFF, ST_ACTIVE, 0, 0, ST_CLAXON, 0, 0, NULL, cmd_claxon_off},

    {EV_BRAKE_ON, ST_ACTIVE, 0, 0, 0, (ST_BRAKE), 0, NULL, cmd_brake_on},
    {EV_BRAKE_OFF, ST_ACTIVE, 0, 0, ST_BRAKE, 0, 0, NULL, cmd_brake_off},

    {EV_PILOT_ON, ST_ACTIVE, 0, 0, 0, (ST_PILOT), 0, NULL, cmd_pilot_on},
    {EV_PILOT_OFF, ST_ACTIVE, 0, 0, ST_PILOT, 0, 0, NULL, cmd_pilot_off},

    {EV_LIGHT_ON, ST_ACTIVE, 0, 0, 0, (ST_LIGHT), 0, NULL, cmd_light_on},
    {EV_LIGHT_OFF, ST_ACTIVE, 0, 0, ST_LIGHT, 0, 0, NULL, cmd_light_off},

    {EV_ALARM_SET_ON, ST_ACTIVE, 0, 0, 0, (ST_ALARM_SET), 0, NULL,
     cmd_alarm_set_on},
    {EV_ALARM_SET_OFF, ST_ACTIVE, 0, 0, ST_ALARM_SET, 0, 0, NULL,
     cmd_alarm_set_off},

    {EV_IGN_ON, 0, 0, (ST_IDLE | ST_ALARM), 0, 0, 0, NULL, cmd_ign_on},
    {EV_IGN_OFF, ST_ACTIVE, 0, 0, 0, 0, 0, NULL, cmd_ign_off},

    {EV_WARNING_ON, ST_ACTIVE, 0, 0, 0, (ST_WARNING), 0, NULL, cmd_warning_on},
    {EV_WARNING_OFF, ST_ACTIVE, 0, 0, ST_WARNING, 0, 0, NULL, cmd_warning_off},
    {EV_WARNING_TOGGLE, ST_ACTIVE, 0, 0, ST_WARNING, 0, 0, NULL,
     cmd_warning_toggle},

    {EV_NEUTRAL_ON, ST_ACTIVE, 0, 0, 0, (ST_NEUTRAL), 0, NULL, cmd_neutral_on},
    {EV_NEUTRAL_OFF, ST_ACTIVE, 0, 0, ST_NEUTRAL, 0, 0, NULL, cmd_neutral_off},

    {EV_GEAR1_ON, ST_ACTIVE, 0, 0, 0, (ST_GEAR1), 0, NULL, cmd_g1_on},
    {EV_GEAR1_OFF, ST_ACTIVE, 0, 0, ST_GEAR1, 0, 0, NULL, cmd_g1_off},

    {EV_GEAR2_ON, ST_ACTIVE, 0, 0, 0, (ST_GEAR2), 0, NULL, cmd_g2_on},
    {EV_GEAR2_OFF, ST_ACTIVE, 0, 0, ST_GEAR2, 0, 0, NULL, cmd_g2_off},

    {EV_GEAR3_ON, ST_ACTIVE, 0, 0, 0, (ST_GEAR3), 0, NULL, cmd_g3_on},
    {EV_GEAR3_OFF, ST_ACTIVE, 0, 0, ST_GEAR3, 0, 0, NULL, cmd_g3_off},

    {EV_GEAR4_ON, ST_ACTIVE, 0, 0, 0, (ST_GEAR4), 0, NULL, cmd_g4_on},
    {EV_GEAR4_OFF, ST_ACTIVE, 0, 0, ST_GEAR4, 0, 0, NULL, cmd_g4_off},

    {EV_ALARM_TRIGGER, ST_ALARM, 0, 0, 0, 0, 0, NULL, cmd_alarm_trigger},
};

t_state_info g_state_info = {ST_IDLE, 0, 0};

// rx/tx ring buffers
static uint8_t event_buff[EVENT_SIZE];
static uint8_t event_buff_head;
static uint8_t event_buff_tail;

void init_state(void) {
  g_state_info =
      (t_state_info){.superstate = ST_IDLE, .substate = 0, .cur_event = 0};
}
uint8_t check_superstate_mask(uint8_t mask) {
  return (g_state_info.superstate & mask);
}

uint8_t check_superstate_neithermask(uint8_t mask) {
  return ((g_state_info.superstate & ~(mask)) == g_state_info.superstate);
}

uint8_t check_superstate_eithermask(uint8_t mask) {
  return (g_state_info.superstate & (mask));
}

uint32_t check_substate_mask(uint32_t mask) {
  return (g_state_info.substate & mask);
}
uint32_t check_substate_neithermask(uint32_t mask) {
  return ((g_state_info.substate & ~(mask)) == g_state_info.substate);
}
uint32_t check_substate_eithermask(uint32_t mask) {
  return (g_state_info.substate & (mask));
}

void flush_events() { event_buff_head = event_buff_tail = 0; }

uint8_t event_rdy() {
  if (event_buff_head >= event_buff_tail)
    return event_buff_head - event_buff_tail;
  return EVENT_SIZE + event_buff_head - event_buff_tail;
}

uint8_t get_event() {
  uint8_t ev;
  if (event_rdy()) {
    ev = event_buff[event_buff_tail];
    event_buff_tail = (event_buff_tail + 1) % EVENT_SIZE;
    return ev;
  } else {
    return 0;
  }
}

void set_event(uint8_t ev) {
  event_buff[event_buff_head] = ev;
  event_buff_head = (event_buff_head + 1) % EVENT_SIZE;
  if (event_buff_head ==
      event_buff_tail) {  // full buffer, push the tail one ahead
    event_buff_tail = (event_buff_tail + 1) % EVENT_SIZE;
  }
}

uint8_t check_superstate(uint8_t st) { return (g_state_info.superstate & st); }
uint32_t check_substate(uint32_t st) { return (g_state_info.substate & st); }
void set_superstate(uint8_t st) { g_state_info.superstate = st; }
void set_substate(uint32_t st) { g_state_info.substate |= st; }
void remove_substate(uint32_t st) { g_state_info.substate &= ~st; }

void process_events() {
  bool ev_found;
  // handle the complete event queue in one go
  while ((g_state_info.cur_event = get_event())) {
    ev_found = false;
    for (uint8_t i = 0; i < TR_CNT; i++) {
      if (g_state_info.cur_event == g_transition[i].event) {
        ev_found = true;
#if EASY_DEBUG
        debug_write("Found event!\n");
#endif
        // check all guards, any "0 condition guard result" will break the loop
        // and no event handler will be launched
        if (g_transition[i].superstate_mask) {
          if (!check_superstate_mask(g_transition[i].superstate_mask)) break;
        }
        if (g_transition[i].superstate_neithermask) {
          if (!check_superstate_neithermask(
                  g_transition[i].superstate_neithermask))
            break;
        }
        if (g_transition[i].superstate_eithermask) {
          if (!check_superstate_eithermask(
                  g_transition[i].superstate_eithermask))
            break;
        }
        if (g_transition[i].substate_mask) {
          if (!check_substate_mask(g_transition[i].substate_mask)) break;
        }
        if (g_transition[i].substate_neithermask) {
          if (!check_substate_neithermask(g_transition[i].substate_neithermask))
            break;
        }
        if (g_transition[i].substate_eithermask) {
          if (!check_substate_mask(g_transition[i].substate_eithermask)) break;
        }
        if (g_transition[i].check_mask) {
          if (!g_transition[i].check_mask()) break;
        }
        // at this point, launch the event handler
        g_transition[i].event_handler();
        break;
      }
    }
    if (!ev_found) {
#if EASY_DEBUG
      debug_write("Unknown event found!\n");
#endif
      cm3_assert_not_reached();
    }
  }
}

void set_neutral_pending(uint8_t flag) { g_neutral_pending = flag; }
uint8_t get_neutral_pending() { return g_neutral_pending; }

void set_indicator_toggle(uint8_t flag) { g_indicator_toggle = flag; }
uint8_t get_indicator_toggle() { return g_indicator_toggle; }

