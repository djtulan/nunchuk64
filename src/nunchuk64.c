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
#include "timer.h"

#include "driver_nes_classic.h"
#include "driver_wii_classic.h"
#include "driver_nunchuk.h"

// ===================================
// activate DEBUG
// ===================================
// #define DEBUG

// ===================================
// activate SINGLE CONTROLLER
// ===================================
#define CONTROLLER_A        1
#define CONTROLLER_B        2
// #define SINGLE_CONTROLLER   CONTROLLER_A

Driver *GetDriver(ControllerID id) {
  switch (id) {
    case ID_Nunchuck:
      return &drv_nunchuk;

    case ID_Wii_Classic:
    case ID_Wii_Classic_Pro:
      return &drv_wii_classic;

    case ID_NES_Classic_Mini_Clone:
      return &drv_nes_classic;

    default:
      return &drv_nunchuk;
  }
}

void init(void) {
  // ===================================
  // init modules
  // ===================================
  led_init();         // init led output
  button_init();      // init button input
  init_selector();    // init i2c bus selector
  i2c_init();         // init i2c routines
  joystick_init();    // init joystick outputs
  paddle_init();      // init paddle outputs
  timer_init();       // init timer interrupt

  led_switch(LED_ON); // diagnose (in init)

  _delay_ms(1);

  // ===================================
  // select i2c port A
  // ===================================
#if SINGLE_CONTROLLER != CONTROLLER_B
  switch_selector(PORT_A);
  _delay_ms(1);
  controller_init();  // init controller A
  _delay_ms(1);
#endif

  // ===================================
  // select i2c port B
  // ===================================
#if SINGLE_CONTROLLER != CONTROLLER_A
  switch_selector(PORT_B);
  _delay_ms(1);
  controller_init();  // init controller B
  _delay_ms(1);
#endif

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
  // ===================================
  // init everything
  // ===================================
  init();
  led_switch(LED_BLINK1); // diagnose (init done)

  // ===================================
  // get the correct driver
  // ===================================
  Driver *driver[NUMBER_PORTS] = {NULL, NULL};

  ContollerData cd[NUMBER_PORTS];  // controller data
  Joystick joystick[NUMBER_PORTS] = {0, 0}; // joystick data

#if SINGLE_CONTROLLER == CONTROLLER_A

  for (uint8_t p = PORT_A; p <= PORT_A; p++) {
#elif SINGLE_CONTROLLER == CONTROLLER_B

  for (uint8_t p = PORT_B; p <= PORT_B; p++) {
#else

  for (uint8_t p = PORT_A; p <= PORT_B; p++) {
#endif
    switch_selector(p);
    _delay_ms(1);

    driver[p] = GetDriver(get_id());
  }

  led_switch(LED_OFF); // diagnose (init done)

#ifdef DEBUG
  uint8_t toggle = 0;
#endif

  // ===================================
  // MAIN LOOP
  while (1) {

    button_debounce();

    if (button_get()) {
      led_setnextstate();
    }

    // get data from controller and let translate it by the driver
#if SINGLE_CONTROLLER == CONTROLLER_A

    for (uint8_t p = PORT_A; p <= PORT_A; p++) {
#elif SINGLE_CONTROLLER == CONTROLLER_B

    for (uint8_t p = PORT_B; p <= PORT_B; p++) {
#else

    for (uint8_t p = PORT_A; p <= PORT_B; p++) {
#endif
      // select I2C port
      switch_selector(p);

      // get data from port
      controller_read(&cd[p]);

      // translate the controller date to joystick data
      driver[p]->get_joystick_state(&cd[p], &joystick[p]);
    }

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

  //  MAIN LOOP
  // ===================================

  return 0;
}


