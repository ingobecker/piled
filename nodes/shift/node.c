#include "types.h"
#include "fifo.h"
#include "node.h"

// this animation dims up the led
// but the animation begins after an offset
// calculated by the address of this node

#define OUTPUT_END          0
#define OUTPUT_PROCESSING   1
#define OUTPUT_START        2

uint8_t counter_offset;
uint8_t counter;
uint8_t output_state;

void sim_node_setup(){
  sim_node_define_context(3, &counter, &counter_offset, &output_state);
}

void node_setup(node_t *node){
  // node setup
  output_state = _BV(OUTPUT_END);
  node->data_length = 1;

  // node context setup
  counter = 0;
  counter_offset = 0;
}

void node_data_rx_handler(node_t *node){

  uint8_t address = fifo_get(&node->data_rx_buffer);

  if(node->address > address){
    output_state = _BV(OUTPUT_START);
    counter_offset = address;
    fifo_put(&node->brightness_buffer, 0);
  }
}

void node_sensor_handler(node_t *node, uint8_t data){
  output_state = _BV(OUTPUT_START);
  counter_offset = node->address;
  fifo_put(&node->data_tx_buffer, node->address);
}

void node_loop(node_t *node){

  if(output_state & _BV(OUTPUT_START)){
    output_state = _BV(OUTPUT_PROCESSING);
    counter = 0;
  }

  if(output_state & _BV(OUTPUT_PROCESSING)){
    if(counter_offset == node->address){
      uint8_t v;
      v = 0xff - ((counter & 15) * 17);
      //printf("offset=%d, v=%d\n", counter_offset, v);
      fifo_put(&node->brightness_buffer, v);

      counter++;

      if(counter == 16)
          output_state = _BV(OUTPUT_END);
    }
    else
      counter_offset++;
  }
}
