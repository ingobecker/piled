#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "config.h"
#include "fifo.h"
#include "linked_list.h"
#include "node.h"
#include "sim_node.h"

// TODO: include __SIM__ into build-script instead of config.h!

/*
  Usage:

  uint8_t cnt;
  uint8_t offset;
  uint8_t foo;

  void node_setup(){
    node_create_context(3, &cnt, &offset, &foo);
  }

*/

linked_list_t *sim_nodes_g = NULL;
linked_list_t *sim_node_context_vars_g = NULL;

void sim_node_define_context(int count, ...){
  va_list context_vars;
  va_start(context_vars, count);

  while(count--){

    uint8_t *context_var;
    context_var = va_arg(context_vars, uint8_t *);

    if(sim_node_context_vars_g == NULL)
      sim_node_context_vars_g = linked_list_create(context_var);
    else
      sim_node_context_vars_g = linked_list_add(sim_node_context_vars_g, context_var);
  }
  va_end(context_vars);
}

void sim_node_save_context(linked_list_t *ctx_vals){

  linked_list_t *ctx_vars = sim_node_context_vars_g;

  while(ctx_vars){
    uint8_t *ctx_var = ctx_vars->val;
    uint8_t *ctx_val = ctx_vals->val;
    *ctx_val = *ctx_var;
    ctx_vals = ctx_vals->next;
    ctx_vars = ctx_vars->next;
  }
}

void sim_node_load_context(linked_list_t *ctx_vals){
  
  linked_list_t *ctx_vars = sim_node_context_vars_g;

  while(ctx_vars){
    uint8_t *ctx_var = ctx_vars->val;
    uint8_t *ctx_val = ctx_vals->val;

    *ctx_var = *ctx_val;
    ctx_vals = ctx_vals->next;
    ctx_vars = ctx_vars->next;
  }
}

linked_list_t *sim_node_alloc_context(){

  linked_list_t *ctx_vars = sim_node_context_vars_g;
  linked_list_t *ctx_vals = NULL ;
  uint8_t *ctx_val;
  uint8_t *ctx_var;

  while(ctx_vars){
    if(!(ctx_val = malloc(sizeof(uint8_t)))){
      puts("unable to allocate memory for node context value");
      exit(1);
    }
    ctx_var = ctx_vars->val;
    *ctx_val = *ctx_var;

    if(ctx_vals == NULL)
      ctx_vals = linked_list_create(ctx_val);
    else
      ctx_vals = linked_list_add(ctx_vals, ctx_val);

    ctx_vars = ctx_vars->next;
  }
  return ctx_vals;
}


linked_list_t *sim_nodes_alloc(int count){

  linked_list_t *sim_nodes = NULL;
  sim_node_t *sim_node = NULL;

  while(count--){
    if(!(sim_node = malloc(sizeof(sim_node_t)))){
      puts("unable to allocate memory for sim_node_t");
      exit(1);
    }

    // TODO
    // node could be abstracted by building
    // a node_init function
    // (having nodes with different hardware...)

    // brightness fifo
    fifo_init(&sim_node->node.brightness_buffer);

    // data fifos
    fifo_init(&sim_node->node.data_rx_buffer);
    fifo_init(&sim_node->node.data_tx_buffer);

    // address
    sim_node->node.address = count;
    sim_node->node.address_max = PIXEL_COUNT;

    // animation register

    // sim node context...
    sim_node->node_context = sim_node_alloc_context();

    // run node setup
    sim_node_load_context(sim_node->node_context);
    node_setup(&sim_node->node);
    sim_node_save_context(sim_node->node_context);

    if(sim_nodes == NULL)
      sim_nodes = linked_list_create(sim_node);
    else
      sim_nodes = linked_list_add(sim_nodes, sim_node);
  }
  return sim_nodes;
}
