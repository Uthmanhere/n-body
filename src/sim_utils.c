#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "sim_utils.h"

void echoSimulationParams(int N, particle_type ** p_nBodySystem) {
	for (int i=0; i<N; ++i) {
		printf("particle: %d\n", i);
		printf("\tposition:\n");
		printf("\t\tx: %f\n", (*p_nBodySystem+i)->position.x);
		printf("\t\ty: %f\n", (*p_nBodySystem+i)->position.x);
		printf("\tacceleration:\n");
		printf("\t\tx: %f\n", (*p_nBodySystem+i)->acceleration.x);
		printf("\t\ty: %f\n", (*p_nBodySystem+i)->acceleration.x);
		printf("\tvelocity:\n");
		printf("\t\tx: %f\n", (*p_nBodySystem+i)->velocity.x);
		printf("\t\ty: %f\n", (*p_nBodySystem+i)->velocity.x);
		printf("\tforce::\n");
		printf("\t\tx: %f\n", (*p_nBodySystem+i)->force.x);
		printf("\t\ty: %f\n", (*p_nBodySystem+i)->force.x);
		printf("\tmass: %f\n", (*p_nBodySystem+i)->mass);
	}
}

void initialSimulationParams(int N, particle_type ** nBodySystem) {
	*nBodySystem = (particle_type *)malloc(N * sizeof(particle_type));

	srand(time(0));
	for (int i=0; i<N; ++i) {
		((*nBodySystem)+i)->position.x = 2*(rand()%2-0.5) * ( rand()%100000 / 10000.0 );
		((*nBodySystem)+i)->position.y = 2*(rand()%2-0.5) * ( rand()%100000 / 10000.0 );
		((*nBodySystem)+i)->acceleration.x = 0;
		((*nBodySystem)+i)->acceleration.y = 0;
		((*nBodySystem)+i)->velocity.x = 0;
		((*nBodySystem)+i)->velocity.y = 0;
		((*nBodySystem)+i)->force.x = 0;
		((*nBodySystem)+i)->force.y = 0;
		((*nBodySystem)+i)->mass = rand()%100 / 1.0;
	}
}

void updateState(float t, int N, int b, particle_type ** pp_nBodySystem_prev, particle_type ** pp_nBodySystem_current) {
	for (int i=0; i<N; ++i) {
		//updateForce(N, p_nBodySystem_prev, p_nBodySystem_current);
		updateAcceleration(i, N, pp_nBodySystem_prev, pp_nBodySystem_current);
		updateVelocity(i, t, N, pp_nBodySystem_prev, pp_nBodySystem_current);
		updatePosition(i, t, N, b,  pp_nBodySystem_prev, pp_nBodySystem_current);
	}
}

void updateAcceleration(int p, int N,
		particle_type ** pp_nBodySystem_prev,
		particle_type ** pp_nBodySystem_current) {
	particle_type * p_nBodySystem_prev = *pp_nBodySystem_prev;
	particle_type * p_nBodySystem_current = *pp_nBodySystem_current;
	coordinate_type particlePosition = p_nBodySystem_prev[p].position;
	float direction;
	coordinate_type acceleration = { 0, 0 };
	for (int i=0; i<N; ++i) {
		if (i==p) continue;
		direction = ( p_nBodySystem_prev[i].position.x - particlePosition.x ) /
			( p_nBodySystem_prev[i].position.x - particlePosition.x );
		acceleration.x += p_nBodySystem_prev[i].mass  * \
											direction / \
											powf(p_nBodySystem_prev[i].position.x - particlePosition.x, 3);
		direction = ( p_nBodySystem_prev[i].position.y - particlePosition.y ) /
			( p_nBodySystem_prev[i].position.y - particlePosition.y );
		acceleration.y += p_nBodySystem_prev[i].mass * \
											direction / \
											powf(p_nBodySystem_prev[i].position.y - particlePosition.y, 3);
	}
	p_nBodySystem_current[p].acceleration = acceleration;
}

