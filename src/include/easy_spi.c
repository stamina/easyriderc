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
#include "easy_spi.h"

#include "easy_clock.h"
#include "easy_debug.h"
#include "easy_state.h"
#include "stdio.h"

// MCP23S17 start opcode format: [0 1 0 0 A2 A1 A0 R/W]
#define SENSE_READ_CONTROL 0b01000001
#define SENSE_WRITE_CONTROL 0b01000000
#define RELAY_READ_CONTROL 0b01000011
#define RELAY_WRITE_CONTROL 0b01000010

#define MCP_DUMMY_SEND 0xFF

typedef enum {
  MCP_IODIRA,
  MCP_IODIRB,
  MCP_IPOLA,
  MCP_IPOLB,
  MCP_GPINTENA,
  MCP_GPINTENB,
  MCP_DEFVALA,
  MCP_DEFVALB,
  MCP_INTCONA,
  MCP_INTCONB,
  MCP_IOCONA,
  MCP_IOCONB,
  MCP_GPPUA,
  MCP_GPPUB,
  MCP_INTFA,
  MCP_INTFB,
  MCP_INTCAPA,
  MCP_INTCAPB,
  MCP_GPIOA,
  MCP_GPIOB,
  MCP_OLATA,
  MCP_OLATB
} t_mcp_registers;

uint8_t mcp_internal_reg[22];

void setup_mcp_hardware_addr(void);
void setup_senses(void);
void setup_relays(void);
uint8_t read_sense(uint8_t reg);
uint8_t read_relay(uint8_t reg);

// NOTE: we need to use the spi_xfer() everytime, instead of spi_write/read
// since SPI shifts a byte in, everytime we shift a byte out -> no
// over/underflow errors.
void init_spi() {
  // SPI1
  // MCP23S17 I/O expanders, first byte is the control byte:
  // format: [0 1 0 0 A2 A1 A0 R/W]
  // A is 3bit address
  // W = 0, R = 1
  // SPI1 -> SS (PA4), CLK (PA5), MISO (PA6), MOSI (PA7)
  // 2 INTERRUPTS -> INT_SENSES (PB1), INT_RELAYS (PB15)
  rcc_periph_clock_enable(RCC_SPI1);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5 | GPIO6 | GPIO7);
  gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO6 | GPIO7);
  // Manual SS pin
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4);
  // SS is active low, so pull it high for now
  gpio_set(GPIOA, GPIO4);
  spi_reset(SPI1);
  // 90Mhz/DIV16 = 5.624 Mhz baud rate, sample 1 byte on rising edge of high
  // clock
  spi_init_master(
      SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_16, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
      SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);
  spi_set_full_duplex_mode(SPI1);
  // Set NSS management to software.
  // NOTE:
  // Setting nss high is very important, even if we are controlling the GPIO
  // ourselves this bit needs to be at least set to 1, otherwise the spi
  // peripheral will not send any data out.
  spi_enable_software_slave_management(SPI1);
  spi_set_nss_high(SPI1);
  spi_enable(SPI1);
  setup_mcp_hardware_addr();
  setup_senses();
  setup_relays();
}

void setup_mcp_hardware_addr() {
  // ss low
  gpio_clear(GPIOA, GPIO4);
  // initially send these bytes to both IO expanders
  // to set hardware address mode
  spi_xfer(SPI1, SENSE_WRITE_CONTROL);
  spi_xfer(SPI1, MCP_IOCONA);
  spi_xfer(SPI1, 0b01001010);
  // ss high
  gpio_set(GPIOA, GPIO4);
}

