#pragma once

// CLib headers used only for definitions like uint8_t, size_t, ...
// This headers can be included from libgcc or created manually
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint8_t *begin;
    uint8_t *end;
} dumb_block;

typedef struct
{
    uint8_t *heap;
    size_t heap_size;
    dumb_block *blocks;
    size_t block_count;
    void (*panic)(const char *);
} dumb_alloc;

extern void *dumb_malloc(dumb_alloc *al, size_t size);
extern void dumb_free(dumb_alloc *al, void *ptr);
extern void *dumb_realloc(dumb_alloc *al, void *ptr, size_t new_size);

extern void *dumb_memcpy(void *dest, const void *src, size_t n);

//extern void dumb_memdump(dumb_alloc *al, const char *comment);

/**
 * Creates heap with size SIZE in stack and allocator pointed to it with name VARIABLE_NAME
 * Max block count = BLOCK_COUNT
 * Panic handler = PANIC (function pointer of signature: void (*)(const char *error_message))
 */
#define DUMB_NEW_ALLOCATOR_IN_STACK(VARIABLE_NAME, SIZE, BLOCK_COUNT, PANIC) \
    uint8_t __heap_##VARIABLE_NAME##__[SIZE]; \
    dumb_block __blocks_##VARIABLE_NAME##__[BLOCK_COUNT]; \
    dumb_alloc VARIABLE_NAME; \
    VARIABLE_NAME.heap = __heap_##VARIABLE_NAME##__; \
    VARIABLE_NAME.heap_size = sizeof(__heap_##VARIABLE_NAME##__); \
    VARIABLE_NAME.blocks = __blocks_##VARIABLE_NAME##__; \
    VARIABLE_NAME.block_count = sizeof(__blocks_##VARIABLE_NAME##__) \
                                / sizeof(__blocks_##VARIABLE_NAME##__[0]); \
    VARIABLE_NAME.panic = PANIC; \
    for (size_t i = 0; \
         i < sizeof(__blocks_##VARIABLE_NAME##__) / sizeof(__blocks_##VARIABLE_NAME##__[0]); \
         ++i) { \
        __blocks_##VARIABLE_NAME##__[i].end = NULL; \
    }

#define DUMB_NEW_ALLOCATOR_IN_DATA_SECTION(VARIABLE_NAME, SIZE, BLOCK_COUNT, PANIC) \
    static uint8_t __heap_##VARIABLE_NAME##__[SIZE]; \
    static dumb_block __blocks_##VARIABLE_NAME##__[BLOCK_COUNT]; \
    static bool __inited_##VARIABLE_NAME##__ = false; \
    static dumb_alloc VARIABLE_NAME() \
    { \
        if (!__inited_##VARIABLE_NAME##__) { \
            for (size_t i = 0; i < sizeof(__blocks_##VARIABLE_NAME##__) \
                                       / sizeof(__blocks_##VARIABLE_NAME##__[0]); \
                 ++i) { \
                __blocks_##VARIABLE_NAME##__[i].end = NULL; \
            } \
            __inited_##VARIABLE_NAME##__ = true; \
        } \
        dumb_alloc result; \
        result.heap = __heap_##VARIABLE_NAME##__; \
        result.heap_size = sizeof(__heap_##VARIABLE_NAME##__); \
        result.blocks = __blocks_##VARIABLE_NAME##__; \
        result.block_count = sizeof(__blocks_##VARIABLE_NAME##__) \
                             / sizeof(__blocks_##VARIABLE_NAME##__[0]); \
        result.panic = PANIC; \
        return result; \
    }

#ifdef __cplusplus
}
#endif
