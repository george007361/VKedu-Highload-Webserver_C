#include "cqueue.h"



int cqueue_push(cqueue *q, void *ptr_item) {
  cqueue_node_t *new_node = (cqueue_node_t *)malloc(sizeof(cqueue_node_t));
  if (!new_node) {
    return EXIT_FAILURE;
  }

  new_node->next = NULL;
  new_node->ptr_item = ptr_item;

  if (!q->top) {
    q->top = q->tail = new_node;
    ++q->size;
    return EXIT_SUCCESS;
  }

  q->tail->next = new_node;
  q->tail = new_node;
  ++q->size;

  return EXIT_SUCCESS;
}