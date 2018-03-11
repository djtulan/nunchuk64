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
/// @file   selector.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   January, 2018
/// @brief  selector for i2c bus
//=============================================================================
#include "ioconfig.h"

#include "selector.h"

void selector_init(void) {
  BIT_SET(DDR_SEL1, BIT_SEL1);    // enable output
  BIT_SET(DDR_SEL2, BIT_SEL2);    // enable output

  BIT_CLEAR(PORT_SEL1, BIT_SEL1); // set to 0 (unselected)
  BIT_CLEAR(PORT_SEL2, BIT_SEL2); // set to 0 (unselected)
}

void selector_switch(Port port) {
  if (port == PORT_A) {
    BIT_CLEAR(PORT_SEL1, BIT_SEL1);   // set to 0
    BIT_SET(PORT_SEL2, BIT_SEL2);     // set to 1

  } else if (port == PORT_B) {
    BIT_CLEAR(PORT_SEL2, BIT_SEL2);   // set to 0
    BIT_SET(PORT_SEL1, BIT_SEL1);     // set to 1
  }
}
