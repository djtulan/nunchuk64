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

  // dataregister for joystick outputs
  PORT_JOY_A0   &= ~_BV(BIT_JOY_A0); // UP
  PORT_JOY_A1   &= ~_BV(BIT_JOY_A1); // DOWN
  PORT_JOY_A2   &= ~_BV(BIT_JOY_A2); // LEFT
  PORT_JOY_A3   &= ~_BV(BIT_JOY_A3); // RIGHT
  PORT_BUTTON_A &= ~_BV(BIT_BUTTON_A);

  PORT_JOY_B0   &= ~_BV(BIT_JOY_B0); // UP
  PORT_JOY_B1   &= ~_BV(BIT_JOY_B1); // DOWN
  PORT_JOY_B2   &= ~_BV(BIT_JOY_B2); // LEFT
  PORT_JOY_B3   &= ~_BV(BIT_JOY_B3); // RIGHT
  PORT_BUTTON_B &= ~_BV(BIT_BUTTON_B);

  // ddr for joystick outputs
  DDR_JOY_A0    &= ~_BV(BIT_JOY_A0);
  DDR_JOY_A1    &= ~_BV(BIT_JOY_A1);
  DDR_JOY_A2    &= ~_BV(BIT_JOY_A2);
  DDR_JOY_A3    &= ~_BV(BIT_JOY_A3);
  DDR_BUTTON_A  &= ~_BV(BIT_BUTTON_A);

  DDR_JOY_B0    &= ~_BV(BIT_JOY_B0);
  DDR_JOY_B1    &= ~_BV(BIT_JOY_B1);
  DDR_JOY_B2    &= ~_BV(BIT_JOY_B2);
  DDR_JOY_B3    &= ~_BV(BIT_JOY_B3);
  DDR_BUTTON_B  &= ~_BV(BIT_BUTTON_B);
}

void joystick_poll(struct ContollerData *cd) {
/*
  // UP
  if (cd->byte5 & _BV(0)) {
    DDR_JOY_A1 |= _BV(BIT_JOY_A1);
  } else {
    DDR_JOY_A1 &= ~(_BV(BIT_JOY_A1));
  }

  // DOWN
  if (cd->byte4 & _BV(6)) {
    DDR_JOY_A1 |= _BV(BIT_JOY_A1);
  } else {
    DDR_JOY_A1 &= ~(_BV(BIT_JOY_A1));
  }

  // LEFT
  if (cd->byte5 & _BV(1)) {
    DDR_JOY_A2 |= _BV(BIT_JOY_A2);
  } else {
    DDR_JOY_A2 &= ~(_BV(BIT_JOY_A2));
  }

  // RIGHT
  if (cd->byte4 & _BV(7)) {
    DDR_JOY_A3 |= _BV(BIT_JOY_A3);
  } else {
    DDR_JOY_A3 &= ~(_BV(BIT_JOY_A3));
  }

  // BUTTON
  if ((cd->byte5 & _BV(6)) || (cd->byte5 & _BV(5))) {
    DDR_BUTTON_A |= _BV(BIT_BUTTON_A);
  } else {
    DDR_BUTTON_A &= ~(_BV(BIT_BUTTON_A));
  }
*/



  // UP
  if (cd->byte5 & _BV(0)) {
    DDR_JOY_B1 |= _BV(BIT_JOY_B1);
  } else {
    DDR_JOY_B1 &= ~(_BV(BIT_JOY_B1));
  }

  // DOWN
  if (cd->byte4 & _BV(6)) {
    DDR_JOY_B1 |= _BV(BIT_JOY_B1);
  } else {
    DDR_JOY_B1 &= ~(_BV(BIT_JOY_B1));
  }

  // LEFT
  if (cd->byte5 & _BV(1)) {
    DDR_JOY_B2 |= _BV(BIT_JOY_B2);
  } else {
    DDR_JOY_B2 &= ~(_BV(BIT_JOY_B2));
  }

  // RIGHT
  if (cd->byte4 & _BV(7)) {
    DDR_JOY_B3 |= _BV(BIT_JOY_B3);
  } else {
    DDR_JOY_B3 &= ~(_BV(BIT_JOY_B3));
  }

  // BUTTON
  if ((cd->byte5 & _BV(6)) || (cd->byte5 & _BV(5))) {
    DDR_BUTTON_B |= _BV(BIT_BUTTON_B);
  } else {
    DDR_BUTTON_B &= ~(_BV(BIT_BUTTON_B));
  }
}

void joystick_toggle(void) {
}
