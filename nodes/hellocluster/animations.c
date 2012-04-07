#include <stdint.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "config.h"
#include "linked_list.h"
#include "fifo.h"
#include "node.h"
#include "animation.h"

// this animation dims up the led
// but the animation begins after an offset
// calculated by the address of this node

void animation_dim_up_state(node_t *node){
#ifdef _SIM_
  uint8_t counter_offset;
  uint8_t counter;
  animation_restore_state(2, &counter_offset, &counter);
#else
  static uint8_t counter = 0;
#endif
  //printf("counter: %d\n", counter);
  if(counter_offset > node->address)
    if(counter < (0x60 - 2)){
      int v = (counter++ & 31) * 10;
      fifo_put(&node->brightness_buffer, v);
    }
    else
      node->animation_reg |= (1<<ANIEOF);
   else
     counter_offset++;

#ifdef _SIM_
  animation_save_state(2, counter_offset, counter);
#endif
}

animation_t animation_index[] = { 
  animation_dim_up_state
  };
