#include "cqueue.h"

void cqueue_destroy(cqueue *q, void (*item_destroyer)(void *)) {
  if (!q) return;
  //   if(!q->top) return;

  while (q->top) {
    cqueue_node_t *tmp = q->top;
    q->top = q->top->next;
    if (item_destroyer) {
      item_destroyer(tmp->ptr_item);
    }
    free(tmp);
    --q->size;
  }

  q->top = q->tail = NULL;
  q->size = 0;
}