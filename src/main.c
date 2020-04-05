#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "particle_type.h"
#include "sim_utils.h"

void sim_nBodySystem(float, int, int, int, particle_type **);


int main(int argc, char ** argv) {
	printf("angument count: %d\n", argc);
	for (int i=0; i<argc; ++i)
		printf("argument %d is %s\n", i, argv[i]);

	int b = 0;
	int N = 100, I = 10000;
	float t = 0.001;

	int opt;
	while ( (opt = getopt(argc, argv, "t:N:I:b:")) != -1 ) {
		switch (opt) {
			case 'N':
				N = atoi(optarg);
				printf("N: %s\n", optarg);
				break;
			case 'I':
				I = atoi(optarg);
				printf("I: %s\n", optarg);
				break;
			case 't':
				t = atof(optarg);
				printf("t: %s\n", optarg);
				break;
			case 'b':
				b = atoi(optarg);
			default:
				printf("invalid argument: %c", *optarg);
		}
	}

	printf("time interval: %f\n", t);
	printf("total particles: %d\n", N);
	printf("total iterations: %d\n", I);


	particle_type * p_nBodySystem;

	char * filename = "n-bodyConfig.csv";
	initialSimulationParams(N, &p_nBodySystem);

	sim_nBodySystem(t, I, N, b, &p_nBodySystem);



	return 0;
}

void sim_nBodySystem(float t, int I, int N, int b, particle_type ** pp_nBodySystem) {
	particle_type * p_nBodySystem = *pp_nBodySystem;
	particle_type * p_nBodySystem_updated = (particle_type *)malloc(N*sizeof(particle_type));
	particle_type * p_tmpParticle;
	char * filename = "n-bodySim.csv";
	storeState(0, N, &p_nBodySystem, filename);
	for (int i=0; i<I; ++i) {
		updateState(t, N, b, &p_nBodySystem, &p_nBodySystem_updated);
		storeState(t*i, N, &p_nBodySystem_updated, filename);
		p_tmpParticle = p_nBodySystem;
		p_nBodySystem = p_nBodySystem_updated;
		p_nBodySystem_updated = p_tmpParticle;
	}
}





