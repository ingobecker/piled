#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <SDL/SDL.h>
#include "linked_list.h"
#include "fifo.h"
#include "node.h"
#include "pixel.h"
#include "animation.h"
#include "config.h"
extern int frame_cnt;

linked_list_t **animation_current_state;

void animation_render_frame(linked_list_t *pixels){

  while(pixels){
    //printf("render frame from pixel %d\n", cnt++);
    struct sim_pixel *pixel = pixels->val;
    node_t *node = &pixel->node;
    if(!(node->animation_reg & (1<<ANIEOF))){
      printf("render node id=%d\n", node->address);
      animation_current_state = &pixel->animation_state;
      animation_index[node->animation_next](node);
      node->animation_reg &= ~(1<<ANIFF);
    }
    pixels = pixels->next;
  }
}

void animation_output_frame(SDL_Surface *screen, linked_list_t *pixels){

  while(pixels){
    struct sim_pixel *pixel = pixels->val;
    if(pixel->node.brightness_buffer.fill_size){
      int color = fifo_get(&pixel->node.brightness_buffer);
      SDL_FillRect(screen, &(pixel->rect), SDL_MapRGB(screen->format, color, color, color));
    }
    pixels = pixels->next;
  }
}

linked_list_t *animation_state_alloc(int count){

  linked_list_t *state = NULL;
  while(count--){

    uint8_t *state_val = malloc(sizeof(uint8_t));
    *state_val = 0;
    if(state == NULL)
      state = linked_list_create(state_val);
    else
      state = linked_list_add(state, state_val);
  }

  return state;
}

void animation_restore_state(int count, ...){
  va_list state_vars;
  va_start(state_vars, count);

  linked_list_t *state = *animation_current_state;

  if(state == NULL){
    *animation_current_state = animation_state_alloc(count);
    state = *animation_current_state;
  }

  while(count-- > 0){

    uint8_t *restored_value;
    restored_value = va_arg(state_vars, uint8_t *);

    uint8_t *val = state->val;
    *restored_value = *val;
    state = state->next;
  }
  va_end(state_vars);
}

void animation_save_state(int count, ...){
  va_list state_vars;
  va_start(state_vars, count);

  linked_list_t *state = *animation_current_state;

  while(count-- > 0){

    uint8_t restored_value;
    restored_value = va_arg(state_vars, int);

    uint8_t *val = state->val;
    *val = restored_value;
    state = state->next;
  }
  va_end(state_vars);
}

/*

void animation_dim_up_state(node_t *node){
#ifdef _SIM_
  uint8_t counter_offset;
  uint8_t counter;
  animation_restore_state(2, &counter_offset, &counter);
#else
  static uint8_t counter = 0;
#endif
  //printf("counter: %d\n", counter);
  if(counter_offset > node->address)
    if(counter < (0x60 - 2)){
      int v = (counter++ & 31) * 10;
      fifo_put(&node->brightness_buffer, v);
    }
    else
      node->animation_reg |= (1<<ANIEOF);
   else
     counter_offset++;

#ifdef _SIM_
  animation_save_state(2, counter_offset, counter);
#endif
}
*/

void animation_dim_up(node_t *node){
  int v = (frame_cnt & 15) * 10;
  fifo_put(&node->brightness_buffer, v);
  //printf("brightnes val: %d\n", v);
}

//animation_t animation_index[] = { animation_dim_up, animation_dim_up_state };

/*
void animation_dim_up_down(struct fifo *b, uint8_t frame_cnt, uint8_t seq_reg){

  #ifdef _SIM_
  linked_list_t *l = animation_get_current_state();
  uint8_t direction;
  if(!l){
    direction = 0;
    l = linked_list_create(direction);
    animation_set_current_state(l);
  }
  else{
    direction = l->val;
  }
  #else
  static uint8_t direction = 1;
  #endif

  if(direction)
    fifo_put(b, (frame_cnt & 15) * 10);
  else
    fifo_put(b, 150 - ((frame_cnt & 15) * 10));

  if((frame_cnt & 15) == 15){
    if(direction)
      direction = 0;
    else
      direction = 1;
  }

  #ifdef _SIM_
  l->val = direction;
  #endif
}
*/
