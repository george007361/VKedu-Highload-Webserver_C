#include "thread_pool.h"


void thread_pool_add_task(thread_pool *pool, void *(*routine)(void *),
                          void *args) {
  if (!pool) {
    fprintf(stderr, "thread_pool_add_task(): NULL pull\n");
    return;
  }

  if (!routine) {
    fprintf(stderr, "thread_pool_add_task(): Routine func not provided\n");
    return;
  }

  task_t *new_task = (task_t *)malloc(sizeof(task_t));
  if (!new_task) {
    perror("thread_pool_add_task(): ");
    return;
  }

  new_task->args = args;
  new_task->routine = routine;

  if (pthread_mutex_lock(&pool->mu)) {
    fprintf(stderr, "thread_pool_add_task(): Cant lock mutex\n");
    free(new_task);
    return;
  }

  // if (queue_size(&pool->task_queue) == 0) {
  pthread_cond_broadcast(&pool->sig_new_task);
  // }

  cqueue_push(&pool->task_queue, (void *)new_task);

  pthread_mutex_unlock(&pool->mu);
}
