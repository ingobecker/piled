#ifndef __SIM_LINKEDLIST__
#define __SIM_LINKEDLIST__
struct linked_list{
  struct linked_list *next;
  void *val;
};

typedef struct linked_list linked_list_t;

extern linked_list_t *linked_list_create(void *val);
extern linked_list_t *linked_list_add(linked_list_t *l, void *val);
extern void linked_list_free(linked_list_t *l);
#endif
