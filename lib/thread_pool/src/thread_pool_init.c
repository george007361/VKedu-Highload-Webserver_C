#include "thread_pool.h"


thread_pool *thread_pool_init(const int max_threads) {
  thread_pool *pool = (thread_pool *)malloc(sizeof(thread_pool));
  if (!pool) {
    perror("thread_pool_init(): ");
    return NULL;
  }

  pthread_cond_init(&pool->sig_new_task, NULL);
  pthread_cond_init(&pool->sig_vacant_thread, NULL);
  pthread_mutex_init(&pool->mu, NULL);
  pool->task_queue = cqueue_create();
  pool->on_work = 0;
  pool->max_threads = max_threads < 1 ? MAX_THREADS_DEFAULT : max_threads;

  pthread_create(&pool->manager_thread, NULL, thread_pool_manager,
                 (void *)pool);

  return pool;
}