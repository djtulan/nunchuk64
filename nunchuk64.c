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
/// @file   nunchuk64.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  main
//=============================================================================
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "i2c_master.h"
#include "ioconfig.h"
#include "controller.h"
#include "joystick.h"
#include "paddle.h"

void init_select(void) {
  DDR_SEL1 |= _BV(BIT_SEL1);    // enable output
  DDR_SEL2 |= _BV(BIT_SEL2);    // enable output

  PORT_SEL1 &= ~_BV(BIT_SEL1);  // set to 0 (selected)
  PORT_SEL2 &= ~_BV(BIT_SEL2);  // set to 0 (selected)
}

void switch_select(uint8_t port) {
  if (port == PORT_A) {
    PORT_SEL2 &= ~_BV(BIT_SEL2);  // set to 0 (not selected)
    PORT_SEL1 |= _BV(BIT_SEL1);   // set to 1 (selected)

  } else if (port == PORT_B) {
    PORT_SEL1 &= ~_BV(BIT_SEL1);  // set to 0 (not selected)
    PORT_SEL2 |= _BV(BIT_SEL2);   // set to 1 (selected)
  }
}

void init(void) {
  DDR_LED |= _BV(BIT_LED);      // enable output
  PORT_LED |= _BV(BIT_LED);     // set to 1 => LED ON

  // init modules
  i2c_init();
  init_select();
  joystick_init();
  paddle_init();

  _delay_ms(1);

  // select i2c port A
  switch_select(PORT_A);
  _delay_ms(1);
  controller_init();
  _delay_ms(1);

  // select i2c port B
  switch_select(PORT_B);
  _delay_ms(1);
  controller_init();
  _delay_ms(1);
}

int main(void) {
  init();

  uint8_t port = PORT_A;
  ContollerData cd;

  // MAIN LOOP
  while (1) {

    // select I2C port
    switch_select(port);

    // get data from port
    controller_read(&cd);

    // give data to c64 joystick port
    joystick_poll(&cd, port);

    // toogle port
    port = (port == PORT_A) ? PORT_B : PORT_A;

    /*
        if (cd.byte5 & _BV(6)) {
          PORT_LED |= _BV(BIT_LED);
        } else {
          PORT_LED &= ~_BV(BIT_LED);
          _delay_ms(1);
        }
    */
  }

  return 0;
}
