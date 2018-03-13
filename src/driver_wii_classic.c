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
/// @file   driver_wii_classic.cpp
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   January, 2018
/// @brief  driver wii classic
//=============================================================================
#include "enums.h"
#include "joystick.h"
#include "led.h"

#include "driver_wii_classic.h"

static inline uint16_t left_x(const ContollerData *cd) {
  return (cd->byte[0] & 0x3f);
}

static inline uint16_t left_y(const ContollerData *cd) {
  return (cd->byte[1] & 0x3f);
}

static void get_joystick_state_wii_classic(const ContollerData *cd, Joystick *joystick) {

  // see: http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller

  (*joystick) = 0;

  // BDU - DPAD U
  if ((cd->byte[5] & 0x01) == 0) {
    (*joystick) |= UP;
  }

  // BDD - DPAD D
  if ((cd->byte[4] & 0x40) == 0) {
    (*joystick) |= DOWN;
  }

  // BDL - DPAD L
  if ((cd->byte[5] & 0x02) == 0) {
    (*joystick) |= LEFT;
  }

  // BDR - DPAD R
  if ((cd->byte[4] & 0x80) == 0) {
    (*joystick) |= RIGHT;
  }

  // ------------------------------

  // BA - A Button
  if ((cd->byte[5] & 0x10) == 0) {
    (*joystick) |= UP;
  }

  // BB - B Button
  if ((cd->byte[5] & 0x40) == 0) {
    (*joystick) |= BUTTON;
  }

  // BX - X Button
  if ((cd->byte[5] & 0x08) == 0) {
    (*joystick) |= BUTTON2;
  }

  // BY - Y Button
  if ((cd->byte[5] & 0x20) == 0) {
    (*joystick) |= AUTOFIRE;
  }

  // ------------------------------

  // BLT - Button left (trigger)
  if ((cd->byte[4] & 0x20) == 0) {
    (*joystick) |= LEFT;
  }

  // BRT - Button right (trigger)
  if ((cd->byte[4] & 0x02) == 0) {
    (*joystick) |= RIGHT;
  }

  // ------------------------------

  // B+ - Button Start
  if ((cd->byte[4] & 0x04) == 0) {
    (*joystick) |= SPACE;
  }

  // B- - Button Select
  if ((cd->byte[4] & 0x10) == 0) {
    (*joystick) |= BUTTON;
  }

  switch (led_get_state()) {
    case LED_OFF:
    case LED_ON:
    case NUMBER_LED_STATES: {

      uint8_t lx = left_x(cd);

      if (lx > 43) {
        (*joystick) |= RIGHT;
      } else if (lx < 20) {
        (*joystick) |= LEFT;
      }

      // Analog Joystick Y
      uint8_t ly = left_y(cd);

      if (ly > 43) {
        (*joystick) |= UP;
      } else if (ly < 20) {
        (*joystick) |= DOWN;
      }
    }
    break;

    case LED_BLINK1:
    case LED_BLINK2:
//  case LED_BLINK3:
      break;
  }
}

static void get_paddle_state_wii_classic(const ContollerData *cd, Paddle *paddle) {
  if (led_get_state() == LED_BLINK1 || led_get_state() == LED_BLINK2) {

    int16_t x = left_x(cd) << 4;
    int16_t y = left_y(cd) << 4;

    x = scale(x, 1.6);
    y = scale(y, 1.6);

    paddle->axis_x = x;
    paddle->axis_y = y;
  }
}

uint8_t get_paddle_enabled_wii_classic(void) {
  switch (led_get_state()) {
    case LED_BLINK1:
    case LED_BLINK2:
      return TRUE;

    default:
      return FALSE;
  }
}

Driver drv_wii_classic = {
  get_joystick_state_wii_classic,
  get_paddle_state_wii_classic,
  get_paddle_enabled_wii_classic
};
