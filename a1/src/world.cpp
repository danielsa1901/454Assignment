// world.cpp


#include "headers.h"
#include "world.h"
#include "mc.h"


#define BORDER  0.05                      // border on screen inside which world is drawn

#define INIT_OUTGOING_MISSILE_SPEED 0.7   // world units per second
#define INIT_INCOMING_MISSILE_SPEED 0.1   // have the incoming missle be slower
#define MAX_TIME 2000
#define EXPLOSION_RADIUS 0.03
#define EXPLOSION_SPEED 0.05



// Draw each of the objects in the world

void World::draw() 

{
  gpu->activate();
    
  for (int i=0; i<silos.size(); i++)
    if (silos[i].isAlive())
      silos[i].draw( gpu, worldToScreen );

  for (int i=0; i<cities.size(); i++)
    if (cities[i].isAlive())
      cities[i].draw( gpu, worldToScreen );

  for (int i=0; i<missilesIn.size(); i++)
    missilesIn[i].draw( gpu, worldToScreen );

  for (int i=0; i<missilesOut.size(); i++)
    missilesOut[i].draw( gpu, worldToScreen );

  for (int i=0; i<explosions.size(); i++)
    explosions[i].draw( gpu, worldToScreen );

  gpu->deactivate();

  // Status message

  static char buffer[1000];

  sprintf( buffer, "MISSILE COMMAND" );
  strokeFont->drawStrokeString( buffer, 0, 0.85, 0.07, 0, CENTRE, vec3(0.2,0.7,0.5) );

  // Draw text for silo missile count
  for (int i = 0; i < silos.size(); i++)
  {
      if (silos[i].isAlive())
      {
          sprintf(buffer, "%d", silos[i].missileCount());
          vec4 siloScreenPos = worldToScreen * vec4(silos[i].position());
          strokeFont->drawStrokeString(buffer, siloScreenPos.x, siloScreenPos.y + SILO_RADIUS / 2, 0.07, 0, CENTRE, vec3(0.2, 0.7, 0.5));
      }
  }
}


// Process a keystroke
//
// Used to fire missiles


void World::handleKeystroke( int key, float x, float y )

{
  // YOUR CODE HERE (Step 1)
  // From the main file (mc.cpp), they already get converted to 'Screen coords' before being passed to us
  // Use the inverse matrix function from the linalg library and apply it to the 'worldToScreen' matrix, and then apply it to the mouse position vector
  vec4 mousePosVec4 = vec4(x, y, 0, 1);
  mousePosVec4 = worldToScreen.inverse() * mousePosVec4;
  vec3 worldMousePos = vec3(mousePosVec4.x, mousePosVec4.y, mousePosVec4.z);

  // Handle key press
    
  switch (key) {

  case 'S':
    fireMissile( 0, worldMousePos );  // left silo
    break;

  case 'D':
    fireMissile( 1, worldMousePos );  // middle silo
    break;

  case 'F':
    fireMissile( 2, worldMousePos );  // right silo
    break;
  }
}


// Compute the worldTop and worldToScrren transform based on the
// window dimensions.  The world is [0,1] x [0,worldTop] and gets
// mapped to the screen coordinates [-1,1] x [-1,1] but with a small
// BORDER of 0.02, so the world *really* gets mapped to
// [-0.98,0.98]x[-0.98,0.98].

void World::reshape( int width, int height )

{
  const float windowLeft   = -1 + BORDER;
  const float windowRight  =  1 - BORDER;
  const float windowBottom = -1 + BORDER;
  const float windowTop    =  1 - BORDER;

  // The world is 1 unit wide, so the world's height must be the
  // window's aspect ratio (height/width).
  
  worldTop = height / (float) width;

  worldToScreen =
    translate( windowLeft, windowBottom, 0 ) * 
    scale( (windowRight - windowLeft) / (1 - 0), (windowTop - windowBottom) / (worldTop - 0), 1 );
}


// Update the state of the world after some time interval, deltaT


void World::updateState( float deltaT )

