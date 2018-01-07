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
/// @file   controller.h
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  i2c controller
//=============================================================================
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

/// \brief 6 byte data
typedef struct {
  uint8_t byte0;  ///< databyte 0
  uint8_t byte1;  ///< databyte 1
  uint8_t byte2;  ///< databyte 2
  uint8_t byte3;  ///< databyte 3
  uint8_t byte4;  ///< databyte 4
  uint8_t byte5;  ///< databyte 5
} ContollerData;

uint8_t controller_init(void);
uint8_t controller_read(ContollerData *n);

#endif
