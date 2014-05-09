/*
 * This file is an example of a list that
 * is derived from the dynamic type list in dynamic_type_list.c
 *
 * This list type defines a list that is of a static type once initialized
 */

#include <stdlib.h> //malloc, ...
#include <string.h> //memcpy, ...
#include <assert.h> //assert, ...

#include "utils.h"  //special types, ie) u32int, u8int
#include "dynamic_type_list.h"  //list definitions
#include "static_type_list.h"

//all functions add a simple size check
bool static_type_list_push_front(list_t *self, void *elem, size_t size)
{
  if(!self || !self->reserve)
    return false;

  assert(((static_list_reserve_t*)self->reserve)->element_sz == size);

  return list_push_front(self, elem, size);
}

bool static_type_list_push_back(list_t *self, void *elem, size_t size)
{
  if(!self || !self->reserve)
    return false;

  assert(((static_list_reserve_t*)self->reserve)->element_sz == size);

  return list_push_back(self, elem, size);
}

bool static_type_list_insert(list_t *self, u32int index, void *elem, size_t size)
{
  if(!self || !self->reserve)
    return false;

  assert(((static_list_reserve_t*)self->reserve)->element_sz == size);
  
  return list_insert(self, index, elem, size);
}

void init_static_type_list_empty_head(list_t *list, size_t elem_sz)
{
  //all values to be set to 0
  list->head = list->tail = 0;
  list->size = 0;

  //set the reserve properly, since element_sz is a constant member
  // it has to be initilized like so and then copied over to avoid the
  // compiler from complaining
  static_list_reserve_t reserve = { .element_sz = elem_sz};

  list->reserve = (void*)malloc(sizeof(static_list_reserve_t));
  memcpy(list->reserve, &reserve, sizeof(static_list_reserve_t));
  list->reserve_sz = sizeof(static_list_reserve_t);

  //assign internal list functions not meant to be called by the user
  // this function is the same for this list type and the dynamic one
  list->__get_node_at__ = __list_get_node_at__;

  //assign corresponding functions
  // only functions that were defined above are needed to be changed
  list->push_front = static_type_list_push_front;
  list->pop_front  = list_pop_front;
  list->push_back  = static_type_list_push_back;
  list->pop_back   = list_pop_back;
  list->at         = list_at;
  list->insert     = static_type_list_insert;
  list->clone      = list_clone;
  list->free_all   = list_free_all;

  return;
}
