#ifndef PQUEUE_H
#define PQUEUE_H

#include "Event.h"
typedef struct PQueue
{
    // heap array to store pointers of Events
    Event **arr;
    // current size of PQueue
    size_t size;
    // total capacity of PQueue
    size_t capacity;
} PQueue;

// creates a Priority queue of size  s and return pointer to it
PQueue *createPQ(size_t n);

int isEmptyPQ(PQueue *q);

// De-allocates memory for a given Priority Queue
void deletePQ(PQueue *q);

// Add an element inside the Priority Queue
void enqueuePQ(PQueue *q, Event *data);

// Removes the element with the greatest priority from  Queue
Event *dequeuePQ(PQueue *q);

// returns item with most priority withoout removing it
Event *peekPQ(PQueue *q);

void resizePQ(PQueue *q, size_t t);
#endif