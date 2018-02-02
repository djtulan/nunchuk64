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

enum Joystick_State {
  UP     = (1 << 0), ///< up
  DOWN   = (1 << 1), ///< down
  LEFT   = (1 << 2), ///< left
  RIGHT  = (1 << 3), ///< right
  BUTTON = (1 << 4)  ///< fire button
};

/// \brief Joystick, holds the state of one joystick
typedef uint8_t Joystick;

/**
* @brief Init Joystick Ports
*
* All joystick output bits are set to zero
* All joystick ddr bits are set to zero (Tri-state)
* Should be default anyway
*/
extern void joystick_init(void);

/**
* @brief Update Joystick state
*
* @param [in] port_a byte representing the state of Port A
* @param [in] port_b byte representing the state of Port A
*
* The bit (see Joystick_State) are set inside the respective parameter
*/
extern void joystick_update(Joystick port_a, Joystick port_b);

#endif
