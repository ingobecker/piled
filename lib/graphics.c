#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


void graphics_draw_background(SDL_Surface *screen){
  SDL_Surface *bkg = IMG_Load("media/bkg.jpg");
  SDL_Surface *gpl = IMG_Load("media/gpl3.jpg");

  SDL_BlitSurface(bkg, NULL, screen, NULL);
  SDL_BlitSurface(gpl3, NULL, screen, NULL);
}
