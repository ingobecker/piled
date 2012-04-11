#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "linked_list.h"
#include "fifo.h"
#include "node.h"
#include "pixel.h"
#include "animation.h"
#include "config.h"


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

linked_list_t *pixels;

uint16_t frame_cnt;

void main_task(SDL_Surface *screen){

  animation_render_frame(pixels);
  animation_output_frame(screen, pixels);

  struct sim_pixel *pixel = pixel_get_clicked(pixels);
  if(pixel != NULL){
    printf("click detected on pixel %d\n", pixel->node.address);
    sensor_handler(&pixel->node, 1);
  }

/*
  linked_list_t *p = pixels;
  while(p){
    struct sim_pixel *pixel = p->val;
    if(pixel->node.data_rx_buffer.fill_size == pixel->node.data_length)
      data_rx(&pixel->node.rx_buffer);
    p = p->next;
  }
  */
}

void init_task(SDL_Surface *screen){

  pixels = pixels_alloc(PIXEL_COUNT);
  pixels_init(pixels, screen);
  linked_list_t *p = pixels;
  while(p){
    struct sim_pixel *pixel = p->val;
    pixel->node.animation_next = 0;
    p = p->next;
  }
  frame_cnt = 0;

}

SDL_Surface *sdl_init(){
  
  SDL_Surface *screen;
  
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1){
    printf("Can't init SDL:  %s\n", SDL_GetError());
    exit(1);
  }

  atexit(SDL_Quit); 
  screen = SDL_SetVideoMode(640, 480, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if(screen == NULL){
    printf("Can't set video mode: %s\n", SDL_GetError());
    exit(1);
  }

  return screen;
}

int main(int argc, char *argv[])
{
  SDL_Surface *screen;
  SDL_Event event;
  int done = 0;

  screen = sdl_init();
  init_task(screen);
  //debug_task();

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
  pixels_free(pixels);
  
  return 0;
}
