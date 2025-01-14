// drawSegs.cpp

#include "drawSegs.h"


void drawSegs( GPUProgram *gpu, GLuint primitiveType, vec3 *pts, vec3 *colours, int nPts, mat4 &M )

{
  GLuint VAO;
  
  glGenVertexArrays( 1, &VAO );
  glBindVertexArray( VAO );

  // Set up points
  
  GLuint VBO0;

  glGenBuffers( 1, &VBO0 );
  glBindBuffer( GL_ARRAY_BUFFER, VBO0 );
  glBufferData( GL_ARRAY_BUFFER, nPts * sizeof(vec3), pts, GL_STATIC_DRAW );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( 0 );

  // Set up colours
  
  GLuint VBO1;

  glGenBuffers( 1, &VBO1 );
  glBindBuffer( GL_ARRAY_BUFFER, VBO1 );
  glBufferData( GL_ARRAY_BUFFER, nPts * sizeof(vec3), colours, GL_STATIC_DRAW );
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( 1 );

  // Draw

  gpu->setMat4( "M", M );

  glDrawArrays( primitiveType, 0, nPts );

  // Clean up
  
  glDeleteBuffers( 1, &VBO0 );
  glDeleteBuffers( 1, &VBO1 );

  glDisableVertexAttribArray( 0 );
  glDisableVertexAttribArray( 1 );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  glBindVertexArray( 0 );
  glDeleteVertexArrays( 1, &VAO );
}

