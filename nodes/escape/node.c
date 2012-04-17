#include <stdint.h>
#include <stdio.h>
#include "linked_list.h"
#include "fifo.h"
#include "node.h"

extern uint8_t counter_offset;
extern uint8_t active_node;
extern uint8_t skip_nodes;

void node_data_rx_handler(node_t *node){

  uint8_t next_node = fifo_get(&node->data_rx_buffer);

  // calculating animation offset
  counter_offset = 0;
  if(next_node < active_node){
    if(node->address <= next_node)
      counter_offset = node->address_max - active_node + node->address;
    if(node->address > active_node)
      counter_offset = node->address - active_node;
  }
  else
    if(node->address >active_node && node->address <= next_node)
      counter_offset = node->address - active_node;

  if(counter_offset)
    node->animation_reg = (1<<ANIFF);
  
  active_node = next_node;
}

void node_sensor_handler(node_t *node, uint8_t data){
  if(active_node == node->address){

    node->animation_next = 0;
    node->animation_reg = (1<<ANIFF);

    uint8_t next_node = node->address + skip_nodes;
    if(next_node > node->address_max)
      next_node = next_node - node->address_max;
    
    fifo_put(&node->data_tx_buffer, next_node);
  }
}
