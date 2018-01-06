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
/// @file   nunchuck64.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  main
//=============================================================================

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/sleep.h>
//#include <avr/pgmspace.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>

#include "ioconfig.h"
#include "controller.h"
#include "joystick.h"
#include "paddle.h"

void init(void) {
  DDR_LED |= _BV(BIT_LED);    // enable output
  PORT_LED |= _BV(BIT_LED);   // set to 1 => LED ON

  DDR_SEL1 |= _BV(BIT_SEL1);    // enable output
  PORT_SEL1 &= ~_BV(BIT_SEL1);   // set to 1

  DDR_SEL2 |= _BV(BIT_SEL2);    // enable output
  PORT_SEL2 |= _BV(BIT_SEL2);   // set to 0

  nunchuck_init();
  joystick_init();
  paddle_init();
}

struct ContollerData cd;

int main(void) {
  init();

  while (1) {

    //_delay_ms(40);
    nunchuck_read(&cd);
    joystick_poll(&cd);

    if (cd.byte5 & _BV(6)) {
      PORT_LED |= _BV(BIT_LED);
    } else {
      PORT_LED &= ~_BV(BIT_LED);
      _delay_ms(1);
    }
  }

  return 0;
}
