#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "thread_pool_internal.h"

#define MAX_THREADS_DEFAULT 8
#define POOL_STOP_NOW -1
#define POOL_STOP_WAIT 0

void thread_pool_add_task(thread_pool *pool, void *(*routine)(void *),
                          void *args);
void thread_pool_wait(thread_pool *pool, int timeout);
thread_pool *thread_pool_init(const int max_threads);
void thread_pool_destroy(thread_pool *pool, int timeout);

#endif  // THREAD_POOL_H_
