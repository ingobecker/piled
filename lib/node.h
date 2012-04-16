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
  uint8_t animation_next;
  uint8_t animation_reg;
  struct fifo brightness_buffer;
  struct fifo data_rx_buffer;
  struct fifo data_tx_buffer;
};

typedef struct node node_t;

/*
  Animation Register:
  | - | - | - | - | ANIFF | ANIOF |

  ANIFF:  Signals that the output-loop is
          at the first frame.
  ANIEOF: Signals the output-loop that the
          animation reached it's last frame.
*/

#define ANIEOF        0
#define ANIFF         1

extern void node_data_tx(uint8_t data);

/*
  The following functions have to be
  implemented on an application specific manner.

  node_data_rx_handler(): This function is called every time
                          the node receives an incomming data-frame.  node_sensor_handler():  This function is called every time
                          the nodes sensor was triggered.
*/

extern void node_data_rx_handler(node_t *node);
extern void node_sensor_handler(node_t *node, uint8_t data);
#endif
