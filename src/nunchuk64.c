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

static volatile Driver *driver[NUMBER_PORTS] = {NULL, NULL};
static volatile uint8_t ext[NUMBER_PORTS] = {1, 1};

static Driver *GetDriver(ControllerID id) {
  switch (id) {
    case ID_Nunchuck:
      return &drv_nunchuk;

    case ID_Wii_Classic:
    case ID_Wii_Classic_Pro:
    case ID_NES_Classic_Mini_Clone_Encrypted:
    case ID_8Bitdo_SF30:
      return &drv_wii_classic;

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

static Port switch_port(Port p) {
  if (p == PORT_A)
    return PORT_B;
  else
    return PORT_A;
}

static void handle_paddle_enabled(uint8_t switched_ports) {
  for (Port p = PORT_A; p <= PORT_B; p++) {

    Port setport = p;

    if (switched_ports == TRUE) {
      setport = switch_port(setport);
    }

    if (driver[p] != NULL && driver[p]->get_paddle_enabled() == TRUE) {
      ext[p] = 0;
      paddle_start(setport);
    } else {
      paddle_stop(setport);
      ext[p] = 1;
    }
  }
}

int main(void) {
  // ===================================
  // init everything
  // ===================================
  init();


  ContollerData cd[NUMBER_PORTS];  // controller data
  Joystick joystick[NUMBER_PORTS] = {0, 0}; // joystick data
  Paddle paddle[NUMBER_PORTS] = {{0, 0}, {0, 0}}; // joystick data
  uint8_t switched_ports = FALSE;

  // ===================================
  // MAIN LOOP
  while (1) {

    // ================
    // calm watchdog down
    // ================
    wdt_reset();

    // ================
    // read button
    // ================
    button_debounce();

    // short press
    if (button_get() == TRUE) {
      // set to next led state
      led_setnextstate();

      handle_paddle_enabled(switched_ports); // handle paddle enabled
    }

    // long press
    if (button_get_long() == TRUE) {
      switched_ports = (switched_ports == FALSE) ? TRUE : FALSE;

      handle_paddle_enabled(switched_ports); // handle paddle disabled

      led_quick_blink(switched_ports ? 2 : 1);
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

        // new driver found
        if (driver[p] != NULL) {
          handle_paddle_enabled(switched_ports);
        }

        // ===================================
        // read data from controller
        // ===================================
      } else {
        // controller read failed? -> delete driver
        if (controller_read(&cd[p]) == FALSE) {
          driver[p] = NULL;
          joystick[p] = 0; // delete old data
          handle_paddle_enabled(switched_ports);
        }
      }

      // translate the controller date to joystick data
      if (driver[p] != NULL) {
        driver[p]->get_joystick_state(&cd[p], &joystick[p]);
        driver[p]->get_paddle_state(&cd[p], &paddle[p]);
      }
    }

    // switched mode?
    if (switched_ports == FALSE) {

      joystick_update(joystick[PORT_A], ext[PORT_A], joystick[PORT_B], ext[PORT_B]);
      paddle_update(&paddle[PORT_A], &paddle[PORT_B]);
    } else {
      joystick_update(joystick[PORT_B], ext[PORT_B], joystick[PORT_A], ext[PORT_A]);
      paddle_update(&paddle[PORT_B], &paddle[PORT_A]);
    }
  }

  //  MAIN LOOP
  // ===================================

  return 0;
}
