#include "thread_pool.h"


void thread_pool_wait(thread_pool *pool, int timeout) {
  DEB("---Waiting tasks done---\n");
  struct timespec now;
  struct timespec wait_until;

  clock_gettime(CLOCK_REALTIME, &wait_until);
  wait_until.tv_sec += timeout;

  pthread_mutex_lock(&pool->mu);
  while (cqueue_size(&pool->task_queue) > 0 || pool->on_work > 0) {
    if (timeout > 0) {
      clock_gettime(CLOCK_REALTIME, &now);
      DEB("---Waiting tasks by timeout. Remaining time: %ld---\n",
          wait_until.tv_sec - now.tv_sec);
      if (wait_until.tv_sec - now.tv_sec <= 0) {
        DEB("---Waiting tasks stopped by timeout---\n");
        break;
      }
      while (!pthread_cond_timedwait(&pool->sig_vacant_thread, &pool->mu,
                                     &wait_until))
        ;
    } else {
      DEB("---Waiting all tasks finish---\n");
      pthread_cond_wait(&pool->sig_vacant_thread, &pool->mu);
    }
  }
  DEB("---All done---\n");

  pthread_mutex_unlock(&pool->mu);
}