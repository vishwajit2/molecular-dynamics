#ifndef FILEIO_H
#define FILEIO_H

#include "Simulation.h"
CollisionSystem *CollisionSystemFromFile(char *filename);
void simulationfromFile(Simulation *sim, char *filename);
void saveStateToFile(Simulation *sim);
#endif