#ifndef __SIM_PIXEL__
#define __SIM_PIXEL__

struct sim_pixel{
  node_t node;
  //struct fifo brightness_fifo;
  //void (*animation)(struct fifo *b, uint8_t frame_cnt, uint8_t seq_reg);
  linked_list_t *animation_state;
  SDL_Rect rect;
};

extern linked_list_t *pixels_alloc(int count);
extern void pixels_free(linked_list_t *pixels);
extern void pixels_init(linked_list_t *pixels, SDL_Surface *screen);
extern void pixels_draw_border(linked_list_t *pixels, SDL_Surface *screen);
extern struct sim_pixel *pixel_get_clicked(linked_list_t *pixels);

#endif
