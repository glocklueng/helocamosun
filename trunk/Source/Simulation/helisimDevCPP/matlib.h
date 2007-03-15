/* -*- c -*- */
/* $Header: c:/work/repository/matlib/matlib.h,v 1.8 2002/02/18 21:38:48 Administrator Exp $*/

#ifndef matlib_h
#define matlib_h

/******************************************************
*
*	This file contains some useful matrix and general
*	math utilities.  The maximum size of the matrix which
*	can be used is set at 11x11.  
*
*	Author: Aaron Kahn, Suresh Kannan, Eric Johnson
*	copyright 2001
*
* @pkg			matlib
* @version		$Revision: 1.8 $
*
*******************************************************/

#define MAX( a, b )         ((a) > (b) ? (a) : (b))
#define MIN( a, b )         ((a) < (b) ? (a) : (b))
/* #define LIMIT( x, low, hi ) (MAX(MIN((x),(hi)),(low))) 
   that's a really sluggish and wasteful way to do things */
#define LIMIT( x, low, hi) (((x)<=(hi)) ? (((x)>=(low)) ? (x) : (low)) : (hi))
#define FABS( x )           ((x) < 0.0f ? -(x) : (x))
#define ABS( x )            ((x) < 0.0 ? -(x) : (x))
#define SQR( x )            ((x)*(x))
#define FTYPE_ABS FABS


/* conversions.h */

#define C_IN2M          0.0254          /* inches to meters */
#define C_FT2M          0.3048          /* (C_IN2M*12) feet to meters */
#define C_M2FT          3.280839895     /* (1.0/C_FT2M) */
#define C_YD2M          0.9144          /* (C_FT2M*3) yards to meters */

#define C_IN2M_F        0.0254f         /* inches to meters */
#define C_FT2M_F        0.3048f         /* (C_IN2M*12) feet to meters */
#define C_M2FT_F        3.280839895f    /* (1.0/C_FT2M) */
#define C_YD2M_F        0.9144f         /* (C_FT2M*3) yards to meters */


#define C_NMI2M         1852.0          /* nautical miles to meters */
#define C_MI2M          1609.344        /* (C_FT2M*5280) miles to meters */

#define C_NMI2M_F       1852.0f         /* nautical miles to meters */
#define C_MI2M_F        1609.344f       /* (C_FT2M*5280) miles to meters */

#define C_G0MPERSEC2    9.80665         /* g0 in m/s^2 */
#define C_G0            32.17404856     /* (C_G0MPERSEC2/C_FT2M) 
                                            standard gravity */
#define C_P0N_M2        1.01325e5       /* p0 in N/m^2 */
#define C_P0            14.692125       /* (C_P0N_M2*1.450e-4)
                                            standard atmospheric pressure */

#define C_G0MPERSEC2_F  9.80665f        /* g0 in m/s^2 */
#define C_G0_F          32.17404856f    /* (C_G0MPERSEC2/C_FT2M) 
                                            standard gravity */
#define C_P0N_M2_F      1.01325e5f      /* p0 in N/m^2 */
#define C_P0_F          14.692125f      /* (C_P0N_M2*1.450e-4)
                                            standard atmospheric pressure */

#define C_LBM2KG        0.45359237      /* lb mass */
#define C_KG2LBM        2.204622622     /* (1.0/C_LBM2KG) */
#define C_LBF2N         4.448221615     /* (C_LBM2KG*C_G0MPERSEC2) lb force */
#define C_SLUG2KG       14.59390294     /* (C_LBM2KG*C_G0) slugs */

#define C_LBM2KG_F      0.45359237f     /* lb mass */
#define C_KG2LBM_F      2.204622622f    /* (1.0/C_LBM2KG) */
#define C_LBF2N_F       4.448221615f    /* (C_LBM2KG*C_G0MPERSEC2) lb force */
#define C_SLUG2KG_F     14.59390294f    /* (C_LBM2KG*C_G0) slugs */


/* math constants */
/* from CRC Standard Mathematical Tables, 27th edition, 1984 */

#define C_PI    3.14159265358979323846264338327950288419716939937511
#define C_ROOT2 1.41421356237309504880168872420969807856967187537695
#define C_ROOT3 1.73205080756887729352744634150587236694280525381039
#define C_E 2.71828182845904523536028747135266249775724709369996

