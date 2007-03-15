
#ifndef simlib_h
#define simlib_h

/********************************************************************************
*
*	This library is used for rigid body 6-DOF simulations of vehicles.  It contains
* many useful functions, such as 6-DOF rigid body dynamics, 4th order RK integrator,
* contact/collision simulation, IMU emulator, servo-motor actuator model, gravity model,
* and wind model.
*	For each of these features, there is a structure that is utilized to access
* the internal states of these features.  In particular, the 6-DOF dynamics
* are done in round-earth WGS-84 approximation.  The position out is in both
* geodetic latitude, longitude, and altitude above mean-sea-level (MSL), and 
* ECEF coordinates.  Other parameters are in body axis.  
*
*	For more information, see the descriptions with each of the functions.
*
*	NOTE: This library uses the matlib library for matrix functions.
*
*	Author: Aaron Kahn
*	copyright: 2001
*	Sources:	The Global Positioning System and Inertial Navigation
*					Farrel, Jay A., Barth, Matthew.
*
*				Aircraft Control and Simulation
*					Stevens, Brian L., Lewis, Frank L.
*
*
***********************************************************************************/

#ifdef WIN32
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include "matlib.h"
#endif

// VxWorks
#ifndef WIN32
#include "vxWorks.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

#include "c:/projects/matlib/matlib.h"
#endif


/*	This is an IMU emulation function.  Given some geometric information
* on the position of the IMU relative the the position of the aircraft,
* a new inertial solution can be solved.  The IMU that is being 
* simulated is a full-state strapdown IMU.  It is capable of delivering
* acceleration, velocity, position, angular rate, and attitude at the 
* IMU's current location and orientation.  
*
* See the structures below for the input and output arguments.  All of the
* input arguments must be defined.  Units are commented next to each of 
* the values.
*/

struct imu_inputs_def {
	double cg2imu[MAXSIZE];					// position vector from CG->IMU body axis [X Y Z] (ft)
	double body2imu[MAXSIZE][MAXSIZE];		// rotation matrix body axis->IMU axis
	double cg_accel[MAXSIZE];				// body axis accel. at CG [X Y Z] (ft/s/s)
	double cg_uvw[MAXSIZE];					// body axis veloctiy at CG [X Y Z] (ft/s)
	double cg_pos[MAXSIZE];					// ECEF pos. at CG [X Y Z] (ft)
	double cg_alpha[MAXSIZE];				// body axis angular accel. at CG [Pdot Qdot Rdot] (rad/s/s)
	double cg_pqr[MAXSIZE];					// body axis angular rates at CG [P Q R] (rad/s)
	double cg_THETA[MAXSIZE];				// body axis attitude at CG [phi theta psi] (rad)
};

struct imu_outputs_def {
	double accel[MAXSIZE];					// IMU axis accel at IMU [X Y Z] (ft/s/s)
	double uvw[MAXSIZE];					// IMU axis velocity at IMU [X Y Z] (ft/s)
	double ECEFpos[MAXSIZE];				// ECEF pos of IMU [X Y Z] (ft)
	double LLHpos[MAXSIZE];					// LLH pos of IMU [latitude longitude altitude] (rad)(rad)(ft MSL + up)
	double pqr[MAXSIZE];					// IMU axis angular rate at IMU [P Q R] (rad/s)
	double THETA[MAXSIZE];					// IMU axis attitude at IMU [phi thata psi] (rad)
};

void IMUsim(struct imu_inputs_def *pIn, struct imu_outputs_def *pOut);


/*	This is a 4th order Runga-Kutta integration routine.  It is designed to work
* with a vector of inputs, and produce a vector of outputs at the next time step.
* A time step input is needed for the dt value, and the number of states to integrate.
* There is the requirement that the function to be integrated is in a specific format...
*
*	RK4(double *state,					// state vector (INPUT and OUTPUT) 
*		double *state_dot,				// derivatives of state vector (OUTPUT)
*		double *force,					// any forcing values (INPUT)
*		double t,						// current time (INPUT)
*		double *args,					// any arguments for the called derivative function (INPUT)
*		int n,							// number of states
*		double dt,						// time step
*		void (*pfunc)					// pointer to called function (INPUT)
*		(double *Xdot,					// returned state derivative (OUTPUT)
*		 double *X,						// state vector (INPUT called by RK4)
*		 double *t,						// time (INPUT called by RK4)
*		 double *U,						// passed forcing values (INPUT called by RK4)
*		 double *args) );				// passed arguments for derivative function (INPUT called by RK4)
*
*	The state out of this function is the state at t+dt.
*/
void RK4(double *state, double *state_dot, double t, double *force, double *args, int n, double dt, 
		 void (*pfunc)(double *Xdot, double *X, double *t, double *U, double *args) );



