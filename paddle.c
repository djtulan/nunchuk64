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
/// @file   paddle.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  analog paddle input part
//=============================================================================
#include <inttypes.h>
#include <avr/interrupt.h>

#include "ioconfig.h"
#include "enums.h"

#include "paddle.h"

static volatile uint16_t ocr1a_load = 41; ///< precalculated OCR1A value (A XPOT)
static volatile uint16_t ocr1b_load = 41; ///< precalculated OCR1B value (A YPOT)
static volatile uint16_t ocr0a_load = 41; ///< precalculated OCR0A value (B XPOT)
static volatile uint16_t ocr0b_load = 41; ///< precalculated OCR0B value (B YPOT)

void paddle_init(void) {
  // SID sensing port
  DDR_SENSE_A  &= ~_BV(BIT_SENSE_A); // SENSE is input
  PORT_SENSE_A &= ~_BV(BIT_SENSE_A); // pullup off, hi-biased by OC1B

  // SID POTX/POTY port
  PORT_PADDLE_A_X &= ~(_BV(BIT_PADDLE_A_X) | _BV(BIT_PADDLE_A_Y)); // = PORT_PADDLE_A_Y
  DDR_PADDLE_A_X  &= ~(_BV(BIT_PADDLE_A_X) | _BV(BIT_PADDLE_A_Y)); // = DDR_PADDLE_A_Y


  // SID sensing port
  DDR_SENSE_B  &= ~_BV(BIT_SENSE_B); // SENSE is input
  PORT_SENSE_B &= ~_BV(BIT_SENSE_B); // pullup off, hi-biased by OC1B

  // SID POTX/POTY port
  PORT_PADDLE_B_X &= ~(_BV(BIT_PADDLE_B_X) | _BV(BIT_PADDLE_B_Y)); // = PORT_PADDLE_B_Y
  DDR_PADDLE_B_X  &= ~(_BV(BIT_PADDLE_B_X) | _BV(BIT_PADDLE_B_Y)); // = DDR_PADDLE_B_Y

  // interrupt
  EIMSK &= ~_BV(INT0);                // disable INT0
  EICRA &= ~(_BV(ISC01) | _BV(ISC00));
  EICRA |= _BV(ISC01);                // ISC11:ISC10 == 10, @negedge

  // interrupt
  EIMSK &= ~_BV(INT1);                // disable INT1
  EICRA &= ~(_BV(ISC11) | _BV(ISC10));
  EICRA |= _BV(ISC11);                // ISC11:ISC10 == 10, @negedge
}

void paddle_start(void) {
  // Initialize Timer1 and use OC1A/OC1B to output values
  // don't count yet
  TCCR1B = 0;
  TCCR0B = 0;

  // POTX/Y normally controlled by output compare unit
  // initially should be pulled up to provide high bias on SENSE pin
  DDR_PADDLE_A_X  |= _BV(BIT_PADDLE_A_X) | _BV(BIT_PADDLE_A_Y);   // enable POTX/POTY as outputs
  PORT_PADDLE_A_X |= _BV(BIT_PADDLE_A_X) | _BV(BIT_PADDLE_A_Y);   // output "1" on both

  DDR_PADDLE_B_X  |= _BV(BIT_PADDLE_B_X) | _BV(BIT_PADDLE_B_Y);   // enable POTX/POTY as outputs
  PORT_PADDLE_B_X |= _BV(BIT_PADDLE_B_X) | _BV(BIT_PADDLE_B_Y);   // output "1" on both

  EIFR  |= _BV(INTF0);  // clear INT1 flag
  EIMSK |= _BV(INT0);   // enable INT1

  EIFR  |= _BV(INTF1);  // clear INT1 flag
  EIMSK |= _BV(INT1);   // enable INT1
}

