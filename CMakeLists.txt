cmake_minimum_required(VERSION 3.12)

project(hl-web-server)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)

if(DEBUG)
    add_compile_definitions(DEBUG)
endif(DEBUG)

if(NO_INFO)
    add_compile_definitions(NO_INFO)
endif(NO_INFO)

if(NO_ERR)
    add_compile_definitions(NO_ERR)
endif(NO_ERR)

if(TIME_IT)
    add_compile_definitions(HTTP_TIME_IT)
endif(TIME_IT)

add_compile_options(-Wall -Wextra -pedantic -fPIC)

add_subdirectory(src)

add_subdirectory(lib/server)
add_subdirectory(lib/http)
add_subdirectory(lib/cqueue)
add_subdirectory(lib/thread_pool)
add_subdirectory(lib/logger)

add_executable(${PROJECT_NAME} src/main.c)

target_link_libraries(${PROJECT_NAME} server http )
