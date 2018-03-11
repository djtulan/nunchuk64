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
/// @file   selector.h
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   January, 2018
/// @brief  selector for i2c bus
//=============================================================================
#ifndef _SELECTOR_H_
#define _SELECTOR_H_

#include "enums.h"

/**
* @brief init select outputs for i2c select
*
*/
extern void selector_init(void);

/**
* @brief select i2c device
*
* @param port PORT_A or PORT_B
*/
extern void selector_switch(Port port);

#endif
