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
#ifndef EASY_CMD_H_INCLUDED
#define EASY_CMD_H_INCLUDED

#include <libopencm3/cm3/assert.h>

void cmd_reset(void);
void cmd_alarm_trigger(void);
void cmd_ri_on(void);
void cmd_ri_off(void);
void cmd_ri_toggle(void);
void cmd_li_on(void);
void cmd_li_off(void);
void cmd_li_toggle(void);
void cmd_hazard_on(void);
void cmd_hazard_off(void);
void cmd_hazard_toggle(void);
void cmd_alarm_set_on(void);
void cmd_alarm_set_off(void);
void cmd_warning_on(void);
void cmd_warning_off(void);
void cmd_warning_toggle(void);
void cmd_light_on(void);
void cmd_light_off(void);
void cmd_pilot_on(void);
void cmd_pilot_off(void);
void cmd_brake_on(void);
void cmd_brake_off(void);
void cmd_claxon_on(void);
void cmd_claxon_off(void);
void cmd_ign_on(void);
void cmd_ign_off(void);
void cmd_g1_on(void);
void cmd_g1_off(void);
void cmd_g2_on(void);
void cmd_g2_off(void);
void cmd_g3_on(void);
void cmd_g3_off(void);
void cmd_g4_on(void);
void cmd_g4_off(void);
void cmd_neutral_on(void);
void cmd_neutral_off(void);

void process_cmds(void);

#endif
