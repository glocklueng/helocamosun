
/****************************************************************************
*
*	This file contains the code for the SimLib function.
*
*	Author: Aaron Kahn
*
*****************************************************************************/

#include "simlib.h"

#define _N	0
#define _E	1
#define _D	2

/*	The below are hidden structures and functions for the propogation of the
* 6-DOF simulation model.
*/
struct sixdofX_def {
	double Vned[MAXSIZE];				// [Vn Ve Vd] (ft/s)
	double llh[MAXSIZE];				// [lat lon alt] (rad)(rad)(ft)
	double pqr[MAXSIZE];				// [P Q R] (rad/s)
	double Q[MAXSIZE];					// [q0 q1 q2 q3]
};

struct sixdofX_fe_def {
	double Vned[MAXSIZE];				// [Vn Ve Vd] (ft/s)
	double NED[MAXSIZE];				// [N E D] (ft)
	double pqr[MAXSIZE];				// [P Q R] (rad/s)
	double Q[MAXSIZE];					// [q0 q1 q2 q3]
};

struct sixdofXdot_def {
	double Vned_dot[MAXSIZE];			// [Vndot Vedot Vddot] (ft/s/s)
	double llh_dot[MAXSIZE];			// [latdot londot altdot] (rad/s)(rad/s)(ft/s)
	double pqr_dot[MAXSIZE];			// [Pdot Qdot Rdot] (rad/s/s)
	double Q_dot[MAXSIZE];				// [q0dot q1dot q2dot q3dot]
};

struct sixdofXdot_fe_def {
	double Vned_dot[MAXSIZE];			// [Vndot Vedot Vddot] (ft/s/s)
	double NED_dot[MAXSIZE];			// [Ndot Edot Ddot] (ft/s)
	double pqr_dot[MAXSIZE];			// [Pdot Qdot Rdot] (rad/s/s)
	double Q_dot[MAXSIZE];				// [q0dot q1dot q2dot q3dot]
};

void sixdof_derivs(struct sixdofXdot_def *pXdot, 
				   struct sixdofX_def *pX, 
				   struct sixdof_inputs_def *pU);

void integrate_sixdof(struct sixdofXdot_def *pXdot, 
					  struct sixdofX_def *pX, 
					  struct sixdof_inputs_def *pU, 
					  double dt);

void sixdof_fe_derivs(struct sixdofXdot_fe_def *pXdot, 
					  struct sixdofX_fe_def *pX, 
					  struct sixdof_fe_inputs_def *pU);

void integrate_sixdof_fe(struct sixdofXdot_fe_def *pXdot, 
						 struct sixdofX_fe_def *pX, 
						 struct sixdof_fe_inputs_def *pU, 
						 double dt);


/* End of hidden functions */

/* The below is a hidden function for the servo-actuator dynamics */
void servo_derivs(double *Xdot, double *X, double *t, double *u, double *args);
/* End of hidden functions */

/* The below is a hidden function for the wind model dynamcis */
void wind_derivs(double *Xdot, double *X, double *t, double *u, double *args);
/* End of hidden functions */

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
void IMUsim(struct imu_inputs_def *pIn, struct imu_outputs_def *pOut)
{
	double wx[MAXSIZE][MAXSIZE];			// omega-cross matrix
	double cBE[MAXSIZE][MAXSIZE];			// earth -> body transformation matrix
	double cEB[MAXSIZE][MAXSIZE];			// body -> earth transformation matrix
	double cBI[MAXSIZE][MAXSIZE];			// IMU -> body transformation matrix
	double cIB[MAXSIZE][MAXSIZE];			// body -> IMU transformation matrix
	double LLHcg[MAXSIZE];					// lat, long, alt of the cg (rad)(rad)(ft)
	double tempM1[MAXSIZE][MAXSIZE];
	double tempV1[MAXSIZE];
	double tempV2[MAXSIZE];
	double tempV3[MAXSIZE];
	int i,j;
	double lat, lon;

	// make some of the matricies that will be needed
	// make the omega-cross matrix 
	eulerWx(wx, pIn->cg_pqr[0], pIn->cg_pqr[1], pIn->cg_pqr[2]);

	// make the cBE matrix
	eulerDC(cBE, pIn->cg_THETA[0], pIn->cg_THETA[1], pIn->cg_THETA[2]);

	// make the cEB matrix
	transpose(cBE, cEB, 3, 3);

	// make the cIB matrix
	for(i=0; i<3; ++i)
	{
		for(j=0; j<3; ++j)
		{
			cIB[i][j] = pIn->body2imu[i][j];
		}
	}

	// make the cBI matrix
	transpose(cIB, cBI, 3, 3);

	// get the CG LLH values in ft
	for(i=0; i<3; ++i)
	{
		tempV1[i] = pIn->cg_pos[i]*C_FT2M;
	}
	ECEF2llh(tempV1, LLHcg);
	LLHcg[2] = LLHcg[2]*C_M2FT;
	

	lat = LLHcg[0];
	lon = LLHcg[1];


	// NOW DO SOME MATH

	// compute the accelerations at the IMU
	// a_imu = cIB(a_cg + alpha X pos + w X w X pos)
	cross(pIn->cg_alpha, pIn->cg2imu, tempV1);
	MMmult(wx, wx, tempM1, 3, 3, 3);
	MVmult(tempM1, pIn->cg2imu, tempV2, 3, 3);
	VVadd(tempV1, tempV2, tempV3, 3);
	VVadd(tempV3, pIn->cg_accel, tempV1, 3);
	MVmult(cIB, tempV1, pOut->accel, 3, 3);

	// compute the velocity at the IMU
	// v_imu = cIB(v_cg + w X pos)
	MVmult(wx, pIn->cg2imu, tempV1, 3, 3);
	VVadd(pIn->cg_uvw, tempV1, tempV2, 3);
	MVmult(cIB, tempV2, pOut->uvw, 3, 3);

	// compute the position of the IMU
	// rotate the body cg->imu vector to TP coordinates
	MVmult(cEB, pIn->cg2imu, tempV1, 3, 3);
	// rotate the TP cg->imu vector into ECEF coordinates
	Tangent2ECEF(tempV2, tempV1, lat, lon);
	// compute the ECEF position of the IMU
	VVadd(tempV2, pIn->cg_pos, pOut->ECEFpos, 3);
	// compute the LLH position of the IMU
	for(i=0; i<3; ++i)
	{
		tempV1[i] = pOut->ECEFpos[i]*C_FT2M;
	}
	ECEF2llh(tempV1, pOut->LLHpos);
	pOut->LLHpos[2] = pOut->LLHpos[2]*C_M2FT;

	// compute the angular velocity of the IMU
	MVmult(cIB, pIn->cg_pqr, pOut->pqr, 3, 3);

	// compute the attitude of the IMU
	MMmult(cIB, cBE, tempM1, 3, 3, 3);
	pOut->THETA[0] = atan2(tempM1[1][2], tempM1[2][2]);
	pOut->THETA[1] = -asin(tempM1[0][2]);
	pOut->THETA[2] = atan2(tempM1[0][1], tempM1[0][0]);


}

