// The world


#ifndef STATE_H
#define STATE_H



#include "seq.h"
#include "linalg.h"
#include "missile.h"
#include "silo.h"
#include "city.h"
#include "circle.h"
#include "gpuProgram.h"
#include "strokefont.h"


// The world lives in a box of size [0,1] x [0,worldTop].


class World {

 public:

  World( int width, int height ) {

    round = 1;
    score = 0;

    initWorld();

    strokeFont = new StrokeFont();

    // record window info

    reshape( width, height );

    // Set up shaders

    gpu = new GPUProgram();
    gpu->init( vertexShader, fragmentShader, "World constructor" );
  }

  void initWorld();
  void draw();
  void fireMissile( int siloIndex, vec3 worldMousePos );
  void updateState( float deltaT );
  void handleKeystroke( int key, float x, float y );
  void reshape( int width, int height );

 private:

  float currentTime;		// current game time
  float timeOfIncomingFlight;	// time taken for incoming missile to reach bottom
  float worldTop;               // y coordinate at top of world (where incoming missiles arrive from)
  mat4 worldToScreen;           // transform from world coordinates to [-1,1]x[-1,1] screen coordinates

  int round;                    // current round
  int score;
  int citiesLeftAlive;
  
  int scorePerDestroyedMissile; // should increase in subsequent rounds

  // GPU stuff
  
  GPUProgram *gpu;
  static char *vertexShader;
  static char *fragmentShader;

  StrokeFont *strokeFont;       // for drawing characters

  // Game objects
  
  seq<Silo> silos;
  seq<City> cities;
  seq<Missile> missilesIn;
  seq<Missile> missilesOut;
  seq<Circle> explosions;
};


#endif