void updateVelocity(int p, float t, int N,
		particle_type ** pp_nBodySystem_prev,
		particle_type ** pp_nBodySystem_current) {
	particle_type * p_nBodySystem_prev = *pp_nBodySystem_prev;
	particle_type * p_nBodySystem_current = *pp_nBodySystem_current;
	p_nBodySystem_current[p].velocity.x = p_nBodySystem_prev[p].velocity.x;
	p_nBodySystem_current[p].velocity.y = p_nBodySystem_prev[p].velocity.y;
	p_nBodySystem_current[p].velocity.x += ( p_nBodySystem_current[p].acceleration.x * t );
	p_nBodySystem_current[p].velocity.y += ( p_nBodySystem_current[p].acceleration.y * t );
}

void updatePosition(int p, float t, int N, int b,
		particle_type ** pp_nBodySystem_prev,
		particle_type ** pp_nBodySystem_current) {
	particle_type * p_nBodySystem_prev = *pp_nBodySystem_prev;
	particle_type * p_nBodySystem_current = *pp_nBodySystem_current;
	p_nBodySystem_current[p].position.x = p_nBodySystem_prev[p].position.x + \
																				p_nBodySystem_prev[p].velocity.x * t + \
																						0.5 * p_nBodySystem_current[p].acceleration.x * t * t;
	p_nBodySystem_current[p].position.y = p_nBodySystem_prev[p].position.y + \
																				p_nBodySystem_prev[p].velocity.y * t + \
																						0.5 * p_nBodySystem_current[p].acceleration.y * t * t;

	if (p_nBodySystem_current[p].position.x > b) {
		p_nBodySystem_current[p].acceleration.x = -p_nBodySystem_current[p].acceleration.x;
		p_nBodySystem_current[p].velocity.x = -p_nBodySystem_current[p].velocity.x;
//		p_nBodySystem_current[p].position.x = b+10*rand()/RAND_MAX;
	}
	if (p_nBodySystem_current[p].position.x < -b) {
		p_nBodySystem_current[p].acceleration.x = -p_nBodySystem_current[p].acceleration.x;
		p_nBodySystem_current[p].velocity.x = -p_nBodySystem_current[p].velocity.x;
//		p_nBodySystem_current[p].position.x = -b+10*rand()/RAND_MAX;
	}
	if (p_nBodySystem_current[p].position.y > b) {
		p_nBodySystem_current[p].acceleration.y = -p_nBodySystem_current[p].acceleration.y;
		p_nBodySystem_current[p].velocity.y = -p_nBodySystem_current[p].velocity.y;
//		p_nBodySystem_current[p].position.y = b+10*rand()/RAND_MAX;
	}
	if (p_nBodySystem_current[p].position.y < -b) {
		p_nBodySystem_current[p].acceleration.y = -p_nBodySystem_current[p].acceleration.y;
		p_nBodySystem_current[p].velocity.y = -p_nBodySystem_current[p].velocity.y;
//		p_nBodySystem_current[p].position.y = -b+10*rand()/RAND_MAX;
	}

	if (isnan(p_nBodySystem_current[p].acceleration.x)
			|| isnan(p_nBodySystem_current[p].acceleration.x)) {
		p_nBodySystem_current[p].position.x = b+100000;
		p_nBodySystem_current[p].position.y = b+100000;
		p_nBodySystem_current[p].velocity.x = 0;
		p_nBodySystem_current[p].velocity.y = 0;
		p_nBodySystem_current[p].acceleration.x = 0;
		p_nBodySystem_current[p].acceleration.y = 0;
		p_nBodySystem_current[p].mass = 0;
	}


}

void storeState(float t, int N, particle_type ** pp_nBodySystem, char * filename) {
	particle_type * p_nBodySystem = *pp_nBodySystem;
	FILE * p_simfile_csv = fopen(filename, "a");
	if (!p_simfile_csv) {
		puts("error: cannot open file");
		return;
	}
	for (int i=0; i<N; ++i) {
		fprintf(p_simfile_csv,
				"%f, %d, %f, %f, %f, %f, %f, %f, %f\n",
				t, i,
				p_nBodySystem[i].mass,
				p_nBodySystem[i].position.x,
				p_nBodySystem[i].position.y,
				p_nBodySystem[i].velocity.x,
				p_nBodySystem[i].velocity.y,
				p_nBodySystem[i].acceleration.x,
				p_nBodySystem[i].acceleration.y);
	}
	fclose(p_simfile_csv);
}
