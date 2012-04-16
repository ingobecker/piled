#include <stdio.h>
#include <stdint.h>
#include <SDL/SDL.h>
#include "linked_list.h"
#include "fifo.h"
#include "node.h"
#include "pixel.h"
#include "config.h"

linked_list_t *pixels_g = NULL;

linked_list_t *pixels_alloc(int count){

  linked_list_t *pixels = NULL;
  
  while(count--){

    void *pixel = malloc(sizeof(pixel_t));
    if(pixels == NULL)
      pixels = linked_list_create(pixel);
    else
      pixels = linked_list_add(pixels, pixel);
  }

  return pixels;
}

void pixels_init(linked_list_t *pixels, SDL_Surface *screen){

  int x_new, y_new;
  int row_index = 0;
  int col_index = 0;

  while(pixels){
    //printf("init pixel id: %d\n", id);
    pixel_t *pixel = pixels->val;

    x_new = ((PIXEL_SPACE + PIXEL_SIZE) * col_index) + PIXEL_SPACE;
    y_new = ((PIXEL_SPACE + PIXEL_SIZE) * row_index) + PIXEL_SPACE;

    if(x_new + PIXEL_SPACE + PIXEL_SIZE > screen->w){
      row_index++;
      col_index = 0;
      x_new = ((PIXEL_SPACE + PIXEL_SIZE) * col_index) + PIXEL_SPACE;
      y_new = ((PIXEL_SPACE + PIXEL_SIZE) * row_index) + PIXEL_SPACE;
      if(y_new + PIXEL_SPACE + PIXEL_SIZE > screen->h){
        printf("can't display that much pixel.\n");
        exit(1);
      }
    }
      col_index++;

    printf("creating pixel: x=%d, y=%d\n", x_new, y_new);

    // sdl rect
    pixel->rect.h = PIXEL_SIZE;
    pixel->rect.w = PIXEL_SIZE;
    pixel->rect.x = x_new;
    pixel->rect.y = y_new;

    pixels = pixels->next;
  }
}

void pixels_draw_border(linked_list_t *pixels, SDL_Surface *screen){
  while(pixels){
    pixel_t *pixel = pixels->val;

    SDL_Rect border_line;
    border_line.w = PIXEL_BORDER;
    border_line.h = PIXEL_SIZE;
    border_line.y = pixel->rect.y;

    // draw left border
    border_line.x = pixel->rect.x - PIXEL_BORDER;
    SDL_FillRect(screen, &border_line, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));

    // draw right border
    border_line.x = pixel->rect.x + PIXEL_SIZE;
    SDL_FillRect(screen, &border_line, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));

    // draw top border
    border_line.h = PIXEL_BORDER;
    border_line.w = PIXEL_SIZE + PIXEL_BORDER * 2;
    border_line.x = pixel->rect.x - PIXEL_BORDER;
    border_line.y = pixel->rect.y - PIXEL_BORDER;
    SDL_FillRect(screen, &border_line, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));

    // draw bottom border
    border_line.y = pixel->rect.y + PIXEL_SIZE;
    SDL_FillRect(screen, &border_line, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));

    pixels = pixels->next;
  }
}

void pixels_free(linked_list_t *pixels){
  while(pixels){
    pixel_t *pixel = pixels->val;
    linked_list_t *p = pixels;
    pixels = pixels->next;
    free(pixel);
    free(p);
  }
}

int pixel_is_clicked(pixel_t *pixel){
  int x, y;
  uint8_t buttons;

  buttons = SDL_GetMouseState(&x, &y);
  if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
    if((x > pixel->rect.x) && (x < (pixel->rect.x + pixel->rect.w)))
      if((y > pixel->rect.y) && (y < (pixel->rect.y + pixel->rect.h))){
        return 1;
      }
  }
  return 0;
}