void paddle_update(Joystick port_a, Joystick port_b) {


// UP
  if (port_a & UP) {
    ocr1a_load += 1;
  }

  // DOWN
  if (port_a & DOWN) {
    ocr1a_load -= 1;
  }

  // LEFT
  if (port_a & LEFT) {
    ocr1b_load += 1;
  }

  // RIGHT
  if (port_a & RIGHT) {
    ocr1b_load -= 1;
  }

  // ===================================
  //  CONTROL PORT B
  // ===================================

  // UP
  if (port_b & UP) {
    ocr0a_load += 1;
  }

  // DOWN
  if (port_b & DOWN) {
    ocr0a_load -= 1;
  }

  // LEFT
  if (port_b & LEFT) {
    ocr0b_load += 1;
  }

  // RIGHT
  if (port_b & RIGHT) {
    ocr0b_load -= 1;
  }


  // ocrXY_load should be in range of 20 - 53
  /*
  diff += adder;

  if (diff >= 53) {
    adder = -1;
  }

  if (diff <= 20) {
    adder = 1;
  }
  */
  /*
  uint8_t up = 0;
  uint8_t down = 0;
  uint8_t left = 0;
  uint8_t right = 0;

  switch (cd->byte5 & 0x0f) {
    case 0x08:
      up = 1;
      break;

    case 0x09:
      left = 1;
      break;

    case 0x0a:
      left = 1;
      up = 1;
      break;
  }

  switch (cd->byte4 & 0xf0) {
    case 0xb0:
      down = 1;
      break;

    case 0x70:
      right = 1;
      break;

    case 0x30:
      right = 1;
      down = 1;
      break;
  }

  if (port == PORT_A) {
    if (up == 1) {
      ocr1a_load += 1;
    }

    if (down == 1) {
      ocr1a_load -= 1;
    }

    if (left == 1) {
      ocr1b_load += 1;
    }

    if (right == 1) {
      ocr1b_load -= 1;
    }

  } else {
    if (up == 1) {
      ocr0a_load += 1;
    }

    if (down == 1) {
      ocr0a_load -= 1;
    }

    if (left == 1) {
      ocr0b_load += 1;
    }

    if (right == 1) {
      ocr0b_load -= 1;
    }
  }
  */

  if (ocr1a_load < 20) ocr1a_load = 20;
  else if (ocr1a_load > 53) ocr1a_load = 53;

  if (ocr1b_load < 20) ocr1b_load = 20;
  else if (ocr1b_load > 53) ocr1b_load = 53;

  if (ocr0a_load < 20) ocr0a_load = 20;
  else if (ocr0a_load > 53) ocr0a_load = 53;

  if (ocr0b_load < 20) ocr0b_load = 20;
  else if (ocr0b_load > 53) ocr0b_load = 53;
}

/// SID measuring cycle detected.
///
/// 1. SID pulls POTX low\n
/// 2. SID waits 256 cycles us\n
/// 3. SID releases POTX\n
/// 4. 0 to 255 cycles until the cap is charged\n
///
/// This handler stops the Timer1, clears OC1A/OC1B outputs,
/// loads the timer with values precalculated in potmouse_movt()
/// and starts the timer.
///
/// OC1A/OC1B (YPOT/XPOT) lines will go up by hardware.
/// Normal SID cycle is 512us. Timer will overflow not before 65535us.
/// Next cycle will begin before that so there's no need to stop the timer.
/// Output compare match interrupts are thus not used.

ISR(INT1_vect) {

  // ===========================================================
  // SID started to measure the pots, uuu
  // disable INT0 until the measurement cycle is complete


  // ===========================================================
  // stop the timer
  TCCR0B = 0;

  // clear OC0A/OC0B:
  // 1. set output compare to clear OC0A/OC0B ("10" in table 37 on page 97)
  TCCR0A = _BV(COM0A1) | _BV(COM0B1);
  // 2. force output compare to make it happen
  TCCR0B |= _BV(FOC0A) | _BV(FOC0B);

  // Set OC0A/OC0B on Compare Match (Set output to high level)
  // WGM13:0 = 00, normal mode: count from BOTTOM to MAX
  TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1) | _BV(COM0B0);

  // load the timer
  TCNT0 = 0;

  // init the output compare values
  OCR0A = ocr0a_load;
  OCR0B = ocr0b_load;

  // start timer with prescaler clk/8 (1 count = 1us)
  //TCCR0B |= _BV(CS01) | _BV(CS00);
  TCCR0B |= _BV(CS01);
}

ISR(INT0_vect) {
  // ===========================================================
  // SID started to measure the pots, uuu
  // disable INT1 until the measurement cycle is complete


  // ===========================================================
  // stop the timer
  TCCR1B = 0;

  // clear OC1A/OC1B:
  // 1. set output compare to clear OC1A/OC1B ("10" in table 37 on page 97)
  TCCR1A = _BV(COM1A1) | _BV(COM1B1);
  // 2. force output compare to make it happen
  TCCR1C |= _BV(FOC1A) | _BV(FOC1B);

  // Set OC1A/OC1B on Compare Match (Set output to high level)
  // WGM13:0 = 00, normal mode: count from BOTTOM to MAX
  TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0);

  // load the timer
  TCNT1 = 0;

  // init the output compare values
  OCR1A = ocr1a_load;
  OCR1B = ocr1b_load;

  // start timer with prescaler clk/8 (1 count = 1us)
  // TCCR1B |= _BV(CS11) | _BV(CS10);
  TCCR1B |= _BV(CS11);
}
