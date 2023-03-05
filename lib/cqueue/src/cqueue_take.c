#include "cqueue.h"

void *cqueue_take(cqueue *q) {
  void *ptr_item = q->top->ptr_item;
  cqueue_node_t *tmp = q->top;
  q->top = q->top->next;
  free(tmp);
  --q->size;

  return ptr_item;
}
