#include "CollisionSystem.h"
#include "checker.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

CollisionSystem *createCollisionSystem(Particle **p, int n)
{
    NP_CHECK(p);
    CollisionSystem *cs = (CollisionSystem *)malloc(sizeof(CollisionSystem));
    NP_CHECK(cs);
    cs->n = n;
    cs->particles = (Particle **)malloc(n * sizeof(Particle *));
    for (size_t i = 0; i < n; i++) {
        cs->particles[i] = p[i];
    }
    cs->pq = createPQ(n * n);
    cs->t = 0;

    return cs;
}

CollisionSystem *randomCollisionSystem(int n)
{
    CollisionSystem *cs = (CollisionSystem *)malloc(sizeof(CollisionSystem));
    srand(time(0));
    NP_CHECK(cs);
    cs->n = n;
    cs->pq = createPQ(n * n);
    cs->t = 0;
    cs->particles = (Particle **)malloc(n * sizeof(Particle *));
    Particle *p;
    for (int i = 0; i < n; i++) {
        p = createRandomParticle();
        for (int j = 0; j < i; j++) {
            while (isOverlapping(p, cs->particles[j])) {
                p->rx = p->radius + ((double)rand() * (1 - 2 * p->radius)) / (double)RAND_MAX;
                p->ry = p->radius + ((double)rand() * (1 - 2 * p->radius)) / (double)RAND_MAX;
            }
        }
        cs->particles[i] = p;
    }
    return cs;
}

void deleteCollisionSystem(CollisionSystem *cs)
{
    free(cs->particles);
    free(cs->pq);
    free(cs);
    return;
}

void redraw(CollisionSystem *cs, double limit)
{
    drawTerminal(cs->particles, cs->n);
    Sleep(20);
    if (cs->t < limit) {
        enqueuePQ(cs->pq, newEvent(NULL, NULL, (cs->t + 1.0 / HZ)));
    }
}

#define MYPRINT                                                                \
    for (int i = 0; i < cs->n; i++) {                                          \
        printf("%d %lf %lf\n", i, cs->particles[i]->rx, cs->particles[i]->ry); \
    }

// the bug that spent 4 hours debugging was due to function call to newEvent
// which was receiving int as parameter. double was expected.
//  such a silly mistake !!!
void predict(CollisionSystem *cs, Particle *a, double limit)
{

    if (!a) return;
    double dt, dtX, dtY;
    // particle-particle collisions
    for (int i = 0; i < cs->n; i++) {
        dt = timeToHit(a, cs->particles[i]);
        if (cs->t + dt <= limit) {
            if (dt < 0) {
                enqueuePQ(cs->pq, newEvent(a, cs->particles[i], cs->t));
            }
            else {
                enqueuePQ(cs->pq, newEvent(a, cs->particles[i], cs->t + dt));
            }
        }
    }

    // particle-wall collisions
    dtX = timeToHitVerticalWall(a);
    dtY = timeToHitHorizontalWall(a);

    if (cs->t + dtX <= limit) enqueuePQ(cs->pq, newEvent(a, NULL, cs->t + dtX));
    if (cs->t + dtY <= limit) enqueuePQ(cs->pq, newEvent(NULL, a, cs->t + dtY));
}

void simulate(CollisionSystem *cs, double limit)
{
    PQueue *pq = cs->pq;
    Particle **particles = cs->particles;
    for (size_t i = 0; i < cs->n; i++) {
        predict(cs, particles[i], limit);
    }
    enqueuePQ(pq, newEvent(NULL, NULL, 0.0)); // redraw event

    Event *e;
    Particle *a, *b;

    // the main event-driven simulation loop
    while (!isEmptyPQ(pq)) {

        // get next event, discard if invalidated
        e = dequeuePQ(pq);
        if (!isValid(e)) continue;
        a = e->particle1;
        b = e->particle2;

        // printf("\n%lf %I64d\n", cs->t, cs->pq->size);
        // printf("%d %lf \n", e->type, e->time);

        // physical collision, so update positions, and then simulation clock
        if (e->time > cs->t)
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
        else if (e->type == noEvent)
            redraw(cs, limit); // redraw event

        // update the priority queue with new collisions involving a or b
        predict(cs, a, limit);
        predict(cs, b, limit);
    }
}

int main(int argc, char const *argv[])
{
    int n = 200;
    CollisionSystem *cs = randomCollisionSystem(n);
    simulate(cs, 1000);
    return 0;
}
