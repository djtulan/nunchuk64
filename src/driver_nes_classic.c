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

#ifdef xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

#include "enums.h"
#include "joystick.h"

#include "driver_nes_classic.h"

// Cross and Buttons are encoded into byte[4] & byte[5]
// Stupid thing, some of the codes use same bits
// It seems that goups of buttons/joystick axes are encoded into a nibble(half-byte)

// ================================================================================
//  Part   byte[0]     byte[1]     byte[2]     byte[3]     byte[4]     byte[5]
//  Part   b7654-3210  b7654-3210  b7654-3210  b7654-3210  b7654-3210  b7654-3210
// ================================================================================
//  UP      0000-0000   0000-0000   0000-0000   0000-0000   0000-0000   0000-0xxx
// -------------------------------------------------------------------------------
//  DOWN    0000-0000   0000-0000   0000-0000   0000-0000   0x00-0000   0000-0000
// -------------------------------------------------------------------------------
//  LEFT    0000-0000   0000-0000   0000-0000   0000-0000   0000-0000   0000-0xx0
// -------------------------------------------------------------------------------
//  RIGHT   0000-0000   0000-0000   0000-0000   0000-0000   x000-0000   0000-0000
// -------------------------------------------------------------------------------
//  A       0000-0000   0000-0000   0000-0000   0000-0000   0000-0000   0xxx-0000
// -------------------------------------------------------------------------------
//  B       0000-0000   0000-0000   0000-0000   0000-0000   0000-0000   0x00-0000
// -------------------------------------------------------------------------------
//  X       0000-0000   0000-0000   0000-0000   0000-0000   0000-0000   0000-x000
// -------------------------------------------------------------------------------
//  Y       0000-0000   0000-0000   0000-0000   0000-0000   0000-0000   0xx0-0000
// -------------------------------------------------------------------------------
//  START   0000-0000   0000-0000   0000-0000   0000-0000   0000-0x00   0000-0000
// -------------------------------------------------------------------------------
//  SELECT  0000-0000   0000-0000   0000-0000   0000-0000   0xxx-0000   0000-0000
// -------------------------------------------------------------------------------
//  L       0000-0000   0000-0000   0000-0000   0000-0000   0xx0-0000   0000-0000
// -------------------------------------------------------------------------------
//  R       0000-0000   0000-0000   0000-0000   0000-0000   0000-0xx0   0000-0000
// ================================================================================

// ================================================================================
//      Group                          byte                      mask
// ================================================================================
//  1.  UP, LEFT, X                    5                         0x0f
//  2.  A, B, Y                        5                         0xf0
//  3.  START, R                       4                         0x0f
//  4.  DOWN, RIGHT, SELECT, L         4                         0xf0
// ================================================================================
static void get_joystick_state_nes(const ContollerData *cd, Joystick *joystick) {
  (*joystick) = 0;

  // UP, LEFT, X
  switch (cd->byte[5] & 0x0f) {
    // UP
    case 0x08:
      (*joystick) |= UP;
      break;

    // LEFT
    case 0x09:
      (*joystick) |= LEFT;
      break;

    // X
    case 0x07:
      (*joystick) |= BUTTON;
      break;

    // UP and LEFT
    case 0x0a:
      (*joystick) |= UP | LEFT;
      break;

    // UP and X
    case 0x00:
      (*joystick) |= UP | BUTTON;
      break;

    // LEFT and X
    case 0x01:
      (*joystick) |= LEFT | BUTTON;
      break;

    // UP and LEFT and X
    case 0x02:
      (*joystick) |= LEFT | UP | BUTTON;
      break;
  }

  // A, B, Y
  switch (cd->byte[5] & 0xf0) {
    // B
    case 0xb0:
      (*joystick) |= BUTTON;
      break;

    // A
    case 0x80:
      (*joystick) |= UP;
      break;

    // Y
    case 0x90:
      (*joystick) |= AUTOFIRE;
      break;

    // A and B
    case 0x40:
      (*joystick) |= UP | BUTTON;
      break;

    // A and Y
    case 0xa0:
      (*joystick) |= UP;
      break;

    // B and Y
    case 0x50:
      (*joystick) |= BUTTON;
      break;

    // A and B and Y
    case 0x60:
      (*joystick) |= UP | BUTTON;
      break;
  }

  // START, R
  switch (cd->byte[4] & 0x0f) {
    // START
    case 0x0b:
      (*joystick) |= SPACE;
      break;

    // R
    case 0x09:
      (*joystick) |= RIGHT;
      break;

    // START and R
    case 0x05:
      (*joystick) |= SPACE | RIGHT;
      break;
  }

  // DOWN, RIGHT, SELECT, L
  switch (cd->byte[4] & 0xf0) {
    // DOWN
    case 0xb0:
      (*joystick) |= DOWN;
      break;

    // RIGHT
    case 0x70:
      (*joystick) |= RIGHT;
      break;

    // SELECT
    case 0x80:
      (*joystick) |= BUTTON;
      break;

    // L
    case 0x90:
      (*joystick) |= LEFT;
      break;

    // DOWN and RIGHT
    case 0x30:
      (*joystick) |= RIGHT | DOWN;
      break;

    // DOWN and SELECT
    case 0x40:
      (*joystick) |= DOWN | BUTTON;
      break;

    // DOWN and L
    case 0x50:
      (*joystick) |= DOWN | LEFT;
      break;

    // RIGHT and SELECT
    case 0x00:
      (*joystick) |= RIGHT | BUTTON;
      break;

    // RIGHT and L
    case 0x10:
      (*joystick) |= RIGHT | LEFT;
      break;

    // SELECT and L
    case 0xa0:
      (*joystick) |= BUTTON | LEFT;
      break;

    // DOWN and RIGHT and SELECT
    case 0xc0:
      (*joystick) |= DOWN | RIGHT | BUTTON;
      break;

    // DOWN and RIGHT and L
    case 0xd0:
      (*joystick) |= DOWN | RIGHT | LEFT;
      break;

    // DOWN and SELECT and L
    case 0x60:
      (*joystick) |= DOWN | BUTTON | LEFT;
      break;

    // RIGHT and SELECT and L
    case 0x20:
      (*joystick) |= RIGHT | BUTTON | LEFT;
      break;

    // DOWN and RIGHT and SELECT and L
    case 0xe0:
      (*joystick) |= DOWN | RIGHT | BUTTON | LEFT;
      break;
  }
}

static void get_paddle_state_nes(const ContollerData *cd, Paddle *paddle) {
}

uint8_t get_paddle_enable_nes(void) {
  return FALSE;
}

Driver drv_nes_classic = {
  get_joystick_state_nes,
  get_paddle_state_nes,
  get_paddle_enable_nes
};

#endif // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
