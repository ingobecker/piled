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
  //printf("render node id=%d\n", node->address);
  node_loop(node);
}

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

void animation_dim_up(node_t *node){
  int v = (frame_cnt & 15) * 10;
  fifo_put(&node->brightness_buffer, v);
  //printf("brightnes val: %d\n", v);
}
