#include "fifo.h"

uint8_t fifo_get(struct fifo *f){
  if(f->r_index == FIFO_MAX)
    f->r_index = 0;
  else
    f->r_index++;
  f->fill_size--;
  return f->buffer[f->r_index];
}

void fifo_put(struct fifo *f, uint8_t v){
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
