/*
 * WARNING WHIS EXAMPLE WORKING ONLY ON LINUX x86_64
 * CLIB NOT USED IN EXAMPLE SO SYSCALLs MADE DIRECTLY
 * HOWEVER CLIB HEADERS USE BUT NO FUNCTIONS FROM THEM ARE CALLED, ONLY DEFINITIONS
 */

#include "../../src/alloc.h"

#include "fake_libc.h"

int main()
{
    void *m0 = malloc(16);
    void *m1 = malloc(9);

    dumb_memcpy(m1, "gogadoda", 9);
    free(m0);

    write(STDOUT_FD, "m1: ", 4);
    write(STDOUT_FD, m1, 8);
    write(STDOUT_FD, "\n", 1);

    free(m1);

    return 0;
}
