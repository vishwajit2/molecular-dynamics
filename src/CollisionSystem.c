#include "CollisionSystem.h"
#include "checker.h"
#include <stdio.h>
#include <stdlib.h>
CollisionSystem *createCollisionSystem(Particle **p, size_t n)
{
    NP_CHECK(p);
    CollisionSystem *cs = (CollisionSystem *)malloc(sizeof(CollisionSystem));
    NP_CHECK(cs);
    cs->n = n;
    cs->particles = (Particle **)malloc(n * sizeof(Particle *));
    for (size_t i = 0; i < n; i++) {
        cs->particles[i] = p[i];
    }
    cs->pq = createPQ(n * n / 2);

    return cs;
}

void predict(CollisionSystem *cs, Particle *a, double limit)
{
    if (!a) return;
    double dt, dtX, dtY;
    Event *e;
    // particle-particle collisions
    for (int i = 0; i < cs->n; i++) {
        dt = timeToHit(a, cs->particles[i]);
        if (cs->t + dt <= limit) {
            enqueuePQ(cs->pq, newEvent(a, cs->particles[i], cs->t + dt));
        }
    }

    // particle-wall collisions
    dtX = timeToHitVerticalWall(a);
    dtY = timeToHitHorizontalWall(a);
    if (cs->t + dtX <= limit) enqueuePQ(cs->pq, newEvent(a, NULL, cs->t + dtX));
    if (cs->t + dtY <= limit) enqueuePQ(cs->pq, newEvent(NULL, a, cs->t + dtY));
}

void deleteCollisionSystem(CollisionSystem *cs)
{
    free(cs->particles);
    free(cs->pq);
    free(cs);
    return;
}