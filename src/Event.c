#include <stdlib.h>
#include "Event.h"

int involveParticle(Event *event, Particle *particle)
{
    return (event->particle1 == particle || (event->type == particleCollision && event->particle2 == particle));
}

Event *newEvent(Particle *a, Particle *b, int time)
{
    Event *event = (Event *)malloc(sizeof(Event));
    event->time = time;
    event->particle1 = a;
    event->particle2 = b;
    // if a is null set countA to -1
    event->countA = a ? a->count : -1;
    event->countB = b ? b->count : -1;
    event->type = (a && b) ? particleCollision : wallCollision;

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

bool isValid(Event *t)
{
    if (t->particle1 && t->particle1->count != t->countA)
        return false;
    if (t->particle2 && t->particle2->count != t->countB)
        return false;
    return true;
}