set(CQUEUE_LIB_SOURCES
src/cqueue_create.c
src/cqueue_destroy.c
src/cqueue_push.c
src/cqueue_take.c
src/cqueue_size.c 
)

add_library(cqueue STATIC ${CQUEUE_LIB_SOURCES})

target_include_directories(cqueue PUBLIC include)
