set(HTTP_LIB_SOURCES 
src/http_get.c
src/http_head.c
src/http_parse_request.c
src/http_handler.c
src/http_read_request.c
)
add_library(http STATIC ${HTTP_LIB_SOURCES})

target_include_directories(http PUBLIC include)
