#include <iostream>
#include "integrator.h"
#include "pendulumSystem.h"
#include <assert.h>

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
#include "simpleSystem.h"


/*
 * Euler according to X(t+h) = X + h*f(X,t)
 */
vector<Vector3f> Integrator::evalEuler(ParticleSystem* system, float h) {
  vector<Vector3f> state = system->getState();
  vector<Vector3f> f = system->evalF(state); // the derivative of the state

  vector<Vector3f> newState; // updated based on the derivative
  
  for (unsigned int ii = 0; ii < state.size(); ii++)
    {
      Vector3f pos = state[ii];
      Vector3f f = f[ii];
      Vector3f newPos = pos + h*f;
      newState.push_back(newPos);
    }
   return newState;
}

/*
 *  Trapezoid according to X(t + h) = X + (h/2) * (f0 + f1)
 *  where f0 = f(X,t)
 *  and f1 = f(X+h, t+h)
 */
vector<Vector3f> Integrator::evalTrapezoid(ParticleSystem* system, float h) {
    vector<Vector3f> state = system->getState();
    vector<Vector3f> f = system->evalF(state);
    vector<Vector3f> vec; 
    
    for (unsigned int ii = 0; ii < state.size(); ii++) {
	  vec.push_back(state[ii] + h*f[ii]);
    }

    vector<Vector3f> f1 = system->evalF(vec);
    
    vector<Vector3f> newState;
    for (unsigned int ii = 0; ii < state.size(); ii++) {
        Vector3f pos = state[ii];
        Vector3f f0 = f[ii];
        Vector3f newPos = pos + (h*0.5)*(f0 + f1[ii]);
        newState.push_back(newPos);
    }
    
    return newState; 
}

/*
 * Gazer according to Opengazer coordinates
 */
vector<Vector3f> Integrator::evalGazer(ParticleSystem* system, float h) {
    h*=3.0;
    
    vector<Vector3f> state = system->getState();
    vector<Vector3f> f0 = system->evalF(state);
    vector<Vector3f> vec; 
    
    for (unsigned int ii = 0; ii < state.size(); ii++) {
	  vec.push_back(state[ii] + h*f0[ii]);
    }
   
    system->center = system->center + h*(SimpleSystem::getGazeCoordinates() - system->center);

    vector<Vector3f> f1 = system->evalF(vec);
    
    vector<Vector3f> newState;
    for (unsigned int ii = 0; ii < state.size(); ii++) {
        Vector3f pos = state[ii];
	
        Vector3f newPos = pos + h*0.5*(f1[ii] + f0[ii]);
        newState.push_back(newPos);
    }
    
    
    return newState; 
    


    
}



