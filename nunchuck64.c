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
/// @file   nunchuck64.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  main
//=============================================================================
 
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/sleep.h>
//#include <avr/pgmspace.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>

void init(void) {
        PORTB |= _BV(5);   // enable POTX/POTY as outputs
        DDRB  |= _BV(5);   // output "1" on both
  
}

int main(void) {
  init();
  
  while(1) {
    PORTB |= _BV(5);   // enable POTX/POTY as outputs
    _delay_ms(1000);

    PORTB &= ~_BV(5);   // enable POTX/POTY as outputs
    _delay_ms(1000);
  }
    
  return 0;
}
