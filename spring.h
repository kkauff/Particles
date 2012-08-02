/* 
 * File:   spring.h
 * Author: eitak
 *
 */

#ifndef SPRING_H
#define	SPRING_H

#include <vector>
#include <vecmath.h>


using namespace std;

class Spring {
public:
    Spring(float k, float r, int from, int to);  
    float k;
    float r;
    int from;
    int to;

    Vector3f getForce(Vector3f from_pos, Vector3f to_pos);

    static bool leftEdge(int width, int jj);
    static bool rightEdge(int width, int jj);
    static bool topEdge(int ii);
    static bool bottomEdge(int height, int ii);

    static Vector3f getNetForce(int index, const vector<vector<Spring*> > &springs, const vector<Vector3f> &state);
    static void drawSprings(const vector<Vector3f> &state, const vector<Spring> &springs);

    static vector<Spring> getSpringsForParticle(const vector<Spring> &springs, int index);

    static bool springInSprings(const vector<Spring> &springs, int particle1, int particle2);
};

class PositionFinder{
public: 
    static Vector3f getPosition(const vector<Vector3f> &state, int index);
    static Vector3f getVelocity(const vector<Vector3f> &state, int index);
    
};

#endif	/* SPRING_H */