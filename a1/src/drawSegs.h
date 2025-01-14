// drawSegs.h

#ifndef DRAWSEGS_H
#define DRAWSEGS_H

#include "headers.h"
#include "linalg.h"
#include "gpuProgram.h"

void drawSegs( GPUProgram *gpu, GLuint primitiveType, vec3 *segs, vec3 *colours, int nSegs, mat4 &M );

#endif
