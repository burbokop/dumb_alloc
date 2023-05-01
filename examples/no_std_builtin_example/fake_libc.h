#pragma once

#include <stddef.h>
#include <sys/types.h>

#define STDOUT_FD 1
#define STDERR_FD 2

extern void *malloc(size_t size);
extern void free(void *ptr);
extern void *realloc(void *ptr, size_t new_size);

extern void *memcpy(void *dest, const void *src, size_t n);

size_t strlen(const char *str);
ssize_t write(int fd, void const *data, size_t nbytes);
