// A Silo


#ifndef SILO_H
#define SILO_H

#include "headers.h"
#include "drawSegs.h"

#define SILO_RADIUS 0.04
#define NUM_SEMICIRCLE_PTS 100


class Silo {

 public:

  // Constructors 

  Silo() {}

  Silo( vec3 _pos ) {

    roundsLeft = 15;
    alive = true;
    pos = _pos;
  }

  bool canShoot() {
    return (alive && roundsLeft > 0);
  }

  void decrMissiles() {
    roundsLeft--;
  }

  vec3 position() {
    return pos;
  }

  int missileCount() {
    return roundsLeft;
  }

  bool isAlive() {
    return alive;
  }

  void deactivate() {
    alive = false;
  }

  // Draw the silo

  void draw( GPUProgram *gpu, mat4 &worldToScreen ) {

    // YOUR CODE HERE (Step 4)
    
    const vec3 colour(1,1,1);

    vec3 points[ NUM_SEMICIRCLE_PTS ];
    vec3 colours[ NUM_SEMICIRCLE_PTS ];

    for (int i=0; i<NUM_SEMICIRCLE_PTS; i++) {
      float theta = i * M_PI / (float) NUM_SEMICIRCLE_PTS;
      // when defining points, don't apply scaling or translation yet
      points[i]  = vec3( cos(theta), sin(theta), 0 );
      colours[i] = colour;
    }

    // scale to correct size, then translate to correct position (offload to GPU)
    mat4 M = worldToScreen * translate( pos) * scale (SILO_RADIUS, SILO_RADIUS, SILO_RADIUS);
    
    drawSegs( gpu, GL_LINE_LOOP, points, colours, NUM_SEMICIRCLE_PTS, M );
  }

 private:

  int roundsLeft;
  bool alive;
  vec3 pos;
};
  

#endif
