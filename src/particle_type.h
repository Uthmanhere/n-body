#ifndef _PARTICLE_TYPE_H__
#define _PARTICLE_TYPE_H__

typedef struct coordinate_type {
	float x;
	float y;
} coordinate_type;

typedef struct particle_type {
	coordinate_type force;
	coordinate_type acceleration;
	coordinate_type velocity;
	coordinate_type position;
	float mass;
} particle_type;

#endif