/*	This is a 4th order Runga-Kutta integration routine.  It is designed to work
* with a vector of inputs, and produce a vector of outputs at the next time step.
* A time step input is needed for the dt value, and the number of states to integrate.
* There is the requirement that the function to be integrated is in a specific format...
*
*	RK4(double *state,					// state vector (INPUT and OUTPUT) 
*		double *state_dot,				// derivatives of state vector (OUTPUT)
*		double t,						// current time (INPUT)
*		double *force,					// any forcing values (INPUT)
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
		 void (*pfunc)(double *Xdot, double *X, double *t, double *U, double *args) )
{
	double *X0;			// for saving in intial state vector
	double *X;			// for making the new state vector, for called function
	double *Xdot;		// state derivative vector of called funtion
	double *k1;
	double *k2;
	double *k3;
	double *k4;
	double time;
	int i;
	size_t numbytes = n*sizeof(double);

	
	
	// initializing the memory for the arrays
	X0 = (double *)malloc(numbytes);
	X = (double *)malloc(numbytes);
	Xdot = (double *)malloc(numbytes);
	k1 = (double *)malloc(numbytes);
	k2 = (double *)malloc(numbytes);
	k3 = (double *)malloc(numbytes);
	k4 = (double *)malloc(numbytes);


	// backup the original state vector
	time = t;
	for(i=0; i<n; ++i)
	{
		X0[i] = state[i];
	}

	// the first step
	(*pfunc)(Xdot, X0, &time, force, args);
	for(i=0; i<n; ++i)
	{
		k1[i] = dt*Xdot[i];					// save the k value for later
		X[i] = X0[i] + k1[i]/2.0;			// make the next state vector
		time = t + dt/2.0;					// make the time for next step
	}

	// the second step
	(*pfunc)(Xdot, X, &time, force, args);
	for(i=0; i<n; ++i)
	{
		k2[i] = dt*Xdot[i];					// save the k value for later
		X[i] = X0[i] + k2[i]/2.0;			// make the next state vector
		time = t + dt/2.0;					// make the time for the next step
	}

	// the third step
	(*pfunc)(Xdot, X, &time, force, args);
	for(i=0; i<n; ++i)
	{
		k3[i] = dt*Xdot[i];					// save the k value for later
		X[i] = X0[i] + k3[i];				// make the next state vector
		time = t + dt;						// make the time for the next step
	}
	
	// the forth step
	(*pfunc)(Xdot, X, &time, force, args);
	for(i=0; i<n; ++i)
	{
		k4[i] = dt*Xdot[i];					// save the k value for later
	}

	// make the final result
	for(i=0; i<n; ++i)
	{
		state[i] = X0[i] + k1[i]/6.0 + k2[i]/3.0 + k3[i]/3.0 + k4[i]/6.0;
	}

	// freeing the memory allocated for the arrays (to prevent memory leaks)
	free(X0);
	free(X);
	free(Xdot);
	free(k1);
	free(k2);
	free(k3);
	free(k4);

}





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
void sixdof_init(struct sixdof_init_inputs_def *pInit, 
				 struct sixdof_inputs_def *pIn,
				 struct sixdof_state_def *pX)
{
	double gamma;						// used for the Jinv calculation
	double cBE[MAXSIZE][MAXSIZE];		// earth -> body transformation matrix
	double cEB[MAXSIZE][MAXSIZE];		// body -> earth transformation matrix
	double tempV1[MAXSIZE];
	int i;


	// compute the cBE matrix
	eulerDC(cBE, pInit->THETA[0], pInit->THETA[1], pInit->THETA[2]);		// rad

	// compute the cEB matrix
	transpose(cBE, cEB, 3, 3);

	// compute J
	pIn->J[0][0] = pInit->Ixx;					// slug-ft^2
	pIn->J[0][1] = 0.0;
	pIn->J[0][2] = -pInit->Ixz;					// slug-ft^2
	pIn->J[1][0] = 0.0;
	pIn->J[1][1] = pInit->Iyy;					// slug-ft^2
	pIn->J[1][2] = 0.0;		
	pIn->J[2][0] = -pInit->Ixz;					// slug-ft^2
	pIn->J[2][1] = 0.0;
	pIn->J[2][2] = pInit->Izz;					// slug-ft^2

	// compute Jinv
	gamma = pInit->Ixx*pInit->Izz - SQR(pInit->Ixz);
	pIn->Jinv[0][0] = pInit->Izz/gamma;
	pIn->Jinv[0][1] = 0.0;
	pIn->Jinv[0][2] = pInit->Ixz/gamma;
	pIn->Jinv[1][0] = 0.0;
	pIn->Jinv[1][1] = 1.0/pInit->Iyy;
	pIn->Jinv[1][2] = 0.0;
	pIn->Jinv[2][0] = pInit->Ixz/gamma;
	pIn->Jinv[2][1] = 0.0;
	pIn->Jinv[2][2] = pInit->Ixx/gamma;

	// initialize the quaternion
	euler2quat(pX->Q, pInit->THETA[0], pInit->THETA[1], pInit->THETA[2]); // rad

	// initialize the LLH
	pX->Pllh[0] = pInit->latitude;				// rad
	pX->Pllh[1] = pInit->longitude;				// rad
	pX->Pllh[2] = pInit->altitude;				// ft

	// initialize the ECEF
	tempV1[0] = pX->Pllh[0];					// need rad
	tempV1[1] = pX->Pllh[1];					// need rad
	tempV1[2] = pX->Pllh[2]*C_FT2M;				// need m
	llh2ECEF(tempV1, pX->Pecef);				// to use this function, all dims rad, m
	pX->Pecef[0] *= C_M2FT;						// convert m -> ft
	pX->Pecef[1] *= C_M2FT;						// convert m -> ft
	pX->Pecef[2] *= C_M2FT;						// convert m -> ft

	// initialize the all of the other values
	for(i=0; i<3; ++i)
	{
		pX->accel[i] = 0.0;						// ft/s/s
		pX->alpha[i] = 0.0;						// rad/s/s
		pX->rate[i] = pInit->pqr[i];			// rad/s
		pX->Vb[i] = pInit->uvw[i];				// ft/s
		pX->THETA[i] = pInit->THETA[i];			// rad
		
		pIn->F[i] = 0.0;						// lbs
		pIn->M[i] = 0.0;						// lb-ft
	}

	// initialize the velocity in earth frame
	MVmult(cEB, pX->Vb, pX->Ve, 3, 3);			// ft/s


	// provide a placeholder for the mass value
	pIn->m = 1.0;								// slug

}

void sixdof_derivs(struct sixdofXdot_def *pXdot, struct sixdofX_def *pX, struct sixdof_inputs_def *pU)
{
	double wx[MAXSIZE][MAXSIZE];					// omega-cross matrix
	double E[MAXSIZE][MAXSIZE];						// quaternion strapdown matrix
	double cBE[MAXSIZE][MAXSIZE];					// earth TP -> body transformation matrix
	double cEB[MAXSIZE][MAXSIZE];					// body -> earth TP transformation matrix
	double tempV1[MAXSIZE];
	double tempV2[MAXSIZE];
	double Vb_n[MAXSIZE];							// velocity of the body in NED frame (m)
	double Fned[MAXSIZE];							// specific body force in NED frame (m/s/s)
	double Fb[MAXSIZE];								// copy of body force to play with
	double win[MAXSIZE];
	double clat;
	double f, e, Rlat, Rlon, h;

	// make the wx matrix
	eulerWx(wx, pX->pqr[0], pX->pqr[1], pX->pqr[2]);

	// make the cBE matrix
	quatDC(cBE, pX->Q);

	// make the cEB matrix
	transpose(cBE, cEB, 3, 3);

	// make the radii for the propogation of the latitude, longitude, altitude
	// see pg 191, Farrel for the llh propogation equations
	f = (C_WGS84_a - C_WGS84_b)/C_WGS84_a;
	e = sqrt(f*(2 - f));
	Rlat = ( C_WGS84_a*(1 - SQR(e)) )/pow( 1-SQR(e)*SQR(sin(pX->llh[0])), 1.5 );	// m
	Rlon = C_WGS84_a/pow( 1-SQR(e)*SQR(sin(pX->llh[0])), 0.5 );						// m
	clat = cos(pX->llh[0]);
	h = pX->llh[2]*C_FT2M;															// ft -> m

	// compute the derivative of the latitude, longitude and altitude
	// first rotate body axis velocity --> TP velocity components
	Vb_n[_N] = pX->Vned[_N]*C_FT2M;													// m/s
	Vb_n[_E] = pX->Vned[_E]*C_FT2M;													// m/s
	Vb_n[_D] = pX->Vned[_D]*C_FT2M;													// m/s
	// compute the rate of change of latitude, longitude, altitude
	pXdot->llh_dot[0] = 1.0/( Rlat + h)*Vb_n[_N];									// rad/s
	pXdot->llh_dot[1] = 1.0/( (Rlon + h)*clat )*Vb_n[_E];							// rad/s
	pXdot->llh_dot[2] = -1.0*Vb_n[_D]*C_M2FT;										// ft/s


	// compute the state time derivative of the angular rate.
	// wdot_body = -Jinv*Omega*J*w_body + Jinv*M_body
	MVmult(pU->J, pX->pqr, tempV1, 3, 3);
	MVmult(wx, tempV1, tempV2, 3, 3);
	MVmult(pU->Jinv, tempV2, tempV1, 3, 3);
	tempV1[0] = -tempV1[0];
	tempV1[1] = -tempV1[1];
	tempV1[2] = -tempV1[2];
	MVmult(pU->Jinv, pU->M, tempV2, 3, 3);
	VVadd(tempV1, tempV2, pXdot->pqr_dot, 3);

	// check holds on angular rates
	if( pU->hold_p )
		pXdot->pqr_dot[0] = 0.0;
	if( pU->hold_q )
		pXdot->pqr_dot[1] = 0.0;
	if( pU->hold_r )
		pXdot->pqr_dot[2] = 0.0;

	// check for holds on velocities
	Fb[0] = pU->F[0];
	Fb[1] = pU->F[1];
	Fb[2] = pU->F[2];
	if( pU->hold_u )
		Fb[0] = 0.0;
	if( pU->hold_v )
		Fb[1] = 0.0;
	if( pU->hold_w )
		Fb[2] = 0.0;

	// compute the time derivative of body velocity in Earth TP frame
	// compute the body specific force in NED frame
	MVmult(cEB, Fb, tempV1, 3, 3);
	Fned[_N] = tempV1[_N]/pU->m*C_FT2M;												// m/s/s
	Fned[_E] = tempV1[_E]/pU->m*C_FT2M;												// m/s/s
	Fned[_D] = tempV1[_D]/pU->m*C_FT2M;												// m/s/s

	pXdot->Vned_dot[_N] = Fned[_N] -
						( Vb_n[_N]/((Rlon + h)*clat) )*Vb_n[_E]*sin(pX->llh[0]) +
						( Vb_n[_N]*Vb_n[_D] )/( Rlat + h );

	pXdot->Vned_dot[_E] = Fned[_E] +
						( Vb_n[_E]/((Rlon + h)*clat) )*Vb_n[_N]*sin(pX->llh[0]) + 
						( Vb_n[_E]*Vb_n[_D] )/( Rlon + h ); 

	pXdot->Vned_dot[_D] = Fned[_D] -
						SQR(Vb_n[_E])/(Rlon + h) -
						SQR(Vb_n[_N])/(Rlat + h);

	pXdot->Vned_dot[0] *= C_M2FT;													// m/s/s -> ft/s/s
	pXdot->Vned_dot[1] *= C_M2FT;													// m/s/s -> ft/s/s
	pXdot->Vned_dot[2] *= C_M2FT;													// m/s/s -> ft/s/s
	



	// compute the derivative of the quaternion, but first compute the
	// corrected angular rate due to motion over the earth.  The attitude
	// is measured from the TP under the aircraft.  This is moving, and thus
	// a new angular rate needs to be computed to account for the curvature
	// of the world.
	win[0] = (pXdot->llh_dot[1])*cos(pX->llh[0]);
	win[1] = -pXdot->llh_dot[0];
	win[2] = -(pXdot->llh_dot[1])*sin(pX->llh[0]);

	MVmult(cBE, win, tempV1, 3, 3);
	tempV1[0] *= -1.0;
	tempV1[1] *= -1.0;
	tempV1[2] *= -1.0;
	
	VVadd(pX->pqr, tempV1, tempV2, 3);

	// make the E matrix
	quatW(E, tempV2[0], tempV2[1], tempV2[2]);

	// compute the derivative of the quaternion
	MVmult(E, pX->Q, pXdot->Q_dot, 4, 4);


}		

void integrate_sixdof(struct sixdofXdot_def *pXdot, struct sixdofX_def *pX, struct sixdof_inputs_def *pU, double dt)
{
	struct sixdofX_def X;					// to run the derivative function
	struct sixdofX_def X0;					// to backup the input state
	struct sixdofXdot_def Xdot;				// to run the derivative function
	int i;
	double k1[13];
	double k2[13];
	double k3[13];
	double k4[13];

	// backing up the current input state
	for(i=0; i<3; ++i)
	{
		X0.llh[i] = pX->llh[i];
		X0.pqr[i] = pX->pqr[i];
		X0.Vned[i] = pX->Vned[i];
		X0.Q[i] = pX->Q[i];
	}
	X0.Q[3] = pX->Q[3];


	// Well, lets get started with the first step
	// first, make the X state value
	for(i=0; i<3; ++i)
	{
		X.llh[i] = X0.llh[i];
		X.pqr[i] = X0.pqr[i];
		X.Vned[i] = X0.Vned[i];
		X.Q[i] = X0.Q[i];
	}
	X.Q[3] = X0.Q[3];
	normq(X.Q);

	// run the function and get the Xdot values
	sixdof_derivs(&Xdot, &X, pU);
	for(i=0; i<3; ++i)
	{
		// save the k step
		k1[i] = dt*Xdot.llh_dot[i];			// latitude, longitude, altitude
		k1[i+3] = dt*Xdot.pqr_dot[i];		// rates
		k1[i+6] = dt*Xdot.Vned_dot[i];		// accelerations
		k1[i+9] = dt*Xdot.Q_dot[i];			// quaternion

		// make the state value for the next step
		X.llh[i] = X0.llh[i] + k1[i]/2.0;
		X.pqr[i] = X0.pqr[i] + k1[i+3]/2.0;
		X.Vned[i] = X0.Vned[i] + k1[i+6]/2.0;
		X.Q[i] = X0.Q[i] + k1[i+9]/2.0;
	}
	k1[12] = dt*Xdot.Q_dot[3];
	X.Q[3] = X0.Q[3] + k1[12]/2.0;
	normq(X.Q);

	// save the Xdot values for output
	for(i=0; i<3; ++i)
	{
		pXdot->llh_dot[i] = Xdot.llh_dot[i];
		pXdot->pqr_dot[i] = Xdot.pqr_dot[i];
		pXdot->Vned_dot[i] = Xdot.Vned_dot[i];
		pXdot->Q_dot[i] = Xdot.Q_dot[i];
	}
	pXdot->Q_dot[3] = Xdot.Q_dot[3];
	

	// run the function and get the Xdot values
	sixdof_derivs(&Xdot, &X, pU);
	for(i=0; i<3; ++i)
	{
		// save the k step
		k2[i] = dt*Xdot.llh_dot[i];			// latitude, longitude, altitude
		k2[i+3] = dt*Xdot.pqr_dot[i];		// rates
		k2[i+6] = dt*Xdot.Vned_dot[i];		// accelerations
		k2[i+9] = dt*Xdot.Q_dot[i];			// quaternion

		// make the state value for the next step
		X.llh[i] = X0.llh[i] + k2[i]/2.0;
		X.pqr[i] = X0.pqr[i] + k2[i+3]/2.0;
		X.Vned[i] = X0.Vned[i] + k2[i+6]/2.0;
		X.Q[i] = X0.Q[i] + k2[i+9]/2.0;
	}
	k2[12] = dt*Xdot.Q_dot[3];
	X.Q[3] = X0.Q[3] + k2[12]/2.0;
	normq(X.Q);

	// run the function and get the Xdot values
	sixdof_derivs(&Xdot, &X, pU);
	for(i=0; i<3; ++i)
	{
		// save the k step
		k3[i] = dt*Xdot.llh_dot[i];			// latitude, longitude, altitude
		k3[i+3] = dt*Xdot.pqr_dot[i];		// rates
		k3[i+6] = dt*Xdot.Vned_dot[i];		// accelerations
		k3[i+9] = dt*Xdot.Q_dot[i];			// quaternion

		// make the state value for the next step
		X.llh[i] = X0.llh[i] + k3[i];
		X.pqr[i] = X0.pqr[i] + k3[i+3];
		X.Vned[i] = X0.Vned[i] + k3[i+6];
		X.Q[i] = X0.Q[i] + k3[i+9];
	}
	k3[12] = dt*Xdot.Q_dot[3];
	X.Q[3] = X0.Q[3] + k3[12];
	normq(X.Q);

	// run the function and get the Xdot values
	sixdof_derivs(&Xdot, &X, pU);
	for(i=0; i<3; ++i)
	{
		// save the k step
		k4[i] = dt*Xdot.llh_dot[i];			// latitude, longitude, altitude
		k4[i+3] = dt*Xdot.pqr_dot[i];		// rates
		k4[i+6] = dt*Xdot.Vned_dot[i];		// accelerations
		k4[i+9] = dt*Xdot.Q_dot[i];			// quaternion
	}
	k4[12] = dt*Xdot.Q_dot[3];

	
	// assemble the final result for the state propogation
	for(i=0; i<3; ++i)
	{
		pX->llh[i] = X0.llh[i] + k1[i]/6.0 + k2[i]/3.0 + k3[i]/3.0 + k4[i]/6.0;
		pX->pqr[i] = X0.pqr[i] + k1[i+3]/6.0 + k2[i+3]/3.0 + k3[i+3]/3.0 + k4[i+3]/6.0;
		pX->Vned[i] = X0.Vned[i] + k1[i+6]/6.0 + k2[i+6]/3.0 + k3[i+6]/3.0 + k4[i+6]/6.0;
		pX->Q[i] = X0.Q[i] + k1[i+9]/6.0 + k2[i+9]/3.0 + k3[i+9]/3.0 + k4[i+9]/6.0;
	}
	pX->Q[3] = X0.Q[3] + k1[12]/6.0 + k2[12]/3.0 + k3[12]/3.0 + k4[12]/6.0;


}

void sixdof(struct sixdof_state_def *pX, struct sixdof_inputs_def *pU, double dt)
{
	double cBE[MAXSIZE][MAXSIZE];				// earth TP -> body transformation matrix
	double cEB[MAXSIZE][MAXSIZE];				// body -> earth TP transformation matrxi
	struct sixdofX_def X;
	struct sixdofXdot_def Xdot;
	int i;


	// make the earth TP -> body matrix
	quatDC(cBE, pX->Q);

	// make the body -> earth TP matrix
	transpose(cBE, cEB, 3, 3);

	// equate the state vectors and such
	for(i=0; i<3; ++i)
	{
		X.pqr[i] = pX->rate[i];
		X.Vned[i] = pX->Ve[i];
		X.llh[i] = pX->Pllh[i];
	}

	for(i=0; i<4; ++i)
	{
		X.Q[i] = pX->Q[i];
	}

	normq(X.Q);					// normalize the quaternion

	// call integrate_sixdof
	integrate_sixdof(&Xdot, &X, pU, dt);

	// solve for the unknown state values for the next time step
	normq(X.Q);					// normalize the quaternion

	for(i=0; i<3; ++i)
	{
		pX->alpha[i] = Xdot.pqr_dot[i];				// body axis angular accelerations
		pX->rate[i] = X.pqr[i];						// body axis angular rates
		pX->accel[i] = pU->F[i]/pU->m;				// body axis acceleration
		pX->Ve[i] = X.Vned[i];						// body axis velocity in earth frame
		pX->Pllh[i] = X.llh[i];						// lat, lon, alt position
	}
	for(i=0; i<4; ++i)
	{
		pX->Q[i] = X.Q[i];							// quaternion
	}

	// compute the ECEF position
	X.llh[2] *= C_FT2M;
	llh2ECEF(X.llh, pX->Pecef);
	pX->Pecef[0] *= C_M2FT;
	pX->Pecef[1] *= C_M2FT;
	pX->Pecef[2] *= C_M2FT;


	quat2euler(X.Q, pX->THETA);						// euler angle convertions
	
	MVmult(cBE, X.Vned, pX->Vb, 3, 3);				// body velocity in body frame

}

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
void sixdof_fe_init(struct sixdof_fe_init_inputs_def *pInit, 
				 struct sixdof_fe_inputs_def *pIn,
				 struct sixdof_fe_state_def *pX)
{
	double gamma;						// used for the Jinv calculation
	double cBE[MAXSIZE][MAXSIZE];		// earth -> body transformation matrix
	double cEB[MAXSIZE][MAXSIZE];		// body -> earth transformation matrix
	int i;


	// compute the cBE matrix
	eulerDC(cBE, pInit->THETA[0], pInit->THETA[1], pInit->THETA[2]);		// rad

	// compute the cEB matrix
	transpose(cBE, cEB, 3, 3);

	// compute J
	pIn->J[0][0] = pInit->Ixx;					// slug-ft^2
	pIn->J[0][1] = 0.0;
	pIn->J[0][2] = -pInit->Ixz;					// slug-ft^2
	pIn->J[1][0] = 0.0;
	pIn->J[1][1] = pInit->Iyy;					// slug-ft^2
	pIn->J[1][2] = 0.0;		
	pIn->J[2][0] = -pInit->Ixz;					// slug-ft^2
	pIn->J[2][1] = 0.0;
	pIn->J[2][2] = pInit->Izz;					// slug-ft^2

	// compute Jinv
	gamma = pInit->Ixx*pInit->Izz - SQR(pInit->Ixz);
	pIn->Jinv[0][0] = pInit->Izz/gamma;
	pIn->Jinv[0][1] = 0.0;
	pIn->Jinv[0][2] = pInit->Ixz/gamma;
	pIn->Jinv[1][0] = 0.0;
	pIn->Jinv[1][1] = 1.0/pInit->Iyy;
	pIn->Jinv[1][2] = 0.0;
	pIn->Jinv[2][0] = pInit->Ixz/gamma;
	pIn->Jinv[2][1] = 0.0;
	pIn->Jinv[2][2] = pInit->Ixx/gamma;

	// initialize the quaternion
	euler2quat(pX->Q, pInit->THETA[0], pInit->THETA[1], pInit->THETA[2]); // rad


	// initialize the all of the other values
	for(i=0; i<3; ++i)
	{
		pX->accel[i] = 0.0;						// ft/s/s
		pX->alpha[i] = 0.0;						// rad/s/s
		pX->rate[i] = pInit->pqr[i];			// rad/s
		pX->Vb[i] = pInit->uvw[i];				// ft/s
		pX->THETA[i] = pInit->THETA[i];			// rad
		
		pIn->F[i] = 0.0;						// lbs
		pIn->M[i] = 0.0;						// lb-ft

		pX->NED[i] = pInit->NED[i];				// ft
	}

	// initialize the velocity in earth frame
	MVmult(cEB, pX->Vb, pX->Ve, 3, 3);			// ft/s

	pIn->m = pInit->m;								// slug

}

void sixdof_fe_derivs(struct sixdofXdot_fe_def *pXdot, 
					  struct sixdofX_fe_def *pX, 
					  struct sixdof_fe_inputs_def *pU)
{
	double wx[MAXSIZE][MAXSIZE];					// omega-cross matrix
	double E[MAXSIZE][MAXSIZE];						// quaternion strapdown matrix
	double cBE[MAXSIZE][MAXSIZE];					// earth TP -> body transformation matrix
	double cEB[MAXSIZE][MAXSIZE];					// body -> earth TP transformation matrix
	double tempV1[MAXSIZE];
	double tempV2[MAXSIZE];
	double Vb_b[MAXSIZE];							// velocity of the body in body frame (ft/s)

	// make the wx matrix
	eulerWx(wx, pX->pqr[0], pX->pqr[1], pX->pqr[2]);

	// make the cBE matrix
	quatDC(cBE, pX->Q);

	// make the cEB matrix
	transpose(cBE, cEB, 3, 3);

	// Compute the body accelerations
	// rotate the ned velocity to body frame
	MVmult(cBE, pX->Vned, Vb_b, 3, 3);
	// a = F/m - w X v
	MVmult(wx, Vb_b, tempV1, 3, 3);
	sVmult(1.0/pU->m, pU->F, tempV2, 3);
	VVsub(tempV2, tempV1, tempV2, 3);
	
	// check holds
	if( pU->hold_u )
		tempV2[0] = 0.0;
	if( pU->hold_v )
		tempV2[1] = 0.0;
	if( pU->hold_w )
		tempV2[2] = 0.0;

	// rotate the body acceleration into NED frame
	MVmult(cEB, tempV2, pXdot->Vned_dot, 3, 3);

	// compute NED rate of change
	pXdot->NED_dot[0] = pX->Vned[0];
	pXdot->NED_dot[1] = pX->Vned[1];
	pXdot->NED_dot[2] = pX->Vned[2];

	// Compute body angular accelerations
	// wdot_body = -Jinv*Omega*J*w_body + Jinv*M_body
	MVmult(pU->J, pX->pqr, tempV1, 3, 3);
	MVmult(wx, tempV1, tempV2, 3, 3);
	MVmult(pU->Jinv, tempV2, tempV1, 3, 3);
	tempV1[0] = -tempV1[0];
	tempV1[1] = -tempV1[1];
	tempV1[2] = -tempV1[2];
	MVmult(pU->Jinv, pU->M, tempV2, 3, 3);
	VVadd(tempV1, tempV2, pXdot->pqr_dot, 3);

	// check holds on angular rates
	if( pU->hold_p )
		pXdot->pqr_dot[0] = 0.0;
	if( pU->hold_q )
		pXdot->pqr_dot[1] = 0.0;
	if( pU->hold_r )
		pXdot->pqr_dot[2] = 0.0;

	// Compute the quaternion derivatives
	// make the E matrix
	quatW(E, pX->pqr[0], pX->pqr[1], pX->pqr[2]);

	// compute the derivative of the quaternion
	MVmult(E, pX->Q, pXdot->Q_dot, 4, 4);
}

void integrate_sixdof_fe(struct sixdofXdot_fe_def *pXdot, 
						 struct sixdofX_fe_def *pX, 
						 struct sixdof_fe_inputs_def *pU, 
						 double dt)
{
	struct sixdofX_fe_def X;					// to run the derivative function
	struct sixdofX_fe_def X0;					// to backup the input state
	struct sixdofXdot_fe_def Xdot;				// to run the derivative function
	int i;
	double k1[13];
	double k2[13];
	double k3[13];
	double k4[13];

	// backing up the current input state
	for(i=0; i<3; ++i)
	{
		X0.NED[i] = pX->NED[i];
		X0.pqr[i] = pX->pqr[i];
		X0.Vned[i] = pX->Vned[i];
		X0.Q[i] = pX->Q[i];
	}
	X0.Q[3] = pX->Q[3];


	// Well, lets get started with the first step
	// first, make the X state value
	for(i=0; i<3; ++i)
	{
		X.NED[i] = X0.NED[i];
		X.pqr[i] = X0.pqr[i];
		X.Vned[i] = X0.Vned[i];
		X.Q[i] = X0.Q[i];
	}
	X.Q[3] = X0.Q[3];
	normq(X.Q);

	// run the function and get the Xdot values
	sixdof_fe_derivs(&Xdot, &X, pU);
	for(i=0; i<3; ++i)
	{
		// save the k step
		k1[i] = dt*Xdot.NED_dot[i];			// north east down
		k1[i+3] = dt*Xdot.pqr_dot[i];		// rates
		k1[i+6] = dt*Xdot.Vned_dot[i];		// accelerations
		k1[i+9] = dt*Xdot.Q_dot[i];			// quaternion

		// make the state value for the next step
		X.NED[i] = X0.NED[i] + k1[i]/2.0;
		X.pqr[i] = X0.pqr[i] + k1[i+3]/2.0;
		X.Vned[i] = X0.Vned[i] + k1[i+6]/2.0;
		X.Q[i] = X0.Q[i] + k1[i+9]/2.0;
	}
	k1[12] = dt*Xdot.Q_dot[3];
	X.Q[3] = X0.Q[3] + k1[12]/2.0;
	normq(X.Q);

	// save the Xdot values for output
	for(i=0; i<3; ++i)
	{
		pXdot->NED_dot[i] = Xdot.NED_dot[i];
		pXdot->pqr_dot[i] = Xdot.pqr_dot[i];
		pXdot->Vned_dot[i] = Xdot.Vned_dot[i];
		pXdot->Q_dot[i] = Xdot.Q_dot[i];
	}
	pXdot->Q_dot[3] = Xdot.Q_dot[3];
	

	// run the function and get the Xdot values
	sixdof_fe_derivs(&Xdot, &X, pU);
	for(i=0; i<3; ++i)
	{
		// save the k step
		k2[i] = dt*Xdot.NED_dot[i];			// north east down
		k2[i+3] = dt*Xdot.pqr_dot[i];		// rates
		k2[i+6] = dt*Xdot.Vned_dot[i];		// accelerations
		k2[i+9] = dt*Xdot.Q_dot[i];			// quaternion

		// make the state value for the next step
		X.NED[i] = X0.NED[i] + k2[i]/2.0;
		X.pqr[i] = X0.pqr[i] + k2[i+3]/2.0;
		X.Vned[i] = X0.Vned[i] + k2[i+6]/2.0;
		X.Q[i] = X0.Q[i] + k2[i+9]/2.0;
	}
	k2[12] = dt*Xdot.Q_dot[3];
	X.Q[3] = X0.Q[3] + k2[12]/2.0;
	normq(X.Q);

	// run the function and get the Xdot values
	sixdof_fe_derivs(&Xdot, &X, pU);
	for(i=0; i<3; ++i)
	{
		// save the k step
		k3[i] = dt*Xdot.NED_dot[i];			// north east down
		k3[i+3] = dt*Xdot.pqr_dot[i];		// rates
		k3[i+6] = dt*Xdot.Vned_dot[i];		// accelerations
		k3[i+9] = dt*Xdot.Q_dot[i];			// quaternion

		// make the state value for the next step
		X.NED[i] = X0.NED[i] + k3[i];
		X.pqr[i] = X0.pqr[i] + k3[i+3];
		X.Vned[i] = X0.Vned[i] + k3[i+6];
		X.Q[i] = X0.Q[i] + k3[i+9];
	}
	k3[12] = dt*Xdot.Q_dot[3];
	X.Q[3] = X0.Q[3] + k3[12];
	normq(X.Q);

	// run the function and get the Xdot values
	sixdof_fe_derivs(&Xdot, &X, pU);
	for(i=0; i<3; ++i)
	{
		// save the k step
		k4[i] = dt*Xdot.NED_dot[i];			// north east down
		k4[i+3] = dt*Xdot.pqr_dot[i];		// rates
		k4[i+6] = dt*Xdot.Vned_dot[i];		// accelerations
		k4[i+9] = dt*Xdot.Q_dot[i];			// quaternion
	}
	k4[12] = dt*Xdot.Q_dot[3];

	
	// assemble the final result for the state propogation
	for(i=0; i<3; ++i)
	{
		pX->NED[i] = X0.NED[i] + k1[i]/6.0 + k2[i]/3.0 + k3[i]/3.0 + k4[i]/6.0;
		pX->pqr[i] = X0.pqr[i] + k1[i+3]/6.0 + k2[i+3]/3.0 + k3[i+3]/3.0 + k4[i+3]/6.0;
		pX->Vned[i] = X0.Vned[i] + k1[i+6]/6.0 + k2[i+6]/3.0 + k3[i+6]/3.0 + k4[i+6]/6.0;
		pX->Q[i] = X0.Q[i] + k1[i+9]/6.0 + k2[i+9]/3.0 + k3[i+9]/3.0 + k4[i+9]/6.0;
	}
	pX->Q[3] = X0.Q[3] + k1[12]/6.0 + k2[12]/3.0 + k3[12]/3.0 + k4[12]/6.0;
	normq(pX->Q);


}

void sixdof_fe(struct sixdof_fe_state_def *pX, struct sixdof_fe_inputs_def *pU, double dt)
{
	double cBE[MAXSIZE][MAXSIZE];				// earth TP -> body transformation matrix
	double cEB[MAXSIZE][MAXSIZE];				// body -> earth TP transformation matrxi
	struct sixdofX_fe_def X;
	struct sixdofXdot_fe_def Xdot;
	int i;


	// make the earth TP -> body matrix
	quatDC(cBE, pX->Q);

	// make the body -> earth TP matrix
	transpose(cBE, cEB, 3, 3);

	// equate the state vectors and such
	for(i=0; i<3; ++i)
	{
		X.pqr[i] = pX->rate[i];
		X.Vned[i] = pX->Ve[i];
		X.NED[i] = pX->NED[i];
	}

	for(i=0; i<4; ++i)
	{
		X.Q[i] = pX->Q[i];
	}

	normq(X.Q);					// normalize the quaternion

	// call integrate_sixdof
	integrate_sixdof_fe(&Xdot, &X, pU, dt);

	// solve for the unknown state values for the next time step
	for(i=0; i<3; ++i)
	{
		pX->alpha[i] = Xdot.pqr_dot[i];				// body axis angular accelerations
		pX->rate[i] = X.pqr[i];						// body axis angular rates
		pX->accel[i] = pU->F[i]/pU->m;				// body axis acceleration
		pX->Ve[i] = X.Vned[i];						// body axis velocity in earth frame
		pX->NED[i] = X.NED[i];						// lat, lon, alt position
	}
	for(i=0; i<4; ++i)
	{
		pX->Q[i] = X.Q[i];							// quaternion
	}


	quat2euler(X.Q, pX->THETA);						// euler angle convertions
	
	MVmult(cBE, X.Vned, pX->Vb, 3, 3);				// body velocity in body frame

}








/*	This function will provide a servo-actuator model.  It contains two main
* components.  The first is a second-order dynamic model which is tuneable 
* via wn and zeta values.  The second, is a hysterisous model.  This feature
* is designed around basic deadband slop that may be found in system linkages.
*	See the structure for details.  Also, this system contains a state structure
* that needs to be maintained for state propogation.
*/
void servo_derivs(double *Xdot, double *X, double *t, double *u, double *args)
{
	double wn, zeta;

	wn = args[0];
	zeta = args[1];

	Xdot[0] = X[1];
	Xdot[1] = -2.0*zeta*wn*X[1] - wn*wn*X[0] + *u;
}


