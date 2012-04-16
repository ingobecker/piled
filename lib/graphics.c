#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "config.h"


void graphics_draw_background(SDL_Surface *screen){
  SDL_Surface *bkg = IMG_Load("media/bkg.jpg");
  //SDL_Surface *gpl = IMG_Load("media/gpl3.jpg");

  SDL_BlitSurface(bkg, NULL, screen, NULL);
  //SDL_BlitSurface(gpl3, NULL, screen, NULL);
}

SDL_Surface *sdl_init(){
  
  SDL_Surface *screen;
  
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1){
    printf("Can't init SDL:  %s\n", SDL_GetError());
    exit(1);
  }

  atexit(SDL_Quit); 
  screen = SDL_SetVideoMode(WINDOW_W, WINDOW_H, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if(screen == NULL){
    printf("Can't set video mode: %s\n", SDL_GetError());
    exit(1);
  }

  return screen;
}
