#include "CollisionSystem.h"
#include "checker.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

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
    cs->pq = createPQ(n * n);
    cs->t = 0;

    return cs;
}
#define MYPRINT                                                                \
    for (int i = 0; i < cs->n; i++) {                                          \
        printf("%d %lf %lf\n", i, cs->particles[i]->rx, cs->particles[i]->ry); \
    }
void predict(CollisionSystem *cs, Particle *a, double limit)
{

    if (!a) return;
    double dt, dtX, dtY;
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

void redraw(CollisionSystem *cs, double limit)
{
    drawTerminal(cs->particles, cs->n);
    Sleep(2000);
    if (cs->t < limit) {
        enqueuePQ(cs->pq, newEvent(NULL, NULL, cs->t + 1.0 / HZ));
    }
}

void simulate(CollisionSystem *cs, double limit)
{
    PQueue *pq = cs->pq;
    Particle **particles = cs->particles;
    for (size_t i = 0; i < cs->n; i++) {
        predict(cs, particles[i], limit);
    }
    enqueuePQ(pq, newEvent(NULL, NULL, 0)); // redraw event

    Event *e;
    Particle *a, *b;

    // the main event-driven simulation loop
    while (!isEmptyPQ(pq)) {
        // MYPRINT
        // get next event, discard if invalidated
        e = dequeuePQ(pq);
        // infoEvent(e);
        if (!isValid(e)) continue;
        a = e->particle1;
        b = e->particle2;

        // printf("%lf %lf %d \n", e->time, cs->t, e->type);
        // if (a)
        //     InfoParticle(a);
        // if (b)
        //     InfoParticle(b);
        // printf("\n");

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
        else if (e->type == noEvent)
            redraw(cs, limit); // redraw event

        // update the priority queue with new collisions involving a or b
        predict(cs, a, limit);
        predict(cs, b, limit);
    }
}

int main(int argc, char const *argv[])
{
    int n = 20;
    Particle *p[20];
    for (size_t i = 0; i < n; i++) {
        p[i] = createRandomParticle();
        // InfoParticle(p[i]);
    }
    CollisionSystem *cs = createCollisionSystem(p, n);
    simulate(cs, 2000);
    return 0;
}
