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

  // Cross and Buttons are encoded into byte[4] & byte[5]
  // Stupid thing, some of the codes use same bits

  // Cross down, right  Button SELECT
  switch (cd->byte[4] & 0xf0) {
    // right
    case 0x70:
      (*joystick) |= RIGHT;
      break;

      // down
    case 0xb0:
      (*joystick) |= DOWN;
      break;

    // SELECT Button
    case 0x80:
      (*joystick) |= BUTTON;
      break;

    // right and down
    case 0x30:
      (*joystick) |= RIGHT | DOWN;
      break;

    // right and SELECT Button
    case 0x00:
      (*joystick) |= RIGHT | BUTTON;
      break;

    // right and SELECT Button
    case 0x40:
      (*joystick) |= DOWN | BUTTON;
      break;

    // right and SELECT Button
    case 0xc0:
      (*joystick) |= RIGHT | DOWN | BUTTON;
      break;
  }

  // START
  switch (cd->byte[4] & 0x0f) {
    // START Button
    case 0x0b:
      // DO NOTHING :D
      break;
  }

  // A, B, A+, B+ (A+ or B+ are rapid fire)
  switch (cd->byte[5] & 0xf0) {
    // B  or  B+
    case 0xb0:
      (*joystick) |= BUTTON;
      break;

    // A  or  A+
    case 0x80:
      (*joystick) |= UP;
      break;

    // Both A and B
    case 0x40:
      (*joystick) |= UP | BUTTON;
      break;
  }

  // Cross up, left
  switch (cd->byte[5] & 0x0f) {
    // up
    case 0x08:
      (*joystick) |= UP;
      break;

    // left
    case 0x09:
      (*joystick) |= LEFT;
      break;

    // up and left
    case 0x0a:
      (*joystick) |= LEFT | UP;
      break;
  }
}

static void get_paddle_state_nes(const ContollerData *cd, uint8_t *paddle) {
}

Driver drv_nes_classic = {
  get_joystick_state_nes,
  get_paddle_state_nes
};