{
  // Update the time

  currentTime += deltaT;

  // Generate some new missiles.  The rate of missle generation
  // increases with time.
  //
  // YOUR CODE HERE (Step 5a)

  // Define a probability which is compared to an arbitrary maximum runtime
  // where the probability will slowly increase over time, maxing out at 100%
  float probability = currentTime / MAX_TIME;

  // clamp probability to 1 (100%)
  if (probability > 1.0f)
      probability = 1.0f;

  // generate a random number between 0 and 1
  float randomValue = static_cast<float>(std::rand()) / RAND_MAX;

  // fire an imcoming missile if the random number is greater than the calculated probability
  bool fireIncoming = randomValue <= probability;

  if (fireIncoming)
  {
      // make a new random number for the missle's starting x position
      // we need a different rand number or else all the incoming missle positions
      // will be gated by the probability number
      vec3 startPos = vec3(static_cast<float>(std::rand()) / RAND_MAX, worldTop, 0);
      vec3 target = vec3(static_cast<float>(std::rand()) / RAND_MAX, 0, 0);

      // normalize the direction vector, to be scaled
      vec3 direction = vec3(target - startPos).normalize();

      missilesIn.add(Missile(startPos, INIT_INCOMING_MISSILE_SPEED * direction, target, INCOMING_MISSLE_COLOUR));
  }

  // Look for terminating missiles
  
  for (int i=0; i<missilesIn.size(); i++)
    if (missilesIn[i].hasReachedDestination()) {
      // YOUR CODE HERE (Step 5b)
      // add a new circle object to the explosion seq
      explosions.add(Circle(missilesIn[i].position(), EXPLOSION_SPEED, EXPLOSION_RADIUS, INCOMING_MISSLE_COLOUR, false));
      // remove missile that is now exploding
      missilesIn.remove(i);
    }

  for (int i=0; i<missilesOut.size(); i++)
    if (missilesOut[i].hasReachedDestination()) {
      // YOUR CODE HERE (Step 5b)
      // add a new circle object to the explosion seq
      explosions.add(Circle(missilesOut[i].position(), EXPLOSION_SPEED, EXPLOSION_RADIUS, OUTGOING_MISSILE_COLOUR, true));
      // remove missile that is now exploding
      missilesOut.remove(i);
    }

  // Look for terminating explosions
  // Half of a novel feature
  // This will change the colour of the explosion from white to yellow to orange to red based on its size.
  // And will delete the explosion once exceeding the max radius.

  for (int i = 0; i < explosions.size(); i++) {
      if (explosions[i].radius() >= explosions[i].maxRadius()) {
          explosions.remove(i);
          i--;
      }
      else if (explosions[i].radius() >= explosions[i].maxRadius() * 0.75) {
          explosions[i].setColour(vec3(255, 0, 0));
      }
      else if (explosions[i].radius() >= explosions[i].maxRadius() * 0.5) {
          explosions[i].setColour(vec3(255, 165, 0));
      }
      else if (explosions[i].radius() >= explosions[i].maxRadius() * 0.25) {
          explosions[i].setColour(vec3(255, 255, 0));
      }
      else {
          explosions[i].setColour(vec3(255, 255, 255));
      }
  }

  // Update all the moving objects

  for (int i=0; i<missilesIn.size(); i++)
    missilesIn[i].move( deltaT );

  for (int i=0; i<missilesOut.size(); i++)
    missilesOut[i].move( deltaT );

  for (int i=0; i<explosions.size(); i++)
    explosions[i].expand( deltaT );

  // Check for explosions overlapping silos and cities
  //
  // YOUR CODE HERE (Step 5c)

  // Only incoming missle explosions can destroy silos and cities
  for (int i = 0; i < explosions.size(); i++) {
      if (!explosions[i].isOutgoingMissile()) {

		  // Check if the explosion is within the radius of the silo, if so, deactivate the silo
          for (int j = 0; j < silos.size(); j++) {
              if ((explosions[i].position() - silos[j].position()).length() <= SILO_RADIUS + explosions[i].radius()) {

                  silos[j].deactivate();
              }
          }
		  // Check if the explosion is within the radius of the city, if so, deactivate the city
		  // Because incoming explosions only occur at the bottom of the screen, we can just check the bottom cities using the city width.
           for (int j = 0; j < cities.size(); j++) {
               if ((explosions[i].position() - cities[j].position()).length() <= CITY_WIDTH + explosions[i].radius()) {
                   cities[j].deactivate();
                   citiesLeftAlive--;
               }
           }
	  }
  }

  // YOUR CODE HERE (Step 5d)
  // Check for outgoing missiles that have exploded and intersects an incoming missile
  // Check is the difference between the two missile is less than the explosion radius
  for (int i=0; i<explosions.size(); i++) 
    if (explosions[i].isOutgoingMissile()) {
		for (int j = 0; j < missilesIn.size(); j++) {
			if ((explosions[i].position() - missilesIn[j].position()).length() <= EXPLOSION_RADIUS) {
                // remove missile object from seq
				missilesIn.remove(j);
			}
		}
    }

  // Check for advancing to next round (e.g. when a certain number of
  // incoming missile have finished, or when all cities are destroyed).

  if (cities.size() == 0)
    pauseGame = true;
}


