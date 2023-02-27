#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "cqueue.h"

#define MAX_THREADS 1000

#define POOL_STOP_NOW -1
#define POOL_STOP_WAIT 0

#ifdef DEBUG
#define DEB printf
#else
#define DEB(...)
#endif

typedef struct {
  cqueue task_queue;
  int on_work;

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

void thread_pool_add_task(thread_pool *pool, void *(*routine)(void *),
                          void *args);
void *thread_pool_worker(void *varg);
void thread_pool_run_task(thread_pool *pool, task_t *task);

void *thread_pool_manager(void *vpool);

void thread_pool_wait(thread_pool *pool, int timeout);

thread_pool *thread_pool_init();

void thread_pool_destroy(thread_pool *pool, int timeout);

#endif  // THREAD_POOL_H_