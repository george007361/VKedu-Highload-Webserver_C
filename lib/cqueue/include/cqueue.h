#ifndef CQUEUE_H_
#define CQUEUE_H_

#include "cqueue_internal.h"

cqueue cqueue_create();
void cqueue_destroy(cqueue *q, void (*item_destroyer)(void *));
int cqueue_push(cqueue *q, void *ptr_item);
void *cqueue_take(cqueue *q);
size_t cqueue_size(const cqueue *q);

#endif // CQUEUE_H_