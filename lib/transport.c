#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "linked_list.h"
#include "config.h"
#include "transport.h"


enum transport_buttons{
  BUTTON_STOP,
  BUTTON_PLAY,
  BUTTON_FRAME_FORWARD,
  BUTTON_TRANSPORT_COUNT
};

linked_list_t *transport_buttons_alloc(){

  int button_count = 0;
  linked_list_t *buttons = NULL;
  int start_x = (WINDOW_W / 2) - (144 / 2);

  while(button_count < BUTTON_TRANSPORT_COUNT){
    struct transport_button *b;
    if(!(b = malloc(sizeof(struct transport_button)))){
      puts("Unable to allocate memory for transport button.");
      exit(1);
    }
    if(buttons == NULL)
      buttons = linked_list_create(b);
    else
      buttons = linked_list_add(buttons, b);

    b->button_symbol = button_count;
    b->button.rect.w = 48;
    b->button.rect.h = 48;
    b->button.rect.y = WINDOW_H - 100;
    b->button.rect.x = start_x;
    b->button.released = 1;

    start_x += 48;
    button_count++;
  }
  return buttons;
}

int button_check_collision(int x, int y, SDL_Rect *rect){

  if((x > rect->x) && (x < (rect->x + rect->w)))
    if((y > rect->y) && (y < (rect->y + rect->h)))
      return 1;
  return 0;
}

int button_check_single_click(int x, int y, button_t *button){
  if(button_check_collision(x, y, &button->rect)){
    if(button->released){
      button->released = 0;
      return 1;
    }
  }

  return 0;
}

int transport_buttons_scan(linked_list_t *transport_buttons){

  int x, y;
  uint8_t mouse_buttons;

  mouse_buttons = SDL_GetMouseState(&x, &y);
  while(transport_buttons){
    struct transport_button *tbutton = transport_buttons->val;
    
    if(mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
      if(button_check_single_click(x, y, &tbutton->button))
        return tbutton->button_symbol;
    }
    else
      tbutton->button.released = 1;

    transport_buttons = transport_buttons->next;
  }


  return -1;
}

void transport_state_transission(int *transport_state, int new_state){

  switch(new_state){

    case PLAY:

      if(*transport_state == PAUSE){
        *transport_state = PLAY;
        break;
      }

      if(*transport_state == PLAY){
        *transport_state = PAUSE;
        break;
      }

      if(*transport_state == STOP){
        *transport_state = PLAY;
        break;
      }

    case FRAME_FORWARD:

      if(*transport_state == PAUSE){
        *transport_state = FRAME_FORWARD;
        break;
      }

      if(*transport_state == FRAME_FORWARD){
        *transport_state = PAUSE;
        break;
      }
  }
}

void transport_bar_draw(SDL_Surface *screen){

  SDL_Rect icons_dest = {
    .x = (WINDOW_W / 2) - (148 / 2),
    .y = WINDOW_H - 100,
    .h = 48,
    .w = 144
  };
  
  SDL_Surface *icons = IMG_Load("media/icons_transport.png");
  SDL_BlitSurface(icons, NULL, screen, &icons_dest);
}
