// C standard libraries
#include <stdio.h>
#include <stdlib.h>

// my libraries
#include "PQueue.h"
#include "checker.h"

#define LEFT(x) (2 * (x) + 1)
#define RIGHT(x) (2 * (x) + 2)
#define PARENT(x) ((x - 1) / 2)

void heapify_PQ(PQueue *q);

PQueue *createPQ(size_t n)
{
    PQueue *pq = (PQueue *)malloc(sizeof(PQueue));
    NP_CHECK(pq);
    pq->size = 0;
    pq->capacity = n;
    pq->arr = (Event **)malloc(n * sizeof(Event *));
    NP_CHECK(pq->arr);
    return pq;
}

int isEmptyPQ(PQueue *q)
{
    NP_CHECK(q);
    return (0 == q->size);
}

void deletePQ(PQueue *pq)
{
    if (NULL == pq) {
        DEBUG("Priority Queue is already NULL.");
        return;
    }
    free(pq->arr);
    free(pq);
}

void enqueuePQ(PQueue *q, Event *event)
{
    NP_CHECK(q);
    if (q->size >= q->capacity) {
        // size = 3*size/2 + 1 is optimal new size.
        resizePQ(q, ((3 * q->capacity) / 2) + 1);
        if (q->size < q->capacity) {
            printf("Priority Queue is full. we increased capacity");
        }
        else {
            DEBUG("priority queue is full. can't add elements");
            return;
        }
    }
    q->arr[q->size] = event;
    size_t i = q->size++;
    // The new element is swapped with its parent as long as its
    // priority is higher
    Event *tmp = NULL;
    while (i > 0 && compareEvent(q->arr[i], q->arr[PARENT(i)]) == 1) {
        tmp = q->arr[i];
        q->arr[i] = q->arr[PARENT(i)];
        q->arr[PARENT(i)] = tmp;
        i = PARENT(i);
    }
}

Event *peekPQ(PQueue *q)
{
    NP_CHECK(q);
    if (q->size < 1) {
        DEBUG("Priority Queue is empty");
        return NULL;
    }
    return q->arr[0];
}

Event *dequeuePQ(PQueue *q)
{
    Event *data = NULL;
    NP_CHECK(q);
    if (q->size < 1) {
        // Priority Queue is empty
        DEBUG("Priority Queue is empty . Cannot remove another element .");
        return NULL;
    }
    data = q->arr[0];
    q->arr[0] = q->arr[q->size - 1];
    q->size--;
    // Restore heap property s
    heapify_PQ(q);
    return (data);
}

void heapify_PQ(PQueue *q)
{
    /* left index, right index, largest index*/
    Event *tmp = NULL;
    size_t left, right, largest, i = 0;
    NP_CHECK(q);

    while (i < q->size) {

        left = LEFT(i);
        right = RIGHT(i);

        /* Left child exists, compare left child with its parent */
        if (left < q->size && compareEvent(q->arr[left], q->arr[i]) == 1) {
            largest = left;
        }
        else {
            largest = i;
        }

        /* Right child exists, compare right child with the largest element */
        if (right < q->size && compareEvent(q->arr[right], q->arr[largest]) > 0) {
            largest = right;
        }

        /* if largest didn't change, No furthur change required */
        if (largest == i) {
            return;
        }
        else {
            /* Swap between the index at the largest element */
            tmp = q->arr[largest];
            q->arr[largest] = q->arr[i];
            q->arr[i] = tmp;
            /* Moodify i */
            i = largest;
        }
    }
}

void resizePQ(PQueue *q, size_t t)
{
    NP_CHECK(q);
    NP_CHECK(q->arr);
    if (t < q->size) {
        DEBUG("can't resize to size less than current requirement");
    }
    else {
        q->capacity = t;
        q->arr = (Event **)realloc(q->arr, t * sizeof(Event *));
    }
    NP_CHECK(q->arr);
    return;
}

// testing the implementation
void TestPQ()
{
    int n = 10;
    Event *a[10];
    Event *t;
    PQueue *pq = createPQ(n);
    for (int i = 0; i < n; i++) {
        a[i] = newEvent(NULL, NULL, rand());
        enqueuePQ(pq, a[i]);
        printf("%lf ", a[i]->time);
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        t = dequeuePQ(pq);
        printf("%lf ", t->time);
    }
}

// int main()
// {
//     TestPQ();
//     return 0;
// }
