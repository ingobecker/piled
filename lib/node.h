
/*

  node driver backends

    sim
      - everything simulated in software
 
    hw-test
      - communication: uart mpc
      - sensor: switch

    hw-rf
      - communitcation: rfm12
      - sensor: capaitive

    hw-ir
      - communication: ir
      - sensor: capacitive
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

// animation_reg bits

// animation finished
#define ANIEOF        0
// animation first frame requested
#define ANIFF         1


// communication

// handling received data
extern void data_rx_handler(uint8_t data);

// sending data
extern void data_tx(uint8_t data);


// sensors

// handling sensor data
extern void sensor_handler(node_t *node, uint8_t data);


// simulator

#ifdef _SIM_
// setting up simulator configuration
extern void setup(void);
#endif
