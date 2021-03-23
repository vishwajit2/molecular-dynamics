#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "Event.h"
#include "PQueue.h"
#include "Particle.h"

#define HZ 0.5 // number of redraw events per clock tick
typedef struct CollisionSystem {
    size_t n;             // number of particles
    Particle **particles; // array of particle pointers
    PQueue *pq;           // priority queue of particles
    double t;             // time elapsed since start
} CollisionSystem;

CollisionSystem *createCollisionSystem(Particle **p, size_t n);

void predict(CollisionSystem *cs, Particle *a, double limit);

void deleteCollisionSystem(CollisionSystem *cs);

#endif
