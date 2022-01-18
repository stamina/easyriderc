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
#ifndef EASY_ADC_H_INCLUDED
#define EASY_ADC_H_INCLUDED

#include <libopencm3/cm3/assert.h>

void init_adc(void);
void init_dma2(void);
void start_adc_conversion(void);
uint16_t get_adc_vaccu(void);
uint16_t get_adc_acurrent(void);
uint16_t get_adc_vbat(void);

#endif
