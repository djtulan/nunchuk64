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

#include "led.h"
#include "button.h"
#include "i2c_master.h"
#include "ioconfig.h"
#include "enums.h"
#include "selector.h"
#include "controller.h"
#include "joystick.h"
#include "paddle.h"

#include "driver_nes_classic.h"
#include "driver_nunchuk.h"

// #define DEBUG

void init(void) {
  // ===================================
  // init modules
  // ===================================
  led_init();         // init led output
  button_init();      // init button input
  i2c_init();         // init i2c routines
  init_selector();    // init i2c bus selector
  joystick_init();    // init joystick outputs
  paddle_init();      // init paddle outputs

  _delay_ms(1);

  // ===================================
  // select i2c port A
  // ===================================
  switch_selector(PORT_A);
  _delay_ms(1);
  controller_init();  // init controller A
  _delay_ms(1);

  // ===================================
  // select i2c port B
  // ===================================
  switch_selector(PORT_B);
  _delay_ms(1);
  controller_init();  // init controller B
  _delay_ms(1);

  // ===================================
  // enable interrupts
  // ===================================
  sei();

  // ===================================
  // start paddle routines
  // ===================================
  paddle_start();

  button_init();      // init button input
}

int main(void) {
  init();

  Driver *driver[NUMBER_PORTS] = {NULL, NULL};

  ContollerData cd[NUMBER_PORTS];
  uint8_t joystick[NUMBER_PORTS];

  switch_selector(PORT_A);
  _delay_ms(1);

  if (get_id() == ID_Nunchuck) {
    driver[PORT_A] = &drv_nunchuk;
  } else {
    driver[PORT_A] = &nes_classic;
  }

  switch_selector(PORT_B);
  _delay_ms(1);

  if (get_id() == ID_Nunchuck) {
    driver[PORT_B] = &drv_nunchuk;
  } else {
    driver[PORT_B] = &nes_classic;
  }

  uint8_t led_on = 0;

#ifdef DEBUG
  uint8_t toggle = 0;
#endif

  // MAIN LOOP
  while (1) {

    button_debounce();

    if (button_get()) {
      led_on = ~led_on;

      if (led_on) {
        led_switch(1);
      } else {
        led_switch(0);
      }
    }

    // select I2C port
    switch_selector(PORT_A);
    // get data from port
    controller_read(&cd[PORT_A]);

    // select I2C port
    switch_selector(PORT_B);
    // get data from port
    controller_read(&cd[PORT_B]);

    // translate the controller date to joystick data
    driver[PORT_A]->get_joystick_state(&cd[PORT_A], &joystick[PORT_A]);
    driver[PORT_B]->get_joystick_state(&cd[PORT_B], &joystick[PORT_B]);

    joystick_update(joystick[PORT_A], joystick[PORT_B]);
    paddle_update(joystick[PORT_A], joystick[PORT_B]);

#ifdef DEBUG
    _delay_ms(100);

    if (toggle == 1) {
      led_switch(0);
      toggle = 0;
    } else {
      led_switch(1);
      toggle = 1;
    }

#endif
  }

  return 0;
}