void servo(struct servo_state_def *pX, struct servo_inputs_def *pIn, double dt)
{
	double args[2];
	double X[2];
	double Xdot[2];
	double u;
	double y;
	double wn,zeta;



	// the model of the actuator is assumed to be of the following TF
	//		        s + wn^2
	//     -------------------------
	//     s^2 + 2*zeta*wn*s + wn^2

	X[0] = pX->X[0];
	X[1] = pX->X[1];

	wn = pIn->wn;
	zeta = pIn->zeta;
	
	args[0] = wn;
	args[1] = zeta;

	u = pIn->command;

	RK4(X, Xdot, 0, &u, args, 2, dt, &servo_derivs);		// integrate the state

	y = X[1] + wn*wn*X[0];

	pX->output = y;

	// copy the state back to the structure for saving
	pX->X[0] = X[0];
	pX->X[1] = X[1];

}

/*	This is a collision/landing gear model.  The basis of this model is the 
* vertial displacement method.  The idea is that if a collision is detected between
* the point, and a surface, then the displacement is computed.  The is then 
* used to compute a force based on Hook's law.  To allow for in-elastic collisions,
* a damping constant can be given.
*	For now, this function assumes that the contact surface is the ground (for landing gear).
* For this assumption, the ground is taken as 0 altitude.
*	For more information, see the structure definitions below.
*/
void gear(struct gear_inputs_def *pIn, struct gear_outputs_def *pOut)
{
	double wx[MAXSIZE][MAXSIZE];			// omega-cross matrix
	double delta;							// the displacement of the contact force under ground
	double tempV1[MAXSIZE];
	double tempV2[MAXSIZE];
	double Vw_b[MAXSIZE];					// velocity of wheel in body frame
	double Vw_e[MAXSIZE];					// velocity of wheel in earth frame
	double Pw_b[MAXSIZE];					// position of wheel in body frame
	double Pw_e[MAXSIZE];					// position of wheel in earth frame
	double cEB[MAXSIZE][MAXSIZE];			// body -> earth transformation matrix
	double cBE[MAXSIZE][MAXSIZE];			// earth -> body transformation matrix
	double Fw_e[MAXSIZE];					// force on wheel in earth frame

	Pw_b[0] = pIn->cg2point[0];
	Pw_b[1] = pIn->cg2point[1];
	Pw_b[2] = pIn->cg2point[2];

	// make the cBE matrix
	eulerDC(cBE, pIn->THETA[0], pIn->THETA[1], 0.0);		// don't care about yaw

	// make the cEB matrix
	transpose(cBE, cEB, 3, 3);

	// make the wx matrix
	eulerWx(wx, pIn->pqr[0], pIn->pqr[1], pIn->pqr[2]);

	// compute the position of wheel in earth frame
	MVmult(cEB, Pw_b, Pw_e, 3, 3);
	Pw_e[2] += pIn->altitude;

	// compute the displacement
	delta = Pw_e[2];

	if( delta > 0.0 )						// underground
	{
		// make the true position of the wheel (earth frame)
		Pw_e[2] = 0.0;
		// "                                " (body frame)
		MVmult(cBE, Pw_e, Pw_b, 3, 3);

		// compute the velocity of the wheel (body frame)
		MVmult(wx, Pw_b, tempV1, 3, 3);
		VVadd(pIn->uvw, tempV1, Vw_b, 3);

		// "                              " (earth frame)
		MVmult(cEB, Vw_b, Vw_e, 3, 3);

		// if the wheel has steering ability
		if( pIn->rotation != 0.0 )
		{
			// rotate the body velocity by the rotation of the wheel (+ rotation about Z body axis)
			tempV1[0] = pIn->uvw[0]*cos(pIn->rotation) + pIn->uvw[1]*sin(pIn->rotation);
			tempV1[1] = -pIn->uvw[0]*sin(pIn->rotation) + pIn->uvw[1]*cos(pIn->rotation);
			tempV1[2] = pIn->uvw[2];

			// recompute the velocity of wheel in body frame
			MVmult(wx, Pw_b, tempV2, 3, 3);
			VVadd(tempV1, tempV2, Vw_b, 3);

			// "                               " earth frame
			MVmult(cEB, Vw_b, Vw_e, 3, 3);
		}

		// compute the magnitude of the vertical force (earth frame)
		Fw_e[2] = pIn->k*delta + pIn->b*Vw_e[2];

		// compute the lateral forces on the wheel in earth frame
		// X
		if( Vw_e[0] != 0.0 )
		{
			Fw_e[0] = -pIn->mu_x*Fw_e[2]*Vw_e[0]/fabs(Vw_e[0]);
		}
		else
		{
			Fw_e[0] = 0.0;
		}
		// Y
		if( Vw_e[1] != 0.0 )
		{
			Fw_e[1] = -pIn->mu_y*Fw_e[2]*Vw_e[1]/fabs(Vw_e[1]);
		}
		else
		{
			Fw_e[1] = 0.0;
		}

		// take care of the sign of the vertical force on the wheel in earth frame
		Fw_e[2] *= -1.0;

		// compute the wheel force in body frame for output
		MVmult(cBE, Fw_e, pOut->F, 3, 3);

		// compute the moment in earth frame
		tempV1[0] = -Pw_e[0];
		tempV1[1] = -Pw_e[1];
		tempV1[2] = -Pw_e[2];
		cross(Fw_e, tempV1, tempV2);

		// compute the moment in body frame for output
		MVmult(cBE, tempV2, pOut->M, 3, 3);
	}
	else
	{
		Vinit(pOut->F, 3);
		Vinit(pOut->M, 3);
	}
}
	

