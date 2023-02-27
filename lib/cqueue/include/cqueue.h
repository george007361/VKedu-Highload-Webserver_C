#ifndef CQUEUE_H_
#define CQUEUE_H_

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

struct cqueue_node_t {
  void *ptr_item;
  struct cqueue_node_t *next;
};

typedef struct cqueue_node_t cqueue_node_t;

typedef struct cqueue {
  cqueue_node_t *top;
  cqueue_node_t *tail;
  size_t size;
} cqueue;

cqueue cqueue_create();
void cqueue_destroy(cqueue *q, void (*item_destroyer)(void *));
int cqueue_push(cqueue *q, void *ptr_item);
void *cqueue_take(cqueue *q);
size_t cqueue_size(const cqueue *q);

#endif // CQUEUE_H_