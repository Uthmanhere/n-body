#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "particle_type.h"

void echoSimulationParams(int, particle_type **);
void initialSimulationParams(int, particle_type **);

void updateState(float, int, int, particle_type **, particle_type **);
void storeUpdate(int, particle_type **);

void updateAcceleration(int, int, particle_type **, particle_type **);
void updateVelocity(int, float, int, particle_type **, particle_type **);
void updatePosition(int, float, int, int,  particle_type **, particle_type **);

void storeState(float, int, particle_type **, char *);

