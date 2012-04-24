#include <avr/io.h>
#include <avr/interrupt.h>
#include "fifo.h"
#include "node.h"

/*

  OCR1A = 100 Hz
  OCR1B = brightness

*/

#define FLED            (100 * 0xff)

#define TIMER_CS        64
#define TIMER_CLK       (F_CPU / TIMER_CS)
#define TIMER_OCR_FACT  (uint16_t)(TIMER_CLK / FLED)

#define LED_DDR         DDRB
#define LED_PORT        PORTB
#define LED_BIT         1

volatile uint8_t brightness = 40;

void main(){

  DDRB = _BV(1);
  // WGM = CTC, CS = 64
  TCCR1B = (_BV(WGM12) | _BV(CS10) | _BV(CS11));

  // enable irq on output compare match
  TIMSK = _BV(OCIE1A);// | _BV(OCIE1B));
  
  OCR1A = 2550;
  TCNT1 = 0;
  //OCR1B = brightness * 10;

  //PORTB = _BV(1);
  sei();
  while(1){
    ;
  }

}

ISR(TIMER1_COMPA_vect){
  // recalculate refresh rates
  //LED_PORT &= ~_BV(LED_BIT);
  //LED_PORT =~ LED_PORT;
  PORTB = 2;
}

/*
ISR(TIMER1_COMPB_vect){
  //LED_PORT |= _BV(LED_BIT);
}
*/
