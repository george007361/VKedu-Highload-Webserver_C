#include "thread_pool.h"

void thread_pool_wait(thread_pool *pool, int timeout) {
  L_INFO("thread_pool", "wait", "Waiting tasks to be done");

  struct timespec now;
  struct timespec wait_until;

  clock_gettime(CLOCK_REALTIME, &wait_until);
  wait_until.tv_sec += timeout;

  pthread_mutex_lock(&pool->mu);
  while (cqueue_size(&pool->task_queue) > 0 || pool->on_work > 0) {
    if (timeout > 0) {
      clock_gettime(CLOCK_REALTIME, &now);
      L_DEB("thread_pool", "wait",
            "Waiting tasks to be done. Remaining time: %ld sec",
            wait_until.tv_sec - now.tv_sec);

      if (wait_until.tv_sec - now.tv_sec <= 0) {
        L_INFO("thread_pool", "wait",
               "Waiting tasks to be done stopped by timeout");
        break;
      }
      while (!pthread_cond_timedwait(&pool->sig_vacant_thread, &pool->mu,
                                     &wait_until))
        ;
    } else {
      pthread_cond_wait(&pool->sig_vacant_thread, &pool->mu);
    }
  }

  L_DEB("thread_pool", "wait", "Waiting tasks ended");

  pthread_mutex_unlock(&pool->mu);
}