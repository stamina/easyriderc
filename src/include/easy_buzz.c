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
#include "easy_buzz.h"

#include "easy_clock.h"
#include "easy_debug.h"
#include "easy_state.h"
#include "sound/alarm.h"
#include "sound/frogger.h"
#include "sound/furelise.h"
#include "sound/indicator.h"
#include "sound/larry.h"
#include "sound/pipi.h"
#include "sound/popcorn.h"
#include "stdio.h"

void set_buzzer_period_duty(uint16_t arr_period, uint16_t oc2_duty);
uint32_t get_note_duration(const uint16_t length);
void stop_buzzer(void);
void start_buzzer(void);

typedef struct {
  uint8_t sound_song_idx;
  uint16_t sound_note_idx;
  uint8_t sound_tempo;  // NOTE: max tempo =  255
  const uint16_t* notes;
} t_sound;

t_sound current_sound = {0, 0, 120, NULL};

void init_buzzer() {
  // port PB7 TIM4_CH2
  // TIM4 is a 16bit up/down counter with a 16bit prescaler
  rcc_periph_clock_enable(RCC_TIM4);
  rcc_periph_clock_enable(RCC_GPIOB);
  // pin PB7: AF2 (alternate function 2 TIM4_CH2 according datasheet table)
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO7);
  gpio_set_af(GPIOB, GPIO_AF2, GPIO7);
  gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO7);
  rcc_periph_reset_pulse(RST_TIM4);
  timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_prescaler(TIM4,
                      89);  // 89 plus 1 -> 90Mhz APB1 timer bus / 90 = 1Mhz
  timer_continuous_mode(TIM4);
  timer_enable_preload(TIM4);
  timer_set_oc_mode(TIM4, TIM_OC2, TIM_OCM_PWM1);
  timer_enable_oc_preload(TIM4, TIM_OC2);
  timer_enable_oc_output(TIM4, TIM_OC2);
}

void set_buzzer_period_duty(uint16_t arr_period, uint16_t oc2_duty) {
  timer_set_oc_value(TIM4, TIM_OC2, oc2_duty);
  timer_set_period(TIM4, arr_period);
}

// duration of the current note
// 1000x 1ms ticks * 60sec / tempo in bmp * (4 beats / length)
// e.g. 6000 / 120 * (4 / 4) -> 500ms long 1/4 note at 120bpm
// length is normally 1, 2, 4, 8 or 16 parts of a full note
uint32_t get_note_duration(const uint16_t length) {
  return (1000 * 60 / current_sound.sound_tempo * 4 / length);
}

void set_buzzer_sound(uint8_t sound_song_idx) {
  set_substate(ST_BUZZ);
  current_sound.sound_song_idx = sound_song_idx;
  current_sound.sound_note_idx = 0;
  switch (sound_song_idx) {
    case BUZZ_SOUND_POPCORN:
      current_sound.notes = g_music_popcorn;
      break;
    case BUZZ_SOUND_FROGGER:
      current_sound.notes = g_music_frogger;
      break;
    case BUZZ_SOUND_FURELISE:
      current_sound.notes = g_music_furelise;
      break;
    case BUZZ_SOUND_LARRY:
      current_sound.notes = g_music_larry;
      break;
    case BUZZ_SOUND_PIPI:
      current_sound.notes = g_music_pipi;
      break;
    case BUZZ_SOUND_ALARM:
      current_sound.notes = g_music_alarm;
      break;
    case BUZZ_SOUND_INDICATOR_HALFSECOND:
      current_sound.notes = g_indicator_half;
      break;
    case BUZZ_SOUND_INDICATOR_QUARTERSECOND:
      current_sound.notes = g_indicator_quarter;
      break;
    default:
#if EASY_DEBUG
      debug_write("Unknown sound_song_idx!\n");
#endif
      cm3_assert_not_reached();
      break;
  }
  // first byte is the tempo
  current_sound.sound_tempo = current_sound.notes[current_sound.sound_note_idx];
}

void process_buzzer() {
  if (check_substate(ST_BUZZ)) {
    if (!get_1ms_tick_slot(
            BUZZ_1MS_SLOT)) {  // time is up for current note duration
      current_sound.sound_note_idx++;
      if (current_sound.notes[current_sound.sound_note_idx] != BUZZ_END) {
        set_1ms_tick_slot(
            BUZZ_1MS_SLOT,
            get_note_duration(
                current_sound
                    .notes[current_sound.sound_note_idx]));  // set new note
                                                             // duration for
                                                             // countdown
        current_sound.sound_note_idx++;
        if (current_sound.notes[current_sound.sound_note_idx] == BUZZ_PAUSE) {
          // set duty to 0% to keep it silent for a while
          set_buzzer_period_duty(
              current_sound.notes[current_sound.sound_note_idx], 0);
        } else {
          // set duty to 50% as the default signal
          set_buzzer_period_duty(
              current_sound.notes[current_sound.sound_note_idx],
              current_sound.notes[current_sound.sound_note_idx] / 2);
        }
        start_buzzer();
      } else {
#if EASY_DEBUG
        debug_write("BUZZ_END\n");
#endif
        remove_substate(ST_BUZZ);
        stop_buzzer();
      }
    }
  } else {
    if (TIM_CR1(TIM4) &
        TIM_CR1_CEN) {  // force a timer stop, when ST_BUZZ has been cleared
      stop_buzzer();
    }
  }
}

void start_buzzer() { timer_enable_counter(TIM4); }
void stop_buzzer() { timer_disable_counter(TIM4); }

