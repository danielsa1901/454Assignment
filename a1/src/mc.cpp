// Missile Command


#include "headers.h"
#include "world.h"
#include "strokefont.h"
#include "gpuProgram.h"


// Window and world dimensions


#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 400

#define WINDOW_X_POS  100
#define WINDOW_Y_POS   50

World *world;
GPUProgram *gpu;
StrokeFont *strokeFont;

bool pauseGame = false;

float windowWidth  = WINDOW_WIDTH;
float windowHeight = WINDOW_HEIGHT;

float getTime();


// Mouse click handler

void mouseButtonCallback( GLFWwindow* window, int button, int action, int mods )

{
  if (action == GLFW_PRESS)
    cout << "Press S, D, F to shoot from left, middle, right silos." << endl;
}


// Key press handler

void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
  
{
  if (action == GLFW_PRESS)

    if (key == GLFW_KEY_ESCAPE)
      glfwSetWindowShouldClose( window, GL_TRUE ); // quit

    else if (key == 'P')
      pauseGame = !pauseGame;
  
    else {
      double x, y;
      glfwGetCursorPos(window, &x, &y );

      // Provide mouse coordinates in the openGl [-1,1]x[-1,1]
      // coordinate system, but flip the y coordinates so that it
      // increases upward.

      world->handleKeystroke( key, 
			      x/(float)windowWidth*2-1, 
			      (windowHeight-y)/(float)windowHeight*2-1 );
    }
}


// Callbacks for when window size changes

void windowReshapeCallback( GLFWwindow* window, int width, int height )

{
  world->reshape( width, height );

  windowWidth  = width;
  windowHeight = height;
}


void framebufferReshapeCallback( GLFWwindow* window, int width, int height )

{
  glViewport( 0, 0, width, height );
}


// Error callback

void errorCallback( int error, const char* description )

{
  cerr << "Error " << error << ": " << description << endl;
  exit(1);
}


// The main program

int main()
  
{
  // Set up GLFW

  GLFWwindow* window;

  if (!glfwInit()) {
    cerr << "GLFW failed to initialize" << endl;
    return 1;
  }
  
  glfwSetErrorCallback( errorCallback );
  
  // Open window

#ifdef MACOS
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#else
  glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
#endif

  window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Missile Command", NULL, NULL);
  
  if (!window) {
    glfwTerminate();
    cerr << "GLFW failed to create a window" << endl;

#ifdef MACOS
    const char *descrip;
    int code = glfwGetError( &descrip );
    cerr << "GLFW code:  " << code << endl;
    cerr << "GLFW error: " << descrip << endl;
#endif
    
    return 1;
  }

  glfwSetWindowPos(window, WINDOW_X_POS, WINDOW_Y_POS );

  glfwMakeContextCurrent( window );
  
  glfwSwapInterval( 1 );
  
  // Set OpenGL function bindings

  gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress );

  // Set up callbacks

  glfwSetKeyCallback( window, keyCallback );
  glfwSetMouseButtonCallback( window, mouseButtonCallback );
  glfwSetWindowSizeCallback( window, windowReshapeCallback );
  glfwSetFramebufferSizeCallback( window, framebufferReshapeCallback );
  
  // Fonts

  strokeFont = new StrokeFont();

  // Set up world

  world = new World( WINDOW_WIDTH, WINDOW_HEIGHT );

  // Run

  float prevTime, thisTime;
  prevTime = getTime();

  while (!glfwWindowShouldClose( window )) {

    // Find elapsed time since last render

    thisTime = getTime();
    float elapsedSeconds = thisTime - prevTime;
    prevTime = thisTime;

    // Update the world state

    if (!pauseGame)
      world->updateState( elapsedSeconds );

    // Display the world

    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT );

    world->draw();

    glfwSwapBuffers( window );
    
    // Check for new events

    glfwPollEvents();
  }

  glfwDestroyWindow( window );
  glfwTerminate();
  return 0;
}


float getTime()

{
  static time_t initialSeconds = 0;  // subtract this from times to avoid loss of float precision
  
#ifdef _WIN32

  struct timeb thisTime;
  ftime( &thisTime );

  if (initialSeconds == 0)
    initialSeconds = thisTime.time;

  return (thisTime.time-initialSeconds) + thisTime.millitm / 1000.0;

#else

  struct timeval thisTime;
  gettimeofday( &thisTime, NULL );

  if (initialSeconds == 0)
    initialSeconds = thisTime.tv_sec;

  return (thisTime.tv_sec-initialSeconds) + thisTime.tv_usec / 1000000.0;

#endif
}
