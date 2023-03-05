#ifndef CQUEUE_INTERNAL_H_
#define CQUEUE_INTERNAL_H_

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

#endif  // CQUEUE_INTERNAL_H_
