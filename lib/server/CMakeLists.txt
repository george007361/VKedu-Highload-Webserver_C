set(SERVER_LIB_SOURCES
src/server_create_sock.c
src/server_run.c
src/server_stop.c
src/server_destroy.c
src/server_init.c
src/server_worker.c
)

add_library(server STATIC ${SERVER_LIB_SOURCES})

target_include_directories(server PUBLIC include)

target_link_libraries(server thread_pool)