set(HTTP_LIB_SOURCES 
src/http_get.c
src/http_head.c
src/http_parse_request.c
src/http_handler.c
src/http_read_request.c
src/http_unknown_method.c
src/http_forbidden.c
src/http_not_found.c
src/http_close_safe.c
src/http_create_path.c
src/http_internal_error.c
src/flength.c
src/fsend_mmap.c
src/fsend_buff.c
src/http_decode_request.c
src/http_content_type.c
# src/http_content_type_magic.c
)

add_library(http STATIC ${HTTP_LIB_SOURCES})

target_include_directories(http PUBLIC include)

# target_link_libraries(http magic)
