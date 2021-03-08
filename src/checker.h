// Assert ptr is not a null pointer
#ifndef CHECKER_H
#define CHECKER_H
#include <stdio.h>
#define NP_CHECK(ptr)                                   \
    {                                                   \
        if (NULL == (ptr)) {                            \
            fprintf(stderr, "%s:%d NULL POINTER: %s n", \
                    __FILE__, __LINE__, #ptr);          \
            exit(-1);                                   \
        }                                               \
    }

// for debugging
#define DEBUG(msg) fprintf(stderr, "%s:%d %s", __FILE__, __LINE__, (msg))

#endif