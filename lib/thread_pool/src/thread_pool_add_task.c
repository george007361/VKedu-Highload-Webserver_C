#include "thread_pool.h"

int thread_pool_add_task(thread_pool *pool, void *(*routine)(void *),
                          void *args) {
  if (!pool) {
    L_ERR("thread_pool", "add_task", "Pool not provided");
    return ST_ERROR;
  }

  if (!routine) {
    L_ERR("thread_pool", "add_task", "Routine not provided");
    return ST_ERROR;
  }

  pthread_mutex_lock(&pool->mu);
  const int q_size = cqueue_size(pool->task_queue);
  pthread_mutex_unlock(&pool->mu);

  if(q_size >= pool->task_queue_size){
    return ST_QUEUE_OVERFLOW;
  }

  task_t *new_task = (task_t *)malloc(sizeof(task_t));
  if (!new_task) {
    L_ERR("thread_pool", "add_task", "Can't allocate memory for new task");
        return ST_ERROR;
  }

  new_task->args = args;
  new_task->routine = routine;

  pthread_mutex_lock(&pool->mu);

  pthread_cond_broadcast(&pool->sig_new_task);
  cqueue_push(&pool->task_queue, (void *)new_task);

  pthread_mutex_unlock(&pool->mu);

  return ST_OK;
}
