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
/// @file   joystick.h
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  digital joystick part
//=============================================================================
#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <inttypes.h>

#include "controller.h"

enum {
  PORT_A, ///< select Port A
  PORT_B, ///< select Port A
};

void joystick_init(void);

void joystick_poll(ContollerData *cd, uint8_t port);

void joystick_toggle(void);

#endif
