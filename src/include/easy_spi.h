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
#ifndef EASY_SPI_H_INCLUDED
#define EASY_SPI_H_INCLUDED

#include <libopencm3/cm3/assert.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/timer.h>

#define RELAY_ON 1
#define RELAY_TOGGLE 2
#define RELAY_OFF 0

// porta
#define RELAY_PIN_XTRA3 1
#define RELAY_PIN_PILOT 2
#define RELAY_PIN_XTRA2 3
#define RELAY_PIN_XTRA1 4
#define RELAY_PIN_PIT_STATUS 5
#define RELAY_PIN_PIT_INDICATOR 6
#define RELAY_PIN_BRAKE 7
// portb
#define RELAY_PIN_RIF 8
#define RELAY_PIN_RIB 9
#define RELAY_PIN_LIF 10
#define RELAY_PIN_LIB 11
#define RELAY_PIN_LIGHT 12
#define RELAY_PIN_CLAXON 13
#define RELAY_PIN_XTRA4 14
#define RELAY_PIN_XTRA5 15

void init_spi(void);
uint8_t spi_status_check(void);
uint8_t read_sense_port_a(void);
uint8_t read_sense_port_b(void);
uint8_t read_relay_port_a(void);
uint8_t read_relay_port_b(void);
void write_relay(uint8_t pin, uint8_t value);
uint8_t read_sense_capture_a(void);
uint8_t read_sense_capture_b(void);

#endif
