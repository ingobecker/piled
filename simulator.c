#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "config.h"
#include "linked_list.h"
#include "fifo.h"
#include "node.h"
#include "sim_node.h"
#include "pixel.h"
#include "animation.h"
#include "graphics.h"
#include "misc.h"


/*

Mainloop

  For every frame, do the following:

  - [done] render all pixels if they have values in their fifos
  - [done] create a new frame-value by calling the selected animation for the pixel
  - [done] animation state persistence serialization

Communication Layer

  Basics:

  - if an animation is finished the master pixel is free to broadcast
    new data

  - every master pixel broadcasts a value that represents
    the next pixel and the next animation

  - the master pixel is also the starting point for the animation

  - all pixels are aware of their own address and the total number of
    pixels in the cluster

  - the decision made by a pixel could depend on it's environment
    meaning data acquired by it's sensors

  Advanced stuff:

    - in case of errors, the cluster can recover from it keeping up 
      the animation stream

    - cause all pixels are aware of it's own address and the size of
      the whole cluster, if the pixels miss a frame for a rx timeslot
      the next pixel can become a master-pixel

*/


uint16_t frame_cnt;

void init_task(SDL_Surface *screen){

  graphics_draw_background(screen);

  sim_node_setup();
  // alloc
  sim_nodes_g = sim_nodes_alloc(PIXEL_COUNT);
  pixels_g = pixels_alloc(PIXEL_COUNT);
  //init
  pixels_init(pixels_g, screen);
  pixels_draw_border(pixels_g, screen);

  frame_cnt = 0;
}

void main_task(SDL_Surface *screen){

  linked_list_t *sim_nodes = sim_nodes_g;
  linked_list_t *pixels = pixels_g;

  while(sim_nodes){
    sim_node_t *sim_node = sim_nodes->val;
    pixel_t *pixel = pixels->val;

    sim_node_load_context(sim_node->node_context);

    // draw
    animation_draw_pixel_frame(screen, &sim_node->node, pixel);

    // render
    animation_render_pixel_frame(&sim_node->node, pixel);

    // sensor
    sim_fifo_buffer_monkeypatch(1);
    sensor_scan(pixel, sim_node);
    sim_fifo_buffer_monkeypatch(0);

    sim_node_save_context(sim_node->node_context);
    pixels = pixels->next;
    sim_nodes = sim_nodes->next;
  }

  // handle data transfer
  data_rx_process();
}


int main(int argc, char *argv[])
{
  SDL_Surface *screen;
  SDL_Event event;
  int done = 0;

  screen = sdl_init();
  init_task(screen);

  while(!done){

    SDL_Flip(screen);
    Uint32 start = SDL_GetTicks();

    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT:
          done = 1;
          break;
      }
    }

    main_task(screen);
    frame_cnt++;
    if(frame_cnt == STOP_FRAME)
      done = 1;

    if(1000/FPS > (SDL_GetTicks() - start)){
      SDL_Delay((1000/FPS) - (SDL_GetTicks() - start));
    }
  }
  pixels_free(pixels_g);
  
  return 0;
}
