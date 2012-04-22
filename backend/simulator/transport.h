#ifndef __SIM_TRANSPORT__
#define __SIM_TRANSPORT__
enum transport_state{
  STOP,
  PLAY,
  FRAME_FORWARD,
  PAUSE,
  TRANSPORT_STATE_COUNT
};

struct button{
  int released;
  SDL_Rect rect;
};

typedef struct button button_t;

struct transport_button{
  int button_symbol;
  button_t button;
};

extern linked_list_t *transport_buttons_alloc(void);
extern void transport_bar_draw(SDL_Surface *screen);
extern int transport_buttons_scan(linked_list_t *transport_buttons);
extern void transport_state_transission(int *transport_state, int new_state);
#endif
