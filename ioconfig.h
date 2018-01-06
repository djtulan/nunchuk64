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
/// @brief  main
//=============================================================================
#ifndef _IOCONFIG_H
#define _IOCONFIG_H

#include <avr/io.h>

#define PORT_LED        PORTD // PD4
#define DDR_LED         DDRD  // PD4
#define BIT_LED         4     // PD4

#define PORT_SEL1       PORTC // PC0
#define DDR_SEL1        DDRC  // PC0
#define BIT_SEL1        0     // PC0

#define PORT_SEL2       PORTC // PC1
#define DDR_SEL2        DDRC  // PC1
#define BIT_SEL2        0     // PC0


// dataregister for joystick outputs
#define PORT_JOY_A0     PORTD // PD1
#define PORT_JOY_A1     PORTD // PD0
#define PORT_JOY_A2     PORTC // PC3
#define PORT_JOY_A3     PORTC // PC2
#define PORT_BUTTON_A   PORTB // PB7
#define PORT_JOY_B0     PORTB // PB5
#define PORT_JOY_B1     PORTB // PB3
#define PORT_JOY_B2     PORTB // PB0
#define PORT_JOY_B3     PORTD // PD7
#define PORT_BUTTON_B   PORTB // PB4

// ddr for joystick outputs
#define DDR_JOY_A0      DDRD // PD1
#define DDR_JOY_A1      DDRD // PD0
#define DDR_JOY_A2      DDRC // PC3
#define DDR_JOY_A3      DDRC // PC2
#define DDR_BUTTON_A    DDRB // PB7
#define DDR_JOY_B0      DDRB // PB5
#define DDR_JOY_B1      DDRB // PB3
#define DDR_JOY_B2      DDRB // PB0
#define DDR_JOY_B3      DDRD // PD7
#define DDR_BUTTON_B    DDRB // PB4

// bits for joystick outputs
#define BIT_JOY_A0      1 // PD1
#define BIT_JOY_A1      0 // PD0
#define BIT_JOY_A2      3 // PC3
#define BIT_JOY_A3      2 // PC2
#define BIT_BUTTON_A    7 // PB7
#define BIT_JOY_B0      5 // PB5
#define BIT_JOY_B1      3 // PB3
#define BIT_JOY_B2      0 // PB0
#define BIT_JOY_B3      7 // PD7
#define BIT_BUTTON_B    4 // PB4

#endif
