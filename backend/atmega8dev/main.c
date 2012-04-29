#include <avr/io.h>
#include <avr/interrupt.h>
#include "fifo.h"
#include "node.h"

/*
  sfreg - internal used virtual status-flag register

  This register is used to signal event-states from
  within event handler to other handler-routines.

  It has the following flags that could be raised:

  BEGIN_CYCLE_FLAG  we are at the beginning
                    of a refresh-period

  SENSOR_FLAG       the sensor handler needs
                    to be called

  OUTPUT_FLAG       an frame from the fifo needs
                    to be prepared for output

*/

#define BEGIN_CYCLE_FLAG   0
#define SENSOR_FLAG        1
#define OUTPUT_FLAG        2

volatile uint8_t sfreg;

void main(){

  // do following things once per refresh-period
  if(sfreg & _BV(BEGIN_CYCLE)){
    sfreg |= _BV(SENSOR) | _BV(OUTPUT);
    sfreg &= ~_BV(BEGIN_CYCLE);
  }

  // prepare output frame
  if(sfreg & OUTPUT_FLAG){

    if(node->brightness_buffer.fill_size > 1){
      // get one from fifo and calulate ocr values
      uint8_t brightness = fifo_get(&node->brightness_buffer);
      output_ocr_off = brightness * FLED; 
      output_ocr_on = (0xff - brightness) * FLED;

      // lookup the next value for corretct ocr-mode
      uint8_t brightness_lookup = fifo_lookup(&node->brightness_buffer);
      if(brightness_lookup)
        output_oc_mode_on = COM_HIGH;
      else
        output_oc_mode_on = COM_LOW;

      sfreg &= ~_BV(OUTPUT_FLAG);
    }
    else{
      if(node->brightness_buffer.fill_size == 1){
        if(node->animation_reg & _BV(ANIEOF)){
          uint8_t brightness = fifo_get(&node->brightness_buffer);
          output_ocr_off = brightness * FLED; 
          output_ocr_on = (0xff - brightness) * FLED;
          // we have the last frame here
          // so lets turn the led of after it...
          output_oc_mode_on = COM_LOW;
          // no lookup needed here..
          sfreg &= ~_BV(OUTPUT_FLAG);
        }
      }
    }
  }

  // handle sensor
  if(sfreg & SENSOR_FLAG){
    sfreg &= ~_BV(SENSOR_FLAG);
  }

  // render frame
  if(!(node->animation_reg & _BV(ANIEOF))){
    animation_index[animation_next]();
  }
  // handle data tx
  // handle data rx

}
