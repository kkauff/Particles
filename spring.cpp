#include <iostream>
#include "spring.h"
#include "pendulumSystem.h"
#include <assert.h>

Spring::Spring(float k, float r, int from, int to) {
    this->k = k;
    this->r = r;
    this->from = from;
    this->to = to;
}

bool Spring::leftEdge(int width, int jj) {
    return (jj%width == 0);
}
bool Spring::rightEdge(int width, int jj){
    return ((jj+1)%width == 0);
}
bool Spring::topEdge(int ii){
    return (ii == 0);
}
bool Spring::bottomEdge(int height, int ii){
    return (ii == height-1);
}

Vector3f Spring::getNetForce(int index, const vector<vector<Spring*> > &springs, const vector<Vector3f> &state) { 
    assert ((unsigned) index < springs.size());
    vector<Spring*> particleSprings = springs[index];
    Vector3f netForce = Vector3f(0,0,0);
    
    for (unsigned int ii = 0; ii < particleSprings.size(); ii++) {
        Spring* spr = particleSprings[ii];
        float k = spr->k;
        float r = spr->r; 
        int from = spr->from;
        int to = spr->to;
         
        Vector3f xi = PositionFinder::getPosition(state, from);
        Vector3f xj = PositionFinder::getPosition(state, to);
        
        int multiplier = (index == to) ? 1 : -1;
        
        Vector3f d = multiplier*(xj - xi);
        float dMag = d.abs();
        Vector3f d_norm = d.normalized();
        
        Vector3f force = -k*(dMag - r)*d_norm;
        
        netForce += force;
        
    }
    return netForce;
}
    
 


void Spring::drawSprings(const vector<Vector3f> &state, const vector<Spring> &springs) {
    for (unsigned int ii = 0; ii < springs.size(); ii++) {
	Spring spr = springs[ii];
	assert ((unsigned) spr.to < state.size()/2);
	Vector3f pos0 = PositionFinder::getPosition(state, spr.from);
	Vector3f pos1 = PositionFinder::getPosition(state, spr.to);

	glLineWidth(0.5);
	glBegin(GL_LINES);
	glVertex3f(pos0[0], pos0[1], pos0[2]);
	glVertex3f(pos1[0], pos1[1], pos1[2]); 
	glEnd();
    }
}

bool Spring::springInSprings(const vector<Spring> &springs, int particle1, int particle2) {
    for (int ii = 0; (unsigned) ii < springs.size(); ii ++){
        Spring spr = springs[ii];
        if (spr.to == particle1 and spr.from == particle2) {
            return true;
        }
        if (spr.to == particle2 and spr.from == particle1) {
            return true;
        }
    }
    return false;
}

Vector3f PositionFinder::getPosition(const vector<Vector3f> &state, int index) {
    return state[2*index];
}

Vector3f PositionFinder::getVelocity(const vector<Vector3f> &state, int index) {
    return state[2*index+1];
}