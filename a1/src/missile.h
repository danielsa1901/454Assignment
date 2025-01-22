// Missile class


#ifndef MISSILE_H
#define MISSILE_H

#include "linalg.h"
#include "gpuProgram.h"
#include "drawSegs.h"

#define INCOMING_MISSLE_COLOUR vec3(255, 255, 0)
#define OUTGOING_MISSILE_COLOUR vec3(1, 0, 0)
#define MISSILE_SCALE 0.008
#define MISSILE_TRAIL_LIMIT 30


class Missile {
 public:

  // Constructors

  Missile() {}

  Missile( vec3 initPosition, vec3 initVelocity, vec3 target, vec3 _colour ) {
    pos0 = initPosition;
    pos1 = initPosition;
    velocity = initVelocity;
    colour = _colour;
    destY = target.y;
    destX = target.x;
    currentTime = 0;
    for (int i = 0; i < MISSILE_TRAIL_LIMIT; i++)
    {
        trailPoints.add(pos0);
    }
  }

  // Draw the missile and its trail

  void draw( GPUProgram *gpu, mat4 &worldToScreen ) {

    const int numPts = 2;

    trailPoints.remove(0);
    trailPoints.add(pos1);

    vec3 points[  numPts ] = { pos0,   pos1 };
    vec3 colours[ numPts ] = { colour, colour };
    vec3 trail[MISSILE_TRAIL_LIMIT];
    vec3 trailcolour[MISSILE_TRAIL_LIMIT];
    
    for (int i = 0; i < MISSILE_TRAIL_LIMIT && i < trailPoints.size(); i++) {
        trail[i] = trailPoints[i]; // Copy each element from seq
        trailcolour[i] = colour;
    }

    mat4 M = worldToScreen;
    
    drawSegs( gpu, GL_LINE_STRIP, trail, trailcolour, MISSILE_TRAIL_LIMIT, M );

    // define missile shape
    const int missilePts = 5;

    vec3 missilePoints[missilePts] = {
        vec3(-1, -1, 0), //bottom left
        vec3(1, -1, 0), //bottom right
        vec3(-1, 1, 0), //top left
        vec3(1, 1, 0), //top right
        vec3(0, 2, 0) //triangle top
    };

    const int shapePoints = 9;

    vec3 missileTriangles[shapePoints] = {
        missilePoints[0], missilePoints[1], missilePoints[2], // Triangle 1: bottom left, bottom right, top left
        missilePoints[2], missilePoints[1], missilePoints[3], // Triangle 2: top left, bottom right, top right
        missilePoints[2], missilePoints[3], missilePoints[4], // Triangle 1: top left, top right, triangle top
    };

    vec3 missileColours[shapePoints] = {
        colour,
        colour,
        colour,
        colour,
        colour,
        colour,
        colour,
        colour,
        colour
    };

    vec3 xAxis = vec3(0, 1, 0);
    vec3 direction = trailPoints[29] - trailPoints[28];

    // We convert to screen coords last, we need to scale, rotate, and translate it first
    mat4 M2 = worldToScreen * translate(pos1) * rotate(xAxis, direction) * scale(MISSILE_SCALE, MISSILE_SCALE, MISSILE_SCALE);

    drawSegs(gpu, GL_TRIANGLES, missileTriangles, missileColours, 9, M2);
  }

  // Move the missile over a time interval, deltaT

  void move( float deltaT ) {
    pos1 = pos1 + deltaT * velocity;

    // YOUR CODE HERE (Step 6)
    currentTime += deltaT;

    if (this->colour == OUTGOING_MISSILE_COLOUR)
        pos1 = pos1 + currentTime * currentTime * 0.5 * (vec3(0, -0.005, 0));
    else if (this->colour == INCOMING_MISSLE_COLOUR) {
        // significantly reduce the gravity effect for incoming missiles to prevent them from reaching too high of a speed
        // while still allowing them to follow a parabolic trajectory
        pos1 = pos1 + currentTime * currentTime * 0.01 * (vec3(0, -0.005, 0));
    }
  }

  // Return the current position 

  vec3 position() {
    return pos1;
  }

  // Has the missile reached its destination?

  bool hasReachedDestination() {

    // YOUR CODE HERE (Step 3)
    // something with checking it's current position versus something else

      bool flag = false;

      // If we hit the boundary, return true automatically
      if (pos1.y < 0 || pos1.x > 1 || pos1.x < 0)
          return true;
      else if (velocity.y < 0)
      {
          flag = (pos1.y <= destY);
      }
      else if (velocity.y > 0)
      {
          flag = (pos1.y >= destY);
      }
      
      if (velocity.x > 0) // if missile is going left
          flag = (pos1.x >= destX);
      else if (velocity.x < 0) // if missile is going right
          flag = (pos1.x <= destX);

    return flag;
  }

 private:

  vec3 pos0;       // start position
  vec3 pos1;       // current position
  vec3 velocity;   // velocity
  vec3 colour;     // colour of missile trail
  float destY;     // y position at destination
  float destX;     // x position at destination
  float currentTime; // Used for velocity calculations
  seq<vec3> trailPoints; // track the trajectory of the missile
};


#endif
