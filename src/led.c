//=============================================================================
// *** Nunchuk64 ***
// Copyright (c) Robert Grasböck, All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//=============================================================================
/// @file   led.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   January, 2018
/// @brief  led
//=============================================================================
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "ioconfig.h"

#include "led.h"

const uint8_t FLASH_DATA[NUMBER_LED_STATES][7] PROGMEM = {
// on, off,  on, off, on, off
  {0,  0,  0,  0,  0,  0,  0}, ///< OFF
  {0,  0,  0,  0,  0,  0,  0}, ///< ON
  {2, 12,  0,  0,  0,  0,  0}, ///< F1
  {2,  4,  2, 12,  0,  0,  0}  ///< F2
};

static LED_State led_state = LED_OFF;
static uint8_t led_flash_index = 0;
static uint8_t led_flash_timer = 0;
static uint8_t led_lock = 0;

static void led_set(uint8_t on) {
  if (on) {
    BIT_SET(PORT_LED, BIT_LED);
  } else {
    BIT_CLEAR(PORT_LED, BIT_LED);
  }
}

void led_init(void) {
  BIT_SET(DDR_LED, BIT_LED);   // enable output
  BIT_CLEAR(PORT_LED, BIT_LED);  // set to 0 => LED OFF
}

void led_switch(LED_State state) {
  led_state = state;

  switch (led_state) {
    case LED_OFF:
    case LED_ON:
      led_set(led_state); // switch led
      break;

    case LED_BLINK1:
      led_set(1);
      break;

    case LED_BLINK2:
      led_set(1);
      break;

    case NUMBER_LED_STATES:
      break;
  }

  led_flash_index = 0; // start counting
  led_flash_timer = 0; // reset timer
}

void led_setnextstate(void) {
  LED_State s = led_get_state();

  s ++;

  if (s == NUMBER_LED_STATES)
    s = LED_OFF;

  led_switch(s);
}

LED_State led_get_state(void) {
  return (led_state);
}

void led_quick_blink(uint8_t number) {
  led_lock = 1;

  led_set(0);
  _delay_ms(200);
  wdt_reset();

  for (uint8_t i = 0; i < number; i++) {
    led_set(1);
    _delay_ms(50);

    wdt_reset();

    led_set(0);
    _delay_ms(50);
  }

  _delay_ms(400);
   wdt_reset();

  led_lock = 0;

  led_switch(led_state);
}

void led_poll(void) {

  // if ON or OFF do nothing
  if (led_state == LED_OFF || led_state == LED_ON || led_lock)
    return;

  // set timer
  if (led_flash_timer == 0) {
    uint8_t wait = pgm_read_byte(&(FLASH_DATA[led_state][led_flash_index]));

    if (wait) {
      led_flash_timer = wait;
    } else {
      led_flash_index = 0; // start from beginning
      led_set(1);
    }

    // count down timer
  } else {

    led_flash_timer --;

    if (led_flash_timer == 0) {
      // toggle led
      led_flash_index ++;
      led_set((led_flash_index + 1)  % 2);
    }
  }
}
