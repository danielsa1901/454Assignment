// Missile class


#ifndef MISSILE_H
#define MISSILE_H

#include "linalg.h"
#include "gpuProgram.h"
#include "drawSegs.h"


class Missile {
 public:

  // Constructors

  Missile() {}

  Missile( vec3 initPosition, vec3 initVelocity, float stopAtY, vec3 _colour ) {
    pos0 = initPosition;
    pos1 = initPosition;
    velocity = initVelocity;
    colour = _colour;
    destY = stopAtY;
  }

  // Draw the missile and its trail

  void draw( GPUProgram *gpu, mat4 &worldToScreen ) {

    const int numPts = 2;

    vec3 points[  numPts ] = { pos0,   pos1 };
    vec3 colours[ numPts ] = { colour, colour };

    mat4 M = worldToScreen;
    
    drawSegs( gpu, GL_LINES, points, colours, numPts, M );
  }

  // Move the missile over a time interval, deltaT

  void move( float deltaT ) {
    pos1 = pos1 + deltaT * velocity;

    // YOUR CODE HERE (Step 6)
  }

  // Return the current position 

  vec3 position() {
    return pos1;
  }

  // Has the missile reached its destination?

  bool hasReachedDestination() {

    // YOUR CODE HERE (Step 3)
    // something with checking it's current position versus something else

    return false;
  }

 private:

  vec3 pos0;       // start position
  vec3 pos1;       // current position
  vec3 velocity;   // velocity
  vec3 colour;     // colour of missile trail
  float destY;     // y position at destination
};


#endif
