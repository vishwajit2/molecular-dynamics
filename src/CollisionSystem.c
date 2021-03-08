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

void simulate(CollisionSystem *cs, double limit)
{
    PQueue *pq = cs->pq;
    Event **particles = cs->particles;
    for (size_t i = 0; i < cs->n; i++) {
        predict(cs, particles[i], limit);
    }
    enqueuePQ(pq, newEvent(NULL, NULL, 0)); // redraw event

    Event *e;
    Particle *a, *b;
    // the main event-driven simulation loop
    while (!isEmpty(pq)) {

        // get next event, discard if invalidated
        e = dequeuePQ(pq);
        if (!isValid(e)) continue;
        a = e->particle1;
        b = e->particle2;

        // physical collision, so update positions, and then simulation clock
        for (int i = 0; i < cs->n; i++) {
            move(particles[i], e->time - cs->t);
        }
        cs->t = e->time;

        // process event
        if (e->type == particleCollision)
            bounceOff(a, b); // particle-particle collision
        else if (e->type == wallCollisionX)
            bounceOffVerticalWall(a); // particle-wall collision
        else if (e->type == wallCollisionY)
            bounceOffHorizontalWall(b); // particle-wall collision
        // else if (e->type == noEvent)
        // redraw(limit); // redraw event

        // update the priority queue with new collisions involving a or b
        predict(cs, a, limit);
        predict(cs, b, limit);
    }
}