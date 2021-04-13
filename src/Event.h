#ifndef EVENT_H
#define EVENT_H

#include "Particle.h"
#include <stdbool.h>
#include <limits.h>

typedef enum eventType
{
    particleCollision,
    wallCollisionX, // vertical walll
    wallCollisionY, // horizontal wall
    redrawEvent,
} eventType;

// Event structure which has pointers to particles
typedef struct Event
{
    double time;              // time that event is scheduled to occur
    eventType type;           // type of event
    int particle1, particle2; // indices of particles involved in event, possibly INT_MAX
    int countA, countB;       // collision counts at event creation
} Event;

// return pointer to Eventp with given parameters
Event *newEvent(double time, eventType type, int a, int b, Particle **particles);
Event *newRedrawEvent(double time);
// return 1 if particle is involved in event, else return 0
int involveParticle(Event *event, int index);

// returns 1 if p happens before q, 0 if both take place at same time, otherwise -1.
int compareEvent(Event *p, Event *q);

// has any collision occurred between when event was created and now?
bool isValid(Event *t, Particle **particles);

void infoEvent(Event *e);

// Event structure which stores pointers to particles
typedef struct Eventp
{
    double time;                     // time that event is scheduled to occur
    eventType type;                  // type of event
    Particle *particle1, *particle2; // particles involved in event, possibly null
    int countA, countB;              // collision counts at event creation
} Eventp;

// return pointer to Eventp with given parameters
Eventp *newEventp(double time, eventType type, Particle *a, Particle *b);

// return 1 if particle is involved in event, else return 0
int involveParticlep(Eventp *event, Particle *particle);

// returns 1 if p happens before q, 0 if both take place at same time, otherwise -1.
int compareEventp(Eventp *p, Eventp *q);

// has any collision occurred between when event was created and now?
bool isValidp(Eventp *p);

void infoEventp(Eventp *e);

#endif