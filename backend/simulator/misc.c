#include <stdio.h>
#include <SDL/SDL.h>
#include "config.h"
#include "fifo.h"
#include "linked_list.h"
#include "node.h"
#include "sim_node.h"
#include "pixel.h"

void sensor_scan(pixel_t * pixel, sim_node_t *sim_node){
  if(pixel_is_clicked(pixel)){
    printf("click detected on pixel %d\n", sim_node->node.address);
    node_sensor_handler(&sim_node->node, 1);
  }
}


void data_rx_process(){
  // TODO: there is room for optimization..
  linked_list_t *sim_nodes = sim_nodes_g;
  sim_node_t *lookup_sim_node = sim_nodes_g->val;
  uint8_t frame_length = lookup_sim_node->node.data_length;

  while(sim_tx_buffer_g.fill_size >= frame_length){
    puts("global fifo filled..");
    printf("fillsize=%d\n", sim_tx_buffer_g.fill_size);
    sim_nodes = sim_nodes_g;
    uint8_t backup_r_index = sim_tx_buffer_g.r_index;
    uint8_t backup_fill_size = sim_tx_buffer_g.fill_size;

    while(sim_nodes){
      sim_node_t *sim_node = sim_nodes->val;
      uint8_t byte_to_read = frame_length;
      sim_tx_buffer_g.r_index = backup_r_index;
      sim_tx_buffer_g.fill_size = backup_fill_size;

      while(byte_to_read--){
        uint8_t data = fifo_get(&sim_tx_buffer_g);
        fifo_put(&sim_node->node.data_rx_buffer, data);
      }
      sim_node_load_context(sim_node->node_context);
      node_data_rx_handler(&sim_node->node);
      sim_node_save_context(sim_node->node_context);
      sim_nodes = sim_nodes->next;
    }
  }
}

/*
void data_rx_process(linked_list_t *sim_nodes){
  while(sim_nodes){
    sim_node_t *sim_node = sim_nodes->val;
    if(sim_node->node.data_rx_buffer.fill_size > 0){
      sim_node_load_context(sim_node->node_context);
      data_rx_handler(&sim_node->node);
      sim_node_save_context(sim_node->node_context);
    }
    sim_node = sim_node->next;
  }
}

void data_tx_process(lin){
  if(pixel != NULL){
    if(pixel->node.data_tx_buffer.fill_size > 0){
      puts("found data to transmit");
      uint8_t data = fifo_get(&pixel->node.data_tx_buffer);
      linked_list_t *p = pixels;
      while(p){
        struct sim_pixel *pixel_walk = p->val;
        fifo_put(&pixel_walk->node.data_rx_buffer, data);
        p = p->next;
      }
    }
  }
}

*/
