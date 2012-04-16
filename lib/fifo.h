#ifndef __CROSS_FIFO__
#define __CROSS_FIFO__
#ifdef __AVR__
  #include <avr/io.h>
#else
  #include <stdint.h>
#endif

#define FIFO_SIZE       128
#define FIFO_MAX      (FIFO_SIZE - 1)

struct fifo{
  uint8_t buffer[FIFO_SIZE];
  uint8_t fill_size;
  uint8_t r_index;
  uint8_t w_index;
};

#ifdef _SIM_
extern struct fifo sim_tx_buffer_g;
void sim_fifo_buffer_monkeypatch(int on);
#endif

extern void fifo_init(struct fifo *f);
extern uint8_t fifo_get(struct fifo *f);
extern void fifo_put(struct fifo *f, uint8_t v);
#endif
