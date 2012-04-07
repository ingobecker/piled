#ifndef __SIM_ANIMATION__
#define __SIM_ANIMATION__

/*
static inline linked_list_t *animation_get_current_state(){
  return *animation_current_state;
}

static inline void animation_set_current_state(linked_list_t **l){
  animation_current_state = l;
}
*/

extern linked_list_t **animation_current_state;

extern void animation_render_frame(linked_list_t *pixels);
extern void animation_output_frame(SDL_Surface *screen, linked_list_t *pixels);
extern void animation_free_states(linked_list_t *pixels);
extern void animation_restore_state(int count, ...);
extern void animation_save_state(int count, ...);
extern linked_list_t *animation_state_alloc(int count);

typedef void(*animation_t)(node_t *node);
extern animation_t animation_index[];

#endif