/*	This is a 6 degree-of-freedom (6-DOF) rigid body simulation.  The values
* that are needed are listed in the input structure.  The other structure, the
* state structure needs to be initialized with the desired starting values.  
* An Init function is provided to help with this initialization.  It only needs
* to be called once.  The structure for the Init function is sixdof_init_inputs_def.
*	It is noted that this simulation is based on quaternion parameters.  For this 
* reason, euler2quat function needs to be used for the attitude initialization.
* Latitude, longitude, and altitude (rad)(rad)(ft MSL + up) need to be used for
* the initialization.  ECEF is generated from this.  Units are listed in the
* structure comments.  
*	Propogation of the state vector is done with RK4.
*
*	ALL STATE VALUES ARE ASSUMED TO BE AT THE CG OF THE VEHICLE!!!
*
*	Initialization function:
*	sixdof_init(struct sixdof_init_inputs_def *pInit,		// see the structure (INPUT)
*				 struct sixdof_inputs_def *pIn,				// see the structure (RETURN)
*				 struct sixdof_state_def *pX);				// see the structure, assuming no acceleration (RETURN)
*
*	Sixdof function:
*	void sixdof(struct sixdof_state_def *pX,				// see structure (INPUT and RETURN) 
*				struct sixdof_inputs_def *pU,				// see structure (INPUT)
*				double dt);									// time step for integration (sec)
*/
struct sixdof_init_inputs_def {
	double Ixx;								// moment of inertia about body X axis (slug-ft^2)
	double Iyy;								// moment of inertia about body Y axis (slug-ft^2)
	double Izz;								// moment of inertia about body Z axis (slug-ft^2)
	double Ixz;								// cross moment of inertia about body XZ axis (slug-ft^2)
	double m;								// mass of vehicle (slug)
	double latitude;						// starting geodetic latitude (rad)
	double longitude;						// starting geodetic longitude (rad)
	double altitude;						// starting altitude (ft MSL + up)
	double uvw[MAXSIZE];					// starting body translational velocity [u v w] (ft/s)
	double pqr[MAXSIZE];					// starting body angular rates [P Q R] (rad/s)
	double THETA[MAXSIZE];					// starting body attitude [phi theta psi] (rad) (level to TP)
};

struct sixdof_inputs_def {
	double J[MAXSIZE][MAXSIZE];				// inertia tensor matrix (slug-ft^2)
	double Jinv[MAXSIZE][MAXSIZE];			// inverse of inertia tensor matrix (slug-ft^2)
	double m;								// vehicle mass (slug)
	double F[MAXSIZE];						// total force on vehicle CG (lbs)
	double M[MAXSIZE];						// total moment on vehicle CG (lbs-ft)
	double hold_u;							// hold the X body velocity constant (1=hold, 0=free)
	double hold_v;							// hold the Y body velocity constant (1=hold, 0=free)
	double hold_w;							// hold the Z body velocity constant (1=hold, 0=free)
	double hold_p;							// hold the X body angular rate constant (1=hold, 0=free)
	double hold_q;							// hold the Y body angular rate constant (1=hold, 0=free)
	double hold_r;							// hold the Z body angular rate constant (1=hold, 0=free)
};

struct sixdof_state_def {
	double accel[MAXSIZE];					// acceleration body frame at cg [X Y Z] (ft/s/s)
	double Vb[MAXSIZE];						// velocity body frame at cg [u v w] (ft/s)
	double Ve[MAXSIZE];						// velocity TP frame at cg [Vnorth Veast Vdown] (ft/s)
	double Pecef[MAXSIZE];					// position ECEF frame at cg [X Y Z] (ft)
	double Pllh[MAXSIZE];					// position LLH frame at cg [lat lon alt] (rad)(rad)(ft MSL + up)
	double alpha[MAXSIZE];					// angular acceleration body frame [Pdot Qdot Rdot] (rad/s/s)
	double rate[MAXSIZE];					// angular rates body frame [P Q R] (rad/s)
	double Q[MAXSIZE];						// quaternion [q0 q1 q2 q3]
	double THETA[MAXSIZE];					// attitude euler angles [phi theta psi] (rad) (level to TP)
};

void sixdof_init(struct sixdof_init_inputs_def *pInit, 
				 struct sixdof_inputs_def *pIn,
				 struct sixdof_state_def *pX);

void sixdof(struct sixdof_state_def *pX, struct sixdof_inputs_def *pU, double dt);