#define C_PI_F    3.14159265358979323846264338327950288419716939937511f
#define C_ROOT2_F 1.41421356237309504880168872420969807856967187537695f
#define C_ROOT3_F 1.73205080756887729352744634150587236694280525381039f
#define C_E_F 2.71828182845904523536028747135266249775724709369996f


#define C_PIOTWO         1.57079632679489661923
#define C_TWOPI          6.28318530717958647692

#define C_PIOTWO_F       1.57079632679489661923f
#define C_TWOPI_F        6.28318530717958647692f

#define C_DEG2RAD        0.017453292519943295
#define C_DEG2RAD_F      0.017453292519943295f
#define C_RAD2DEG       57.295779513082323
#define C_RAD2DEG_F     57.295779513082323f
                      
#define C_MM2M           0.001
#define C_MM2M_F         0.001f
#define C_M2MM        1000.0
#define C_M2MM_F      1000.0f

#define C_IN2M           0.0254
#define C_IN2M_F         0.0254f
#define C_M2IN          39.37007874
#define C_M2IN_F        39.37007874f

#define C_IN2MM         25.4
#define C_IN2MM_F       25.4f
#define C_MM2IN          0.03937007874
#define C_MM2IN_F        0.03937007874f

#define C_FPS2KT         0.5924838
#define C_FPS2KT_F       0.5924838f
#define C_KT2FPS         1.68780986
#define C_KT2FPS_F       1.68780986f

#define C_SQIN2SQFT      0.00694444444444444444444
#define C_SQIN2SQFT_F    0.00694444444444444444444f
#define C_SQFT2SQIN    144.0
#define C_SQFT2SQIN_F  144.0f

#define C_GPM2CFS        0.0022280093
#define C_GPM2CFS_F      0.0022280093f
#define C_CFS2GPM      448.83117
#define C_CFS2GPM_F    448.83117f

#define C_DEGF0_R        459.69
#define C_DEGC0_T        273.16
#define C_DEGC0_DEGF      32.0
#define C_DEGF_PER_DEGC    1.8


#define C_C2K           273.16
#define C_C2K_F         273.16f
#define C_F2R           459.69
#define C_F2R_F         459.69f

#define C_G_CONST       1.068944098e-09   /*6.6732e-11*CUBE(C_M2FT)/C_KG2LBM*/
#define C_EARTH_MASS    1.317041554e25    /*5.974e24*C_KG2LBM   */
#define C_N0_AVOGADRO   6.02205e23
#define C_R_IDEAL_SU    8.31434
#define C_K_BOLTZMANN   1.380622e-23
#define C_C_LIGHT       983571194.2       /*2.9979250e+8*C_M2FT*/
#define C_ECHARGE       1.6021917e-19

#define C_DOFA          0.080719353       /*1.293*C_KG2LBM/CUBE(C_M2FT)  */
#define C_DOFH2O        62.427960576      /*1.000e3*C_KG2LBM/CUBE(C_M2FT)*/
#define C_STOFH2O       75.6
#define C_VOFH2O        1.787e-3
#define C_SOUND0VEL     1087.598425       /* 331.5*C_M2FT */
#define C_SOUND20VEL    1126.64042        /* 343.4*C_M2FT */

#define C_G_CONST_F     1.068944098e-09f  /*6.6732e-11*CUBE(C_M2FT)/C_KG2LBM*/
#define C_EARTH_MASS_F  1.317041554e25f   /*5.974e24*C_KG2LBM   */
#define C_N0_AVOGADRO_F 6.02205e23f
#define C_R_IDEAL_SU_F  8.31434f
#define C_K_BOLTZMANN_F 1.380622e-23f
#define C_C_LIGHT_F     983571194.2f      /*2.9979250e+8*C_M2FT*/
#define C_ECHARGE_F     1.6021917e-19f

#define C_DOFA_F        0.080719353f      /*1.293*C_KG2LBM/CUBE(C_M2FT)  */
#define C_DOFH2O_F      62.427960576f     /*1.000e3*C_KG2LBM/CUBE(C_M2FT)*/
#define C_STOFH2O_F     75.6f
#define C_VOFH2O_F      1.787e-3f
#define C_SOUND0VEL_F   1087.598425f      /* 331.5*C_M2FT */
#define C_SOUND20VEL_F  1126.64042f       /* 343.4*C_M2FT */

#define C_WGS84_a		6378137.0		  /* WGS-84 semimajor axis (m) */
#define C_WGS84_a_F		6378137.0f
#define C_WGS84_b		6356752.3142	  /* WGS-84 semiminor axis (m) */
#define C_WGS84_b_F		6356752.3142f
#define C_WIE			7.2321151467e-05  /* WGS-84 earth rotation rate (rad/s) */
#define C_WIE_F			7.2321151467e-05f

/*
 * macros for temperature conversions
 */
#define DEGF_TO_DEGR( f ) ((f) + C_DEGF0_R)
#define DEGR_TO_DEGF( r ) ((r) - C_DEGF0_R)
#define DEGC_TO_DEGK( c ) ((c) + C_DEGC0_T)
#define DEGK_TO_DEGC( k ) ((k) - C_DEGC0_T)
#define DEGC_TO_DEGF( c ) ((c)*C_DEGF_PER_DEGC + C_DEGC0_DEGF)
#define DEGF_TO_DEGC( f ) (((f) - C_DEGC0_DEGF)/C_DEGF_PER_DEGC)


/* inversionf.h, inversion_code.h */

#define MAXSIZE  11
#define MAXSIZE2 121 /* this is MAXSIZE*MAXSIZE */
#define MAXITS   30
#define TINY     1.0e-20f

#define fn(name) name##_f
#define struc(name) name##_tf
#define FTYPE  float
#define FZERO  0.0f
#define F0p2   0.2f
#define F0p5   0.5f
#define F0p8   0.8f
#define FONE   1.0f
#define FTWO   2.0f
#define FTHREE 3.0f
#define FFOUR  4.0f
#define FFIVE  5.0f
#define FSIX   6.0f
#define FSEVEN 7.0f
#define FEIGHT 8.0f
#define FNINE  9.0f

/********************************************************************/
/************                                   *********************/
/************ MATRIX MATH FUNCTION DECLERATIONS *********************/
/************                                   *********************/
/********************************************************************/

/* This will zero out the matrix A
zeros(n,m) = A
*/
void Minit(double A[MAXSIZE][MAXSIZE], int n, int m);

/* This will zero out the vector a
zeros(n,1) = a
*/
void Vinit(double a[MAXSIZE], int n);

/* This will generate an identity matrix I
eye(n) = A(n,n)
*/
void eye(double I[MAXSIZE][MAXSIZE], int n);

/* This will multiply scalar value s will all elements of matrix A(m,n)
* placing result into matrix B(m,n)
* s.*A(m,n) = B(m,n)
*/
void sMmult(double s, double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], int m, int n);

/* This will multiply matrix A and matrix B return in matrix C
A(m,n)*B(n,p) = C(m,p)
*/
void MMmult(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], double C[MAXSIZE][MAXSIZE], int m, int n, int p);

/* This will multiply matrix A and vector b return in vector b
A(m,n)*b(n,1) = c(m,1)
*/
void MVmult(double A[MAXSIZE][MAXSIZE], double b[MAXSIZE], double c[MAXSIZE], int m, int n);

/* This will multiply vector a and matrix B return in vector c
a(1,m)*B(m,n) = c(1,n)
*/
void VMmult(double a[MAXSIZE], double B[MAXSIZE][MAXSIZE], double c[MAXSIZE], int m, int n);

/* This will traspose a matrix/vector A return in matrix/vector B
A(m,n) = B(n,m)
*/
void transpose(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], int m, int n);

/* This will add matrix A and matrix B return in matrix C
A(n,m) + B(n,m) = C(n,m)
*/
void MMadd(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], double C[MAXSIZE][MAXSIZE], int m, int n);

/* This will subtract matrix A from matrix B placing result in matrix C
* A(n,m) - B(n,m) = C(n,m)
*/
void MMsub(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], double C[MAXSIZE][MAXSIZE], int m, int n);

/* This will multiply scalar s will all the elements of vector a(n)
* placing the result in vector b(n)
* s.*a(n) = b(n)
*/
void sVmult(double s, double a[MAXSIZE], double b[MAXSIZE], int n);

