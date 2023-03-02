#include "thread_pool.h"

void *thread_pool_manager(void *vpool) {
  if (!vpool) {
    L_ERR("thread_pool", "manager", "Pool not provided");
    return NULL;
  }

  thread_pool *pool = (thread_pool *)vpool;

  while (1) {
    pthread_mutex_lock(&pool->mu);
    while (pool->on_work == pool->max_threads ||
           cqueue_size(&pool->task_queue) == 0) {
      if (pool->on_work == pool->max_threads) {
        L_DEB("thread_pool", "manager", "All threads are busy. Waiting...");
        pthread_cond_wait(&pool->sig_vacant_thread, &pool->mu);
      }

      if (cqueue_size(&pool->task_queue) == 0) {
        L_DEB("thread_pool", "manager", "No tasks. Waiting...");
        pthread_cond_wait(&pool->sig_new_task, &pool->mu);
      }
    }

    L_DEB("thread_pool", "manager", "Picking next task");

    task_t *next_task = (task_t *)cqueue_take(&pool->task_queue);
    if (!next_task) {
      L_ERR("thread_pool", "manager", "Picking from empty queue");
      continue;
    }

    pthread_mutex_unlock(&pool->mu);

    thread_pool_run_task(pool, next_task);
  }

  return NULL;
}
