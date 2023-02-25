cmake_minimum_required(VERSION 3.12)

project(hl-web-server)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)


add_compile_options(-Wall -Wextra -pedantic -fPIC)

add_subdirectory(src)

add_subdirectory(lib/server)
add_subdirectory(lib/http)

add_executable(${PROJECT_NAME} src/main.c)

target_link_libraries(${PROJECT_NAME} server http)
