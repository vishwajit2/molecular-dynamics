#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "utilities.h"
#include "Simulation.h"
#include "fileIO.h"

dPairNode *newdPairNode(double x, double y)
{
    dPairNode *t = (dPairNode *)malloc(sizeof(dPairNode));
    t->x = x;
    t->y = y;
    t->next = NULL;
    return t;
}

TrailNode *newTrailNode(int index, Color color, int limit)
{
    TrailNode *t = (TrailNode *)malloc(sizeof(TrailNode));
    t->index = index;
    t->color = color;
    t->head = 0;
    t->isFull = false;
    t->data = (iPair *)calloc(max_trail_length, sizeof(iPair));
    t->next = NULL;
}

Simulation *newSimulation(double limit, char *filename, bool calculateSP, int n, ParticleConfig *conf)
{
    Simulation *sim = (Simulation *)malloc(sizeof(Simulation));
    sim->limit = limit;
    sim->pause = false;
    sim->last_sampling_time = 0;
    sim->records = NULL;
    if (filename)
    {
        simulationfromFile(sim, filename);
        sim->sp = newSystemProperties(sim->cs->n);
    }
    else
    {
        if (conf)
        {
            setParticleConfig(*conf);
        }
        sim->cs = randomCollisionSystem(n);
        if (calculateSP)
            sim->sp = newSystemProperties(n);
        sim->traildata = NULL;
        sim->wallBalls = 0;
    }
    return sim;
}

ParticleRecord *newParticleRecord(int particle_index)
{
    ParticleRecord *record = (ParticleRecord *)malloc(sizeof(ParticleRecord));
    record->index = particle_index;
    record->next = NULL;
    record->pos_head = NULL;
    record->position = NULL;
    record->vel_head = NULL;
    record->velocity = NULL;
    return record;
}

// set particle indices to keep records
void setParticleforRecord(Simulation *sim, int particle_index)
{
    if (!sim || !sim->cs)
        return;

    if (particle_index >= sim->cs->n)
    {
        printf("index should be within bounds\n");
        return;
    }

    ParticleRecord *rec = newParticleRecord(particle_index);
    rec->next = sim->records;
    sim->records = rec;
}

void updateParticleRecord(Simulation *sim)
{
    if (!sim)
        return;
    ParticleRecord *rec = sim->records;

    while (rec)
    {
        Particle *p = sim->cs->particles[rec->index];
        if (!rec->position)
        {
            rec->position = newdPairNode(p->rx, p->ry);
            rec->pos_head = rec->position;
            rec->velocity = newdPairNode(p->vx, p->vy);
            rec->vel_head = rec->velocity;
        }
        else
        {
            rec->pos_head->next = newdPairNode(p->rx, p->ry);
            rec->pos_head = rec->pos_head->next;
            rec->vel_head->next = newdPairNode(p->vx, p->vy);
            rec->vel_head = rec->vel_head->next;
        }
        rec = rec->next;
    }
}

SystemProperties *newSystemProperties(int num)
{
    SystemProperties *sp = (SystemProperties *)calloc(1, sizeof(SystemProperties));
    sp->num = num;
    sp->freePath = (double *)calloc(num, sizeof(double));
    sp->freeTime = (double *)calloc(num, sizeof(double));
    return sp;
}

void displaySystemProperties(SystemProperties *sp)
{
    printf("System properties\n");
    printf("Collision Frequency :%lf\nPressure: %lf\nTemparature: %lf\nrms Velocity: %lf\nMean Free Path: %lf\n",
           sp->collisionFreq, sp->pressure, sp->temp, sp->rmsVel, sp->meanFreePath);
    // printf("Free Path and Free Time of individual particles:\n");
    // for (int i = 0; i < 5; i++)
    // {
    //     printf("%d :%lf %lf\n", i + 1, sp->freePath[i], sp->freeTime[i]);
    // }
    // printf("\n");
}

