add_library(server STATIC src/server.c)

target_include_directories(server PUBLIC include)

target_link_libraries(server thread_pool)