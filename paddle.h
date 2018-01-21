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
/// @file   paddle.h
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  analog paddle input part
//=============================================================================
#ifndef _PADDLE_H_
#define _PADDLE_H_

#include <inttypes.h>

#include "controller.h"
#include "joystick.h"

/**
* @brief init paddle-related IOs and interrupts
*/
void paddle_init(void);

/**
* @brief state paddle hardware and interrupt routines
*/
void paddle_start(void);

/**
* @brief update paddle state
*/
void paddle_update(Joystick port_a, Joystick port_b);

#endif
