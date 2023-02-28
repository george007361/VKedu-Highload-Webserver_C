#include "thread_pool.h"


void thread_pool_run_task(thread_pool *pool, task_t *task) {
  if (!pool) {
    fprintf(stderr, "thread_pool_run_task(): Pool is NULL ptr\n");
    return;
  }

  if (!task) {
    fprintf(stderr, "thread_pool_run_task(): Task is NULL ptr\n");
    return;
  }

  worker_arg *arg = (worker_arg *)malloc(sizeof(worker_arg));
  if (!arg) {
    fprintf(stderr, "thread_pool_run_task(): worker_arg mem alloc err\n");
    return;
  }

  pthread_t worker_thread;
  arg->pool = pool;
  arg->task = task;

  // thread_pool_worker((void*)arg);

  if (pthread_create(&worker_thread, NULL, thread_pool_worker, (void *)arg)) {
    fprintf(stderr, "thread_pool_run_task(): Can't create thread for worker\n");
    free(arg);
    return;
  }

  if (pthread_mutex_lock(&pool->mu)) {
    fprintf(stderr, "thread_pool_worker(): Cant lock mutex\n");
    return;
  }

  ++pool->on_work;
  DEB("thread_pool_run_task(): Tasks on work %d, waiting: %ld\n", pool->on_work,
      cqueue_size(&pool->task_queue));

  pthread_mutex_unlock(&pool->mu);
}