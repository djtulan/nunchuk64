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
/// @file   driver_nes_classic.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  driver nes classic
//=============================================================================
#include "enums.h"
#include "joystick.h"

#include "driver_nes_classic.h"

static void get_joystick_state_nes(const ContollerData *cd, Joystick *joystick) {
  (*joystick) = 0;

  // Cross up, left
  switch (cd->byte[5] & 0x0f) {
    case 0x08:
      (*joystick) |= UP;
      break;

    case 0x09:
      (*joystick) |= LEFT;
      break;

    case 0x0a:
      (*joystick) |= LEFT | UP;
      break;
  }

  // Cross down, right
  switch (cd->byte[4] & 0xf0) {
    case 0xb0:
      (*joystick) |= DOWN;
      break;

    case 0x70:
      (*joystick) |= RIGHT;
      break;

    case 0x30:
      (*joystick) |= RIGHT | DOWN;
      break;
  }

  // A, B, A+, B+
  switch (cd->byte[5] & 0xf0) {
    case 0xb0:
      (*joystick) |= BUTTON;
      break;

    case 0x80:
      (*joystick) |= UP;
      break;
  }
}

static void get_paddle_state_nes(const ContollerData *cd, uint8_t *paddle) {
}

Driver drv_nes_classic = {
  get_joystick_state_nes,
  get_paddle_state_nes
};
