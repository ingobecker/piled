#ifndef __NODE__
#define __NODE__
/*
  node_t:   represents the hardware specific state
            of the node.
*/

struct node{
  uint8_t address;
  uint8_t address_max;
  uint8_t data_length;
  struct fifo brightness_buffer;
  struct fifo data_rx_buffer;
  struct fifo data_tx_buffer;
};

typedef struct node node_t;

extern void node_data_tx(uint8_t data);

/*
  The following callback-functions have to be
  implemented on an application specific manner.

  node_setup():           This function is called once the program starts.
                          Use this function to setup an initial state
                          of the node.
  node_loop():            This functions should contains the node-specific application
                          code and will be looped infinitly.
  node_data_rx_handler(): This function is called every time
                          the node receives an incomming data-frame.
  node_sensor_handler():  This function is called every time
                          the nodes sensor was triggered.
*/

extern void node_setup(node_t *node);
extern void node_loop(node_t *node);
extern void node_data_rx_handler(node_t *node);
extern void node_sensor_handler(node_t *node, uint8_t data);

/*
  This functions needs to be used to serialize
  the context(all global variables) of the node.
  As seen from a hardware perspective, only one
  instance of each global variable exists.

  Seen from the simulators perspective, there are
  multiple instances of the global variables,
  one for each node simulated.

  To allow the user to program without this complexity
  in mind, he should define a context with
  sim_node_define_context() to tell the simulator which
  variables it should take care of.

*/
extern void sim_node_define_context(int count, ...);
#endif
