// simulator specific node stuff
#ifndef __SIM_SIM_NODE__
#define __SIM_SIM_NODE__
#ifdef _SIM_

struct sim_node{
  node_t node;
  linked_list_t *node_context;
};

typedef struct sim_node sim_node_t;

extern linked_list_t *sim_nodes_g;
extern linked_list_t *sim_nodes_alloc(int count); // and init...

// node context holds node instance specific
// context variables
extern linked_list_t *sim_node_context_vars_g;
extern void sim_node_define_context(int cnt, ...);
extern void sim_node_load_context(linked_list_t *ctx_vals);
extern void sim_node_save_context(linked_list_t *ctx_vals);
extern void sim_node_free_context(linked_list_t *ctx_vals);

// setting up simulator configuration
extern void sim_node_setup(void);
#endif
#endif
