#include "Event.h"
#include <stdio.h>
#include <stdlib.h>

int involveParticle(Event *event, int p)
{
    return (event->particle1 == p || event->particle2 == p);
}

Event *newEvent(double time, eventType type, int a, int b, Particle **particles)
{
    Event *event = (Event *)malloc(sizeof(Event));
    event->time = time;
    event->type = type;
    event->particle1 = a;
    event->particle2 = b;
    // if a is INT_MIN set countA to -1
    event->countA = (a != INT_MIN) ? particles[a]->count : -1;
    event->countB = (b != INT_MIN) ? particles[b]->count : -1;
    return event;
}

Event *newRedrawEvent(double time)
{
    Event *event = (Event *)malloc(sizeof(Event));
    event->type = redrawEvent;
    event->time = time;
    event->particle1 = INT_MIN;
    event->particle2 = INT_MIN;
    event->countA = -1;
    event->countB = -1;
    return event;
}

int compareEvent(Event *p, Event *q)
{
    if (p->time == q->time)
        return 0;
    if (p->time < q->time)
        return 1;
    else
        return -1;
}

bool isValid(Event *t, Particle **particles)
{
    if (t->particle1 != INT_MIN && particles[t->particle1]->count != t->countA)
        return false;
    if (t->particle2 != INT_MIN && particles[t->particle2]->count != t->countB)
        return false;
    return true;
}

void infoEvent(Event *e)
{
    if (!e)
        printf("No such event");
    printf("time(%lf) particles(%d, %d)\n", e->time, e->particle1, e->particle2);
    printf("\n");
}

// pointer variant functionality

int involveParticlep(Eventp *event, Particle *particle)
{
    return (event->particle1 == particle || event->particle2 == particle);
}

Eventp *newEventp(double time, eventType type, Particle *a, Particle *b)
{
    Eventp *event = (Eventp *)malloc(sizeof(Eventp));
    event->time = time;
    event->type = type;
    event->particle1 = a;
    event->particle2 = b;
    // if a is null set countA to -1
    event->countA = a ? a->count : -1;
    event->countB = b ? b->count : -1;
    return event;
}
int compareEventp(Eventp *p, Eventp *q)
{
    if (p->time == q->time)
        return 0;
    if (p->time < q->time)
        return 1;
    else
        return -1;
}

bool isValidp(Eventp *t)
{
    if (t->particle1 && t->particle1->count != t->countA)
        return false;
    if (t->particle2 && t->particle2->count != t->countB)
        return false;
    return true;
}

void infoEventp(Eventp *e)
{
    if (!e)
        printf("No such event");
    printf("time(%lf) particles(%p %p)\n", e->time, (void *)e->particle1, (void *)e->particle2);
    infoParticle(e->particle1);
    infoParticle(e->particle2);
    printf("\n");
}