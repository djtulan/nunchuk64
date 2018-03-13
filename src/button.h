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
/// @file   button.h
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   January, 2018
/// @brief  button
//=============================================================================
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <inttypes.h>

/**
* @brief init button inputs
*
*/
extern void button_init(void);

/**
* @brief read and debounce button
*
*/
extern void button_debounce(void);

/**
* @brief get buttonpress (short)
* @return 1 ... button was pressed short / 0 ... no button press
*/
extern uint8_t button_get(void);

/**
* @brief get buttonpress (long)
* @return 1 ... button was pressed long / 0 ... no button press
*/
extern uint8_t button_get_long(void);

#endif
