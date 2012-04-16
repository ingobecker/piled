#ifndef __SIM_PIXEL__
#define __SIM_PIXEL__

struct pixel{
  SDL_Rect rect;
};

typedef struct pixel pixel_t;

extern linked_list_t *pixels_g;

extern linked_list_t *pixels_alloc(int count);
extern void pixels_free(linked_list_t *pixels);
extern void pixels_init(linked_list_t *pixels, SDL_Surface *screen);
extern void pixels_draw_border(linked_list_t *pixels, SDL_Surface *screen);
extern int pixel_is_clicked(pixel_t *pixel);
extern pixel_t *pixel_get_clicked(linked_list_t *pixels);

#endif