// advance with system properties calculations
void advanceSP(Simulation *sim)
{
    CollisionSystem *cs = sim->cs;
    SystemProperties *sp = sim->sp;

    PQueue *pq = cs->pq;
    Particle **particles = cs->particles;
    Event *e;
    int a, b;
    eventType type;
    double dt = 0;

    static int *collisionCounts;
    static double *pathLengths;
    static double momentumChange; // to calculate pressures on walls
    if (!collisionCounts)
    {
        collisionCounts = (int *)calloc(cs->n, sizeof(int));
        pathLengths = (double *)calloc(cs->n, sizeof(double));
        momentumChange = 0;
    }
    double collisionTotal = 0;
    double abs_velsq = 0;
    double abs_vel = 0;
    while (!isEmptyPQ(pq))
    {
        // get next event, discard if invalidated
        e = dequeuePQ(pq);
        if (!isValid(e, particles))
            continue;

        a = e->particle1;
        b = e->particle2;
        type = e->type;
        dt = e->time - cs->t;
        cs->t = e->time;
        free(e);
        // printf("\n%lf %ld %d\n", cs->t, pq->size, isEmptyPQ(pq));

        // physical collision, so update positions, and then simulation clock
        if (dt > 0)
            for (int i = 0; i < cs->n; i++)
            {
                move(particles[i], dt);
                abs_velsq = particles[i]->vx * particles[i]->vx + particles[i]->vy * particles[i]->vy;
                pathLengths[i] += (sqrt(abs_velsq) * dt);
            }

        // process event
        if (type == particleCollision)
        {
            bounceOff(particles[a], particles[b]); // particle-particle collision
            collisionCounts[a] += 1;
            collisionCounts[b] += 1;
        }
        else if (type == wallCollisionX)
        {
            abs_vel = (particles[a]->vx < 0) ? -particles[a]->vx : particles[a]->vx;
            momentumChange += (2 * particles[a]->mass * abs_vel);
            collisionCounts[a] += 1;
            bounceOffVerticalWall(particles[a]); // particle-wall collision
        }
        else if (type == wallCollisionY)
        {
            abs_vel = (particles[b]->vy < 0) ? -particles[b]->vy : particles[b]->vy;
            momentumChange += (2 * particles[b]->mass * abs_vel);
            collisionCounts[b] += 1;
            bounceOffHorizontalWall(particles[b]); // particle-wall collision
        }
        // rmsVel = sqrt ( average of v^2 )
        //  temp = average(m*v^2) / (2*Boltzaman constant)
        else if (type == redrawEvent)
        {
            // set properties
            if (cs->t - sim->last_sampling_time >= sampling_interval)
            {
                sp->meanFreePath = 0;
                sp->rmsVel = 0;
                sp->temp = 0;
                collisionTotal = 0;
                for (int i = 0; i < sp->num; i++)
                {
                    abs_velsq = particles[i]->vx * particles[i]->vx + particles[i]->vy * particles[i]->vy;
                    sp->temp += particles[i]->mass * abs_velsq;
                    sp->rmsVel += abs_velsq;
                    if (!collisionCounts[i])
                        continue;
                    collisionTotal += collisionCounts[i];
                    sp->freePath[i] = pathLengths[i] / collisionCounts[i];
                    sp->freeTime[i] = sampling_interval / collisionCounts[i];
                    sp->meanFreePath += sp->freePath[i];
                }
                sp->meanFreePath /= sp->num;
                sp->pressure = (momentumChange / sampling_interval) / 4;
                sp->collisionFreq = collisionTotal / sampling_interval;
                sp->rmsVel = sqrt(sp->rmsVel / cs->n);
                sp->temp /= (2 * cs->n * boltzmann_constant);

                // reset static variable values to zero
                memset(collisionCounts, 0, sizeof(int) * cs->n);
                memset(pathLengths, 0, sp->num * sizeof(double));
                momentumChange = 0;

                // update last sampling time
                sim->last_sampling_time = cs->t;
            }
            if (cs->t + 1 / HZ <= sim->limit)
                enqueuePQ(pq, newRedrawEvent(cs->t + 1 / HZ)); // redraw event
            return;
        }

        // update the priority queue with new collisions involving a or b
        predict(cs, a, sim->limit);
        predict(cs, b, sim->limit);
    }
}