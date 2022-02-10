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
#include "easy_adc.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "easy_debug.h"
#include "stdio.h"
#include "string.h"

// 3 ADC values from 3 channels multiplexed on the ADC1 bus
uint8_t g_adc_channels[3] = {0};
volatile uint16_t g_adc_results[3] = {0};
volatile uint16_t g_vaccu;
volatile uint16_t g_vbat;
volatile uint16_t g_acurrent;

/*To switch off a peripheral connected to a DMA stream */
/*request, it is mandatory to, first, switch off the DMA stream to */
/*which the peripheral is connected, then to wait for EN bit = 0. */

// ADC1 continues scan group mode with DMA writing
void init_adc() {
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO5);
  rcc_periph_clock_enable(RCC_ADC1);
  // APB2 periph runs at 90MHz, prescale by 8 == 11.25MHz
  adc_set_clk_prescale(ADC_CCR_ADCPRE_BY8);
  adc_enable_scan_mode(ADC1);
  adc_disable_discontinuous_mode_regular(ADC1);
  adc_set_right_aligned(ADC1);
  adc_set_resolution(ADC1, ADC_CR1_RES_12BIT);
  // slowest sampling, most accurate results
  adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_480CYC);
  adc_enable_vbat_sensor();
  g_adc_channels[0] = ADC_CHANNEL10;  // Vaccu
  g_adc_channels[1] = ADC_CHANNEL15;  // Vcurrent
  g_adc_channels[2] = ADC_CHANNEL18;  // Vbat
  adc_set_multi_mode(ADC_CCR_MULTI_INDEPENDENT);
  adc_set_regular_sequence(ADC1, 3, g_adc_channels);  // 3 values
  adc_enable_dma(ADC1);
  adc_set_dma_continue(ADC1);
  adc_power_on(ADC1);
}

// ADC1 is mapped to DMA2 on Channel 0: Stream 0 or Stream 4
// using Stream 4
// Direct mode is the default. Data is transferred as soon as a DMA request is
// received. The FIFO is not used. This must not be set when memory to memory
// mode is selected.
void init_dma2() {
  rcc_periph_clock_enable(RCC_DMA2);
  dma_stream_reset(DMA2, DMA_STREAM4);
  nvic_enable_irq(NVIC_DMA2_STREAM4_IRQ);
  dma_set_memory_size(DMA2, DMA_STREAM4, DMA_SxCR_MSIZE_16BIT);
  dma_set_peripheral_size(DMA2, DMA_STREAM4, DMA_SxCR_PSIZE_16BIT);
  dma_enable_memory_increment_mode(DMA2, DMA_STREAM4);
  dma_enable_direct_mode(DMA2, DMA_STREAM4);
  dma_set_transfer_mode(DMA2, DMA_STREAM4, DMA_SxCR_DIR_PERIPHERAL_TO_MEM);
  // source address -> HW ADC1 Data Register
  dma_set_peripheral_address(DMA2, DMA_STREAM4, (uint32_t)&ADC_DR(ADC1));
  // destination address -> our result array
  dma_set_memory_address(DMA2, DMA_STREAM4, (uint32_t)g_adc_results);
  dma_set_number_of_data(DMA2, DMA_STREAM4, 3);  // 3x 16bit data results
  dma_channel_select(DMA2, DMA_STREAM4, DMA_SxCR_CHSEL_0);
  dma_set_priority(DMA2, DMA_STREAM4, DMA_SxCR_PL_LOW);
  dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM4);
  dma_enable_stream(DMA2, DMA_STREAM4);
}

// DMA transfer complete interrupt handler
void dma2_stream4_isr(void) {
  if (dma_get_interrupt_flag(DMA2, DMA_STREAM4, DMA_TCIF)) {
    g_vaccu = g_adc_results[0];
    g_acurrent = g_adc_results[1];
    g_vbat = g_adc_results[2];
    dma_clear_interrupt_flags(DMA2, DMA_STREAM4, DMA_TCIF);
  }
}

// 12 bit adc resolution 0-4095 at 3.3v
// 0.8mV per bit
void start_adc_conversion() {
  adc_start_conversion_regular(ADC1);
  // reinit dma2, else we don't get a new conversion
  init_dma2();
#if EASY_DEBUG_OFF
  char buf[255];
  snprintf(buf, 255, "adc[0-2]: %d %d %d\n", g_adc_results[0], g_adc_results[1],
           g_adc_results[2]);
  debug_write(buf);
#endif
}

// PC0 VACCU ADC IN (ADC123_IN10)
// Vaccu = ( ADC_value * 6 / 4095 ) * 3.3v * 1000
// Times 6 is because of the 1/6 voltage divider
// Returns mV to keep it an integer
uint16_t get_adc_vaccu() { return ((g_vaccu * 6 * 3300) / 4095); }

// PC5 VCURRENT ADC IN (ADC12_IN15)/USART3_RX
// Current sensor is 200mV/A -> 1mV is 5mA
// Returns mA to keep it an integer
uint16_t get_adc_acurrent() { return ((g_acurrent * 3300 * 5) / 4095); }

// ADC1_IN18, reads Vbat/4 by default
// Vbat = ( ADC_value * 4 / 4095 ) * 3.3v * 1000
// Returns mV to keep it an integer
uint16_t get_adc_vbat() { return ((g_vbat * 4 * 3300) / 4095); }

