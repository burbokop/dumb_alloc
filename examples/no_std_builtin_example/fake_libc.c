#include "fake_libc.h"

#include <stdbool.h>

#include "../../src/alloc.h"

extern void *syscall5(void *number, void *arg1, void *arg2, void *arg3, void *arg4, void *arg5);

ssize_t write(int fd, void const *data, size_t nbytes)
{
    return (ssize_t) syscall5((void *) 1, /* SYS_write */
                              (void *) (ssize_t) fd,
                              (void *) data,
                              (void *) nbytes,
                              0, /* ignored */
                              0  /* ignored */
    );
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

static void panic(const char *error_message)
{
    write(STDERR_FD, error_message, strlen(error_message));
    write(STDERR_FD, "\n", 1);
}

extern void *memcpy(void *dest, const void *src, size_t n)
{
    return dumb_memcpy(dest, src, n);
}

DUMB_NEW_ALLOCATOR_IN_DATA_SECTION(heap, 1024 * 1024, 1024, panic)

void *malloc(size_t size)
{
    dumb_alloc alloc = heap();
    return dumb_malloc(&alloc, size);
}

void free(void *ptr)
{
    dumb_alloc alloc = heap();
    return dumb_free(&alloc, ptr);
}

void *realloc(void *ptr, size_t new_size)
{
    dumb_alloc alloc = heap();
    return dumb_realloc(&alloc, ptr, new_size);
}
