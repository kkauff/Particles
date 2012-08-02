#ifndef SIMPLESYSTEM_H
#define SIMPLESYSTEM_H

#include <GL/glut.h>
#include <vecmath.h>
#include <vector>

#include "particleSystem.h"
#include "integrator.h"

using namespace std;

class SimpleSystem: public ParticleSystem {
public:
    SimpleSystem(INTEGRATOR integrator, int numParticles);
    vector<Vector3f> evalF(const vector<Vector3f> &state);
    void draw();
    void shake();
    static Vector3f getGazeCoordinates();
    
    float PARTICLE_RADIUS;
    float SAMPLE_DISTANCE;
    
    
	
};

#endif
