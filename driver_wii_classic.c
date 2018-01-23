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

#include "driver_wii_classic.h"

static void get_joystick_state_nunchuk(const ContollerData *cd, Joystick *joystick) {

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
    (*joystick) |= BUTTON;
  }

  // BY - Y Button
  if ((cd->byte[5] & 0x20) == 0) {
    (*joystick) |= UP;
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
}

static void get_paddle_state_nunchuk(const ContollerData *cd, uint8_t *paddle) {
}

Driver drv_wii_classic = {
  get_joystick_state_nunchuk,
  get_paddle_state_nunchuk
};
