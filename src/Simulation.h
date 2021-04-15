// pair of doubles
#ifndef SIMULATION_H
#define SIMULATION_H

#include "CollisionSystem.h"
#include "Particle.h"

#define boltzmann_constant 1.38 // k Normalized
#define HZ 1                    // Number of redraw events per second of Collision system clock
#define refresh_interval 30     // time for GLUT Timer Function in millisecond
#define sampling_interval 500   // interval for which we accumulate data to calculate properties
#define max_trail_length 10000

typedef struct dPairNode
{
    double x;
    double y;
    struct dPairNode *next;
} dPairNode;

typedef struct iPair
{
    int x;
    int y;
} iPair;

// Node storing data of particle
typedef struct ParticleRecord
{
    int index;
    dPairNode *velocity, *vel_head;
    dPairNode *position, *pos_head;
    struct ParticleRecord *next;
} ParticleRecord;

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
    CollisionSystem *cs;       // collision system to be simulated
    TrailNode *traildata;      // Linked list of TrailData, Null if no trails requested
    bool pause;                // simulation state
    SystemProperties *sp;      // pointer to  system properties structure
    double limit;              // total time period of simulation
    double last_sampling_time; // last time system properties were modified
    ParticleRecord *records;   // linked list of particle record nodes
    int wallBalls;             // number of initilial particles to be treated as walls
} Simulation;

TrailNode *newTrailNode(int index, Color color, int limit);
void displaySystemProperties(SystemProperties *sp);
void advanceSP(Simulation *sim);
SystemProperties *newSystemProperties(int num);

// if filename is provided then value of n won't be considered
Simulation *newSimulation(double limit, char *filename, bool calculateSP, int n, ParticleConfig *conf);

// set particle indices to keep records
void setParticleforRecord(Simulation *sim, int particle_index);

// add new data to records
void updateParticleRecord(Simulation *sim);

int startSimulation(int argc, char *argv[], double limit, char *filename, int num_particles, int calculateSP, ParticleConfig *conf);
#endif