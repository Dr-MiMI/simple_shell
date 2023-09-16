#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

ssize_t get_line(char **lineptr, size_t *n, int fd)
{
    static char buffer[BUFFER_SIZE];
    static char *p;
    static ssize_t len;
    char *newline;
    size_t total = 0;

    if (lineptr == NULL || n == NULL)
        return -1;

    if (*lineptr == NULL)
    {
        *lineptr = malloc(BUFFER_SIZE);
        if (*lineptr == NULL)
            return -1;
        *n = BUFFER_SIZE;
    }

    while (1)
    {
        if (len == 0)
        {
            len = read(fd, buffer, BUFFER_SIZE);
            p = buffer;
            if (len <= 0)
                return len;
        }

        newline = memchr(p, '\n', len);
        if (newline != NULL)
        {
            size_t num_to_copy = newline - p + 1;
            if (total + num_to_copy + 1 > *n)
            {
                *n *= 2;
                *lineptr = realloc(*lineptr, *n);
                if (*lineptr == NULL)
                    return -1;
            }
            memcpy(*lineptr + total, p, num_to_copy);
            total += num_to_copy;
            (*lineptr)[total] = '\0';
            p = newline + 1;
            len -= num_to_copy;
            return total;
        }
        else
        {
            if (total + len + 1 > *n)
            {
                *n *= 2;
                *lineptr = realloc(*lineptr, *n);
                if (*lineptr == NULL)
                    return -1;
            }
            memcpy(*lineptr + total, p, len);
            total += len;
            len = 0;
        }
    }
}

