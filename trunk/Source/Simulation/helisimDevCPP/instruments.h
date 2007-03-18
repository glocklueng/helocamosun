/* -*- c -*- */
/* $Header: c:/work/repository/DWHils/src/graphics/instruments.h,v 1.3 2001/10/24 22:54:39 Administrator Exp $*/

#ifndef instruments_h
#define instruments_h

/***************************************************************************
*
*	This header file is to generate the flight instruments for a UAV
* flight ground station.  These function are called in the graphics context
* running the GLUT library.  
*
*	Author: Aaron Kahn
*	copyright 2001
*
* @pkg				DWHils
* @version			$Revision: 1.3 $
*
*****************************************************************************/

#include "glos.h" // MS specifc stuff

#include <stdio.h>
#include <GL/gl.h> // system OpenGL includes
#include <GL/glu.h>
//#include <GL/glaux.h>
#include <math.h>
#include <GL/glut.h>

#include "matlib.h"		// for some of the constants used

// FUNCTIONS

/*	This will draw an artificial horzion for attitude and heading
* indication for a pilot.  The inputs are pitch, roll, and yaw of the
* vehicle.  Also, the altitude and speed for the side bars.  In addition
* to only displaying numbers, a three character message can be displayed
* as well.
*  The ranges are data are...
*	Pitch	-90 to 90 degrees
*	Roll	-180 to 180 degrees
*	Yaw		-180 to 180 degrees
*	Altitude	NO RANGE (ft)
*	Speed		NO RANGE (knots)
*	Message	3 characters
*/
void horizon(double pitch, double roll, double yaw, double altitude, double speed, char *message);


/*	This is a useful routine to display a string message on the 
* GLUT GL window.
*/
void showMessage( GLfloat x, GLfloat y, char *message, float scale );


/*	This will draw text on the OpenGL screen.
* showMessege uses this to display the charactors.
*/
void drawText( char *message );

#endif
