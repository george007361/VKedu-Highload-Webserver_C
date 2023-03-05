set(THREAD_POOL_LIB_SOURCES
src/thread_pool_add_task.c
src/thread_pool_destroy.c
src/thread_pool_init.c
src/thread_pool_manager.c
src/thread_pool_run_task.c
src/thread_pool_wait.c
src/thread_pool_worker.c
)

add_library(thread_pool STATIC ${THREAD_POOL_LIB_SOURCES})

target_include_directories(thread_pool PUBLIC include)

target_link_libraries(thread_pool cqueue logger pthread)
