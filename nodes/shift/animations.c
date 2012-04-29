#include "types.h"
#include "fifo.h"
#include "node.h"

// this animation dims up the led
// but the animation begins after an offset
// calculated by the address of this node

uint8_t counter_offset;
uint8_t counter;

void sim_node_setup(){
  sim_node_define_context(2, &counter, &counter_offset);
}

void node_setup(node_t *node){
  // node setup
  node->animation_reg = (1<<ANIFF);
  node->animation_next = 0;
  node->data_length = 1;

  // node context setup
  counter = 0;
  counter_offset = 0;
}

void node_loop(node_t *node){

  if(node->animation_reg & (1<<ANIFF)){
    //counter_offset = 0;
    counter = 0;
  }

  if(counter_offset > node->address){
    uint8_t v;
    v = 0xff - ((counter & 15) * 17);
    //printf("offset=%d, v=%d\n", counter_offset, v);
    fifo_put(&node->brightness_buffer, v);

    counter++;

    if(counter == 16)
        node->animation_reg |= (1<<ANIEOF);
  }
  else
    counter_offset++;
}

/*
void shift_fade_pixel(node_t *node){
#ifdef _SIM_
  uint8_t counter_offset;
  uint8_t counter;
  uint8_t down;
  animation_restore_state(3, &counter_offset, &counter, &down);
#else
  static uint8_t counter_offset = 0;
  static uint8_t counter = 0;
  static uint8_t down = 0;
#endif

  //puts("animation");

  if(node->animation_reg & (1<<ANIFF)){
    counter_offset = 0;
    counter = 0;
    down = 0;
  }

  if(counter_offset > node->address){
    uint8_t v;
    if(down){
      v = 0xff - ((counter & 15) * 17);
      printf("v=%d\n", v);
      fifo_put(&node->brightness_buffer, v);
      puts("down");
    }
    else{
      v = (counter & 15) * 17;
      printf("v=%d\n", v);
      fifo_put(&node->brightness_buffer, v);
      puts("up");
    }
    counter++;

    if((counter & 15) == 0){
      puts("switch");
      if(down)
        node->animation_reg |= (1<<ANIEOF);
      down = ~down;
    }
  }
  else
    counter_offset++;

#ifdef _SIM_
  animation_save_state(3, counter_offset, counter, down);
#endif
}
*/
