#include <math.h>
typedef struct Particle
{
    double rx, ry; // position
    double vx, vy; // velocity
    int count;     // number of collisions so far
    double radius; // radius
    double mass;   // mass
    // char *color;   // color
} Particle;

// init partile with given values
Particle *initParticle(double rx, double ry, double vx, double vy, double radius, double mass);

// initializes a particle with random position in unit box and random velocity
Particle *initRandomParticle();

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
