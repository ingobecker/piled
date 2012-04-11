#include <stdint.h>
#include <stdio.h>
#include "linked_list.h"
#include "fifo.h"
#include "node.h"

void data_rx(node_t *node){

  uint8_t address = fifo_get(&node->data_rx_buffer);
  uint8_t animation = fifo_get(&node->data_rx_buffer);

  node->animation_next =  0;
}

void sensor_handler(node_t *node, uint8_t data){
  node->animation_next = 0;
  node->animation_reg = (1<<ANIFF);
  puts("sensi senso");
}
