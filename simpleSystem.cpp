#include <iostream>
#include <algorithm>

#include "simpleSystem.h"
#include "particleSystem.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "fstream"
#include "iostream"
#include "sstream"
#include "iterator"

using namespace std;

SimpleSystem::SimpleSystem(INTEGRATOR integrator, int numParticles) : ParticleSystem(integrator, numParticles) {
    this->integrator = integrator;
    this->PARTICLE_RADIUS = 0.05f;
    this->SAMPLE_DISTANCE = 4;
    
    //define control particles
    ControlParticle cp0 = ControlParticle(2*Vector3f(-1,1,0), Vector3f(1,0,0), 1.0);
    ControlParticle cp1 = ControlParticle(2*Vector3f(1,1,0), Vector3f(0,-1,0), 1.0);
    ControlParticle cp2 = ControlParticle(2*Vector3f(1,-1, 0), Vector3f(-1,0,0), 1.0);
    ControlParticle cp3 = ControlParticle(2*Vector3f(-1,-1,0), Vector3f(0,1,0), 1.0);
//    ControlParticle cp4 = ControlParticle(2*Vector3f(1.0,0.5,0), Vector3f(0,-1,0), 1.0);
//    ControlParticle cp5 = ControlParticle(2*Vector3f(-1.0,0.5,0), Vector3f(0,-1,0), 1.0);
//    ControlParticle cp6 = ControlParticle(2*Vector3f(0,1.0,0), Vector3f(0,1,0), 1.0);
//    ControlParticle cp7 = ControlParticle(2*Vector3f(0.2,1.2,0), Vector3f(1,-1,0), 1.0);
//    ControlParticle cp8 = ControlParticle(2*Vector3f(-0.2,1.2,0), Vector3f(-1,-1,0), 1.0);
    this->controlParticles.push_back(cp0);
    this->controlParticles.push_back(cp1);
    this->controlParticles.push_back(cp2);
    this->controlParticles.push_back(cp3);
//    this->controlParticles.push_back(cp4);
//    this->controlParticles.push_back(cp5);
//    this->controlParticles.push_back(cp6);
//    this->controlParticles.push_back(cp7);
//    this->controlParticles.push_back(cp8);
    
    vector<Vector3f> initState;
    center = Vector3f(0,0,0);
    Vector3f vel = Vector3f(0,0,0);
    double mult = 0.2;
    double disperseFactor = 5.0;
    //initial distribution is a random distribution about the center
    for (int ii = 0; ii < numParticles; ii++) {
	float x = disperseFactor*(((float)rand() / (float)RAND_MAX) - 0.5); // random float between -1 and 1
	float y = disperseFactor*(((float)rand() / (float)RAND_MAX) - 0.5);
	float z = disperseFactor*(0);
	Vector3f pos = center + Vector3f(x, y, z);
	initState.push_back(pos);
	initState.push_back(vel);
    }

    state = initState;
}


/**
 * Gets the coordinates of the gaze tracker and converts them to this system's frame.
 * @return A Vector3f containing the system coordinates of the user's gaze.
 */
Vector3f SimpleSystem::getGazeCoordinates() {
    //get the coordinates from opengazer
    using namespace std;
    ifstream t("/Users/eitak/CGL/xyz.txt");
    stringstream buffer;
    buffer << t.rdbuf();
    string coord = buffer.str();
    int split = coord.find(" ");
    string xstr = coord.substr(0,split);
    string ystr = coord.substr(split+1, split);
    int xgaze = atoi(xstr.c_str());
    int ygaze = atoi(ystr.c_str());
    
    //coordinates are for screen size 1280x800
    //our coordinates go from -absx to +absx 
    // want range to go from 0-->1280 to -5-->5 and 0-->800 to -5-->5
    double absx = 7.75;
    double absy = 4.5;
    // coordinates in particle(s) window
    double x = (xgaze / 1280.0)*2*absx - absx;
    double y = (ygaze / 800.0)*2*absy - absy;
    
    
    return Vector3f(x, -y, 0.0);
}

vector<Vector3f> SimpleSystem::evalF(const vector<Vector3f> &state) {
    
    float V_e = (4.0/3.0)*3.14*pow(PARTICLE_RADIUS, 3); // volume of a particle, r = 0.05 so V_e = 4/3 * pi * 0.05^3
    float h = SAMPLE_DISTANCE/1.5; // 2.5 * average sample distance of control particles
    float w_a = 7.0; // strength of the attractive force from control particles
    float w_v = 10.0; // strength of the velocity force from control particles
    
    float w_og = 0.25; // strength of gaze tracking force
    
    float mass = 0.1;
    float gravity = -9.8;
    
    Vector3f gazeCoord = getGazeCoordinates();
    
    
    vector<Vector3f> f;
    
    for (int ii = 0; ii < numParticles; ii++) {

	Vector3f velocity = PositionFinder::getVelocity(state, ii);
	Vector3f position = PositionFinder::getPosition(state, ii);
	
	Vector3f gazeDiff = gazeCoord - position;
	Vector3f v_gaze = gazeDiff;
	
	
	
	
	Vector3f f_gravity = mass*Vector3f(0, gravity, 0);
	
	Vector3f f_attract = Vector3f(0,0,0);
	Vector3f f_velocity = Vector3f(0,0,0);
	
	for (int kk = 0; kk < controlParticles.size(); kk++) {
	    
	    //h = controlParticles[kk].influenceRadius;
	    Vector3f attDiff = controlParticles[kk].position - position;
	    float d = attDiff.abs(); 
	    //W(d,h)
	    float W = 0; // if d >= h
//	    if (d <= h/2.0) {
//		W = 1.0;
//	    } else if (d > h/2.0 && d < h) {
//		W = 2 - (2.0*d / h);
//	    }
	    if (d < h) {
		W = (315.0/(64*3.14*pow(h, 9)))*(pow((h*h-d*d),3));
		w_og = 1/W;
	    }
	
	    float alpha = 1 - min(1.0f, numParticles*V_e*W); // should multiply V_e*W by numParticles?
	    
	    f_attract = f_attract + alpha*attDiff.normalized()*W;
	    
	    Vector3f velDiff = controlParticles[kk].velocity - velocity;
	    f_velocity = f_velocity + (velDiff*W);
	    
	}
	f_attract = f_attract*w_a;
	f_velocity = f_velocity*w_v;
	
	Vector3f f_gaze = w_og*(gazeCoord - position - velocity);
	
	
	Vector3f netForce = f_velocity + f_attract + 0.01*(gazeCoord - center);
	
	
	f.push_back(velocity); // push back velocity and gaze into position
	f.push_back(f_velocity + f_attract);
	//std::cout << f_gaze.abs() << std::endl;
	
    }
    
    
    return f;

}


void SimpleSystem::draw() {
    for (int ii = 0; ii < numParticles; ii++) {
	Vector3f pos = PositionFinder::getPosition(SimpleSystem::getState(), ii);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glutSolidSphere(PARTICLE_RADIUS,10.0f,10.0f);
	glPopMatrix();
    }
}

void SimpleSystem::shake(){}

