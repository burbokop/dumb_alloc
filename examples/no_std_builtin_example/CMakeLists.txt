#
# THIS EXAMPLE WORKS ONLY IN LINUX x86_64
#

cmake_minimum_required(VERSION 3.5)

project(dumb_alloc_no_std_builtin_example LANGUAGES C ASM)

add_executable(dumb_alloc_no_std_builtin_example
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/syscall.s
    ${CMAKE_CURRENT_LIST_DIR}/fake_libc.c
    ${CMAKE_CURRENT_LIST_DIR}/fake_libc.h)

target_link_libraries(dumb_alloc_no_std_builtin_example
    dumb_alloc)

target_link_options(dumb_alloc_no_std_builtin_example PRIVATE -nostdlib -nodefaultlibs -nolibc)

