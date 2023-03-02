set(LOGGER_LIB_SOURCES
src/logger.c
)

add_library(logger STATIC ${LOGGER_LIB_SOURCES})

target_include_directories(logger PUBLIC include)
