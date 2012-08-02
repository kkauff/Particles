#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>

using namespace std;


enum INTEGRATOR {EULER, TRAPEZOID, GAZER};

class ControlParticle{
public:
    ControlParticle();
    ControlParticle(Vector3f pos, Vector3f vel, float inf);
    Vector3f position;
    Vector3f velocity;
    float influenceRadius;
    
};


class ParticleSystem{
public:

    ParticleSystem();
    ParticleSystem(INTEGRATOR integrator, int numParticles);
    int numParticles;
    INTEGRATOR integrator;
    vector<Vector3f> state;
    Vector3f center;
    bool shaking;
    bool wireframe;
    
    // control particle(s)
    vector<ControlParticle> controlParticles;

    virtual void stepSystem(float h);
    virtual vector<Vector3f> evalF(const vector<Vector3f> &state) = 0;
    vector<Vector3f> getState();
    void setState(const vector<Vector3f> &newState);
    virtual void draw() = 0;
    void toggle();
    virtual void shake() = 0;

	
protected:

};


#endif