// IO expander with externally biased address: 0 0 0
void setup_senses() {
  // all 16 IO's to input
  mcp_internal_reg[MCP_IODIRA] = 0xFF;
  mcp_internal_reg[MCP_IODIRB] = 0xFF;
  // no inverted pin value on GPIO registers
  mcp_internal_reg[MCP_IPOLA] = 0x00;
  mcp_internal_reg[MCP_IPOLB] = 0x00;
  // enable all for interrupt on change
  mcp_internal_reg[MCP_GPINTENA] = 0xFF;
  mcp_internal_reg[MCP_GPINTENB] = 0xFF;
  // no interrupt compare value registers
  mcp_internal_reg[MCP_DEFVALA] = 0x00;
  mcp_internal_reg[MCP_DEFVALB] = 0x00;
  // pins are compared to previous values
  mcp_internal_reg[MCP_INTCONA] = 0x00;
  mcp_internal_reg[MCP_INTCONB] = 0x00;
  // BANK=0: paired registers
  // MIRROR=1: int pins are connected
  // Sequential reads and Hardware addresses used
  // interrupt signal active-high
  // NOTE: there is only 1 IOCON config register,
  // both A/B addresses point to the same
  mcp_internal_reg[MCP_IOCONA] = 0b01001010;
  mcp_internal_reg[MCP_IOCONB] = 0b01001010;
  // 100k ohm weak pullups on all pins
  mcp_internal_reg[MCP_GPPUA] = 0xFF;
  mcp_internal_reg[MCP_GPPUB] = 0xFF;
  // interrupt pin flags, READONLY registers, writes are ignored
  mcp_internal_reg[MCP_INTFA] = 0x00;
  mcp_internal_reg[MCP_INTFB] = 0x00;
  // interrupt saved pin state capture registers, READONLY registers, writes are
  // ignored
  mcp_internal_reg[MCP_INTCAPA] = 0x00;
  mcp_internal_reg[MCP_INTCAPB] = 0x00;
  // writing to these GPIO pin states, modifies the Output Latch registers: not
  // needed
  mcp_internal_reg[MCP_GPIOA] = 0x00;
  mcp_internal_reg[MCP_GPIOB] = 0x00;
  // modifies output pins latches logic-high/low: not needed
  mcp_internal_reg[MCP_OLATA] = 0x00;
  mcp_internal_reg[MCP_OLATB] = 0x00;
  // bulk write all these 22 registers for our initial setup
  // ss low
  gpio_clear(GPIOA, GPIO4);
  spi_xfer(SPI1, SENSE_WRITE_CONTROL);
  spi_xfer(SPI1, MCP_IODIRA);  // first register address
  // chip auto-increments the address pointer by 1
  for (uint8_t i = 0; i < 22; i++) {
    spi_xfer(SPI1, mcp_internal_reg[i]);
  }
  // ss high
  gpio_set(GPIOA, GPIO4);
}

// IO expander with externally biased address: 0 0 1
void setup_relays() {
  // all 16 IO's to output
  mcp_internal_reg[MCP_IODIRA] = 0x00;
  mcp_internal_reg[MCP_IODIRB] = 0x00;
  // no inverted pin value on GPIO registers
  mcp_internal_reg[MCP_IPOLA] = 0x00;
  mcp_internal_reg[MCP_IPOLB] = 0x00;
  // no interrupts, since we're only using outputs
  mcp_internal_reg[MCP_GPINTENA] = 0x00;
  mcp_internal_reg[MCP_GPINTENB] = 0x00;
  // no interrupt compare value registers
  mcp_internal_reg[MCP_DEFVALA] = 0x00;
  mcp_internal_reg[MCP_DEFVALB] = 0x00;
  // pins are compared to previous values
  mcp_internal_reg[MCP_INTCONA] = 0x00;
  mcp_internal_reg[MCP_INTCONB] = 0x00;
  // BANK=0: paired registers
  // MIRROR=1: int pins are connected
  // Sequential reads and Hardware addresses used
  // interrupt signal active-high
  // NOTE: there is only 1 IOCON config register,
  // both A/B addresses point to the same
  mcp_internal_reg[MCP_IOCONA] = 0b01001010;
  mcp_internal_reg[MCP_IOCONB] = 0b01001010;
  // no pullups
  mcp_internal_reg[MCP_GPPUA] = 0x00;
  mcp_internal_reg[MCP_GPPUB] = 0x00;
  // interrupt pin flags, READONLY registers, writes are ignored
  mcp_internal_reg[MCP_INTFA] = 0x00;
  mcp_internal_reg[MCP_INTFB] = 0x00;
  // interrupt saved pin state capture registers, READONLY registers, writes are
  // ignored
  mcp_internal_reg[MCP_INTCAPA] = 0x00;
  mcp_internal_reg[MCP_INTCAPB] = 0x00;
  // writing to these GPIO pin states, modifies the Output Latch registers
  mcp_internal_reg[MCP_GPIOA] = 0x00;
  mcp_internal_reg[MCP_GPIOB] = 0x00;
  // modifies output pins latches logic-high/low: set to low as default
  mcp_internal_reg[MCP_OLATA] = 0x00;
  mcp_internal_reg[MCP_OLATB] = 0x00;
  // bulk write all these 22 registers for our initial setup
  // ss low
  gpio_clear(GPIOA, GPIO4);
  spi_xfer(SPI1, RELAY_WRITE_CONTROL);
  spi_xfer(SPI1, MCP_IODIRA);  // first register address
  // chip auto-increments the address pointer by 1
  for (uint8_t i = 0; i < 22; i++) {
    spi_xfer(SPI1, mcp_internal_reg[i]);
  }
  // ss high
  gpio_set(GPIOA, GPIO4);
}

