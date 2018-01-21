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
/// @file   driver.h
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  template for joystick driver
//=============================================================================
#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "controller.h"
#include "joystick.h"
#include "paddle.h"

/// \driver struct
typedef struct {
  /**
  * @brief get the joystick state from controller data
  * @param [in] cd controller data
  * @param [out] joystick data
  */
  void (*get_joystick_state)(const ContollerData *cd, Joystick *joystick);

  /**
  * @brief get the paddle state from controller data
  * @param [in] cd controller data
  * @param [out] paddle data
  */
  void (*get_paddle_state)(const ContollerData *cd, uint8_t *paddle);

} Driver;

#endif