/*	This is a basic wind/gust model.  It is a dynamic system that will generate
* random winds, up to a maximum value, in both vertical and horizontal directions.
*	To make this model work, like the servo and 6-DOF models, it is propogated over
* time for the dynamics.
*	See the structures for information
*
*	There are two functions.  The first, wind_init is used to initalize the wind model.
* The second, wind, is used to propograte the state of the wind model.
*/

void wind_init(struct wind_inputs_def *pIn, struct wind_state_def *pX)
{
	Vinit(pX->Ve, 3);			// zero out the state
	Vinit(pX->X, 6);			// zero out the state
	srand(pIn->seed);				// seed the random number generator
}

void wind_derivs(double *Xdot, double *X, double *t, double *u, double *args)
{
	double wn, zeta;

	wn = args[0];
	zeta = args[1];

	// Vn component
	Xdot[0] = X[1];
	Xdot[1] = -2.0*zeta*wn*X[1] - wn*wn*X[0] + u[0];

	// Ve component
	Xdot[2] = X[3];
	Xdot[3] = -2.0*zeta*wn*X[3] - wn*wn*X[2] + u[1];

	// Vd component
	Xdot[4] = X[5];
	Xdot[5] = -2.0*zeta*wn*X[5] - wn*wn*X[4] + u[2];
}