// read the main config register back as an indication
// of a working SPI for the status leds
uint8_t spi_status_check() { return (read_sense(MCP_IOCONA) == 0b01001010); }

uint8_t read_sense(uint8_t reg) {
  uint8_t bt;
  // ss low
  gpio_clear(GPIOA, GPIO4);
  spi_xfer(SPI1, SENSE_READ_CONTROL);
  spi_xfer(SPI1, reg);
  bt = spi_xfer(SPI1, MCP_DUMMY_SEND);
  // ss high
  gpio_set(GPIOA, GPIO4);
  return bt;
}

uint8_t read_relay(uint8_t reg) {
  uint8_t bt;
  // ss low
  gpio_clear(GPIOA, GPIO4);
  spi_xfer(SPI1, RELAY_READ_CONTROL);
  spi_xfer(SPI1, reg);
  bt = spi_xfer(SPI1, MCP_DUMMY_SEND);
  // ss high
  gpio_set(GPIOA, GPIO4);
  return bt;
}

void write_relay(uint8_t pin, uint8_t value) {
  uint8_t bt;
  uint8_t reg = MCP_OLATA;
  if (pin > 7) {
    pin -= 8;
    reg = MCP_OLATB;
  }
  // read current output first
  // ss low
  gpio_clear(GPIOA, GPIO4);
  spi_xfer(SPI1, RELAY_READ_CONTROL);
  spi_xfer(SPI1, reg);
  bt = spi_xfer(SPI1, MCP_DUMMY_SEND);
  // ss high
  gpio_set(GPIOA, GPIO4);
  // set new output
  // ss low
  gpio_clear(GPIOA, GPIO4);
  spi_xfer(SPI1, RELAY_WRITE_CONTROL);
  spi_xfer(SPI1, reg);
  if (value == RELAY_TOGGLE) {
    bt ^= (1 << pin);
  } else {
    bt = (value == RELAY_ON) ? (bt | (1 << pin)) : (bt & ~(1 << pin));
  }
  spi_xfer(SPI1, bt);
  // ss high
  gpio_set(GPIOA, GPIO4);
}

uint8_t read_sense_port_a() { return read_sense(MCP_GPIOA); }

uint8_t read_sense_port_b() { return read_sense(MCP_GPIOB); }

uint8_t read_sense_capture_a() { return read_sense(MCP_INTCAPA); }

uint8_t read_sense_capture_b() { return read_sense(MCP_INTCAPB); }

uint8_t read_relay_port_a() { return read_relay(MCP_GPIOA); }

uint8_t read_relay_port_b() { return read_relay(MCP_GPIOB); }

