#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

linked_list_t *linked_list_create(void *val){

  linked_list_t *l;
  if(!(l=malloc(sizeof(linked_list_t)))){
    puts("can't create ani_state_list_t");
    exit(1);
  }
  l->val = val;
  l->next = NULL;
  return l;
}

linked_list_t *linked_list_add(linked_list_t *l, void *val){

  linked_list_t *head;
  head = linked_list_create(val);
  head->next = l;
  return head;
}

