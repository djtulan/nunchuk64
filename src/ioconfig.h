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
/// @file   ioconfig.h
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  io pin configuration
//=============================================================================
#ifndef _IOCONFIG_H_
#define _IOCONFIG_H_

#include <avr/io.h>

#define BIT_GET(ADR,BIT)   ((ADR) & (_BV(BIT)))
#define BIT_SET(ADR,BIT)   ((ADR) |= (_BV(BIT)))
#define BIT_CLEAR(ADR,BIT) ((ADR) &= ~(_BV(BIT)))
#define BIT_FLIP(ADR,BIT)  ((ADR) ^= (_BV(BIT)))

// ========================================================
//  LED
// ========================================================

// led port, ddr and bit
#define PORT_LED           PORTD // PD4
#define DDR_LED             DDRD // PD4
#define BIT_LED                4 // PD4

// ========================================================
//  BUTTON
// ========================================================

// button port, ddr and bit
#define PIN_BUTTON          PINB // PB6
#define PORT_BUTTON        PORTB // PB6
#define DDR_BUTTON          DDRB // PB6
#define BIT_BUTTON             6 // PB6

// ========================================================
//  I2C Select
// ========================================================

// select1 port, ddr and bit
#define PORT_SEL1          PORTC // PC0
#define DDR_SEL1            DDRC // PC0
#define BIT_SEL1               0 // PC0

// select2 port, ddr and bit
#define PORT_SEL2          PORTC // PC1
#define DDR_SEL2            DDRC // PC1
#define BIT_SEL2               1 // PC1

// ========================================================
//  DIGITAL JOYSTICK OUTPUTS
// ========================================================

// io register for joystick outputs
#define PORT_JOY_A0        PORTD // PD1
#define PORT_JOY_A1        PORTD // PD0
#define PORT_JOY_A2        PORTC // PC3
#define PORT_JOY_A3        PORTC // PC2
#define PORT_BUTTON_A      PORTB // PB7
#define PORT_BUTTON2_A     PORTB // PB1
#define PORT_BUTTON3_A     PORTB // PB2 
#define PORT_JOY_B0        PORTB // PB5
#define PORT_JOY_B1        PORTB // PB3
#define PORT_JOY_B2        PORTB // PB0
#define PORT_JOY_B3        PORTD // PD7
#define PORT_BUTTON_B      PORTB // PB4
#define PORT_BUTTON2_B     PORTD // PD5
#define PORT_BUTTON3_B     PORTD // PD6


// ddr for joystick outputs
#define DDR_JOY_A0          DDRD // PD1
#define DDR_JOY_A1          DDRD // PD0
#define DDR_JOY_A2          DDRC // PC3
#define DDR_JOY_A3          DDRC // PC2
#define DDR_BUTTON_A        DDRB // PB7
#define DDR_BUTTON2_A       DDRB // PB1
#define DDR_BUTTON3_A       DDRB // PB2
#define DDR_JOY_B0          DDRB // PB5
#define DDR_JOY_B1          DDRB // PB3
#define DDR_JOY_B2          DDRB // PB0
#define DDR_JOY_B3          DDRD // PD7
#define DDR_BUTTON_B        DDRB // PB4
#define DDR_BUTTON2_B       DDRD // PD6
#define DDR_BUTTON3_B       DDRD // PD5

// bits for joystick outputs
#define BIT_JOY_A0             1 // PD1
#define BIT_JOY_A1             0 // PD0
#define BIT_JOY_A2             3 // PC3
#define BIT_JOY_A3             2 // PC2
#define BIT_BUTTON_A           7 // PB7
#define BIT_BUTTON2_A          1 // PB1
#define BIT_BUTTON3_A          2 // PB2
#define BIT_JOY_B0             5 // PB5
#define BIT_JOY_B1             3 // PB3
#define BIT_JOY_B2             0 // PB0
#define BIT_JOY_B3             7 // PD7
#define BIT_BUTTON_B           4 // PB4
#define BIT_BUTTON2_B          6 // PD6
#define BIT_BUTTON3_B          5 // PD5

// ========================================================
//  PADDLE INPUTS & OUTPUTS
// ========================================================

// io register for paddle inputs / outputs
#define PORT_SENSE_A       PORTD // PD2
#define PORT_PADDLE_A_X    PORTB // PB1
#define PORT_PADDLE_A_Y    PORTB // PB2

#define PORT_SENSE_B       PORTD // PD3
#define PORT_PADDLE_B_Y    PORTD // PD5
#define PORT_PADDLE_B_X    PORTD // PD6

// ddr for paddle inputs / outputs
#define DDR_SENSE_A         DDRD // PD2
#define DDR_PADDLE_A_X      DDRB // PB1
#define DDR_PADDLE_A_Y      DDRB // PB2

#define DDR_SENSE_B         DDRD // PD3
#define DDR_PADDLE_B_X      DDRD // PD6
#define DDR_PADDLE_B_Y      DDRD // PD5

// bits for paddle sense
#define BIT_SENSE_A            2 // PD2
#define BIT_PADDLE_A_X         1 // PB1
#define BIT_PADDLE_A_Y         2 // PB2

#define BIT_SENSE_B            3 // PD3
#define BIT_PADDLE_B_X         6 // PD6
#define BIT_PADDLE_B_Y         5 // PD5

#endif
