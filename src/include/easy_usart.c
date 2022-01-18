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
#include "easy_usart.h"

#include <libopencm3/cm3/nvic.h>

#define RX_SIZE 256
#define TX_SIZE 256

// rx/tx ring buffers
static volatile uint8_t tx_buff[TX_SIZE];
static volatile uint8_t tx_buff_head;
static volatile uint8_t tx_buff_tail;

static volatile uint8_t rx_buff[RX_SIZE];
static volatile uint8_t rx_buff_head;
static volatile uint8_t rx_buff_tail;

void usart2_setup() {
  tx_buff_head = tx_buff_tail = 0;
  rx_buff_head = rx_buff_tail = 0;
  nvic_enable_irq(NVIC_USART2_IRQ);
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_USART2);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
  gpio_set_output_options(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO3);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO3);
  usart_set_baudrate(USART2, 115200);
  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_mode(USART2, USART_MODE_TX_RX);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
  usart_enable_rx_interrupt(USART2);
  usart_enable(USART2);
}

void usart2_send_str(const char *str) {
  while (*str) {
    usart2_send_chr(*str);
    str++;
  }
}

// fill tx_buff, if it's full, it basically overwrites old chars
void usart2_send_chr(const uint8_t chr) {
  tx_buff[tx_buff_head] = chr;
  tx_buff_head = (tx_buff_head + 1) % TX_SIZE;
  if (tx_buff_head == tx_buff_tail) {  // full buffer, push the tail one ahead
    tx_buff_tail = (tx_buff_tail + 1) % TX_SIZE;
  }
  usart_enable_tx_interrupt(USART2);
}

uint8_t usart2_get_chr() {
  uint8_t chr = rx_buff[rx_buff_tail];
  rx_buff_tail = (rx_buff_tail + 1) % RX_SIZE;
  return chr;
}

// always call this first, before trying to get data with usart2_get_chr
uint8_t usart2_data_rdy() {
  if (rx_buff_head >= rx_buff_tail) return rx_buff_head - rx_buff_tail;
  return RX_SIZE + rx_buff_head - rx_buff_tail;
}

void usart2_isr(void) {
  if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(USART2) & USART_SR_RXNE) !=
       0)) {  // Read data register not empty
    rx_buff[rx_buff_head] = usart_recv(USART2);
    rx_buff_head = (rx_buff_head + 1) % RX_SIZE;
    if (rx_buff_head == rx_buff_tail) {  // full buffer, push the tail one ahead
      rx_buff_tail = (rx_buff_tail + 1) % RX_SIZE;
    }
  }
  if (((USART_CR1(USART2) & USART_CR1_TXEIE) != 0) &&
      ((USART_SR(USART2) & USART_SR_TXE) !=
       0)) {                             // Transmit data register empty
    if (tx_buff_head == tx_buff_tail) {  // tx buff is empty already
      usart_disable_tx_interrupt(USART2);
    } else {
      usart_send(USART2, tx_buff[tx_buff_tail]);
      tx_buff_tail = (tx_buff_tail + 1) % TX_SIZE;
    }
  }
}
