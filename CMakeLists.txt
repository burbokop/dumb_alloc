cmake_minimum_required(VERSION 3.5)

project(dumb_alloc LANGUAGES C)

add_library(dumb_alloc STATIC
    ${CMAKE_CURRENT_LIST_DIR}/src/alloc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/alloc.h)

target_link_options(dumb_alloc PRIVATE -nodefaultlibs -nolibc)

install(TARGETS dumb_alloc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples/no_std_example)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples/no_std_builtin_example)
