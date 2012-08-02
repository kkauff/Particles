#include <iostream>
#include "particleSystem.h"
#include "integrator.h"


ParticleSystem::ParticleSystem() {} 

ParticleSystem::ParticleSystem(INTEGRATOR integrator, int numParticles) {
  this->numParticles = numParticles;
  this->integrator = integrator;
}

void ParticleSystem::stepSystem(float h) {
    INTEGRATOR method = integrator;
    vector<Vector3f> nextState;

    if (shaking) {
	shake();
    }
    if (method == EULER){
	nextState = Integrator::evalEuler(this, h);
    }
    else if (method == GAZER) {
	nextState = Integrator::evalGazer(this, h);
    }
    else {
	nextState = Integrator::evalTrapezoid(this, h);	
    }
    setState(nextState);
}

vector<Vector3f> ParticleSystem::getState() {
  return state;
}

void ParticleSystem::setState(const vector<Vector3f> &newState) {
  state = newState;
}

void ParticleSystem::shake()
{
    if (shaking) {
        shaking = false;
    }
    else {
        shaking = true;
    }
}

void ParticleSystem::toggle() {
    if (wireframe) {
        wireframe = false;
    }
    else {
        wireframe = true;
    }
}

ControlParticle::ControlParticle() {}

ControlParticle::ControlParticle(Vector3f pos, Vector3f vel, float inf) {
    this->position = pos;
    this->velocity = vel;
    this->influenceRadius = inf;
}

Vector3f PositionFinder::getPosition(const vector<Vector3f> &state, int index) {
    return state[2*index];
}

Vector3f PositionFinder::getVelocity(const vector<Vector3f> &state, int index) {
    return state[2*index+1];
}