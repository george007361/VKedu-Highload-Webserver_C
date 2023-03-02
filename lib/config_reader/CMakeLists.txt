set(CONFIG_READER_LIB_SOURCES
src/config_reader.c
)

add_library(confreader STATIC ${CONFIG_READER_LIB_SOURCES})

target_include_directories(confreader PUBLIC include)
