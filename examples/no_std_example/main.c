/*
 * WARNING WHIS EXAMPLE WORKING ONLY ON LINUX
 * CLIB NOT USED IN EXAMPLE SO SYSCALLs MADE DIRECTLY
 * HOWEVER CLIB HEADERS USE BUT NO FUNCTIONS FROM THEM ARE CALLED, ONLY DEFINITIONS
 */

#include "../../src/alloc.h"
#include <sys/types.h>

#define STDOUT_FD 1
#define STDERR_FD 2

extern void *syscall5(void *number, void *arg1, void *arg2, void *arg3, void *arg4, void *arg5);

typedef size_t uintptr;
typedef ssize_t intptr;

static intptr write(int fd, void const *data, uintptr nbytes)
{
    return (intptr) syscall5((void *) 1, /* SYS_write */
                             (void *) (intptr) fd,
                             (void *) data,
                             (void *) nbytes,
                             0, /* ignored */
                             0  /* ignored */
    );
}

static size_t strlen(const char *str)
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

int main()
{
    DUMB_NEW_ALLOCATOR_IN_STACK(alloc, 1024 * 1024, 1024, panic)

    void *m0 = dumb_malloc(&alloc, 16);
    void *m1 = dumb_malloc(&alloc, 9);

    dumb_memcpy(m1, "gogadoda", 9);
    dumb_free(&alloc, m0);

    write(STDOUT_FD, "m1: ", 4);
    write(STDOUT_FD, m1, 8);
    write(STDOUT_FD, "\n", 1);

    dumb_free(&alloc, m1);

    return 0;
}
