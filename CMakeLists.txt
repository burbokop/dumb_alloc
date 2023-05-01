cmake_minimum_required(VERSION 3.5)

project(dumb_alloc
    VERSION 0.0.1
    LANGUAGES C)

option(ENABLE_TESTS "Enable tests to build" ON)
option(ENABLE_EXAMPLES "Enable examples to build" ON)
option(LINK_FLAGS "Linker flags" "-nodefaultlibs -nolibc")

add_library(dumb_alloc STATIC
    ${CMAKE_CURRENT_LIST_DIR}/src/alloc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/alloc.h)

target_link_options(dumb_alloc PRIVATE ${LINK_FLAGS})

include(GNUInstallDirs)

include(GNUInstallDirs)

install(TARGETS dumb_alloc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${CMAKE_CURRENT_LIST_DIR}/src/alloc.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dumb)

if(${ENABLE_TESTS})
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
endif()

if(${ENABLE_EXAMPLES})
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples/no_std_example)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples/no_std_builtin_example)
endif()

# Package Generator  #######################################################
set(CPACK_PACKAGE_DESCRIPTION ${CMAKE_PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_VENDOR "burbokop")
set(CPACK_PACKAGE_CONTACT "burbokop@gmail.com")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")
include (CPack)
