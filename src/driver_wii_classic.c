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
/// @file   driver_wii_classic.cpp
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   January, 2018
/// @brief  driver wii classic
//=============================================================================
#include <avr/pgmspace.h>

#include "enums.h"
#include "joystick.h"
#include "led.h"

#include "driver_wii_classic.h"

/// \brief different possible buttons
typedef enum {
  A, B, X, Y, START, SELECT, HOME, NUMBER_BUTTONS
} Button;

/// \brief button mapping
const uint16_t button_map[NUMBER_LED_STATES][NUMBER_BUTTONS] PROGMEM = {
//  BUTTON      BUTTON      BUTTON      BUTTON      BUTTON      BUTTON      BUTTON
//  A           B           X           Y           START       SELECT      HOME
  { UP,         BUTTON,     BUTTON2,    AUTOFIRE,   SPACE,      BUTTON,     BUTTON2 },  // LED OFF
  { BUTTON2,    BUTTON,     BUTTON3,    UP,         SPACE,      BUTTON,     BUTTON2 },  // LED ON
  { UP,         DOWN,       BUTTON,     AUTOFIRE,   SPACE,      BUTTON,     BUTTON2 },  // LED F1 (zschunky Mode)
  { RIGHT,      LEFT,       BUTTON,     0,          SPACE,      BUTTON,     0       }   // LED F2
};

static inline uint16_t map_buttons(Button btn) {
  return pgm_read_word(&button_map[led_get_state()][btn]);
}

/// \brief different possible d pads
typedef enum {
  D_UP, D_DOWN, D_LEFT, D_RIGHT, D_TL, D_TR, NUMBER_DPADS
} DPads;

/// \brief button mapping
const uint16_t dpad_map[NUMBER_LED_STATES][NUMBER_BUTTONS] PROGMEM = {
//  D_UP    D_DOWN  D_LEFT  D_RIGHT D_TL    D_TR
  { UP,     DOWN,   LEFT,   RIGHT,  LEFT,   RIGHT   },  // LED OFF
  { UP,     DOWN,   LEFT,   RIGHT,  LEFT,   RIGHT   },  // LED ON
  { 0,      0,      LEFT,   RIGHT,  BUTTON, BUTTON  },  // LED F1 (zschunky Mode)
  { 0,      0,      0,      0,      0,      0       }   // LED F2
};

static inline uint16_t map_dpad(DPads dpad) {
  return pgm_read_word(&dpad_map[led_get_state()][dpad]);
}

static inline uint16_t left_x(const ContollerData *cd) {
  return (cd->byte[0] & 0x3f);
}

static inline uint16_t left_y(const ContollerData *cd) {
  return (cd->byte[1] & 0x3f);
}

static void get_joystick_state_wii_classic(const ContollerData *cd, Joystick *joystick) {

  // see: http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller

  (*joystick) = 0;

  // BDU - DPAD U
  if ((cd->byte[5] & 0x01) == 0) {
    (*joystick) |= map_dpad(D_UP);
  }

  // BDD - DPAD D
  if ((cd->byte[4] & 0x40) == 0) {
    (*joystick) |= map_dpad(D_DOWN);
  }

  // BDL - DPAD L
  if ((cd->byte[5] & 0x02) == 0) {
    (*joystick) |= map_dpad(D_LEFT);
  }

  // BDR - DPAD R
  if ((cd->byte[4] & 0x80) == 0) {
    (*joystick) |= map_dpad(D_RIGHT);
  }

  // ------------------------------

  // BA - A Button
  if ((cd->byte[5] & 0x10) == 0) {
    (*joystick) |= map_buttons(A);
  }

  // BB - B Button
  if ((cd->byte[5] & 0x40) == 0) {
    (*joystick) |= map_buttons(B);
  }

  // BX - X Button
  if ((cd->byte[5] & 0x08) == 0) {
    (*joystick) |= map_buttons(X);
  }

  // BY - Y Button
  if ((cd->byte[5] & 0x20) == 0) {
    (*joystick) |= map_buttons(Y);
  }

  // ------------------------------

  // BLT - Button left (trigger)
  if ((cd->byte[4] & 0x20) == 0) {
    (*joystick) |= map_dpad(D_TL);
  }

  // BRT - Button right (trigger)
  if ((cd->byte[4] & 0x02) == 0) {
    (*joystick) |= map_dpad(D_TR);
  }

  // ------------------------------

  // B+ - Button Start
  if ((cd->byte[4] & 0x04) == 0) {
    (*joystick) |= map_buttons(START);
  }

  // Home - Button Start
  if ((cd->byte[4] & 0x08) == 0) {
    (*joystick) |= map_buttons(HOME);
  }

  // B- - Button Select
  if ((cd->byte[4] & 0x10) == 0) {
    (*joystick) |= map_buttons(SELECT);
  }

  // ------------------------------

  // Left X and Left Y
  if (led_get_state() == LED_OFF ||
      led_get_state() == LED_ON ||
      led_get_state() == LED_BLINK1) {

    uint8_t lx = left_x(cd);

    if (lx > 43) {
      (*joystick) |= RIGHT;
    } else if (lx < 20) {
      (*joystick) |= LEFT;
    }

    // Analog Joystick Y
    uint8_t ly = left_y(cd);

    if (ly > 43) {
      (*joystick) |= UP;
    } else if (ly < 20) {
      (*joystick) |= DOWN;
    }
  }
}

static void get_paddle_state_wii_classic(const ContollerData *cd, Paddle *paddle) {

  if (led_get_state() == LED_BLINK1 ||
      led_get_state() == LED_BLINK2) {

    int16_t x = left_x(cd) << 4;
    int16_t y = left_y(cd) << 4;

    // x = scale(x, 1.6);
    // y = scale(y, 1.6);

    paddle->axis_x = x;
    paddle->axis_y = y;
  }
}

uint8_t get_paddle_enabled_wii_classic(void) {
  switch (led_get_state()) {
    case LED_BLINK2:
      return TRUE;

    default:
      return FALSE;
  }
}

Driver drv_wii_classic = {
  get_joystick_state_wii_classic,
  get_paddle_state_wii_classic,
  get_paddle_enabled_wii_classic
};
