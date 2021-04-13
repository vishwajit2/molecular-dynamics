#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "Event.h"
#include "PQueue.h"
#include "Particle.h"

typedef struct CollisionSystem
{
    size_t n;             // number of particles
    Particle **particles; // array of particle pointers
    PQueue *pq;           // priority queue of particles
    double t;             // time elapsed since start
} CollisionSystem;

CollisionSystem *createCollisionSystem(Particle **p, int n);

CollisionSystem *randomCollisionSystem(int n);

CollisionSystem *randomCollisionSystemPure(int n);

void predict(CollisionSystem *cs, int p, double limit);

void buildEventQueue(CollisionSystem *cs, double limit);

void advance(CollisionSystem *cs, double limit);

void simulateOnTerminal(CollisionSystem *cs, double limit);

void deleteCollisionSystem(CollisionSystem *cs);

#endif
