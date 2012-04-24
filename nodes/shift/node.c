#include "types.h"
#include "fifo.h"
#include "node.h"

extern uint8_t counter_offset;

void node_data_rx_handler(node_t *node){

  uint8_t address = fifo_get(&node->data_rx_buffer);

  if(node->address > address){
    node->animation_next =  0;
    node->animation_reg = (1<<ANIFF);
    counter_offset = address;
  }
}

void node_sensor_handler(node_t *node, uint8_t data){
  node->animation_next = 0;
  node->animation_reg = (1<<ANIFF);
  //puts("sensi senso");
  fifo_put(&node->data_tx_buffer, node->address);
}
