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

void animation_render_pixel_frame(node_t *node, pixel_t *pixel){
  if(!(node->animation_reg & (1<<ANIEOF))){
    printf("render node id=%d\n", node->address);
    animation_index[node->animation_next](node);
    if(node->animation_reg & (1<<ANIFF)){
      pixel->blank = 1;
    }
    node->animation_reg &= ~(1<<ANIFF);
  }
}

/*
void animation_render_frame(linked_list_t *sim_nodes){

  while(sim_nodes){
    //printf("render frame from pixel %d\n", cnt++);
    pixel_t *pixel = pixels->val;
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
*/

void animation_draw_pixel_frame(SDL_Surface *screen, node_t *node, pixel_t *pixel){
  if(node->brightness_buffer.fill_size){
    int color = fifo_get(&node->brightness_buffer);
    SDL_FillRect(screen, &pixel->rect, SDL_MapRGB(screen->format, color, color, color));
  }
  else
    if(pixel->blank){
      SDL_FillRect(screen, &pixel->rect, SDL_MapRGB(screen->format, 0, 0, 0));
      pixel->blank = 0;
    }
}

/*
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
*/

void animation_dim_up(node_t *node){
  int v = (frame_cnt & 15) * 10;
  fifo_put(&node->brightness_buffer, v);
  //printf("brightnes val: %d\n", v);
}

//animation_t animation_index[] = { animation_dim_up, animation_dim_up_state };
