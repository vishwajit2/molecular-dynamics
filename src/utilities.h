#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>

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

// random integer between 0 and n. (includes 0 but not n)
#define randomInt(n) (rand() % (n))

// random double between min and max (inclusive)
#define randomDouble(min, max) ((min) + (double)rand() / ((double)RAND_MAX / ((max) - (min))))

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif