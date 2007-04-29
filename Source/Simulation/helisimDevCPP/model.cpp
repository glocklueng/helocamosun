
/******************************************************************
*
*	This is the new X-Cell helicopter simulation model
*
*	Author: Aaron Kahn
*
******************************************************************/


#include "model.h"


/**************** HELICOPTER STRUCTUERS ****************************/
struct heli_def xcell;

double model_dt = 0.001;		// sec

/********************* INTERNAL FUNCTIONS *********************************/
/* this is for the rotor flapping
*           .   . . .
*	Xdot = [a1 b1 d c]
*	X = [a1 b1 d c]
*	A1B1 = [A1 B1]
*	args = [u v p q db1dv da1du w_in w_off kc dir tau Kc Kd]
*/
void RotorFlapDynamics(double Xdot[2], double X[2], double *t, double A1B1[2], double args[13]);


void RotorFlapDynamics(double Xdot[4], double X[4], double *t, double A1B1[2], double args[13])
{
	double u,v,p,q,db1dv,da1du,w_in,w_off,kc,dir,tau,Kc,Kd;
	double a_sum, b_sum;
	double a1dot, b1dot, a1, b1;
	double d_dot, c_dot, d, c;
	double A1, B1;

	u = args[0];
	v = args[1];
	p = args[2];
	q = args[3];
	db1dv = args[4];
	da1du = args[5];
	w_in = args[6];
	w_off = args[7];
	kc = args[8];
	dir = args[9];
	tau = args[10];
	Kc = args[11];
	Kd = args[12];

	a1 = X[0];
	b1 = X[1];
	d = X[2];
	c = X[3];

	// flybar mixing
	// lateral mixing
	A1 = A1B1[0] + Kd*d;
	// longitudinal mixing
	B1 = A1B1[1] + Kc*c;

	a_sum = b1 - A1 + dir*kc*a1 - db1dv*v*0.3;
	b_sum = a1 + B1 - dir*kc*b1 - da1du*u*0.3;
	a1dot = -w_in*b_sum - w_off*a_sum - q;
	b1dot = -w_in*a_sum + w_off*b_sum - p;

	d_dot = -d/tau - p + 0.2731*A1B1[0]/tau;
	c_dot = -c/tau - q - 0.2587*A1B1[1]/tau;

	Xdot[0] = a1dot;
	Xdot[1] = b1dot;
	Xdot[2] = d_dot;
	Xdot[3] = c_dot;
}




/********************** FUNCTIONS *****************************************/




/*	This will do a combined blade element momentum theory thrust, power, torque computation
* on a rotor.  The inputs to the function are all of the input elements of 
* blade_element_def.  Only T, P, Q, and average v are outputs.
*
*	Source: Keys, Pgs 96-98
*			Proudy, Pg 96
*/
void rotorCalc(struct blade_element_def *pBe)
{
	double dR;						// thickness of the blade element
	double r_R;						// ratio of local radius to total radius
	double r;						// local radius
	double theta_r;					// local collective angle
	double theta0;					// root collective angle
	double omega_r;					// local velocity
	double alpha;					// local angle of attack
	double v1[NUMSTATIONS];			// local induced velocity
	double dT[NUMSTATIONS];			// incriment of thrust
	double dD[NUMSTATIONS];			// incriment of profile drag
	double dQ[NUMSTATIONS];			// incriment of torque
	double dP[NUMSTATIONS];			// incriment of power
	double vv;						// abcOmega/2 + 4piVperp
	int n;
	int i;


	vv = pBe->a*pBe->b*pBe->c*pBe->omega/2.0 + 4.0*C_PI*pBe->Vperp;
	dR = (pBe->R - pBe->R0)/100.0;
	theta0 = fabs(pBe->collective) - (0.75 - (pBe->R0/pBe->R))*pBe->twst;

	for(n=1; n<=NUMSTATIONS; ++n)
	{
		i = n-1;
		r_R = (pBe->R0 + (double)(n)*dR)/pBe->R;
		r = r_R*pBe->R;
		theta_r = theta0 + pBe->twst*r_R;
		omega_r = pBe->omega*r;
		alpha = theta_r - pBe->Vperp/omega_r;
		v1[i] = ( -vv + sqrt(fabs( SQR(vv) + 8.0*C_PI*pBe->b*SQR(pBe->omega)*pBe->a*pBe->c*r*alpha )))/(8.0*C_PI);// Proudy pg 96
		dT[i] = 4.0*C_PI*pBe->rho*(pBe->Vperp + v1[i])*v1[i]*r*dR;		// Keys eq 3.11
		dD[i] = 0.5*pBe->Cd0*pBe->rho*SQR(omega_r)*pBe->c*dR;			// Keys eq 3.5
		dQ[i] = (dT[i]*(pBe->Vperp + v1[i])/omega_r + dD[i])*r;			// Keys eq 3.9a
		dP[i] = dQ[i]*pBe->omega;										// Keys eq 3.9b
	}

	pBe->T = 0.0;
	pBe->Q = 0.0;
	pBe->P = 0.0;
	pBe->avg_v1 = 0.0;

	for(n=0; n<NUMSTATIONS; ++n)
	{
		pBe->T += dT[n];
		pBe->Q += dQ[n];
		pBe->P += dP[n];
		pBe->avg_v1 += v1[n];
	}
	pBe->avg_v1 = pBe->avg_v1/(double)(NUMSTATIONS);
	
	if( pBe->collective < 0.0 )
	{
		pBe->T *= -1.0;
		pBe->avg_v1 *= -1.0;
	}

}



/*	This will perform the initialization of the entire model. It is also the function
* to call to reset the model.  For the case of resetting, more than the minimum calculations
* are done, but this miminimizes the number of functions to deal with.
*/
void ModelInit(void)
{
	struct sixdof_fe_init_inputs_def sixinit;
	double rho, temp, pres, sp_sound;

	/******************* LOAD THE RAW DATA FOR THE X-CELL HELICOPTER *********************/
	// CG information
	xcell.cg.fs_cg = 0.0;					// in
	xcell.cg.wl_cg = 10.91;					// in
	xcell.cg.wt = 19.5;						// lbs
	xcell.cg.ix = 0.2184;					// slug-ft^2
	xcell.cg.iy = 0.3214;					// slug-ft^2
	xcell.cg.iz = 0.4608;					// slug-ft^2
	xcell.cg.ixz = 0.0337;					// slug-ft^2
	xcell.cg.hp_loss = 0.1;					// HP
	xcell.cg.m = xcell.cg.wt/32.2;			// slugs
	xcell.cg.altitude = 0.0;				// initial DA ft
	
	// Main Rotor Information
	xcell.m.fs = 0.0;						// in
	xcell.m.wl = 0.0;						// in
	xcell.m.is = 0.0;						// longitudinal shaft tilt (rad)
	xcell.m.e = 0.0225;						// ft
	xcell.m.i_b = 0.0847;					// slug-ft^2
	xcell.m.r = 2.25;						// ft
	xcell.m.ro = 0.6;						// ft
	xcell.m.a = 6.0;						// */rad
	xcell.m.cd0 = 0.01;						// nondimentional
	xcell.m.b = 2;							// # of blades
	xcell.m.c = 0.1979;						// ft
	xcell.m.twst = 0.0;						// rad
	xcell.m.k1 = 0;							// delta-3 hinge	
	xcell.m.dir = -1.0;						// MR direction of rotation viewed from top (1 = ccw; -1 = cw)
	xcell.m.ib = 0.0;						// laterial shaft tilt (rad)

	// Flybar Information (Tischler and Mettler, System Identification Modeling
	// Of a Model-Scale Helicopter )
	xcell.fb.tau = 0.36;					// sec
	xcell.fb.Kd = 0.3;
	xcell.fb.Kc = 0.3;

	// Fuselage Information
	xcell.f.fs = 3.0;						// in
	xcell.f.wl = 12.0;						// in
	xcell.f.xuu = -0.4240;					// ft^2
	xcell.f.yvv = -1.2518;					// ft^2
	xcell.f.zww = -0.8861;					// ft^2

	// Horizontal Fin Information
	xcell.h.fs = 0;							// in
	xcell.h.wl = 0;							// in
	xcell.h.zuu = 0;						// ft^2
	xcell.h.zuw = 0;						// ft^2
	xcell.h.zmax = 0;						// ft^2

	// Vertical Fin Information
	xcell.v.fs = -41.5;						// in
	xcell.v.wl = 7.25;						// in
	xcell.v.yuu = 0;						// ft^2
	xcell.v.yuv = -1.4339;					// ft^2
	xcell.v.ymax = -0.275;					// ft^2
	xcell.v.a = 5.5;						// */rad

	// Tail Rotor Information
	xcell.t.fs = -41.5;						// in
	xcell.t.wl = 7.25;						// in
	xcell.t.r = 0.5417;						// ft
	xcell.t.r0 = 0.083;						// ft
	xcell.t.a = 3.0;						// */rad
	xcell.t.b = 2;							// # of TR blades
	xcell.t.c = 0.099;						// ft
	xcell.t.twst = 0.0;						// rad
	xcell.t.cd0 = 0.01;						// nondimentional
	xcell.t.duct = 0.0;						// duct augmetation (duct*thrust; power/duct)


	// Landing Gear Information
	// right-front gear
	xcell.grIn[0].cg2point[0] = (8.0 - xcell.cg.fs_cg)/12.0;
	xcell.grIn[0].cg2point[1] = 0.65;
	xcell.grIn[0].cg2point[2] = (20.0 - xcell.cg.wl_cg)/12.0;
	xcell.grIn[0].k = 120.0;
	xcell.grIn[0].b = sqrt(2.0*xcell.grIn[0].k);
	xcell.grIn[0].mu_x = 0.8;
	xcell.grIn[0].mu_y = 0.8;
	xcell.grIn[0].rotation = 0.0;

	// right-back gear
	xcell.grIn[1].cg2point[0] = (-6.0 - xcell.cg.fs_cg)/12.0;
	xcell.grIn[1].cg2point[1] = 0.65;
	xcell.grIn[1].cg2point[2] = (20.0 - xcell.cg.wl_cg)/12.0;
	xcell.grIn[1].k = 120.0;
	xcell.grIn[1].b = sqrt(2.0*xcell.grIn[0].k);
	xcell.grIn[1].mu_x = 0.8;
	xcell.grIn[1].mu_y = 0.8;
	xcell.grIn[1].rotation = 0.0;

	// left-front gear
	xcell.grIn[2].cg2point[0] = (8.0 - xcell.cg.fs_cg)/12.0;
	xcell.grIn[2].cg2point[1] = -0.65;
	xcell.grIn[2].cg2point[2] = (20.0 - xcell.cg.wl_cg)/12.0;
	xcell.grIn[2].k = 120.0;
	xcell.grIn[2].b = sqrt(2.0*xcell.grIn[0].k);
	xcell.grIn[2].mu_x = 0.8;
	xcell.grIn[2].mu_y = 0.8;
	xcell.grIn[2].rotation = 0.0;

	// left-back gear
	xcell.grIn[3].cg2point[0] = (-6.0 - xcell.cg.fs_cg)/12.0;
	xcell.grIn[3].cg2point[1] = -0.65;
	xcell.grIn[3].cg2point[2] = (20.0 - xcell.cg.wl_cg)/12.0;
	xcell.grIn[3].k = 120.0;
	xcell.grIn[3].b = sqrt(2.0*xcell.grIn[0].k);
	xcell.grIn[3].mu_x = 0.8;
	xcell.grIn[3].mu_y = 0.8;
	xcell.grIn[3].rotation = 0.0;

	// tail
	xcell.grIn[4].cg2point[0] = (-41.5 - xcell.cg.fs_cg)/12.0;
	xcell.grIn[4].cg2point[1] = 0.0;
	xcell.grIn[4].cg2point[2] = (15.0 - xcell.cg.wl_cg)/12.0;
	xcell.grIn[4].k = 140.0;
	xcell.grIn[4].b = sqrt(2.0*xcell.grIn[0].k);
	xcell.grIn[4].mu_x = 0.8;
	xcell.grIn[4].mu_y = 0.8;
	xcell.grIn[4].rotation = 0.0;

	xcell.num_gear = 5;

	// Servo Information (all servos are generic)
	// B1 (pitch) servo
	xcell.svIn[0].wn = 38.2261;
	xcell.svIn[0].zeta = 0.5118;
	xcell.svIn[0].slop = 0.0;

	// A1 (roll) servo
	xcell.svIn[1].wn = 38.2261;
	xcell.svIn[1].zeta = 0.5118;
	xcell.svIn[1].slop = 0.0;

	// MR Coll servo
	xcell.svIn[2].wn = 38.2261;
	xcell.svIn[2].zeta = 0.5118;
	xcell.svIn[2].slop = 0.0;

	// TR Coll servo
	xcell.svIn[3].wn = 38.2261;
	xcell.svIn[3].zeta = 0.5118;
	xcell.svIn[3].slop = 0.0;


	// Tail Gyro Gain
	xcell.c.gyro_gain = 0;//80.0;
    ///we dont have a tail rotor gyro
    
/************** COMPUTE CG RELATIVE POSITIONS **********************************/
	// Main Rotor
	xcell.m.h = (xcell.cg.wl_cg - xcell.m.wl)/12.0;	// vertical distance to CG (ft)
	xcell.m.d = (xcell.cg.fs_cg - xcell.m.fs)/12.0;	// horizontal distance to CG (ft)

	// Tail Rotor
	xcell.t.h = (xcell.cg.wl_cg - xcell.t.wl)/12.0;	// vertical distance to CG (ft)
	xcell.t.d = (xcell.cg.fs_cg - xcell.t.fs)/12.0;	// horizontal distance to CG (ft)

	// Fuselage
	xcell.f.h = (xcell.cg.wl_cg - xcell.f.wl)/12.0;	// vertical distance to CG (ft)
	xcell.f.d = (xcell.cg.fs_cg - xcell.f.fs)/12.0;	// horizontal distance to CG (ft)

	// Horizontal Fin
	xcell.h.h = (xcell.cg.wl_cg - xcell.h.wl)/12.0;	// vertical distance to CG (ft)
	xcell.h.d = (xcell.cg.fs_cg - xcell.h.fs)/12.0;	// horizontal distance to CG (ft)

	// Vertical Fin
	xcell.v.h = (xcell.cg.wl_cg - xcell.v.wl)/12.0;	// vertical distance to CG (ft)
	xcell.v.d = (xcell.cg.fs_cg - xcell.v.fs)/12.0;	// horizontal distance to CG (ft)

/************** STATE VECTOR AND CONTROL INITIALIZATIONS ************************/
	
	// 6-DOF Initializations
	sixinit.Ixx = xcell.cg.ix;
	sixinit.Iyy = xcell.cg.iy;
	sixinit.Izz = xcell.cg.iz;
	sixinit.Ixz = xcell.cg.ixz;
	sixinit.m = xcell.cg.m;
	Vinit(sixinit.NED, 3);
	sixinit.NED[2] = -2.0;
	Vinit(sixinit.THETA, 3);
	Vinit(sixinit.pqr, 3);
	Vinit(sixinit.uvw, 3);
	sixdof_fe_init(&sixinit, &xcell.sixdofIn, &xcell.sixdofX);

	// CG Initialization
	xcell.cg.altitude = 0.0;
	memcpy(xcell.cg.NED, xcell.sixdofX.NED, 3*sizeof(double));
	memcpy(xcell.cg.uvw, xcell.sixdofX.Vb, 3*sizeof(double));
	memcpy(xcell.cg.V, xcell.sixdofX.Ve, 3*sizeof(double));
	memcpy(xcell.cg.THETA, xcell.sixdofX.THETA, 3*sizeof(double));
	memcpy(xcell.cg.pqr, xcell.sixdofX.rate, 3*sizeof(double));
	xcell.cg.time = 0.0;
	Vinit(xcell.cg.F, 3);
	Vinit(xcell.cg.M, 3);

	// Initial Control Inputs
	xcell.c.A1 = 0.0;						// roll (rad + right wing down)
	xcell.c.B1 = 0.0;						// pitch (rad + nose down)
	xcell.c.mr_col = 2.5*C_DEG2RAD;			// mr col (rad)
	xcell.c.tr_col = 4.5*C_DEG2RAD;			// tr col (rad)
	xcell.c.mr_rev = 1500.0;				// mr RPM
	xcell.c.tr_rev = 4.6*xcell.c.mr_rev;	// tr RPM
	xcell.c.gyro_gain = 0.08;

	// set/clear the holds
	xcell.sixdofIn.hold_p = 0;
	xcell.sixdofIn.hold_q = 0;
	xcell.sixdofIn.hold_r = 0;
	xcell.sixdofIn.hold_u = 0;
	xcell.sixdofIn.hold_v = 0;
	xcell.sixdofIn.hold_w = 0;


	// Servo Initialization
	// B1 (pitch) servo
	Vinit(xcell.svX[0].X, 2);
	xcell.svX[0].output = 0.0;
	xcell.svIn[0].command = xcell.c.B1;

	// A1 (roll) servo
	Vinit(xcell.svX[1].X, 2);
	xcell.svX[1].output = 0.0;
	xcell.svIn[1].command = xcell.c.A1;

	// MR collective servo
	Vinit(xcell.svX[2].X, 2);
	xcell.svX[2].output = 0.0;
	xcell.svIn[2].command = xcell.c.mr_col;

	// TR collective servo
	Vinit(xcell.svX[3].X, 2);
	xcell.svX[3].output = 0.0;
	xcell.svIn[3].command = xcell.c.tr_col;

/**************** MAIN ROTOR DYNAMIC CONSTANT CALUCULATIONS **********************/
	
	atmosphere((xcell.cg.altitude - xcell.cg.NED[2]), &rho, &pres, &temp, &sp_sound);

	xcell.m.omega = xcell.c.mr_rev*C_TWOPI/60.0;							// rad/s
	xcell.m.v_tip = xcell.m.r*xcell.m.omega;									// ft/s
	xcell.m.lock = (rho*xcell.m.a*xcell.m.c*pow(xcell.m.r, 4.0))/xcell.m.i_b;		// mr lock number
	xcell.m.omega_f = (xcell.m.lock*xcell.m.omega/16.0)*(1.0 + (8.0/3.0)*(xcell.m.e/xcell.m.r));	// natural freq shift
	xcell.m.k2 = 0.75*(xcell.m.e/xcell.m.r)*(xcell.m.omega/xcell.m.omega_f);		// cross-couple coef.
	xcell.m.kc = xcell.m.k1 + xcell.m.k2;									// total cross-couple coef.
	xcell.m.tau = 16.0/(xcell.m.omega*xcell.m.lock);							// time constant of rotor
	xcell.m.w_off = xcell.m.omega/(1.0 + SQR(xcell.m.omega/xcell.m.omega_f));	// off-axis flap
	xcell.m.w_in = xcell.m.omega/xcell.m.omega_f*xcell.m.w_off;					// on-axis flap
	xcell.m.dl_db1 = 0.75*(xcell.m.b*xcell.m.c*SQR(xcell.m.r)*rho*\
		SQR(xcell.m.v_tip)*xcell.m.a*xcell.m.e/(xcell.m.lock*xcell.m.r));			// moment coef
	xcell.m.dm_da1 = xcell.m.dl_db1;
	xcell.m.ct = xcell.cg.wt/(rho*C_PI*SQR(xcell.m.r)*SQR(xcell.m.v_tip));		// thrust coef
	xcell.m.sigma = xcell.m.b*xcell.m.c/(C_PI*xcell.m.r);						// solidity
	xcell.m.db1dv = -(2.0/xcell.m.v_tip)*(8.0*xcell.m.ct/(xcell.m.a*xcell.m.sigma) + \
		sqrt(xcell.m.ct/2.0));											// flap back coef
	xcell.m.da1du = -xcell.m.db1dv;										// flab back coef

	xcell.m.vi = 15.0;
	xcell.m.a1 = 0.0;
	xcell.m.b1 = 0.0;
	xcell.m.a1dot = 0.0;
	xcell.m.b1dot = 0.0;
	xcell.m.thrust = 0.0;
	xcell.m.x = 0.0;
	xcell.m.y = 0.0;
	xcell.m.z = 0.0;
	xcell.m.l = 0.0;
	xcell.m.m = 0.0;
	xcell.m.n = 0.0;
	xcell.fb.c = 0.0;
	xcell.fb.c_dot = 0.0;
	xcell.fb.d = 0.0;
	xcell.fb.d_dot = 0.0;

/****************** TAIL ROTOR CALCULATIONS ***************************************/

	xcell.t.omega = xcell.c.tr_rev*C_TWOPI/60.0;
	xcell.t.fr = xcell.t.cd0*xcell.t.r*xcell.t.b*xcell.t.c;
	xcell.t.vi = 10.0;
	xcell.t.thrust = 0.0;
	xcell.t.x = 0.0;
	xcell.t.y = 0.0;
	xcell.t.z = 0.0;
	xcell.t.l = 0.0;
	xcell.t.m = 0.0;
	xcell.t.n = 0.0;
	

}

/*	This will perform the time stepping of the servo model.
* The values of U[7] are as follows...
*	U[0] = main rotor collective (rad)
*	U[1] = A1 swashplate tilt (+ right roll) (rad)
*	U[2] = B1 swashplate tilt (+ nose down) (rad)
*	U[3] = tail rotor collective (rad)
*
*  The svIn[3] and svX[3] are as follows...
*	sv[0] = B1 (pitch servo)
*	sv[1] = A1 (roll servo)
*	sv[2] = main rotor collective
*	sv[3] = tail rotor collective
*/
void Actuator(struct heli_def *heli, 
			  double U[4])
{
	struct control_def *c = &heli->c;
	struct cg_def *cg = &heli->cg;

	struct servo_inputs_def *A1svIn = &heli->svIn[1];
	struct servo_state_def *A1svX = &heli->svX[1];

	struct servo_inputs_def *B1svIn = &heli->svIn[0];
	struct servo_state_def *B1svX = &heli->svX[0];

	struct servo_inputs_def *mrColsvIn = &heli->svIn[2];
	struct servo_state_def *mrColsvX = &heli->svX[2];

	struct servo_inputs_def *trColsvIn = &heli->svIn[3];
	struct servo_state_def *trColsvX = &heli->svX[3];

	mrColsvIn->command = LIMIT(U[0], 2.5*C_DEG2RAD, 18.0*C_DEG2RAD);
	A1svIn->command = LIMIT(U[1], -8.0*C_DEG2RAD, 8.0*C_DEG2RAD);
	B1svIn->command = LIMIT(U[2], -8.0*C_DEG2RAD, 8.0*C_DEG2RAD);
	trColsvIn->command = LIMIT(U[3], -20.0*C_DEG2RAD, 20.0*C_DEG2RAD);

	servo(A1svX, A1svIn, model_dt);
	servo(B1svX, B1svIn, model_dt);
	servo(mrColsvX, mrColsvIn, model_dt);
	servo(trColsvX, trColsvIn, model_dt);

	c->B1 = B1svX->output;
	c->A1 = A1svX->output;
	c->mr_col = mrColsvX->output;
	c->tr_col = trColsvX->output;


}


/*	This will perform the landing gear calculations with the landing gear
* model provided in the simulation library.
*/
void LandingGear(struct heli_def *heli)
{
	struct cg_def *cg = &heli->cg;
	struct control_def *c = &heli->c;
	int i;
	
	for(i=0; i<heli->num_gear; ++i)
	{
		heli->grIn[i].altitude = cg->NED[2];
		memcpy(heli->grIn[i].THETA, cg->THETA, 3*sizeof(double));
		memcpy(heli->grIn[i].uvw, cg->uvw, 3*sizeof(double));
		memcpy(heli->grIn[i].pqr, cg->pqr, 3*sizeof(double));

		gear(&heli->grIn[i], &heli->grOut[i]);

		cg->F[0] += heli->grOut[i].F[0];
		cg->F[1] += heli->grOut[i].F[1];
		cg->F[2] += heli->grOut[i].F[2];

		cg->M[0] += heli->grOut[i].M[0];
		cg->M[1] += heli->grOut[i].M[1];
		cg->M[2] += heli->grOut[i].M[2];
	}
	
		
}


/*	This will perform the forces and moments calculations on the aircraft
* before the calculations of the 6-DOF.  The landing gear calculations are done
* after this function.  The servo and wind models are run after this as well.
*/
void FandM(struct heli_def *heli)
{
	struct mainrotor_def *m = &heli->m;
	struct flybar_def *fb = &heli->fb;
	struct tailrotor_def *t = &heli->t;
	struct verticalfin_def *v = &heli->v;
	struct horizontalfin_def *h = &heli->h;
	struct fuse_def *f = &heli->f;
	struct cg_def *cg = &heli->cg;
	struct blade_element_def *mb = &heli->MRBE;
	struct blade_element_def *tb = &heli->TRBE;
	struct control_def *c = &heli->c;

	double rho;									// density of air (slug/ft^3)
	double pressure;							// air pressure (lb/ft^2)
	double temperature;							// air temperature (deg R)
	double sp_sound;							// local speed of sound (ft/s)
	double densAlt;								// current density altitude (ft MSL + up)

	double cBE[MAXSIZE][MAXSIZE];				// rotation matrix earth TP -> body
	double tempV[MAXSIZE];						// temp vector

	double Xdot[4], X[4], U[2], args[13];		// for RK4 routine (rotor dynamics)


	
	// compute the current atmospheric conditions
	densAlt = cg->altitude - cg->NED[2];
	atmosphere(densAlt, &rho, &pressure, &temperature, &sp_sound);

	// compute the local gravitational force
	tempV[0] = 0.0;
	tempV[1] = 0.0;
	tempV[2] = 32.2*cg->m;
	eulerDC(cBE, cg->THETA[0], cg->THETA[1], cg->THETA[2]);
	MVmult(cBE, tempV, cg->F, 3, 3);

	// Main Rotor Calculations
	mb->a = m->a;
	mb->b = m->b;
	mb->c = m->c;
	mb->Cd0 = m->cd0;
	mb->collective = c->mr_col;
	mb->e = 0.7;					// oswalds efficency factor
	mb->omega = c->mr_rev*C_TWOPI/60.0;
	mb->R = m->r;
	mb->R0 = m->ro;
	mb->rho = rho;
	mb->twst = m->twst;
	mb->Vperp = -(cg->uvw[2] + cg->uvw[0]*(m->is + m->a1) - cg->uvw[1]*(m->ib + m->b1));
	rotorCalc(mb);
	m->thrust = mb->T;
	m->power = mb->P;
	m->torque = mb->Q;
	m->vi = mb->avg_v1;

	m->x = -m->thrust*(m->is + m->a1);
	m->y = m->thrust*(m->ib + m->b1);
	m->z = -m->thrust;

	m->l = m->y*m->h + m->dl_db1*m->b1;
	m->m = m->z*m->d - m->x*m->h + m->dm_da1*m->a1;
	m->n = m->torque*m->dir;

	// Tail Rotor Calculations

	tb->a = t->a;
	tb->b = t->b;
	tb->c = t->c;
	tb->Cd0 = t->cd0;
	tb->collective = c->tr_col;
	tb->e = 0.7;					// oswalds efficency factor
	tb->omega = c->tr_rev*C_TWOPI/60.0;
	tb->R = t->r;
	tb->R0 = t->r0;
	tb->rho = rho;
	tb->twst = t->twst;
	tb->Vperp = (cg->uvw[1] - t->d*cg->pqr[2])*m->dir;
	rotorCalc(tb);
	t->thrust = tb->T + tb->T*t->duct;
	t->power = tb->P - tb->P*t->duct;

	t->x = 0.0;
	t->y = t->thrust*m->dir;
	t->z = 0.0;

	t->l = t->y*t->h;
	t->m = 0.0;
	t->n = -t->y*t->d;

	// Fuselage Calculations
	f->x = (rho/2.0)*cg->uvw[0]*fabs(cg->uvw[0])*f->xuu;
	f->y = (rho/2.0)*cg->uvw[1]*fabs(cg->uvw[1])*f->yvv;
	f->z = (rho/2.0)*f->zww*( cg->uvw[2]*fabs(cg->uvw[2]) - m->vi );

	f->l = f->y*f->h;
	f->m = f->z*f->d - f->x*f->h;
	f->n = -f->y*f->d;

	// Vertical Fin Calculations
	v->x = 0.0;
	v->y = (rho/2.0)*cg->uvw[1]*fabs(cg->uvw[1])*v->yuv;
	v->z = 0.0;

	v->l = v->y*v->h;
	v->m = 0.0;
	v->n = -v->y*v->d;

	// Main Rotor TPP Dynamics
	X[0] = m->a1;
	X[1] = m->b1;
	X[2] = fb->d;
	X[3] = fb->c;
	U[0] = c->A1;
	U[1] = c->B1;
	args[0] = cg->uvw[0];
	args[1] = cg->uvw[1];
	args[2] = cg->pqr[0];
	args[3] = cg->pqr[1];
	args[4] = m->db1dv;
	args[5] = m->da1du;
	args[6] = m->w_in;
	args[7] = m->w_off;
	args[8] = m->kc;
	args[9] = m->dir;
	args[10] = fb->tau;
	args[11] = fb->Kc;
	args[12] = fb->Kd;
	
	RK4(X, Xdot, cg->time, U, args, 4, model_dt, &RotorFlapDynamics);

	m->a1 = X[0];
	m->b1 = X[1];
	fb->d = X[2];
	fb->c = X[3];
	m->a1dot = Xdot[0];
	m->b1dot = Xdot[1];
	fb->d_dot = Xdot[2];
	fb->c_dot = Xdot[3];


	// Sum Up Total Forces and Moments At CG

	cg->F[0] += m->x + t->x + f->x + v->x;
	cg->F[1] += m->y + t->y + f->y + v->y;
	cg->F[2] += m->z + t->z + f->z + v->z;

	cg->M[0] = m->l + t->l + f->l + v->l;
	cg->M[1] = m->m + t->m + f->m + v->m;
	cg->M[2] = m->n + t->n + f->n + v->n;

}


/*	This will perform the entire calculations of everything that needs to happen to 
* make the math model of the vehicle work.  This is the function to call to
* propogate the helicopter model, 6-DOF, landing gear, servos, and wind.
*
*	U[4] = [mr_coll				rad + goes up
*			A1					rad + right wind down
*			B1					rad + nose down
*			tr_coll]			rad + right turn
*/
void ModelGO(double U[4])
{

	FandM(&xcell);
	LandingGear(&xcell);
	Actuator(&xcell, U);

	memcpy(xcell.sixdofIn.F, xcell.cg.F, sizeof(double)*MAXSIZE);
	memcpy(xcell.sixdofIn.M, xcell.cg.M, sizeof(double)*MAXSIZE);


	sixdof_fe(&xcell.sixdofX, &xcell.sixdofIn, model_dt);

	memcpy(xcell.cg.NED, xcell.sixdofX.NED, sizeof(double)*MAXSIZE);
	memcpy(xcell.cg.uvw, xcell.sixdofX.Vb, sizeof(double)*MAXSIZE);
	memcpy(xcell.cg.V, xcell.sixdofX.Ve, sizeof(double)*MAXSIZE);
	memcpy(xcell.cg.THETA, xcell.sixdofX.THETA, sizeof(double)*MAXSIZE);
	memcpy(xcell.cg.pqr, xcell.sixdofX.rate, sizeof(double)*MAXSIZE);

	xcell.cg.time += model_dt;

}