/*	This is a six degree of freedom rigid body dynamic simulator.  It is
* similar the above sixdof() function, but instead of using WGS-84 round
* earth simulation, this will use the flat-earth approximations.  The inputs
* are similar in both cases, except that for this simulation, no information is needed
* on the home latitude and longitude.  All that is needed is the starting 
* NED (north east down) position.  The main difference is that altitude is now + down
* not up.  
*	Propogation of the dynamics is done with RK4.
*
*	NOTE: ALL VALUES ARE AT THE CG OF THE VEHCILE!
*
*	Initialization Function:
*	sixdof_fe_init(struct sixdof_fe_init_inputs_def *pInit,		// see structure (INPUT)
*				struct sixdof_fe_inputs_def *pIn,				// see structure (OUTPUT)
*				struct sixdof_fe_state_def *pX)					// see structure (OUTPUT)
*
*	Propogation Function:
*	sixdof_fe(struct sixdof_fe_inputs_def *pIn,					// see structure (INPUT)
*			struct sixdof_fe_state_def *pX,						// see structure (INPUT/OUTPUT)
*			double dt)											// integration time step (sec)
*/
struct sixdof_fe_init_inputs_def {
	double Ixx;								// moment of inertia about body X axis (slug-ft^2)
	double Iyy;								// moment of inertia about body Y axis (slug-ft^2)
	double Izz;								// moment of inertia about body Z axis (slug-ft^2)
	double Ixz;								// cross moment of inertia about body XZ axis (slug-ft^2)
	double m;								// mass of vehicle (slug)
	double NED[MAXSIZE];					// starting body NED [north east down] position (ft)
	double uvw[MAXSIZE];					// starting body translational velocity [u v w] (ft/s)
	double pqr[MAXSIZE];					// starting body angular rates [P Q R] (rad/s)
	double THETA[MAXSIZE];					// starting body attitude [phi theta psi] (rad) (level to TP)
};

struct sixdof_fe_inputs_def {
	double J[MAXSIZE][MAXSIZE];				// inertia tensor matrix (slug-ft^2)
	double Jinv[MAXSIZE][MAXSIZE];			// inverse of inertia tensor matrix (slug-ft^2)
	double m;								// vehicle mass (slug)
	double F[MAXSIZE];						// total force on vehicle CG (lbs)
	double M[MAXSIZE];						// total moment on vehicle CG (lbs-ft)
	double hold_u;							// hold the X body velocity constant (1=hold, 0=free)
	double hold_v;							// hold the Y body velocity constant (1=hold, 0=free)
	double hold_w;							// hold the Z body velocity constant (1=hold, 0=free)
	double hold_p;							// hold the X body angular rate constant (1=hold, 0=free)
	double hold_q;							// hold the Y body angular rate constant (1=hold, 0=free)
	double hold_r;							// hold the Z body angular rate constant (1=hold, 0=free)
};

struct sixdof_fe_state_def {
	double accel[MAXSIZE];					// acceleration body frame at cg [X Y Z] (ft/s/s)
	double Vb[MAXSIZE];						// velocity body frame at cg [u v w] (ft/s)
	double Ve[MAXSIZE];						// velocity TP frame at cg [Vnorth Veast Vdown] (ft/s)
	double NED[MAXSIZE];						// position NED [north east down] (ft)
	double alpha[MAXSIZE];					// angular acceleration body frame [Pdot Qdot Rdot] (rad/s/s)
	double rate[MAXSIZE];					// angular rates body frame [P Q R] (rad/s)
	double Q[MAXSIZE];						// quaternion [q0 q1 q2 q3]
	double THETA[MAXSIZE];					// attitude euler angles [phi theta psi] (rad) (level to TP)
};

void sixdof_fe_init(struct sixdof_fe_init_inputs_def *pInit, 
				 struct sixdof_fe_inputs_def *pIn,
				 struct sixdof_fe_state_def *pX);

void sixdof_fe(struct sixdof_fe_state_def *pX, struct sixdof_fe_inputs_def *pU, double dt);


