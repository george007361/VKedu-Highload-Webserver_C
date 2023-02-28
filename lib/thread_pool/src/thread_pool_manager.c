#include "thread_pool.h"


void *thread_pool_manager(void *vpool) {
  thread_pool *pool = (thread_pool *)vpool;

  while (1) {
    if (pthread_mutex_lock(&pool->mu)) {
      fprintf(stderr, "thread_pool_manager(): Cant lock mutex\n");
      return NULL;
    }

    while (pool->on_work == pool->max_threads || cqueue_size(&pool->task_queue) == 0) {
      if (pool->on_work == pool->max_threads) {
        DEB("Manager: All threads are busy. Waiting...\n");
        pthread_cond_wait(&pool->sig_vacant_thread, &pool->mu);
      }

      if (cqueue_size(&pool->task_queue) == 0) {
        DEB("Manager: No tasks. Waiting...\n");
        pthread_cond_wait(&pool->sig_new_task, &pool->mu);
      }
    }

    DEB("Manager: Picking task\n");

    task_t *next_task = (task_t *)cqueue_take(&pool->task_queue);
    if (!next_task) {
      fprintf(stderr, "thread_pool_manager(): Picking from empty queue\n");
      continue;
    }
    pthread_mutex_unlock(&pool->mu);

    thread_pool_run_task(pool, next_task);
  }

  return NULL;
}
