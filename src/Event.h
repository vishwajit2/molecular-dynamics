#ifndef EVENT_H
#define EVENT_H

#include "Particle.h"
#include <stdbool.h>
typedef enum eventType {
    particleCollision,
    wallCollisionX, // vertical walll
    wallCollisionY, // horizontal wall
    noEvent,
} eventType;

typedef struct Event {
    double time;                     // time that event is scheduled to occur
    eventType type;                  // type of event
    Particle *particle1, *particle2; // particles involved in event, possibly null
    int countA, countB;              // collision counts at event creation
} Event;

// return pointer to Event with given parameters
Event *newEvent(Particle *a, Particle *b, double time);

// return 1 if particle is involved in event, else return 0
int involveParticle(Event *event, Particle *particle);

// returns 1 if p happens before q, 0 if both take place at same time, otherwise -1.
int compareEvent(Event *p, Event *q);

// has any collision occurred between when event was created and now?
bool isValid(Event *p);

void infoEvent(Event *e);

#endif