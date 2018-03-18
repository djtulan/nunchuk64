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
/// @file   led.h
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   January, 2018
/// @brief  led
//=============================================================================
#ifndef _LED_H_
#define _LED_H_

#include <inttypes.h>

typedef enum {
  LED_OFF,      ///< LED is OFF
  LED_ON,       ///< LED is ON
  LED_BLINK1,   ///< LED flashes once
  LED_BLINK2,   ///< LED flashes twice
  // LED_BLINK3,   ///< LED blinks tree times

  NUMBER_LED_STATES
} LED_State;

/**
* @brief init LED
*
*/
extern void led_init(void);

/**
* @brief switch LED
*
* @param state LED_ON / LED_OFF
*/
extern void led_switch(LED_State state);

/**
* @brief toggle to next state
*
*/
extern void led_setnextstate(void);

/**
* @brief init LED
* @return state of led
*/
extern LED_State led_get_state(void);

/**
* @brief quick flash
*/
extern void led_quick_blink(uint8_t number);

/**
* @brief poll led routines (for flashing)
* @note This function is called by timer interrupt routine
*/
extern void led_poll(void);

#endif
