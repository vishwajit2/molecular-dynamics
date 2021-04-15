// pair of doubles
#ifndef SIMULATION_H
#define SIMULATION_H

#include "CollisionSystem.h"
#include "Particle.h"

#define boltzmann_constant 1.38 // normalized k
#define HZ 1                    // Number of redraw events per second of Collision system clock
#define refresh_interval 30     // time for GLUT Timer Function in millisecond
#define sampling_interval 500   // interval for which we accumulate data to calculate properties
#define max_trail_length 10000

typedef struct iPair
{
    int x;
    int y;
} iPair;

typedef struct TrailNode
{
    Color color;
    int index;
    int head; // Number of elements in data array
    bool isFull;
    iPair *data;
    struct TrailNode *next;
} TrailNode;

typedef struct SystemProperties
{

    double pressure;      // in Pascal
    double temp;          // temperature in Kelvin
    double collisionFreq; // collision frequency of system
    double rmsVel;
    double meanFreePath;
    int num;
    double *freePath; // array of mean free paths of all paricles
    double *freeTime; // array of mean time between collision of particles
} SystemProperties;

typedef struct Simulation
{
    CollisionSystem *cs;
    TrailNode *traildata; // Linked list of TrailData, Null if no trails requested
    bool pause;           // simulation state
    SystemProperties *sp; // pointer to  system properties structure
    double limit;         // total time period of simulation
    double last_sampling_time;
    int wallBalls;
} Simulation;

TrailNode *newTrailNode(int index, Color color, int limit);
void displaySystemProperties(SystemProperties *sp);
void advanceSP(Simulation *sim);
SystemProperties *newSystemProperties(int num);

// if filename is provided then value of n won't be considered
Simulation *newSimulation(double limit, char *filename, bool calculateSP, int n, ParticleConfig *conf);

#endif