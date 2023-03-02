#include "thread_pool.h"

void thread_pool_destroy(thread_pool *pool, int timeout) {
  if (timeout != POOL_STOP_NOW) {
    if (timeout == POOL_STOP_WAIT) {
      L_INFO("thread_pool", "destroy", "Waiting all tasks done");
      thread_pool_wait(pool, POOL_STOP_WAIT);
    } else {
      L_INFO("thread_pool", "destroy", "Waiting for timeout: %d sec", timeout);
      thread_pool_wait(pool, timeout);
    }
  }

  L_INFO("thread_pool", "destroy", "Destroying pool");

  cqueue_destroy(&pool->task_queue, free);
  pthread_cancel(pool->manager_thread);
  pthread_cond_destroy(&pool->sig_new_task);
  pthread_cond_destroy(&pool->sig_vacant_thread);
  pthread_mutex_destroy(&pool->mu);
  pool->on_work = 0;
  free(pool);

  L_DEB("thread_pool", "destroy", "Destroying pool success");
}