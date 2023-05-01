# dumb_alloc
Simple slow one-threaded C allocator without using CLib for embedded of freestanding apps

## Adding as dependency to your CMake project

```
include(ExternalProject)
ExternalProject_Add(
    DumbAlloc
    GIT_REPOSITORY https://github.com/burbokop/dumb_alloc.git
    CMAKE_ARGS
        -DENABLE_TESTS=OFF
        -DENABLE_EXAMPLES=OFF
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/deps-install
)

include_directories(${CMAKE_BINARY_DIR}/deps-install/include)
link_directories(${CMAKE_BINARY_DIR}/deps-install/lib)
```
