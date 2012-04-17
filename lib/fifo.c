#include "fifo.h"

#ifdef _SIM_ 
struct fifo sim_tx_buffer_g = {
  .r_index = 0,
  .w_index = 0,
  .fill_size = 0,
};

int sim_tx_buffer_on = 0;

void sim_fifo_buffer_monkeypatch(int on){
  sim_tx_buffer_on = on;
}
#endif

uint8_t fifo_get(struct fifo *f){
  if(f->r_index == FIFO_MAX)
    f->r_index = 0;
  else
    f->r_index++;
  f->fill_size--;
  return f->buffer[f->r_index];
}

void fifo_put(struct fifo *f, uint8_t v){
#ifdef _SIM_
  if(sim_tx_buffer_on)
    f = &sim_tx_buffer_g;
#endif
  if(f->w_index == FIFO_MAX)
    f->w_index = 0;
  else
    f->w_index++;

  f->buffer[f->w_index] = v;
  f->fill_size++;
}

void fifo_init(struct fifo *f){
  f->w_index = 0;
  f->r_index = 0;
  f->fill_size = 0;
}
