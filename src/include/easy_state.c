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

t_transition g_transition[] = {
    {EV_IGN_ON, 0, 0, (ST_IDLE | ST_ALARM), 0, 0, 0, NULL, cmd_ign_on},
    {EV_IGN_OFF, 0, 0, (ST_ACTIVE), 0, 0, 0, NULL, cmd_ign_off},
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
        // check all guards
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

