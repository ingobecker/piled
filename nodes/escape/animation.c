#include "types.h"
#include "fifo.h"
#include "node.h"

// this animation dims up the led
// but the animation begins after an offset
// calculated by the address of this node

uint8_t active_node;
uint8_t counter_offset;
uint8_t counter;
uint8_t skip_nodes;

void sim_node_setup(){
  sim_node_define_context(3, &counter, &counter_offset, &active_node, &skip_nodes);
}

void node_setup(node_t *node){
  active_node = 0;
  counter = 16;
  counter_offset = 0;
  skip_nodes = 10;

  node->animation_next = 0;
  node->animation_reg = 0;
  node->data_length = 1;
}

void escape(node_t *node){

  if(node->animation_reg & (1<<ANIFF))
    counter = 0;

  if(!counter_offset){

    if(node->address == active_node)
      fifo_put(&node->brightness_buffer, 0xff);
    else{
      if(counter == 16){
        node->animation_reg |= (1<<ANIEOF);
      }
      else{
        uint8_t v;
        v = 0xff - ((counter & 15) * 17);
        printf("offset=%d, v=%d\n", counter_offset, v);
        fifo_put(&node->brightness_buffer, v);
        counter++;
      }
    }
  }
  else
    counter_offset--;
}

animation_t animation_index[] = { 
  escape
};