void wind(struct wind_inputs_def *pIn, struct wind_state_def *pX, double dt)
{
	double Xdot[6];
	double args[2];
	double U[3];
	double xx, yy, zz;

	args[0] = 1.5;
	args[1] = 0.00005;

	xx = (double)(rand())/( (double)(RAND_MAX)/2.0) - 1.0;
	yy = (double)(rand())/( (double)(RAND_MAX)/2.0) - 1.0;
	zz = (double)(rand())/( (double)(RAND_MAX)/2.0) - 1.0;

	U[0] = xx*10.0;
	U[1] = yy*10.0;
	U[2] = yy*10.0;

	RK4(pX->X, Xdot, 0, U, args, 6, dt, &wind_derivs);

	pX->Ve[0] = pX->X[0]*SQR(args[0])*pIn->horz_max;
	pX->Ve[1] = pX->X[2]*SQR(args[0])*pIn->horz_max;
	pX->Ve[2] = pX->X[4]*SQR(args[0])*pIn->vert_max;
	
}

/*	This is a gravity and gravitation model.  It is based on the WGS-84 elipsoid
* model of the Earth.  The source for this model is from Dr. Brian Steven's class.
* See the structures for information.
*/
void gravity(struct grav_inputs_def *pIn, struct grav_outputs_def *pOut)
{
	double GM;
	double f, e;
	double C2, P2, J2;
	double we;
	double Rc, psi;
	double N;
	double pECEF[MAXSIZE];
	double pLLH[MAXSIZE];

	pLLH[0] = pIn->latitude;				// rad
	pLLH[1] = 0.0;							// rad (don't care about the longitude)
	pLLH[2] = pIn->altitude*C_FT2M;			// m MSL + up

	// compute the position in ECEF coordinates
	llh2ECEF(pLLH, pECEF);

	// compute the current distance from you to center of the earth
	Rc = sqrt( SQR(pECEF[0]) + SQR(pECEF[1]) + SQR(pECEF[2]) );		// m

	// compute the geocentric latitude
	psi = asin(pECEF[2]/Rc);										// rad

	// define some constants
	GM = 3.9860015E14;							//m^3/s^2
	f = (C_WGS84_a - C_WGS84_b)/C_WGS84_a;			// flattening of the earth m
	e = sqrt(2.0*f - SQR(f));					// eccentricity of the earth
	C2 = -0.48416685E-3;
	P2 = sqrt(5.0)/2.0*(3.0*SQR(sin(psi)) - 1.0);
	J2 = -sqrt(5.0)*C2;
	we = 2.0*C_PI/(24.0*60.0*60.0);				// rad/sec
	N = C_WGS84_a/sqrt(1.0 - SQR(e)*SQR(sin(pIn->latitude)));	

	// compute the Gravitation vector
	pOut->G[0] = (GM/SQR(Rc))*(-SQR(C_WGS84_a/Rc)*3.0*J2*sin(psi)*cos(psi));
	pOut->G[1] = 0.0;
	pOut->G[2] = (GM/SQR(Rc))*(1.0 - (3.0/2.0)*SQR(C_WGS84_a/Rc)*J2*(3.0*SQR(sin(psi)) - 1.0));

	// compute the gravity vector
	pOut->g[0] = pOut->G[0] - SQR(we)*(N + pIn->altitude*C_FT2M)*cos(pIn->latitude)*sin(pIn->latitude);
	pOut->g[1] = 0.0;
	pOut->g[2] = pOut->G[2] - SQR(we)*(N + pIn->altitude*C_FT2M)*cos(pIn->latitude)*cos(pIn->latitude);


	// convert all from m/s/s to ft/s/s
	pOut->G[0] *= C_M2FT;
	pOut->G[1] *= C_M2FT;
	pOut->G[2] *= C_M2FT;
	pOut->g[0] *= C_M2FT;
	pOut->g[1] *= C_M2FT;
	pOut->g[2] *= C_M2FT;
}

	







