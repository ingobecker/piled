#include <stdio.h>
#include <stdint.h>
#include <SDL/SDL.h>
#include "linked_list.h"
#include "fifo.h"
#include "node.h"
#include "pixel.h"
#include "config.h"

linked_list_t *pixels_alloc(int count){

  linked_list_t *pixels = NULL;
  
  while(count--){

    void *pixel = malloc(sizeof(struct sim_pixel));
    if(pixels == NULL)
      pixels = linked_list_create(pixel);
    else
      pixels = linked_list_add(pixels, pixel);
  }

  return pixels;
}

void pixels_init(linked_list_t *pixels, SDL_Surface *screen){

  int id = 0;
  int x_new, y_new;
  int row_index = 0;
  int col_index = 0;

  while(pixels){
    //printf("init pixel id: %d\n", id);
    struct sim_pixel *pixel = pixels->val;

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

    // brightness fifo
    fifo_init(&pixel->node.brightness_buffer);

    // data fifos
    fifo_init(&pixel->node.data_rx_buffer);
    fifo_init(&pixel->node.data_tx_buffer);

    // address
    pixel->node.address = id;

    // animation register
    pixel->node.animation_reg = 0;

    // animation state
    pixel->animation_state = NULL;

    pixels = pixels->next;
    id++;
  }
}

void pixels_draw_border(linked_list_t *pixels, SDL_Surface *screen){
  while(pixels){
    struct sim_pixel *pixel = pixels->val;

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

void animation_state_free(linked_list_t *state){
  while(state){
    uint8_t *val = state->val;
    free(val);
    linked_list_t *state_last = state;
    state = state->next;
    free(state_last);
  }
}

void pixels_free(linked_list_t *pixels){
  while(pixels){
    struct sim_pixel *pixel = pixels->val;
    linked_list_t *p = pixels;
    pixels = pixels->next;
    //animation_state_free(pixel->animation_state);
    free(pixel);
    free(p);
  }
}

struct sim_pixel *pixel_get_clicked(linked_list_t *pixels){

  int x, y;
  uint8_t buttons;

  buttons = SDL_GetMouseState(&x, &y);
  if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
    while(pixels){
      struct sim_pixel *pixel = pixels->val;
      if((x > pixel->rect.x) && (x < (pixel->rect.x + pixel->rect.w)))
        if((y > pixel->rect.y) && (y < (pixel->rect.y + pixel->rect.h))){
          return pixel;
        }
      pixels = pixels->next;
    }
  }
    return NULL;
}
