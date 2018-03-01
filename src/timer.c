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
/// @file   timer.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   March, 2018
/// @brief  timer for different things
//=============================================================================
#include <avr/interrupt.h>

#include "joystick.h"
#include "led.h"

#include "timer.h"

void timer_init(void) {
  // enable timer overflow interrupt for both Timer2
  TIMSK2 |= _BV(TOIE2);

  // set timer2 counter initial value to 0
  TCNT2 = 0x00;

  // start timer2 with /1024 prescaler
  TCCR2B = _BV(CS22) | _BV(CS21);
}

static inline void timer_poll(void) {
  led_poll();
  joystick_poll();
}

// timer2 overflow
ISR(TIMER2_OVF_vect) {
  timer_poll();
}
