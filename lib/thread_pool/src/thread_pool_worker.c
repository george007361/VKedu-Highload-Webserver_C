#include "thread_pool.h"

void *thread_pool_worker(void *varg) {
  worker_arg *arg = (worker_arg *)varg;

  arg->task->routine(arg->task->args);

  pthread_mutex_lock(&arg->pool->mu);

  --arg->pool->on_work;

  pthread_cond_broadcast(&arg->pool->sig_vacant_thread);

  pthread_mutex_unlock(&arg->pool->mu);

  free(arg->task->args);
  free(arg->task);
  free(arg);

  return NULL;
}
