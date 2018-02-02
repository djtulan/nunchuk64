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
#include "ioconfig.h"

#include "led.h"

static LED_State led_state = LED_OFF;

void led_init(void) {
  BIT_SET(DDR_LED, BIT_LED);   // enable output
  BIT_CLEAR(PORT_LED, BIT_LED);  // set to 0 => LED OFF
}

void led_switch(LED_State state) {
  led_state = state;

  if (state) {
    BIT_SET(PORT_LED, BIT_LED);
  } else {
    BIT_CLEAR(PORT_LED, BIT_LED);
  }
}

LED_State led_get_state(void) {
  return (led_state);
}
