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
/// @file   button.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   January, 2018
/// @brief  button
//=============================================================================
#include "ioconfig.h"

#include "button.h"

static volatile uint8_t button_down = 0;
static volatile uint8_t button_down_long = 0;

void button_init(void) {
  BIT_SET(PORT_BUTTON, BIT_BUTTON); // Enable internal pullup resistor on the input pin
  BIT_CLEAR(DDR_BUTTON, BIT_BUTTON); // pin is input
}

// Check button state and set the button_down variable if a debounced
// button down press is detected.
// Call this function about 100 times per second.
void button_debounce(void) {
  static uint8_t count = 0; // counter for number of equal states
  static uint8_t button_state = 1; // current (debounced) state
  static uint8_t button_trigger = 0; // trigger button down

  static uint8_t count_long = 0; // counter for number of equal states
  static uint8_t button_state_long = 1; // current (debounced) state

  // =====================================================
  // check if button is high or low for the moment
  // =====================================================
  uint8_t current_state = (bit_is_set(PIN_BUTTON, BIT_BUTTON)) ? 1 : 0;

  // =====================================================
  // button state is about to be changed, increase counter
  // =====================================================
  if (current_state != button_state) {
    count ++;

    // the button have not bounced for 3 checks, change state
    if (count >= 4) {
      button_state = current_state; // we are in a new state

      // if the button was pressed (not released), tell main so
      if (current_state == 0) {
        button_trigger = 1;
        // if button was released again
      } else {
        if (button_trigger == 1)
          button_down = 1;
      }

      count = 0;
    }

    // state is similar to old state
  } else {
    // reset counter
    count = 0;
  }

// =====================================================
// button state is about to be changed, increase counter
// =====================================================
  if (current_state != button_state_long) {
    count_long ++;

    // the button have not bounced for 3 checks, change state
    if ((count_long >= 70 && current_state == 0) ||
        (count_long >= 4 && current_state == 1)) {
      button_state_long = current_state; // we are in a new state

      // if the button was pressed long (not released), tell main so
      if (current_state == 0) {
        button_down_long = 1;
        button_trigger = 0;
      }

      count_long = 0;
    }

    // state is similar to old state
  } else {
    // reset counter
    count_long = 0;
  }
}

uint8_t button_get(void) {
  if (button_down == 1) {
    button_down = 0;
    return 1;
  }

  return 0;
}

uint8_t button_get_long(void) {
  if (button_down_long == 1) {
    button_down_long = 0;
    return 1;
  }

  return 0;
}

