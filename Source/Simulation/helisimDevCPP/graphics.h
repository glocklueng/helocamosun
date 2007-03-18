
#ifndef graphics_h
#define graphics_h

/************************************************************
*
*	This defines the graphics of the helicopter models.
*
*	Author: Aaron Kahn
*
*************************************************************/

#include "glos.h" // MS specifc stuff

#include <stdio.h>
#include <GL/gl.h> // system OpenGL includes
#include <GL/glu.h>
//#include <GL/glaux.h>
#include <math.h>
#include <GL/glut.h>
#include <mmsystem.h>

#include "matlib.h"
#include "model.h"

#define SF 2.25

extern float PVP_x;
extern float PVP_y;
extern float PVP_z;
extern double alpha;
extern double beta;
extern double l;

void DrawXcellModel(void);
void DrawXcellShadowModel(void);
void DrawScene(void);



#endif
