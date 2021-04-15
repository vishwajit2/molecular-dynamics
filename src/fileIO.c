#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Particle.h"
#include "fileIO.h"

// file format
// num_trails (number of traildata lines)
// traildata lines follows
// Particle_index trail_R trail_G trail_B   (this isn traildata line structure)
// .
// .
// n (number of particles)
// particle data lines follows
// .
// .
// end

void simulationfromFile(Simulation *sim, char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("we couln't open the file\n");
        exit(0);
    }

    int n;
    Particle **particles;
    double rx, ry, vx, vy, radius, mass;
    color_t r, g, b;
    int num_trails;
    fscanf(fp, "%d", &num_trails);
    if (num_trails > 0)
    {
        TrailNode *tn;
        int particle_index;
        for (int i = 0; i < num_trails; i++)
        {
            Color c;
            fscanf(fp, "%d %hhu %hhu %hhu\n", &particle_index, &c.R, &c.G, &c.B);
            tn = newTrailNode(particle_index, c, sim->limit);
            tn->next = sim->traildata;
            sim->traildata = tn;
        }
    }
    else
        sim->traildata = NULL;

    fscanf(fp, "%d\n", &n);
    particles = (Particle **)malloc(sizeof(Particle *) * n);
    fscanf(fp, "%d\n", &sim->wallBalls);
    for (int i = 0; i < n; i++)
    {
        fscanf(fp, " %lf %lf %lf %lf %lf %lf %hhu %hhu %hhu \n", &rx, &ry, &vx, &vy, &radius, &mass, &r, &g, &b);
        // printf(" %lf %lf %lf %lf %lf %lf %d %d %d \n", rx, ry, vx, vy, radius, mass, r, g, b);
        particles[i] = createParticle(rx, ry, vx, vy, radius, mass);
        particles[i]->color.R = r;
        particles[i]->color.G = g;
        particles[i]->color.B = b;
    }

    // check ovelaps
    int overlapCount = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (isOverlapping(particles[i], particles[j]))
            {
                overlapCount++;
                printf("overlap between particle number %d and %d\n", i, j);
                InfoParticle(particles[i]);
                InfoParticle(particles[j]);
                double dx = particles[j]->rx - particles[i]->rx;
                double dy = particles[j]->ry - particles[i]->ry;
                double sigma = particles[i]->radius + particles[j]->radius;
                printf("%lf %lf \n", dx * dx + dy * dy, sigma * sigma);
            }
        }
    }
    if (overlapCount)
    {
        printf("Particles have  %d ovrlaps. It may not be processed properly\n", overlapCount);
    }
    sim->cs = createCollisionSystem(particles, n);
    // particles to keep record of and save to file if any
    int t;
    while (fscanf(fp, "%d\n", &t) == 1)
    {
        setParticleforRecord(sim, t);
    }

    fclose(fp);
    return;
}

void saveStateToFile(Simulation *sim)
{
    CollisionSystem *cs = sim->cs;
    Particle **p = cs->particles;
    SystemProperties *sp = sim->sp;
    char filename[50];
    struct tm *timenow;

    time_t now = time(NULL);
    timenow = gmtime(&now);
    strftime(filename, sizeof(filename), "output/result-%M:%H-%d-%m-%Y.txt", timenow);
    printf("saving to file : %s\n", filename);
    FILE *fp = fopen(filename, "w");

    if (!fp)
    {
        printf("couldn't save to file : %s\n. create a folder named output\n", filename);
        return;
    }
    // save system properties
    if (sp)
    {
        fprintf(fp, "System Properties\n");
        fprintf(fp, "Collision Frequency :%lf\nPressure: %lf\nTemparature: %lf\nrms Velocity: %lf\nMean Free Path: %lf\n\n",
                sp->collisionFreq, sp->pressure, sp->temp, sp->rmsVel, sp->meanFreePath);
    }
    // save particle data
    fprintf(fp, "particle states\n");
    for (int i = 0; i < cs->n; i++)
    {
        fprintf(fp, "%d %lf %lf %lf %lf %lf %lf %hhu %hhu %hhu \n",
                i, p[i]->rx, p[i]->ry, p[i]->vx, p[i]->vy, p[i]->radius, p[i]->mass, p[i]->color.R, p[i]->color.G, p[i]->color.B);
    }

    if (sim->records)
    {
        fprintf(fp, "\nparticle Records\n");
        ParticleRecord *p = sim->records;
        dPairNode *r, *v;
        while (p)
        {
            fprintf(fp, "Particle %d : rx ry vx vy", p->index);
            r = p->position;
            v = p->velocity;
            while (r && v)
            {
                fprintf(fp, "%lf %lf %lf %lf\n", r->x, r->y, v->x, v->y);
                r = r->next;
                v = v->next;
            }
            fprintf(fp, "\n");
            p = p->next;
        }
    }
    fclose(fp);
}

CollisionSystem *CollisionSystemFromFile(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("we couln't open the file\n");
        exit(0);
    }

    int n;
    double rx, ry, vx, vy, radius, mass;
    color_t r, g, b;
    fscanf(fp, "%d\n", &n);

    Particle **particles = (Particle **)malloc(sizeof(Particle *) * n);
    for (int i = 0; i < n; i++)
    {
        fscanf(fp, " %lf %lf %lf %lf %lf %lf %hhu %hhu %hhu \n", &rx, &ry, &vx, &vy, &radius, &mass, &r, &g, &b);
        // printf(" %lf %lf %lf %lf %lf %lf %d %d %d \n", rx, ry, vx, vy, radius, mass, r, g, b);
        particles[i] = createParticle(rx, ry, vx, vy, radius, mass);
        particles[i]->color.R = r;
        particles[i]->color.G = g;
        particles[i]->color.B = b;
    }
    fclose(fp);

    // check ovelaps
    int overlapCount = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (isOverlapping(particles[i], particles[j]))
            {
                overlapCount++;
                printf("overlap between particle number %d and %d\n", i, j);
                InfoParticle(particles[i]);
                InfoParticle(particles[j]);
                double dx = particles[j]->rx - particles[i]->rx;
                double dy = particles[j]->ry - particles[i]->ry;
                double sigma = particles[i]->radius + particles[j]->radius;
                printf("%lf %lf \n", dx * dx + dy * dy, sigma * sigma);
            }
        }
    }
    if (overlapCount)
    {
        printf("Particles have  %d ovrlaps. It may not be processed properly\n", overlapCount);
    }

    CollisionSystem *cs = createCollisionSystem(particles, n);
    return cs;
}