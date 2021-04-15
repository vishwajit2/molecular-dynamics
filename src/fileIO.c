#include <stdio.h>
#include <stdlib.h>
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
    sim->cs = createCollisionSystem(particles, n);
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