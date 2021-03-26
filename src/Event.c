#include "Event.h"
#include <stdio.h>
#include <stdlib.h>

int involveParticle(Event *event, Particle *particle)
{
    return (event->particle1 == particle || (event->type == particleCollision && event->particle2 == particle));
}

Event *newEvent(Particle *a, Particle *b, double time)
{
    Event *event = (Event *)malloc(sizeof(Event));
    event->time = time;
    event->particle1 = a;
    event->particle2 = b;
    // if a is null set countA to -1
    event->countA = a ? a->count : -1;
    event->countB = b ? b->count : -1;
    // only a: X wall collision
    // only b : Y wall collision
    if (a)
        event->type = b ? particleCollision : wallCollisionX;

    else
        event->type = b ? wallCollisionY : noEvent;
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

void infoEvent(Event *e)
{
    if (!e) printf("No such event");
    printf("time(%lf) particles(%p %p)\n", e->time, (void *)e->particle1, (void *)e->particle2);
    infoParticle(e->particle1);
    infoParticle(e->particle2);
    printf("\n");
}