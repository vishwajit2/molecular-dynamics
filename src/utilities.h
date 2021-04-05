#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

void sleep_ms(int milliseconds);

#define NP_CHECK(ptr)                                   \
    {                                                   \
        if (NULL == (ptr))                              \
        {                                               \
            fprintf(stderr, "%s:%d NULL POINTER: %s n", \
                    __FILE__, __LINE__, #ptr);          \
            exit(-1);                                   \
        }                                               \
    }
// for debugging
#define DEBUG(msg) fprintf(stderr, "%s:%d %s", __FILE__, __LINE__, (msg))

#endif