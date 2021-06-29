#include "../CollisionSystem.h"
#include <stdio.h>
#include <stdlib.h>
void analyseTime()
{
    int n = 1000;
    double limit = 10000;
    CollisionSystem *cs = randomCollisionSystem(n);
    int timestamp[10000];
    int size[10000];
    buildEventQueue(cs, limit);
    int i = 0;
    while (cs->t < limit)
    {
        advance(cs, limit);
        printf("%d %ld\n", (int)cs->t, cs->pq->size);
        timestamp[i] = (int)cs->t;
        size[i] = cs->pq->size;
        i++;
    }
    FILE *f = fopen("experiments/timeAnalysis.txt", "w");
    for (int i = 0; i < 10000; i++)
    {
        fprintf(f, "%d %d\n", timestamp[i], size[i]);
    }
    fclose(f);
}

void analyseSize()
{
    double limit = 1000;
    int pq_size[100] = {0};
    int step = 10;
    for (int i = 0; i < 100; i++)
    {
        int max = 0;
        int n = step + step * i;
        CollisionSystem *cs = randomCollisionSystem(n);
        buildEventQueue(cs, limit);
        while (cs->t < limit)
        {
            advance(cs, limit);
            if (cs->pq->size > max)
                max = cs->pq->size;
        }
        pq_size[i] = max;
        printf("%d %d\n", n, max);
    }
    FILE *f = fopen("experiments/sizeAnalysis.txt", "w");
    for (int i = 0; i < 100; i++)
    {
        fprintf(f, "%d %d\n", step + step * i, pq_size[i]);
    }
    fclose(f);
}

int main(int argc, char const *argv[])
{
    // analyseTime();
    analyseSize();
    return 0;
}