/* This will add vector a and vector b return vector c
* a(n,1) + b(n,1) = c(n,1)
*/
void VVadd(double a[MAXSIZE], double b[MAXSIZE], double c[MAXSIZE], int n);

/* This will subtract vector a(n) from vector b(n) placing result in vector c(n)
* a(n,1) - b(n,1) = c(n,1)
*/
void VVsub(double a[MAXSIZE], double b[MAXSIZE], double c[MAXSIZE], int n);

/* This will perform LU decomp on matrix A return matrix L, matrix U
lu(A(n,n)) => L(n,n) and U(n,n)
*/
void LU(double A[MAXSIZE][MAXSIZE], double L[MAXSIZE][MAXSIZE], double U[MAXSIZE][MAXSIZE], int n);

/* This will perform the inverse on matrix A return in matrix B
inv(A(n,n)) = B(n,n)
*/
void inv(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], int n);

/* This will solve A*x = b, where matrix A is upper triangular
A(n,n)*x(n,1) = b(n,1)
*/
void solveupper(double A[MAXSIZE][MAXSIZE], double b[MAXSIZE], double x[MAXSIZE], int n);

/* This will solve A*x = b, where matrix A is lower triangular
A(n,n)*x(n,1) = b(n,1)
*/
void solvelower(double A[MAXSIZE][MAXSIZE], double b[MAXSIZE], double x[MAXSIZE], int n);

/* This will take column n from matrix A place in vector a
A(:,n) = a(m,1)
m = number of rows
*/
void Mcol(double A[MAXSIZE][MAXSIZE], double a[MAXSIZE], int n, int m);

/* This will take vector a and place into column of matrix A
a(:,1) = A(m,n)
m = number of rows
*/
void Vcol(double a[MAXSIZE], double A[MAXSIZE][MAXSIZE], int n, int m);

/* This will print matrix A to the screen
A(n,m)
*/
void Mprint(double A[MAXSIZE][MAXSIZE], int n, int m);

/* This will print vector a to the screen
a(n,1)
*/
void Vprint(double a[MAXSIZE], int n);

/* This will construct a direction cosine matrix from 
* euler angles in the standard rotation sequence 
* [phi][theta][psi]

  from NED to body frame

  body = tBL(3,3)*NED
*/
void eulerDC(double tBL[MAXSIZE][MAXSIZE], double phi, double theta, double psi);

/* This will construct a direction cosine matrix from 
* quaternions in the standard rotation  sequence
* [phi][theta][psi]

  from NED to body frame

  body = tBL(3,3)*NED
	q(4,1)
*/
void quatDC(double tBL[MAXSIZE][MAXSIZE], double q[MAXSIZE]);

/* This will construct the euler omega-cross matrix
wx(3,3)
p, q, r (rad/sec)
*/
void eulerWx(double Wx[MAXSIZE][MAXSIZE], double p, double q, double r);

/* This will construct the quaternion omega matrix
W(4,4)
p, q, r (rad/sec)
*/
void quatW(double W[MAXSIZE][MAXSIZE], double p, double q, double r);

/* This will take matrix A and insert it as a block into matrix B. A(0,0) will be
placed in B(r,c)
A(n,m) -> B(>n, >m)(r,c)
r=0, c=0 == B(0,0)
*/
void block2M(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], int n, int m, int r, int c);

/* This will take the dot product of two vectors a and b, and return the result.
	   T
 a(n,1) b(n,1) = c(1,1)
*/
double dot(double a[MAXSIZE], double b[MAXSIZE], int n);

/* This will take the cross product of two vectors a,b and return in vector c
a(3,1) X b(3,1) = c(3,1)
*/
void cross(double a[MAXSIZE], double b[MAXSIZE], double c[MAXSIZE]);

/* This will compute the norm of a vector v(n,1)
norm(v(n,1)) = r
*/
double norm(double v[MAXSIZE], int n);

/* This will compute an orthoginal set of vectors from a given set
* of vectors.  The seed set of column vectors are arranged as columns
* of a matrix with full column rank.  The output vectors are arranged
* as columns of the output matrix with full column rank.

  A(n,[V1 V2 ... Vm]) --> Q(n,[V1 V2 ... Vm])
*/
void mgs(double A[MAXSIZE][MAXSIZE], double Q[MAXSIZE][MAXSIZE], int n, int m);


