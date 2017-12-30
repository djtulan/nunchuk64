 
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
