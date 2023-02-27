#include "thread_pool.h"


void *thread_pool_worker(void *varg) {
  worker_arg *arg = (worker_arg *)varg;

  arg->task->routine(arg->task->args);

  if (pthread_mutex_lock(&arg->pool->mu)) {
    fprintf(stderr, "thread_pool_worker(): Cant lock mutex\n");
    free(arg->task->args);
    free(arg->task);
    free(arg);
    // free(varg);
    return NULL;
  }

  --arg->pool->on_work;

  pthread_cond_broadcast(&arg->pool->sig_vacant_thread);

  pthread_mutex_unlock(&arg->pool->mu);

  free(arg->task->args);
  free(arg->task);
  free(arg);
  // free(varg);

  return NULL;
}