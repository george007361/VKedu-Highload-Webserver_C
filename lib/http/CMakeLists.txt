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
src/http_create_full_path.c
src/http_internal_error.c
src/http_file_length.c
src/fsend_mmap.c
src/fsend_buff.c
src/http_decode_request.c
src/http_content_type.c
src/http_is_dir.c
src/http_check_root_escaping.c
src/http_create_index_path.c
)

add_library(http STATIC ${HTTP_LIB_SOURCES})

target_include_directories(http PUBLIC include)

target_link_libraries(http logger)
