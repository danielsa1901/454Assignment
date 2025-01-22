// Circle class


#ifndef CIRCLE_H
#define CIRCLE_H

#include "linalg.h"
#include "drawSegs.h"


#define NUM_CIRCLE_PTS 200  // number of points used to draw circle


class Circle {
 public:

  // Constructors

  Circle() {}

  Circle( vec3 p, float s, float maxRad, vec3 c, bool isOut ) {
    pos = p;
    r = 0;
    maxR = maxRad;
    speed = s;
    colour = c;
    isOutgoing = isOut;
  }

  // Draw the circle

  void draw( GPUProgram *gpu, mat4 &worldToScreen ) {
    
    vec3 points[ NUM_CIRCLE_PTS ];
    vec3 colours[ NUM_CIRCLE_PTS ];

    for (int i=0; i<NUM_CIRCLE_PTS; i++) {
      float theta = i * 2*M_PI / (float) NUM_CIRCLE_PTS;
      points[i] = vec3( pos.x + r * cos(theta), pos.y + r * sin(theta), 0 );
      colours[i] = colour;
    }

    mat4 M = worldToScreen;
    
    drawSegs( gpu, GL_LINE_LOOP, points, colours, NUM_CIRCLE_PTS, M );
  }

  // Expand the circle over a time interval, deltaT

  void expand( float deltaT ) {
    r = r + deltaT * speed;
  }

  // Return the current and max radii

  float radius() {
    return r;
  }

  vec3 position() {
    return pos;
  }

  float maxRadius() {
    return maxR;
  }

  bool isOutgoingMissile() {
    return isOutgoing;
  }

  void setColour(vec3 c) {
      colour = c;
  }

 private:

  vec3  pos;			// centre
  float r;			// current radius
  float maxR;			// maximum radius
  float speed;			// speed at which radius increases
  vec3  colour;		        // colour of circle
  bool  isOutgoing;		// is the explosion of an outgoing missile
};


#endif
