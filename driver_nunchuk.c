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

#include "driver_nunchuk.h"

static void get_joystick_state_nunchuk(const ContollerData *cd, Joystick *joystick) {

  // see: http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
  // Analog stick X returns data from around 35 (fully left) to 228(fully right),
  // while analog stick Y returns from around 27 to 220. Center for both is around 128.

  (*joystick) = 0;

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

  // Z Button
  if ((cd->byte[5] & 0x01) == 0) {
    (*joystick) |= BUTTON;
  }

  // C Button
  if ((cd->byte[5] & 0x02) == 0) {
    (*joystick) |= UP;
  }
}

static void get_paddle_state_nunchuk(const ContollerData *cd, uint8_t *paddle) {
}

Driver drv_nunchuk = {
  get_joystick_state_nunchuk,
  get_paddle_state_nunchuk
};
