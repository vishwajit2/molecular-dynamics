#include "CollisionSystem.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Simulation.h"

#define ExpectedPQSize(n) (n * (n - 1) / 2 + 10)

CollisionSystem *createCollisionSystem(Particle **p, int n)
{
    NP_CHECK(p);
    CollisionSystem *cs = (CollisionSystem *)malloc(sizeof(CollisionSystem));
    NP_CHECK(cs);
    cs->n = n;
    cs->particles = (Particle **)malloc(n * sizeof(Particle *));
    for (size_t i = 0; i < n; i++)
    {
        cs->particles[i] = p[i];
    }
    cs->pq = createPQ(ExpectedPQSize(n));
    cs->t = 0;

    return cs;
}

CollisionSystem *randomCollisionSystemPure(int n)
{
    CollisionSystem *cs = (CollisionSystem *)malloc(sizeof(CollisionSystem));
    srand(time(0));
    NP_CHECK(cs);
    cs->n = n;
    cs->pq = createPQ(ExpectedPQSize(n));
    cs->t = 0;
    cs->particles = (Particle **)malloc(n * sizeof(Particle *));
    Particle *p;
    int try_limit;
    int j = 0;
    for (int i = 0; i < n; i++)
    {
        try_limit == 20;
        p = createRandomParticle();
        while (j < i)
        {
            if (isOverlapping(p, cs->particles[j]))
            {
                if (try_limit < 0)
                {
                    printf(" This method will take long to generate system.\n Try discrete generator (randomCollisionSystem()) \n");
                    exit(0);
                }

                p->rx = p->radius + ((double)rand() * (1 - 2 * p->radius)) / (double)RAND_MAX;
                p->ry = p->radius + ((double)rand() * (1 - 2 * p->radius)) / (double)RAND_MAX;
                j = 0;
                try_limit--;
            }
            else
                j++;
        }
        cs->particles[i] = p;
    }
    return cs;
}

CollisionSystem *randomCollisionSystem(int n)
{
    CollisionSystem *cs = (CollisionSystem *)malloc(sizeof(CollisionSystem));
    srand(time(0));
    NP_CHECK(cs);
    cs->n = n;
    cs->pq = createPQ(ExpectedPQSize(n));
    cs->t = 0;
    cs->particles = (Particle **)malloc(n * sizeof(Particle *));
    Particle **particles = cs->particles;
    // create a grid with distance between lines equal to twice the diameter.
    // put particles at the position of randomly chosen grid intersections
    double radius = getParticleConfig().radius;
    double diameter = 2 * radius;
    double mass = 0.5;
    int p = 0, q = 0;
    int t = 1.0 / (2 * diameter);
    if (n >= (t * t) / 2)
    {
        printf("can't create collision system with this configuration\n");
        printf(" decrease radius or number of particles\n");
        exit(0);
    }

    double rx, ry;
    char repeated = 0;
    double delta = 0.000001;
    for (int i = 0; i < n; i++)
    {
        do
        {
            repeated = 0;
            p = t * (double)rand() / (double)RAND_MAX;
            q = t * (double)rand() / (double)RAND_MAX;
            rx = radius + p * 2 * diameter;
            ry = radius + q * 2 * diameter;

            for (int j = 0; j < i && !repeated; j++)
            {
                if ((particles[j]->rx > rx - delta && particles[j]->rx < rx + delta) && (particles[j]->ry > ry - delta && particles[j]->ry < ry + delta))
                    repeated = 1;
            }
        } while (repeated);
        // printf("%d %lf %lf\n", i, rx, ry);
        particles[i] = defaultParticle();
        particles[i]->rx = rx;
        particles[i]->ry = ry;
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

// the bug that spent 4 hours debugging was due to function call to newEvent
// which was receiving int as parameter. double was expected.
//  such a silly mistake !!!
void predict(CollisionSystem *cs, int p, double limit)
{
    if (p == INT_MIN)
        return;
    Particle *a = cs->particles[p];
    double dt, dtX, dtY;
    // particle-particle collisions
    for (int i = 0; i < cs->n; i++)
    {
        dt = timeToHit(a, cs->particles[i]);
        if (cs->t + dt <= limit)
        {
            if (dt < 0)
            {
                enqueuePQ(cs->pq, newEvent(cs->t, particleCollision, p, i, cs->particles));
            }
            else
            {
                enqueuePQ(cs->pq, newEvent(cs->t + dt, particleCollision, p, i, cs->particles));
            }
        }
    }

    // particle-wall collisions
    dtX = timeToHitVerticalWall(a);
    dtY = timeToHitHorizontalWall(a);

    if (cs->t + dtX <= limit)
        enqueuePQ(cs->pq, newEvent(cs->t + dtX, wallCollisionX, p, INT_MIN, cs->particles));
    if (cs->t + dtY <= limit)
        enqueuePQ(cs->pq, newEvent(cs->t + dtY, wallCollisionY, INT_MIN, p, cs->particles));
}

void advance(CollisionSystem *cs, double limit)
{
    Event *e;
    int a, b;
    PQueue *pq = cs->pq;
    Particle **particles = cs->particles;

    while (!isEmptyPQ(pq))
    {

        // get next event, discard if invalidated
        e = dequeuePQ(pq);
        if (!isValid(e, particles))
            continue;
        a = e->particle1;
        b = e->particle2;

        // printf("\n%lf %I64ld\n", cs->t, pq->size);

        // physical collision, so update positions, and then simulation clock
        if (e->time > cs->t)
            for (int i = 0; i < cs->n; i++)
            {
                move(particles[i], e->time - cs->t);
            }
        cs->t = e->time;

        eventType type = e->type;
        free(e);
        // process event
        if (type == particleCollision)
            bounceOff(particles[a], particles[b]); // particle-particle collision
        else if (type == wallCollisionX)
            bounceOffVerticalWall(particles[a]); // particle-wall collision
        else if (type == wallCollisionY)
            bounceOffHorizontalWall(particles[b]); // particle-wall collision
        else if (type == redrawEvent)
        {
            if (cs->t + 1 / HZ <= limit)
                enqueuePQ(pq, newRedrawEvent(cs->t + 1 / HZ)); // redraw event
            return;                                            // redraw event
        }

        // update the priority queue with new collisions involving a or b
        if (a != INT_MIN)
            predict(cs, a, limit);
        if (b != INT_MIN)
            predict(cs, b, limit);
    }
}

void buildEventQueue(CollisionSystem *cs, double limit)
{
    for (size_t i = 0; i < cs->n; i++)
    {
        predict(cs, i, limit);
    }
    enqueuePQ(cs->pq, newRedrawEvent(0));
    return;
}

void simulateOnTerminal(CollisionSystem *cs, double limit)
{
    buildEventQueue(cs, limit);
    while (cs->t <= limit)
    {
        advance(cs, limit);
        drawTerminal(cs->particles, cs->n);
        printf("\n%lf\n", cs->t);
        sleep_ms(250);
    }
}

// int main(int argc, char const *argv[])
// {
//     int n = 200;
//     CollisionSystem *cs = randomCollisionSystem(n);
//     simulateOnTerminal(cs, 200);
//     return 0;
// }