// Fire a missile


void World::fireMissile( int siloIndex, vec3 worldMousePos )

{
  // YOUR CODE HERE (Step 2)
  // Create a missle based open the siloindex (where it's being fired from, 0->Left, 1->Middle, 2->Right), and the Mouse position (where the missile is going)

    // check if the given silo is eligible to fire a missile
    if (silos[siloIndex].canShoot())
    {
        // Find silo position, which is where the missile is fired from
        vec3 siloPos = silos[siloIndex].position();

        // Find the direction of the missile
        vec3 direction = worldMousePos - siloPos;

        // make the direction unit length, which we can then scale
        direction = direction.normalize();

        // Draw the missle
        missilesOut.add(Missile(siloPos, INIT_OUTGOING_MISSILE_SPEED * direction, worldMousePos, OUTGOING_MISSILE_COLOUR));

        // Decrement the missile count from the given silo
        silos[siloIndex].decrMissiles();
    }
}



// Initialize the world.
//
// Should do things differently for rounds 1,2,3,etc.
//
// Currently, the round is ignored.



void World::initWorld()

{
  // Keep track of the time

  currentTime = 0;

  timeOfIncomingFlight = 6;         // seconds for incoming missile to travel from top to bottom
  scorePerDestroyedMissile = 100;

  // Seed the random number generator

#ifdef LINUX
  struct timeval t;
  gettimeofday( &t, NULL );
  srand( t.tv_sec );
#else
  srand( 12345 );
#endif

  // Silos are added in order left, middle, right so that they have
  // indices 0, 1, and 2 (respectively) in the "silos" array.

  silos.add(  Silo( vec3( 0.1, 0, 0 ) ) );
  silos.add(  Silo( vec3( 0.5, 0, 0 ) ) );
  silos.add(  Silo( vec3( 0.9, 0, 0 ) ) );

  // Cities on the left

  cities.add( City( vec3( 0.2, 0, 0 ) ) );
  cities.add( City( vec3( 0.3, 0, 0 ) ) );
  cities.add( City( vec3( 0.4, 0, 0 ) ) );

  // Cities on the right

  cities.add( City( vec3( 0.6, 0, 0 ) ) );
  cities.add( City( vec3( 0.7, 0, 0 ) ) );
  cities.add( City( vec3( 0.8, 0, 0 ) ) );

  citiesLeftAlive = cities.size();
}



// Define basic shaders


char *World::vertexShader = R"XX(

  #version 300 es
  
  uniform mat4 M;

  layout (location = 0) in vec4 position_in;
  layout (location = 1) in vec3 colour_in;

  smooth out mediump vec3 colour;
  
  void main() {
    gl_Position = M * position_in;
    colour = colour_in;
  }
)XX";


char *World::fragmentShader = R"XX(

  #version 300 es

  smooth in mediump vec3 colour;

  out mediump vec4 fragColour;
  
  void main() {
    fragColour = vec4( colour, 1.0 );
  }
)XX";
