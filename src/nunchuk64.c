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

#include <avr/interrupt.h>
#include <avr/wdt.h>
// #include <util/delay.h>

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

// drivers
#include "driver_nes_classic.h"
#include "driver_nunchuk.h"
#include "driver_wii_classic.h"

static Driver *GetDriver(ControllerID id) {
  switch (id) {
    case ID_Nunchuck:
      return &drv_nunchuk;

    case ID_Wii_Classic:
    case ID_Wii_Classic_Pro:
      return &drv_wii_classic;

    case ID_NES_Classic_Mini_Clone:
      return &drv_nes_classic;

    case MAX_IDs:
      return NULL;
  }

  return NULL;
}

static void init(void) {
  // ===================================
  // init modules
  // ===================================
  led_init();         // init led output
  led_switch(LED_ON); // diagnose (in init)

  button_init();      // init button input
  selector_init();    // init i2c bus selector
  i2c_init();         // init i2c routines
  joystick_init();    // init joystick outputs
  paddle_init();      // init paddle outputs
  timer_init();       // init timer interrupt

  // ===================================
  // enable watchdog, woof
  // ===================================
  wdt_enable(WDTO_500MS);

  // ===================================
  // enable interrupts
  // ===================================
  sei();

  led_switch(LED_OFF); // diagnose (in init)
}

int main(void) {
  // ===================================
  // init everything
  // ===================================
  init();

  // ===================================
  // get the correct driver
  // ===================================
  Driver *driver[NUMBER_PORTS] = {NULL, NULL};

  ContollerData cd[NUMBER_PORTS];  // controller data
  Joystick joystick[NUMBER_PORTS] = {0, 0}; // joystick data
  Paddle paddle[NUMBER_PORTS] = {{0, 0}, {0, 0}}; // joystick data

  // ===================================
  // MAIN LOOP
  while (1) {

    wdt_reset(); // calm watchdog down

    // ================
    // read button
    // ================
    button_debounce();

    if (button_get()) {
      // set to next led state
      led_setnextstate();

      // start / stop paddle depending on state
      switch (led_get_state()) {
        case LED_OFF:
        case LED_ON:
        case LED_BLINK1:
        case NUMBER_LED_STATES:
          paddle_stop();
          break;

        case LED_BLINK2:
        case LED_BLINK3:
          paddle_start();
          break;
      }
      // activate / deactivate paddle
    }

    // ===================================
    // communiate with controllers
    // ===================================
    for (uint8_t p = PORT_A; p <= PORT_B; p++) {
      // select I2C port
      selector_switch(p);
      // _delay_ms(1);

      // ===================================
      // detect controller type, set driver
      // ===================================
      if (driver[p] == NULL) {
        controller_init();

        driver[p] = GetDriver(get_id());

        // ===================================
        // read data from controller
        // ===================================
      } else {
        // controller read failed? -> delete driver
        if (controller_read(&cd[p]) != 0) {
          driver[p] = NULL;
          joystick[p] = 0; // delete old data
        }
      }

      // translate the controller date to joystick data
      if (driver[p] != NULL) {
        driver[p]->get_joystick_state(&cd[p], &joystick[p]);
        driver[p]->get_paddle_state(&cd[p], &paddle[p]);
      }
    }

    // switched mode (while LED == ON)
    if (led_get_state() == LED_ON) {
      joystick_update(joystick[PORT_B], joystick[PORT_A]);
      paddle_update(&paddle[PORT_B], &paddle[PORT_A]);
    } else {
      joystick_update(joystick[PORT_A], joystick[PORT_B]);
      paddle_update(&paddle[PORT_A], &paddle[PORT_B]);
    }
  }

  //  MAIN LOOP
  // ===================================

  return 0;
}
