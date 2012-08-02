#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "vecmath.h"
#include <vector>
#include "particleSystem.h"
#include <netinet/in.h>

using namespace std;

class Integrator {
  
 public: 
  static vector<Vector3f> evalEuler(ParticleSystem* system, float h);
  static vector<Vector3f> evalTrapezoid(ParticleSystem* system, float h);
  static vector<Vector3f> evalGazer(ParticleSystem* system, float h);
  

};



#endif
