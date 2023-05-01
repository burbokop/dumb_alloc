#include "alloc.h"

#include <stdbool.h>

#define OUT_OF_HEAP (al->heap + al->heap_size + 1)

void *dumb_malloc(dumb_alloc *al, size_t size)
{
    if (size == 0) {
        //panic("dumb_malloc: error: trying to allocate zero size");
        return NULL;
    }

    for (size_t i = 0; i < al->block_count; ++i) {
        dumb_block *current_block = &al->blocks[i];
        if (current_block->end == NULL) {
            if (i == 0) {
                current_block->begin = al->heap;
            } else {
                current_block->begin = al->blocks[i - 1].end;
            }
            current_block->end = current_block->begin + size;
            if (current_block->end > (al->heap + al->heap_size)) {
                al->panic("dumb_malloc: error: heap expired");
            }
            return current_block->begin;
        }
    }
    al->panic("dumb_malloc: error: max block count exited");
    return NULL;
}

static void __move_block_to_left__(dumb_alloc *al, uint8_t *dst, dumb_block *b)
{
    if (b->begin > b->end) {
        al->panic("dumb_free: error: block.begin > block.end");
    }
    const size_t size = b->end - b->begin;
    for (size_t o = 0; o < size; ++o) {
        dst[o] = b->begin[o];
    }
    b->begin = dst;
    b->end = dst + size;
}

static void __dealloc__(dumb_alloc *al, dumb_block *b)
{
    b->end = OUT_OF_HEAP; // mark dealloceted
    dumb_block *prev_alloced_block = NULL;
    // move heap to left
    for (size_t i = 0; (i < al->block_count) && al->blocks[i].end; ++i) {
        if (al->blocks[i].end != OUT_OF_HEAP) {
            if (prev_alloced_block && al->blocks[i].begin != prev_alloced_block->end) {
                __move_block_to_left__(al, prev_alloced_block->end, &al->blocks[i]);
            } else if (al->blocks[i].begin != al->heap) {
                __move_block_to_left__(al, al->heap, &al->blocks[i]);
            }
            prev_alloced_block = &al->blocks[i];
        }
    }

    // move blocks to left
    for (size_t i = 0, a = 0; (i < al->block_count) && al->blocks[i].end; ++i) {
        if (al->blocks[i].end != OUT_OF_HEAP) {
            al->blocks[a++] = al->blocks[i];
        }
    }
    // delete dealoced blocks
    for (size_t i = 0, a = 0; (i < al->block_count) && al->blocks[i].end; ++i) {
        if (al->blocks[i].end == OUT_OF_HEAP) {
            al->blocks[i].end = NULL;
        }
    }
}

void dumb_free(dumb_alloc *al, void *ptr)
{
    for (size_t i = 0; i < al->block_count; ++i) {
        if (al->blocks[i].begin == ptr) {
            __dealloc__(al, &al->blocks[i]);
            return;
        }
    }
}

void *dumb_memcpy(void *dest, const void *src, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        ((char *) dest)[i] = ((char *) src)[i];
    }
    return dest;
}

void *dumb_realloc(dumb_alloc *al, void *ptr, size_t new_size)
{
    if (!ptr) {
        return dumb_malloc(al, new_size);
    }
    for (size_t i = 0; i < al->block_count; ++i) {
        if (al->blocks[i].begin == ptr) {
            if (al->blocks[i].end == NULL) {
                al->panic("dumb_realloc: error: ptr is not allocated");
            } else if (al->blocks[i].end == OUT_OF_HEAP) {
                al->panic("dumb_realloc: error: ptr already freed");
            }

            size_t size = al->blocks[i].begin - al->blocks[i].end;
            if (new_size > size) {
                void *new_memory = dumb_malloc(al, new_size);
                dumb_memcpy(new_memory, ptr, size);
                __dealloc__(al, &al->blocks[i]);
                return new_memory;
            } else {
                return ptr;
            }
        }
    }
    al->panic("dumb_realloc: error: block corresponding to ptr not found");
    return NULL;
}

/*
#include <stdio.h>
void dumb_memdump(dumb_alloc *al, const char *comment)
{
    printf("memdump (%s)\n", comment);
    printf("  heap addr (%p)\n", al->heap);
    int last_no_nil_block = -1;
    for (size_t i = 0; i < al->block_count; ++i) {
        printf("  block: %ld\n", i);
        if (al->blocks[i].end == OUT_OF_HEAP) {
            printf("    state: deallocated\n");
        } else if (al->blocks[i].end == NULL) {
            printf("    state: ready to alloc\n");
        } else {
            printf("    state: allocated\n");
            printf("    begin: %p\n", al->blocks[i].begin);
            printf("    end: %p\n", al->blocks[i].end);
            printf("    size: %ld\n", al->blocks[i].end - al->blocks[i].begin);
        }

        if (al->blocks[i].end) {
            last_no_nil_block = i;
        } else {
            break;
        }
    }
    if (last_no_nil_block >= 0) {
        printf("  total heap used: %ld (%d blocks)\n",
               al->blocks[last_no_nil_block].end - al->heap,
               (last_no_nil_block + 1));
    } else {
        printf("  total heap used: %ld (%d blocks)\n", 0l, 0);
    }
    fflush(stdout);
}
*/