/*	This function will provide a servo-actuator model.  It contains two main
* components.  The first is a second-order dynamic model which is tuneable 
* via wn and zeta values.  The second, is a hysterisous model.  This feature
* is designed around basic deadband slop that may be found in system linkages.
*	See the structure for details.  Also, this system contains a state structure
* that needs to be maintained for state propogation.
*
*	void servo(struct servo_state_def *pX,			// state propogation 
*				struct servo_inputs_def *pIn,		// inputs for the model
*				double dt);							// time step size for integration of dynamics
*
*	The TF model of the servo used is as follows...
*
*			          S + wn^2           y
*            ------------------------ =  --
*            S^2 + 2*zeta*wn*S + wn^2    u
*
*	Nominal values of wn and zeta are...  wn = 10.43; zeta = 0.8;  (for KR2000 actuators)
*
* Use this equation to find the appropriate value of wn assuming zeta = 0.8...
*
*  wn =          pi
*        ------------------     Tp = peek time (ex: servo takes 0.6 sec to go 60 deg, Tp = 0.6)
*        Tp*sqrt(1 - zeta^2)
*
*/
struct servo_inputs_def {
	double wn;								// natural freq. of the dynamic model
	double zeta;							// damping ratio of the dynamic model
	double slop;							// half of the total slop in the system for hysterisous (ie: +/- slop)
	double command;							// driving command of the servo
};

struct servo_state_def {
	double X[2];							// state vector (internal)
	double output;							// resulting output value of the command into system
};

void servo(struct servo_state_def *pX, struct servo_inputs_def *pIn, double dt);


/*	This is a collision/landing gear model.  The basis of this model is the 
* vertial displacement method.  The idea is that if a collision is detected between
* the point, and a surface, then the displacement is computed.  The is then 
* used to compute a force based on Hook's law.  To allow for in-elastic collisions,
* a damping constant can be given.
*	For now, this function assumes that the contact surface is the ground (for landing gear).
* For this assumption, the ground is taken as 0 altitude.
*	For more information, see the structure definitions below.
*/
struct gear_inputs_def {
	double cg2point[MAXSIZE];				// the vector from vehicle CG -> contact pt, body frame [X Y Z] (ft)
	double THETA[MAXSIZE];					// vehicle attitude [phi theta psi] (rad) (TP level)
	double pqr[MAXSIZE];					// vehicle body rates [P Q R] (rad/s)
	double uvw[MAXSIZE];					// vehicle body velocity [u v w] (ft/s)
	double altitude;						// vehicle CG altitude above the local level TP (ft MSL + down)
	double k;								// Hook's law spring constant (lbs/ft)
	double b;								// damping constant (lbs/ft/s)
	double mu_x;							// cooefficent of friction on long axis
	double mu_y;							// cooefficent of friction on cross axis
	double rotation;						// relative rotation about vertical axis (for steering) (rad)
};

struct gear_outputs_def {
	double F[MAXSIZE];						// forces on vehicle CG caused by the gear (body axis) [X Y Z] (lbs)
	double M[MAXSIZE];						// moments on vehicle CG caused by gear (body axis) [L M N] (lb-ft)
};

void gear(struct gear_inputs_def *pIn, struct gear_outputs_def *pOut);


/*	This is a basic wind/gust model.  It is a dynamic system that will generate
* random winds, up to a maximum value, in both vertical and horizontal directions.
*	To make this model work, like the servo and 6-DOF models, it is propogated over
* time for the dynamics.
*	See the structures for information
*
*	There are two functions.  The first, wind_init is used to initalize the wind model.
* The second, wind, is used to propograte the state of the wind model.
*
*	void wind_init(struct wind_inputs_def *pIn,		// input values (INPUTS and OUTPUT)
*					struct wind_state_def *pX);		// state values (OUTPUT)
*
*	void wind(struct wind_inputs_def *pIn,			// input values (INPUTS)
*			struct wind_state_def *pX,				// state values (INPUTS and OUTPUTS)
*			double dt);								// time step (sec) (INPUTS)
*
*/
struct wind_inputs_def {
	double horz_max;						// maximum horizontal wind (ft/s)
	double vert_max;						// maximum up/down drafts (ft/s)
	int seed;								// seed variable for the random number generator
};
struct wind_state_def {
	double Ve[MAXSIZE];						// components of wind in earth TP frame [Vn Ve Vd] (ft/s)
	double X[6];							// internal state
};

void wind_init(struct wind_inputs_def *pIn, struct wind_state_def *pX);
void wind(struct wind_inputs_def *pIn, struct wind_state_def *pX, double dt);


/*	This is a gravity and gravitation model.  It is based on the WGS-84 elipsoid
* model of the Earth.  The source for this model is from Dr. Brian Steven's class.
* See the structures for information.
*/
struct grav_inputs_def {
	double latitude;						// current geodetic latitude (rad)
	double altitude;						// current altitude (ft MSL + up)
};
struct grav_outputs_def {
	double g[MAXSIZE];						// local gravity vector in earth TP frame [gN gE gD] (ft/s/s)
	double G[MAXSIZE];						// local gravitation vector in earth TP frame [GN GE GD] (ft/s/s)
};
void gravity(struct grav_inputs_def *pIn, struct grav_outputs_def *pOut);

#endif  

