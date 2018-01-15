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
#include "driver_nes_classic.h"
#include "joystick.h"
#include "paddle.h"
#include "enums.h"

void init_select(void) {
  BIT_SET(DDR_SEL1, BIT_SEL1);    // enable output
  BIT_SET(DDR_SEL2, BIT_SEL2);    // enable output

  BIT_CLEAR(PORT_SEL1, BIT_SEL1); // set to 0 (unselected)
  BIT_CLEAR(PORT_SEL2, BIT_SEL2); // set to 0 (unselected)
}

void switch_select(uint8_t port) {
  if (port == PORT_A) {
    BIT_CLEAR(PORT_SEL1, BIT_SEL1);   // set to 0
    BIT_SET(PORT_SEL2, BIT_SEL2);     // set to 1

  } else if (port == PORT_B) {
    BIT_CLEAR(PORT_SEL2, BIT_SEL2);   // set to 0
    BIT_SET(PORT_SEL1, BIT_SEL1);     // set to 1
  }
}

void init(void) {
  BIT_SET(DDR_LED, BIT_LED);   // enable output
  BIT_SET(PORT_LED, BIT_LED);  // set to 1 => LED ON

  // ===================================
  // init modules
  // ===================================
  i2c_init();
  init_select();
  joystick_init();
  paddle_init();

  _delay_ms(1);

  // ===================================
  // select i2c port A
  // ===================================
  switch_select(PORT_A);
  _delay_ms(1);
  controller_init();
  _delay_ms(1);

  // ===================================
  // select i2c port B
  // ===================================
  switch_select(PORT_B);
  _delay_ms(1);
  controller_init();
  _delay_ms(1);

  // ===================================
  // enable interrupts
  // ===================================
  sei();

  // ===================================
  // start paddle routines
  // ===================================
  paddle_start();
}

int main(void) {
  init();

  Driver *driver = &nes_classic; // current default driver
  ContollerData cd[NUMBER_PORTS];
  uint8_t joystick[NUMBER_PORTS];

  // MAIN LOOP
  while (1) {

    // select I2C port
    switch_select(PORT_A);
    // get data from port
    controller_read(&cd[PORT_A]);

    // select I2C port
    switch_select(PORT_B);
    // get data from port
    controller_read(&cd[PORT_B]);


    // translate the controller date to joystick data
    driver->get_joystick_state(cd, joystick);

    joystick_update(joystick[PORT_A], joystick[PORT_B]);


    // give data to c64 joystick port
    // joystick_poll(&cd, port);

//     _delay_ms(10);

    // give data to c64 paddle port
    // paddle_poll(&cd, port);

    // toogle port
    // port = (port == PORT_A) ? PORT_B : PORT_A;

//     if (port == PORT_A) {
//       PORT_LED |= _BV(BIT_LED);
//     } else {
//       PORT_LED &= ~_BV(BIT_LED);
//     }
  }

  return 0;
}