/* This will normalize a quaternion vector q
q/norm(q)
q(4,1)
*/
void normq(double q[MAXSIZE]);

/* This will convert from ECEF to local tangent plane coordinates
* local(x,y,z) = Re2t*[X;Y;Z]

latitude and longitude are in radians
*/
void ECEF2Tangent(double Local[MAXSIZE], double ECEF[MAXSIZE], double latitude, double longitude);

/* This will convert from local tangent plane coordinates to ECEF
* ECEF[X,Y,Z] = Rt2e*local(x,y,z)
*
* latitude and longitude are in radians
*/
void Tangent2ECEF(double ECEF[MAXSIZE], double Local[MAXSIZE], double latitude, double longitude);

/* This will convert from ECEF to geodedic latitude, longitude, altitude
* based on the WGS-84 elipsoid.  Note: altitude is + up
*
*	latitude and longitude (rad), altitude (m)
*	ECEF (m)
*	ECEF = [X Y Z] (m)
*	llh = [latitude longitude altitude] (rad)(m)
*/
void ECEF2llh(double ECEF[MAXSIZE], double llh[MAXSIZE]);

/*	This will convert from latitude, longitude, and altitude into
* ECEF coordinates.  Note: altitude in + up
*
*	ECEF = [X Y Z] (m)
*	llh = [latitude longitude altitude] (rad)(m)
*/
void llh2ECEF(double llh[MAXSIZE], double ECEF[MAXSIZE]);

/* This will convert from quaternions to euler angles
q(4,1) -> euler[phi;theta;psi] (rad)
*/
void quat2euler(double q[MAXSIZE], double euler[MAXSIZE]);

/* This will convert from euler angles to quaternion vector
phi, theta, psi -> q(4,1)
euler angles in radians
*/
void euler2quat(double q[MAXSIZE], double phi, double theta, double psi);


/* This will perform a Kalman filter Gain, state, and coveriance matrix update.
* What is needed is the linear A matrix, state vector, C matrix, P coveriance matrix,
* measurement vector, and the R matrix.
*
*	A(n,n)		Linear model
*	P(n,n)		Coveriance matrix
*	X(n,1)		State Vector
*	C(m,n)		Measurement matrix; m=# of measurements, n=# of states
*	R(m,n)		Measurement weight matrix 
*	err(m,1)	err = Xmeasurement(m,1) - Xestimate(m,1) vector
*	K(n,m)		Kalman Gain matrix
*/
void kalmanUpdate(double A[MAXSIZE][MAXSIZE], double P[MAXSIZE][MAXSIZE], \
				  double X[MAXSIZE], double C[MAXSIZE][MAXSIZE], double R[MAXSIZE][MAXSIZE], \
				  double err[MAXSIZE], double K[MAXSIZE][MAXSIZE], \
				  int n, int m);

/*	This will compute the current atmospheric properties given the current altitude in 
* feet above mean sea level (MSL).  The output parameters are output with pointers
* with all of the useful information.
*
*	altitude	MSL altitude (ft)
*	density		density (slug/ft^3)
*	pressure	pressure (lb/ft^2)
*	temp		temperature (degrees R)
*	sp_sound	local speed of sound (ft/s)
*/
void atmosphere(double altitude, double *density, double *pressure, double *temp, double *sp_sound);

/*	This will compute the dependant value from an independant value based on a 
* linear function.  The linear function is defined by a set of independant and
* dependant points (x and y in cartisian plane).
*
*	RETURNS		dependant value (y on cartisian plane)
*	x			independant value (x on cartisian plane)
*	Y1			dependant component of point 1 (y on cartisian plane)
*	Y2			dependant component of point 2 (y on cartisian plane)
*	X1			independant component of point 1 (x on cartisian plane)
*	X2			independant component of point 2 (x on cartisian plane)
*/
double line(double x, double Y1, double Y2, double X1, double X2);

/*	This will produce a hysterious effect on the given value if given the
* previous value and the amount of hysterious play.  The use of this
* function is to simulate slop in linkage and gear chains.
*
*	RETURNS		new value with hysterious
*	old_val		previous value
*	current_val	current value to modify
*	play		the magnitude of play to allow (ie: +/- play)
*/
double hyst(double current_val, double old_val, double play);

#endif