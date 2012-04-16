#include <stdint.h>
#include <stdio.h>
#include "linked_list.h"
#include "fifo.h"
#include "node.h"

extern uint8_t counter_offset;

void data_rx_handler(node_t *node){

  uint8_t address = fifo_get(&node->data_rx_buffer);

  if(node->address > address){
    node->animation_next =  0;
    node->animation_reg = (1<<ANIFF);
    counter_offset = node->address - address;
  }
}

void sensor_handler(node_t *node, uint8_t data){
  node->animation_next = 0;
  node->animation_reg = (1<<ANIFF);
  puts("sensi senso");
  fifo_put(&node->data_tx_buffer, node->address);
}
