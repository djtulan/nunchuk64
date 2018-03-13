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
/// @file   joystick.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  digital joystick part
//=============================================================================
#include "ioconfig.h"

#include "joystick.h"

void joystick_init(void) {
  // NOTE not needed, default after reset

  /*
  // dataregister for joystick outputs
  BIT_CLEAR(PORT_JOY_A0,   BIT_JOY_A0);   // UP
  BIT_CLEAR(PORT_JOY_A1,   BIT_JOY_A1);   // DOWN
  BIT_CLEAR(PORT_JOY_A2,   BIT_JOY_A2);   // LEFT
  BIT_CLEAR(PORT_JOY_A3,   BIT_JOY_A3);   // RIGHT
  BIT_CLEAR(PORT_BUTTON_A, BIT_BUTTON_A);

  BIT_CLEAR(PORT_JOY_B0,   BIT_JOY_B0);   // UP
  BIT_CLEAR(PORT_JOY_B1,   BIT_JOY_B1);   // DOWN
  BIT_CLEAR(PORT_JOY_B2,   BIT_JOY_B2);   // LEFT
  BIT_CLEAR(PORT_JOY_B3,   BIT_JOY_B3);   // RIGHT
  BIT_CLEAR(PORT_BUTTON_B, BIT_BUTTON_B);

  // ddr for joystick outputs
  BIT_CLEAR(DDR_JOY_A0,   BIT_JOY_A0);
  BIT_CLEAR(DDR_JOY_A1,   BIT_JOY_A1);
  BIT_CLEAR(DDR_JOY_A2,   BIT_JOY_A2);
  BIT_CLEAR(DDR_JOY_A3,   BIT_JOY_A3);
  BIT_CLEAR(DDR_BUTTON_A, BIT_BUTTON_A);

  BIT_CLEAR(DDR_JOY_B0,   BIT_JOY_B0);
  BIT_CLEAR(DDR_JOY_B1,   BIT_JOY_B1);
  BIT_CLEAR(DDR_JOY_B2,   BIT_JOY_B2);
  BIT_CLEAR(DDR_JOY_B3,   BIT_JOY_B3);
  BIT_CLEAR(DDR_BUTTON_B, BIT_BUTTON_B);
  */
}

static volatile Joystick port_a_old = 0;
static volatile Joystick port_b_old = 0;

static volatile uint8_t  autofire_a = 0;
static volatile uint8_t  autofire_b = 0;

void joystick_update(Joystick port_a, Joystick port_b) {

  // look if same change?
  if (port_a == port_a_old && port_b == port_b_old) {
    // nothing changed
    return;
  }

  port_a_old = port_a;
  port_b_old = port_b;

  // ===================================
  //  CONTROL PORT A
  // ===================================

  // UP
  if (port_a & UP) {
    BIT_SET(DDR_JOY_A0, BIT_JOY_A0);
  } else {
    BIT_CLEAR(DDR_JOY_A0, BIT_JOY_A0);
  }

  // DOWN
  if (port_a & DOWN) {
    BIT_SET(DDR_JOY_A1, BIT_JOY_A1);
  } else {
    BIT_CLEAR(DDR_JOY_A1, BIT_JOY_A1);
  }

  // LEFT
  if (port_a & LEFT) {
    BIT_SET(DDR_JOY_A2, BIT_JOY_A2);
  } else {
    BIT_CLEAR(DDR_JOY_A2, BIT_JOY_A2);
  }

  // RIGHT
  if (port_a & RIGHT) {
    BIT_SET(DDR_JOY_A3, BIT_JOY_A3);
  } else {
    BIT_CLEAR(DDR_JOY_A3, BIT_JOY_A3);
  }

  // BUTTON (small hack to simulate SPACE on both contollers)
  if (port_a & BUTTON || port_a & SPACE || port_b & SPACE) {
    BIT_SET(DDR_BUTTON_A, BIT_BUTTON_A);
  } else {

    // AUTOFIRE BUTTON
    if (port_a & AUTOFIRE) {
      autofire_a = 1;

      // neither AUTOFIRE nor FIRE BUTTON
    } else {
      autofire_a = 0;
      BIT_CLEAR(DDR_BUTTON_A, BIT_BUTTON_A);
    }
  }

  // ===================================
  //  CONTROL PORT B
  // ===================================

  // UP
  if (port_b & UP) {
    BIT_SET(DDR_JOY_B0, BIT_JOY_B0);
  } else {
    BIT_CLEAR(DDR_JOY_B0, BIT_JOY_B0);
  }

  // DOWN
  if (port_b & DOWN) {
    BIT_SET(DDR_JOY_B1, BIT_JOY_B1);
  } else {
    BIT_CLEAR(DDR_JOY_B1, BIT_JOY_B1);
  }

  // LEFT
  if (port_b & LEFT) {
    BIT_SET(DDR_JOY_B2, BIT_JOY_B2);
  } else {
    BIT_CLEAR(DDR_JOY_B2, BIT_JOY_B2);
  }

  // RIGHT
  if (port_b & RIGHT) {
    BIT_SET(DDR_JOY_B3, BIT_JOY_B3);
  } else {
    BIT_CLEAR(DDR_JOY_B3, BIT_JOY_B3);
  }

  // BUTTON
  if (port_b & BUTTON) {
    BIT_SET(DDR_BUTTON_B, BIT_BUTTON_B);
  } else {

    // AUTOFIRE BUTTON
    if (port_b & AUTOFIRE) {
      autofire_b = 1;

      // neither AUTOFIRE nor FIRE BUTTON
    } else {
      autofire_b = 0;
      BIT_CLEAR(DDR_BUTTON_B, BIT_BUTTON_B);
    }
  }
}

void joystick_poll(void) {
  if (autofire_a == 1) {
    // toggle FIRE A
    if (bit_is_set(DDR_BUTTON_A, BIT_BUTTON_A)) {
      BIT_CLEAR(DDR_BUTTON_A, BIT_BUTTON_A);
    } else {
      BIT_SET(DDR_BUTTON_A, BIT_BUTTON_A);
    }
  }

  if (autofire_b == 1) {
    // toggle FIRE B
    if (bit_is_set(DDR_BUTTON_B, BIT_BUTTON_B)) {
      BIT_CLEAR(DDR_BUTTON_B, BIT_BUTTON_B);
    } else {
      BIT_SET(DDR_BUTTON_B, BIT_BUTTON_B);
    }
  }
}
