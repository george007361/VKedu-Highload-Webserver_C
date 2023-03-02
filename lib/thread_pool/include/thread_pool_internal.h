#ifndef THREAD_POOL_INTERNAL_H_
#define THREAD_POOL_INTERNAL_H_

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "cqueue.h"
#include "logger.h"

typedef struct {
  cqueue task_queue;
  int on_work;
  int max_threads;

  pthread_t manager_thread;
  pthread_mutex_t mu;
  pthread_cond_t sig_new_task;
  pthread_cond_t sig_vacant_thread;
} thread_pool;

typedef struct {
  void *(*routine)(void *);
  void *args;
} task_t;

typedef struct {
  thread_pool *pool;
  task_t *task;
} worker_arg;

void *thread_pool_worker(void *varg);
void thread_pool_run_task(thread_pool *pool, task_t *task);
void *thread_pool_manager(void *vpool);

#endif  // THREAD_POOL_INTERNAL_H_