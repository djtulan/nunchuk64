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
/// @file   driver_nunchuk.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   January, 2018
/// @brief  driver nunchuk
//=============================================================================
#include "enums.h"
#include "joystick.h"
#include "led.h"

#include "driver_nunchuk.h"

#define ACCEL_ZEROX   512
#define ACCEL_ZEROY   512
#define ACCEL_ZEROZ   512

static inline uint16_t nunchuk_accelx(const ContollerData *cd) {
  return ((0x0000 | (cd->byte[2] << 2)) + ((cd->byte[5] & 0x0c) >> 2));
}

static inline uint16_t nunchuk_accely(const ContollerData *cd) {
  return ((0x0000 | (cd->byte[3] << 2)) + ((cd->byte[5] & 0x30) >> 4));
}

static inline uint16_t nunchuk_accelz(const ContollerData *cd) {
  return ((0x0000 | (cd->byte[4] << 2)) + ((cd->byte[5] & 0xc0) >> 6));
}

static inline int nunchuk_caccelx(const ContollerData *cd) {
  return (int)(nunchuk_accelx(cd) - ACCEL_ZEROX);
}

static inline int nunchuk_caccely(const ContollerData *cd) {
  return (int)(nunchuk_accely(cd) - ACCEL_ZEROY);
}

static inline int nunchuk_caccelz(const ContollerData *cd) {
  return (int)(nunchuk_accelz(cd) - ACCEL_ZEROZ);
}

static void get_joystick_state_nunchuk(const ContollerData *cd, Joystick *joystick) {

  // see: http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
  // Analog stick X returns data from around 35 (fully left) to 228(fully right),
  // while analog stick Y returns from around 27 to 220. Center for both is around 128.
  (*joystick) = 0;

  switch (led_get_state()) {
    // ===================================
    // LED is OFF/ON (Joystick mode)
    // ===================================
    case LED_OFF: {
      // Analog Joystick X
      uint8_t sx = cd->byte[0];

      if (sx > 180) {
        (*joystick) |= RIGHT;
      } else if (sx < 76) {
        (*joystick) |= LEFT;
      }

      // Analog Joystick Y
      uint8_t sy = cd->byte[1];

      if (sy > 180) {
        (*joystick) |= UP;
      } else if (sy < 76) {
        (*joystick) |= DOWN;
      }
    }
    break;

    // ===================================
    // LED BLINK (Accelerometer mode)
    // ===================================
    case LED_ON: {
      int x = nunchuk_caccelx(cd);
      int y = nunchuk_caccely(cd);

      if (x < -118) {
        (*joystick) |= LEFT;

      } else if (x > 118) {
        (*joystick) |= RIGHT;
      }

      if (y < -118) {
        (*joystick) |= UP;

      } else if (y > 118) {
        (*joystick) |= DOWN;
      }
    }
    break;

    case LED_BLINK1:
    case LED_BLINK2: {
    }
    break;

//     case LED_BLINK3: {
//     }
//     break;

    case NUMBER_LED_STATES: {
    }
    break;
  }

  // Z Button
  if ((cd->byte[5] & 0x01) == 0) {
    (*joystick) |= BUTTON;
  }

  // C Button
  if ((cd->byte[5] & 0x02) == 0) {
    (*joystick) |= AUTOFIRE;
  }
}

static void get_paddle_state_nunchuk(const ContollerData *cd, Paddle *paddle) {
  if (led_get_state() == LED_BLINK1) {

    int16_t x = nunchuk_accelx(cd);
    int16_t y = nunchuk_accely(cd);

    x = scale(x, 2.3);
    y = scale(y, 2.3);

    paddle->axis_x = x;
    paddle->axis_y = y;

  } else if (led_get_state() == LED_BLINK2) {

    int16_t x = cd->byte[0] << 2;
    int16_t y = cd->byte[1] << 2;

    x = scale(x, 1.7);
    y = scale(y, 1.7);

    paddle->axis_x = x;
    paddle->axis_y = y;
  }
}

uint8_t get_paddle_enabled_nunchuk(void) {
  switch (led_get_state()) {
    case LED_BLINK1:
    case LED_BLINK2:
      return TRUE;

    default:
      return FALSE;
  }
}

Driver drv_nunchuk = {
  get_joystick_state_nunchuk,
  get_paddle_state_nunchuk,
  get_paddle_enabled_nunchuk
};
