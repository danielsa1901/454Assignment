// Missile class


#ifndef MISSILE_H
#define MISSILE_H

#include "linalg.h"
#include "gpuProgram.h"
#include "drawSegs.h"

#define INCOMING_MISSLE_COLOUR vec3(255, 255, 0)
#define OUTGOING_MISSILE_COLOUR vec3(1, 0, 0)
#define Y_TOLERANCE 0.1


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
    currentTime = 0;
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
    currentTime += deltaT;

    if (this->colour == OUTGOING_MISSILE_COLOUR)
        pos1 = pos1 + currentTime * currentTime * 0.5 * (vec3(0, -0.005, 0));
  }

  // Return the current position 

  vec3 position() {
    return pos1;
  }

  // Has the missile reached its destination?

  bool hasReachedDestination() {

    // YOUR CODE HERE (Step 3)
    // something with checking it's current position versus something else

    // if the missle is going downwards (dir is negative)
    // check if the current y coord is greater than the destY, other we've hit it
      cout << "posy: " << pos1.y << endl;
      if (pos1.y < 0 || pos1.x > 1 || pos1.x < 0)
          return true;
      else if (velocity.y < 0)
      {
          return (pos1.y <= destY);
      }
      else if (velocity.y > 0)
      {
          return (pos1.y >= destY);
      }

    return false;
  }

 private:

  vec3 pos0;       // start position
  vec3 pos1;       // current position
  vec3 velocity;   // velocity
  vec3 colour;     // colour of missile trail
  float destY;     // y position at destination
  float currentTime; // Used for velocity calculations
};


#endif
