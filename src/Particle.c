#include "Particle.h"
#include "checker.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

Particle *createParticle(double rx, double ry, double vx, double vy, double radius, double mass)
{
    Particle *p = (Particle *)malloc(sizeof(Particle));
    p->vx = vx;
    p->vy = vy;
    p->rx = rx;
    p->ry = ry;
    p->radius = radius;
    p->mass = mass;
    return p;
}

Particle *createRandomParticle()
{
    // srand(time(0));
    Particle *p = (Particle *)malloc(sizeof(Particle));
    p->rx = (double)rand() / (double)RAND_MAX;
    p->ry = (double)rand() / (double)RAND_MAX;
    double low = -0.005, high = 0.005;
    p->vx = ((double)rand() * (high - low)) / (double)RAND_MAX + low;
    p->vy = ((double)rand() * (high - low)) / (double)RAND_MAX + low;
    p->radius = 0.02;
    p->mass = 0.5;
    p->count = 0;
    // p->color
    return p;
}
void move(Particle *p, double dt)
{
    p->rx += p->vx * dt;
    p->ry += p->vy * dt;
}

int isSame(Particle *p1, Particle *p2)
{
    return (p1->rx == p2->rx && p1->ry == p2->ry && p1->vx == p2->vx && p1->vy == p2->vy &&
            p1->mass == p2->mass && p1->radius == p2->radius && p1->count == p2->count);
}

double timeToHit(Particle *p1, Particle *p2)
{
    if (isSame(p1, p2))
        return INFINITY;
    double dx = p2->rx - p1->rx;
    double dy = p2->ry - p1->ry;
    double dvx = p2->vx - p1->vx;
    double dvy = p2->vy - p1->vy;
    double dvdr = dx * dvx + dy * dvy; // dv dot dr
    if (dvdr > 0)
        return INFINITY;
    double dvdv = dvx * dvx + dvy * dvy;
    if (dvdv == 0)
        return INFINITY;
    double drdr = dx * dx + dy * dy;
    double sigma = p1->radius + p2->radius; // distance between particle centers at collison
    double d = (dvdr * dvdr) - dvdv * (drdr - sigma * sigma);
    // if (drdr < sigma*sigma) printf("overlapping particles");
    if (d < 0)
        return INFINITY;
    return -(dvdr + sqrt(d)) / dvdv;
}

double timeToHitVerticalWall(Particle *p)
{
    if (p->vx > 0)
        return (1.0 - p->rx - p->radius) / p->vx;
    else if (p->vx < 0)
        return (p->radius - p->rx) / p->vx;
    else
        return INFINITY;
}
double timeToHitHorizontalWall(Particle *p)
{
    if (p->vy > 0)
        return (1.0 - p->ry - p->radius) / p->vy;
    else if (p->vy < 0)
        return (p->radius - p->ry) / p->vy;
    else
        return INFINITY;
}
void bounceOff(Particle *p1, Particle *p2)
{
    double dx = p2->rx - p1->rx;
    double dy = p2->ry - p1->ry;
    double dvx = p2->vx - p1->vx;
    double dvy = p2->vy - p1->vy;
    double dvdr = dx * dvx + dy * dvy;
    double dist = p1->radius + p2->radius; // distance between particle centers at collison

    // magnitude of normal force
    double magnitude = 2 * p1->mass * p2->mass * dvdr / ((p1->mass + p2->mass) * dist);

    // normal force in x and y directions
    double fx = magnitude * dx / dist;
    double fy = magnitude * dy / dist;

    // update velocities according to normal force
    p1->vx += fx / p1->mass;
    p1->vy += fy / p1->mass;
    p2->vx -= fx / p2->mass;
    p2->vy -= fy / p2->mass;

    // update collision counts
    p1->count++;
    p2->count++;
}

void bounceOffVerticalWall(Particle *p)
{
    p->vx = -p->vx;
    p->count++;
}
void bounceOffHorizontalWall(Particle *p)
{
    p->vy = -p->vy;
    p->count++;
}

double kineticEnergy(Particle *p)
{
    return 0.5 * p->mass * (p->vx * p->vx + p->vy * p->vy);
}

int comparePositionX(Particle *a, Particle *b)
{
    if (a == b) return 0; // both points to same particle or both null
    if (!b) return 1;     // only a
    if (!a) return -1;    // only b

    if (a->rx > b->rx)
        return 1;
    else if (a->rx < b->rx)
        return -1;
    // now a->rx = b->rx. compare ry
    else if (a->ry > b->ry)
        return 1;
    else if (a->ry < b->ry)
        return -1;
    else
        return 0;
}

int comparePositionY(Particle *a, Particle *b)
{
    if (a == b) return 0; // both points to same particle or both null
    if (!b) return 1;     // only a
    if (!a) return -1;    // only b

    if (a->ry > b->ry)
        return 1;
    else if (a->ry < b->ry)
        return -1;
    // now a->ry = b->ry. compare rx
    else if (a->rx > b->rx)
        return 1;
    else if (a->rx < b->rx)
        return -1;
    else
        return 0;
}

int cmpWrapper(const void *a, const void *b)
{
    // a and b are pointer to Particle*
    return comparePositionY(*(Particle **)a, *(Particle **)b);
}

void sort(Particle **particles, size_t n)
{
    qsort(particles, n, sizeof(Particle *), cmpWrapper);
}

int isSorted(Particle **particles, size_t n)
{
    for (size_t i = 0; i < n - 1; i++) {
        if (comparePositionY(particles[i], particles[i - 1]) > 0)
            return 0;
    }
    return 1;
}