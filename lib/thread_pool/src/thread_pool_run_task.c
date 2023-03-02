#include "thread_pool.h"

void thread_pool_run_task(thread_pool *pool, task_t *task) {
  L_DEB("thread_pool", "run_task", "Starting next task");

  if (!pool) {
    L_ERR("thread_pool", "run_task", "Pool not provided");
    return;
  }

  if (!task) {
    L_ERR("thread_pool", "run_task", "Task not provided. Losting pointer?");
    return;
  }

  worker_arg *arg = (worker_arg *)malloc(sizeof(worker_arg));
  if (!arg) {
    L_ERR("thread_pool", "run_task", "Can't allocate memory for worker");
    return;
  }

  pthread_t worker_thread;
  arg->pool = pool;
  arg->task = task;

  if (pthread_create(&worker_thread, NULL, thread_pool_worker, (void *)arg)) {
    L_ERR("thread_pool", "run_task", "Can't create thread for worker");
    free(arg);
    return;
  }

  pthread_mutex_lock(&pool->mu);

  ++pool->on_work;

  L_DEB("thread_pool", "run_task",
        "Next task thread: %ld.\n\tTasks on work %d,\n\tTasks waiting: %ld\n", worker_thread,
        pool->on_work, cqueue_size(&pool->task_queue));

  pthread_mutex_unlock(&pool->mu);
}