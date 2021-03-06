#ifndef PARTICLE_H
#define PARTICLE_H
#include <stddef.h>

typedef unsigned char color_t;
typedef struct Color
{
    // RGB values between 0 to 255
    color_t R;
    color_t G;
    color_t B;
} Color;

typedef struct ParticleConfig
{
    double radius;
    double mass;
    Color color;
    double vel_lo;
    double vel_hi;
} ParticleConfig;

#define InfoParticle(p) (printf("r(%lf,%lf) v(%lf,%lf) m(%lf) rad(%lf) count(%d) \n", p->rx, p->ry, p->vx, p->vy, p->mass, p->radius, p->count))

typedef struct Particle
{
    double rx, ry; // position
    double vx, vy; // velocity
    int count;     // number of collisions so far
    double radius; // radius
    double mass;   // mass
    struct Color color;
} Particle;

Color *newColor(color_t r, color_t g, color_t b);

ParticleConfig getParticleConfig();
void setParticleConfig(ParticleConfig conf);

// init partile with given values
Particle *createParticle(double rx, double ry, double vx, double vy, double radius, double mass);

// initializes a particle with random position in unit box and random velocity
Particle *createRandomParticle();

Particle *defaultParticle();

// returns 1 if p1 and p2 are same particles, else returns 0
int isSame(Particle *p1, Particle *p2);

// Moves particle p in a straight line based on its velocity
// for the dt amount of time.
void move(Particle *p, double dt);

// returns amount of time required for p1 to collide p2 assuming no
// other event takes place with any of them.
// returns INFINITY if collision don't take place
double timeToHit(Particle *p1, Particle *p2);

// returns amount of time required for p to collide with wall assuming no
// other event takes place with p.
// returns INFINITY if collision don't take place
double timeToHitVerticalWall(Particle *p);
double timeToHitHorizontalWall(Particle *p);

// Updates the velocities of particles p1 and p2 according
// to the laws of elastic collision. Assumes that the particles are colliding
// at this instant.
void bounceOff(Particle *p1, Particle *p2);

// Updates velocity of particle p upon collision with a wall
void bounceOffVerticalWall(Particle *p);
void bounceOffHorizontalWall(Particle *p);

// returns kinetic energy of particle p
double kineticEnergy(Particle *p);

// return 0 if two particles are not overlapping
int isOverlapping(Particle *p1, Particle *p2);

// return 1 if a->rx is greater than b->rx,  -1 if a->rx is smaller than b->rx
// break ties with ry. returns zero if particles are at same location
int comparePositionX(Particle *a, Particle *b);

// return 1 if a->ry is greater than b->ry,  -1 if a->ry is smaller than b->ry
// break ties with rx. returns zero if particles are at same location
int comparePositionY(Particle *a, Particle *b);

// sort particle array according to comparePositionY
void sort(Particle **particles, size_t n);

// check if array is sorted
int isSorted(Particle **particles, size_t n);
// draw particles in arrray to terminal

void drawTerminal(Particle **particles, size_t n);

void infoParticle(Particle *p);

#endif