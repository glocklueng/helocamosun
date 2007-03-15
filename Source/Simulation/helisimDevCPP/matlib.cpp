/* -*- c -*- */
/* $Header: c:/work/repository/matlib/matlib.cpp,v 1.8 2002/02/18 21:38:48 Administrator Exp $*/

/***********************************************************8
*
*	This is the source file for the matrix and math utiltiy
*	library.
*
*	Author: Aaron Kahn, Suresh Kannan, Eric Johnson
*	copyright 2001
*
* @pkg		matlib
* @version	$Revision: 1.8 $
*
*************************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "matlib.h"

/* This will zero out the vector a
zeros(n,1) = a
*/
void Vinit(double a[MAXSIZE], int n)
{
	int i;
	for(i=0; i<n; ++i)
	{
		a[i] = 0.0;
	}
}


/* This will zero out the matrix A
zeros(n,m) = A
*/
void Minit(double A[MAXSIZE][MAXSIZE], int n, int m)
{
	int i,j;
	for(i=0; i<n; ++i)
	{
		for(j=0; j<m; ++j)
		{
			A[i][j] = 0.0;
		}
	}
}


/* This will generate an identity matrix I
eye(n) = A(n,n)
*/
void eye(double I[MAXSIZE][MAXSIZE], int n)
{
	int i;
	Minit(I,n,n);
	for(i=0; i<n; ++i)
	{
		I[i][i] = 1.0;
	}
}

/* This will multiply scalar value s will all elements of matrix A(m,n)
* placing result into matrix B(m,n)
* s.*A(m,n) = B(m,n)
*/
void sMmult(double s, double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], int m, int n)
{
	int i, j;

	for(i=0; i<m; ++i)
	{
		for(j=0; j<n; ++j)
		{
			B[i][j] = s*A[i][j];
		}
	}
}


/* This will multiply matrix A and matrix B return in matrix C
A(m,n)*B(n,p) = C(m,p)
*/
void MMmult(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], double C[MAXSIZE][MAXSIZE], int m, int n, int p)
{
	int i,j,k;
	for(i=0; i<m; ++i)
	{
		for(j=0; j<p; ++j)
		{
			C[i][j] = 0.0;
			for(k =0; k<n; ++k)
			{
				C[i][j]=A[i][k]*B[k][j]+C[i][j];
			}
		}
	}
}

/* This will multiply matrix A and vector b return in vector b
A(m,n)*b(n,1) = c(m,1)
*/
void MVmult(double A[MAXSIZE][MAXSIZE], double b[MAXSIZE], double c[MAXSIZE], int m, int n)
{
	int i,j;
	for(j=0; j<m; ++j)
	{
		c[j]=0.0;
		for(i=0; i<n; ++i)
		{
			c[j] = A[j][i]*b[i]+c[j];
		}
	}
}

/* This will multiply vector a and matrix B return in vector c
a(1,m)*B(m,n) = c(1,n)
*/
void VMmult(double a[MAXSIZE], double B[MAXSIZE][MAXSIZE], double c[MAXSIZE], int m, int n)
{
	int i,j;
	for(i=0; i<n; ++n)
	{
		c[i] = 0.0;
		for(j=0; j<m; ++m)
		{
			c[i] = a[j]*B[j][i] + c[i];
		}
	}
}

/* This will traspose a matrix/vector A return in matrix/vector B
A(m,n) = B(n,m)
*/
void transpose(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], int m, int n)
{
	int i,j;

	for(i=0; i<m; ++i)
	{
		for(j=0; j<n; ++j)
		{
			B[j][i] = A[i][j];
		}
	}
}

/* This will add matrix A and matrix B return in matrix C
A(n,m) + B(n,m) = C(n,m)
*/
void MMadd(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], double C[MAXSIZE][MAXSIZE], int m, int n)
{
	int i,j;
	for(i=0; i<n; ++i)
	{
		for(j=0; j<m; ++j)
		{
			C[i][j] = A[i][j] + B[i][j];
		}
	}
}

/* This will subtract matrix A from matrix B placing result in matrix C
* A(n,m) - B(n,m) = C(n,m)
*/
void MMsub(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], double C[MAXSIZE][MAXSIZE], int m, int n)
{
	int i,j;
	for(i=0; i<n; ++i)
	{
		for(j=0; j<m; ++j)
		{
			C[i][j] = A[i][j] - B[i][j];
		}
	}
}


/* This will multiply scalar s will all the elements of vector a(n)
* placing the result in vector b(n)
* s.*a(n) = b(n)
*/
void sVmult(double s, double a[MAXSIZE], double b[MAXSIZE], int n)
{
	int i;

	for(i=0; i<n; ++i)
	{
		b[i] = s*a[i];
	}
}


/* This will add vector a and vector b return vector c
a(n,1) + b(n,1) = c(n,1)
*/
void VVadd(double a[MAXSIZE], double b[MAXSIZE], double c[MAXSIZE], int n)
{
	int i;
	for(i=0; i<n; ++i)
	{
		c[i] = a[i] + b[i];
	}
}

/* This will subtract vector a(n) from vector b(n) placing result in vector c(n)
* a(n,1) - b(n,1) = c(n,1)
*/
void VVsub(double a[MAXSIZE], double b[MAXSIZE], double c[MAXSIZE], int n)
{
	int i;
	for(i=0; i<n; ++i)
	{
		c[i] = a[i] - b[i];
	}
}

/* This will perform LU decomp on matrix A return matrix L, matrix U
lu(A(n,n)) => L(n,n) and U(n,n)
*/
void LU(double A[MAXSIZE][MAXSIZE], double L[MAXSIZE][MAXSIZE], double U[MAXSIZE][MAXSIZE], int n)
{
	int k,i,j;
	double Acopy[MAXSIZE][MAXSIZE];

	/* copy A matrix */
	for(i=0; i<n; ++i)
	{
		for(j=0; j<n; ++j)
		{
			Acopy[i][j] = A[i][j];
		}
	}


/*
% the LU decomp algorithm for no pivoting
for k=1:n-1
   for i=k+1:n
      A(i,k)=A(i,k)/A(k,k);
      for j=k+1:n
         A(i,j)=A(i,j)-A(i,k)*A(k,j);
      end
   end
end
*/
	for(k=0; k<n-1; ++k)
	{
		for(i=k+1; i<n; ++i)
		{
			Acopy[i][k] = Acopy[i][k]/Acopy[k][k];
			for(j=k+1; j<n; ++j)
			{
				Acopy[i][j] = Acopy[i][j] - Acopy[i][k]*Acopy[k][j];
			}
		}
	}

	
	/* make an identity matrix */
	eye(L,n);
	
	/* separate the L matrix */
	for(j=0; j<n-1; ++j)
	{
		for(i=j+1; i<n; ++i)
		{
			L[i][j] = Acopy[i][j];
		}
	}

	/* separate out the U matrix */
	Minit(U,n,n);
	for(i=0; i<n; ++i)
	{
		for(j=i; j<n; ++j)
		{
			U[i][j] = Acopy[i][j];
		}
	}
}

/* This will perform the inverse on matrix A return in matrix B
inv(A(n,n)) = B(n,n)
*/
void inv(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], int n)
{
	double identCol[MAXSIZE];
	double ident[MAXSIZE][MAXSIZE];
	double L[MAXSIZE][MAXSIZE];
	double U[MAXSIZE][MAXSIZE];
	double invUcol[MAXSIZE];
	double invLcol[MAXSIZE];
	double invU[MAXSIZE][MAXSIZE];
	double invL[MAXSIZE][MAXSIZE];
	double detA;

	int i;
	
	if( n == 1)
		B[0][0] = 1.0/A[0][0];

	else if( n == 2 )
	{
		detA = A[0][0]*A[1][1] - A[0][1]*A[1][0];
		B[0][0] = A[1][1]/detA;
		B[0][1] = -A[0][1]/detA;
		B[1][0] = -A[1][0]/detA;
		B[1][1] = A[0][0]/detA;
	}
	else
	{
		/* construct an identity matrix */
		eye(ident,n);
	
		/* perform LU decomp on A */
		LU(A,L,U,n);

		for(i=0; i<n; ++i)
		{
			Mcol(ident,identCol,i,n);	// separates the ith column
			solveupper(U,identCol,invUcol,n);
			solvelower(L,identCol,invLcol,n);
			Vcol(invUcol,invU,i,n);		// places invUcol in ith column of invU
			Vcol(invLcol,invL,i,n);		// places invLcol in ith column of invL
		}

		/* inv(A) = inv(U)*inv(L) */
		MMmult(invU,invL,B,n,n,n);
	}
}

	



/* This will solve A*x = b, where matrix A is upper triangular
A(n,n)*x(n,1) = b(n,1)
*/
void solveupper(double A[MAXSIZE][MAXSIZE], double b[MAXSIZE], double x[MAXSIZE], int n)
{
	int p,i,j;
	
	p=n+1;

	for(i=1; i<=n; ++i)
	{
		x[p-i-1] = b[p-i-1];
		for(j=(p+1-i); j<=n; ++j)
		{
			x[p-i-1] = x[p-i-1] - A[p-i-1][j-1]*x[j-1];
		}
	x[p-i-1] = x[p-i-1]/A[p-i-1][p-i-1];
	}
}


/* This will solve A*x = b, where matrix A is lower triangular
A(n,n)*x(n,1) = b(n,1)
*/
void solvelower(double A[MAXSIZE][MAXSIZE], double b[MAXSIZE], double x[MAXSIZE], int n)
{
	int i,j;

	for(i=1; i<=n; ++i)
	{
		x[i-1] = b[i-1];
		for(j=1; j<=i-1; ++j)
		{
			x[i-1] = x[i-1] - A[i-1][j-1]*x[j-1];
		}
		x[i-1] = x[i-1]/A[i-1][i-1];
	}
	
}

/* This will take column n from matrix A place in vector a
A(:,n) = a(m,1)
m = number of rows
*/
void Mcol(double A[MAXSIZE][MAXSIZE], double a[MAXSIZE], int n, int m)
{
	int i;
	for(i=0; i<m; ++i)
	{
		a[i] = A[i][n];
	}
}


/* This will take vector a and place into column of matrix A
a(:,1) = A(m,n)
m = number of rows
*/
void Vcol(double a[MAXSIZE], double A[MAXSIZE][MAXSIZE], int n, int m)
{
	int i;
	for(i=0; i<m; ++i)
	{
		A[i][n] = a[i];
	}
}

/* This will print matrix A to the screen
A(n,m)
*/
void Mprint(double A[MAXSIZE][MAXSIZE], int n, int m)
{
	int i,j;
	for(i=0; i<n; ++i)
	{
		for(j=0; j<m; ++j)
		{
			printf("%7.5lf ",A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

/* This will print vector a to the screen
a(n,1)
*/
void Vprint(double a[MAXSIZE], int n)
{
	int i;
	for(i=0; i<n; ++i)
	{
		printf("%7.5lf\n",a[i]);
	}
	printf("\n");
}

/* This will construct a direction cosine matrix from 
* euler angles in the standard rotation sequence 
* [phi][theta][psi]

  body = tBL(3,3)*NED
  angles in rad
*/
void eulerDC(double tBL[MAXSIZE][MAXSIZE], double phi, double theta, double psi)
{
	double cpsi, cphi, ctheta;
	double spsi, sphi, stheta;

	cpsi = cos(psi);
	cphi = cos(phi);
	ctheta = cos(theta);
	spsi = sin(psi);
	sphi = sin(phi);
	stheta = sin(theta);

	tBL[0][0] = cpsi*ctheta;
	tBL[0][1] = spsi*ctheta;
	tBL[0][2] = -stheta;

	tBL[1][0] = -spsi*cphi+cpsi*stheta*sphi;
	tBL[1][1] = cpsi*cphi+sphi*stheta*sphi;
	tBL[1][2] = ctheta*sphi;

	tBL[2][0] = spsi*sphi+cpsi*stheta*cphi;
	tBL[2][1] = -cpsi*sphi+spsi*stheta*cphi;
	tBL[2][2] = ctheta*cphi;
}

/* This will construct a direction cosine matrix from 
* quaternions in the standard rotation  sequence
* [phi][theta][psi]

  body = tBL(3,3)*NED
	q(4,1)
*/
void quatDC(double tBL[MAXSIZE][MAXSIZE], double q[MAXSIZE])
{
	double q0, q1, q2, q3;
	double q02, q12, q22, q32;

	q0 = q[0];
	q1 = q[1];
	q2 = q[2];
	q3 = q[3];

	q02 = q0*q0;
	q12 = q1*q1;
	q22 = q2*q2;
	q32 = q3*q3;

	tBL[0][0] = 1-2*(q22 + q32);
	tBL[0][1] = 2*(q1*q2 + q0*q3);
	tBL[0][2] = 2*(q1*q3 - q0*q2);

	tBL[1][0] = 2*(q1*q2 - q0*q3);
	tBL[1][1] = 1-2*(q12 + q32);
	tBL[1][2] = 2*(q2*q3 + q0*q1);

	tBL[2][0] = 2*(q1*q3 + q0*q2);
	tBL[2][1] = 2*(q2*q3 - q0*q1);
	tBL[2][2] = 1-2*(q12 + q22);

}

/* This will construct the euler omega-cross matrix
wx(3,3)
p, q, r (rad/sec)
*/
void eulerWx(double Wx[MAXSIZE][MAXSIZE], double p, double q, double r)
{
	Wx[0][0] = 0.0;
	Wx[0][1] = -r;
	Wx[0][2] = q;

	Wx[1][0] = r;
	Wx[1][1] = 0.0;
	Wx[1][2] = -p;

	Wx[2][0] = -q;
	Wx[2][1] = p;
	Wx[2][2] = 0.0;
}

/* This will construct the quaternion omega matrix
W(4,4)
p, q, r (rad/sec)
*/
void quatW(double W[MAXSIZE][MAXSIZE], double p, double q, double r)
{
	W[0][0] = 0.0;
	W[0][1] = -p/2.0;
	W[0][2] = -q/2.0;
	W[0][3] = -r/2.0;

	W[1][0] = p/2.0;
	W[1][1] = 0.0;
	W[1][2] = r/2.0;
	W[1][3] = -q/2.0;

	W[2][0] = q/2.0;
	W[2][1] = -r/2.0;
	W[2][2] = 0.0;
	W[2][3] = p/2.0;

	W[3][0] = r/2.0;
	W[3][1] = q/2.0;
	W[3][2] = -p/2.0;
	W[3][3] = 0.0;
}

/* This will take matrix A and insert it as a block into matrix B. A(0,0) will be
placed in B(r,c)
A(n,m) -> B(>n, >m)(r,c)
r=0, c=0 == B(0,0)
*/
void block2M(double A[MAXSIZE][MAXSIZE], double B[MAXSIZE][MAXSIZE], int n, int m, int r, int c)
{
	int i,j;

	for(i=0; i<n; ++i)
	{
		for(j=0; j<n; ++j)
		{
			B[i+r][j+c] = A[i][j];
		}
	}
}

/* This will take the dot product of two vectors a and b, and return the result.
	   T
 a(n,1) b(n,1) = c(1,1)
*/
double dot(double a[MAXSIZE], double b[MAXSIZE], int n)
{
	int i;
	double c = 0.0;

	for(i=0; i<n; ++i)
	{
		c = c + a[i]*b[i];
	}
	return(c);
}

/* This will take the cross product of two vectors a,b and return in vector c
a(3,1) X b(3,1) = c(3,1)
*/
void cross(double a[MAXSIZE], double b[MAXSIZE], double c[MAXSIZE])
{

	c[0] = a[1]*b[2] - a[2]*b[1];
	c[1] = a[2]*b[0] - a[0]*b[2];
	c[2] = a[0]*b[1] - a[1]*b[0];
}

/* This will compute the norm of a vector v(n,1)
norm(v(n,1)) = r
*/
double norm(double v[MAXSIZE], int n)
{
	int i;
	double r;
	r = 0.0;
	for(i=0; i<n; ++i)
	{
		r = r + v[i]*v[i];
	}
	return(sqrt(r));
}

/* This will compute an orthoginal set of vectors from a given set
* of vectors.  The seed set of column vectors are arranged as columns
* of a matrix with full column rank.  The output vectors are arranged
* as columns of the output matrix with full column rank.

  A(n,[V1 V2 ... Vm]) --> Q(n,[V1 V2 ... Vm])
*/
void mgs(double A[MAXSIZE][MAXSIZE], double Q[MAXSIZE][MAXSIZE], int n, int m)
{
	int i,j,k;
	double r;
	double Tv1[MAXSIZE];
	double Tv2[MAXSIZE];

	/* copy A matrix */
	for(i=0; i<n; ++i)
	{
		for(j=0; j<m; ++j)
		{
			Q[i][j] = A[i][j];
		}
	}

	for(i=0; i<m; ++i)
	{
		Mcol(Q, Tv1, i, n);
		r = norm(Tv1, n);
		for(k=0; k<n; ++k)
		{
			Q[k][i] = Q[k][i]/r;
		}
		for(j=i+1; j<n; ++j)
		{
			Mcol(Q, Tv1, i, n);
			Mcol(Q, Tv2, j, n);
			r = dot(Tv1, Tv2, n);
			for(k=0; k<n; ++k)
			{
				Q[k][j] = Q[k][j] - r*Q[k][i];
			}
		}
	}
}


/* This will normalize a quaternion vector q
q/norm(q)
q(4,1)
*/
void normq(double q[MAXSIZE])
{
	int i;
	double invsqr;
	double q0, q1, q2, q3;
	double q02, q12, q22, q32;

	q0 = q[0];
	q1 = q[1];
	q2 = q[2];
	q3 = q[3];

	q02 = q0*q0;
	q12 = q1*q1;
	q22 = q2*q2;
	q32 = q3*q3;

	invsqr = 1.0/sqrt(q02 + q12 + q22 + q32);

	for(i=0; i<4; ++i)
	{
		q[i] = q[i]*invsqr;
	}
}

/* This will convert from ECEF to local tangent plane coordinates
* local(x,y,z) = Re2t*[X;Y;Z]

latitude and longitude are in radians
*/
void ECEF2Tangent(double Local[MAXSIZE], double ECEF[MAXSIZE], double latitude, double longitude)
{
	double Re2t[MAXSIZE][MAXSIZE];
	double clat, clon;
	double slat, slon;

	clat = cos(latitude);
	clon = cos(longitude);
	slat = sin(latitude);
	slon = sin(longitude);

	Re2t[0][0] = -slat*clon;
	Re2t[0][1] = -slat*slon;
	Re2t[0][2] = clat;

	Re2t[1][0] = -slon;
	Re2t[1][1] = clon;
	Re2t[1][2] = 0.0;

	Re2t[2][0] = -clat*clon;
	Re2t[2][1] = -clat*slon;
	Re2t[2][2] = -slat;

	MVmult(Re2t,ECEF,Local,3,3);
}

/* This will convert from local tangent plane coordinates to ECEF
* ECEF[X,Y,Z] = Rt2e*local(x,y,z)
*
* latitude and longitude are in radians
*/
void Tangent2ECEF(double ECEF[MAXSIZE], double Local[MAXSIZE], double latitude, double longitude)
{
	double Rt2e[MAXSIZE][MAXSIZE];
	double clat, clon;
	double slat, slon;

	clat = cos(latitude);
	clon = cos(longitude);
	slat = sin(latitude);
	slon = sin(longitude);

	Rt2e[0][0] = -slat*clon;
	Rt2e[1][0] = -slat*slon;
	Rt2e[2][0] = clat;

	Rt2e[0][1] = -slon;
	Rt2e[1][1] = clon;
	Rt2e[2][1] = 0.0;

	Rt2e[0][2] = -clat*clon;
	Rt2e[1][2] = -clat*slon;
	Rt2e[2][2] = -slat;

	MVmult(Rt2e, Local, ECEF, 3, 3);
}


/* This will convert from ECEF to geodedic latitude, longitude, altitude
* based on the WGS-84 elipsoid.
*
*	latitude and longitude (rad), altitude (m)
*	ECEF (m)
*	ECEF = [X Y Z] (m)
*	llh = [latitude longitude altitude] (rad)(m)
*/
void ECEF2llh(double ECEF[MAXSIZE], double llh[MAXSIZE])
{
	double X,Y,Z;
	double sin_lat;
	double f, e;
	double N, h, h_old;
	double hN;
	int n;
	
	X = ECEF[0];
	Y = ECEF[1];
	Z = ECEF[2];

	llh[1] = atan2(Y, X);	// longitude


	f = (C_WGS84_a-C_WGS84_b)/C_WGS84_a;
	e = sqrt(2*f - SQR(f));

	h = 0;
	N = C_WGS84_a;
	h_old = 100;
	for(n=0; n<50; ++n)
	{
		sin_lat = Z/(N*(1 - SQR(e)) + h);
		llh[0] = atan( (Z + SQR(e)*N*sin_lat)/(sqrt(SQR(X) + SQR(Y))) );
		N = C_WGS84_a/sqrt( 1 - SQR(e)*SQR(sin(llh[0])) );
		hN = sqrt(SQR(X) + SQR(Y))/cos(llh[0]);
		h = (hN - N);
	}
	llh[2] = h;

}

/*	This will convert from latitude, longitude, and altitude into
* ECEF coordinates.  Note: altitude in + up
*
*	ECEF = [X Y Z] (m)
*	llh = [latitude longitude altitude] (rad)(m)
*/
void llh2ECEF(double llh[MAXSIZE], double ECEF[MAXSIZE])
{
	double N;
	double e;
	double f;

	f = (C_WGS84_a - C_WGS84_b)/C_WGS84_a;
	e = sqrt( 2*f - SQR(f) );
	N = C_WGS84_a/sqrt( 1 - SQR(e)*SQR(sin(llh[0])));

	ECEF[0] = (N + llh[2])*cos(llh[0])*cos(llh[1]);
	ECEF[1] = (N + llh[2])*cos(llh[0])*sin(llh[1]);
	ECEF[2] = (N*(1-SQR(e)) + llh[2])*sin(llh[0]);
}


/* This will convert from quaternions to euler angles
q(4,1) -> euler[phi;theta;psi] (rad)
*/
void quat2euler(double q[MAXSIZE], double euler[MAXSIZE])
{
	double phi,theta,psi;
	double q0, q1, q2, q3;
	double q02, q12, q22, q32;

	q0 = q[0];
	q1 = q[1];
	q2 = q[2];
	q3 = q[3];

	q02 = q0*q0;
	q12 = q1*q1;
	q22 = q2*q2;
	q32 = q3*q3;

	theta = -asin(2*(q1*q3 - q0*q2));
    phi = atan2( 2*(q2*q3 + q0*q1),(1-2*(q12 + q22)) );
    psi = atan2( 2*(q1*q2 + q0*q3),(1-2*(q22 + q32)) );
 
	euler[0] = phi;
	euler[1] = theta;
	euler[2] = psi;
}
	
/* This will convert from euler angles to quaternion vector
phi, theta, psi -> q(4,1)
euler angles in radians
*/
void euler2quat(double q[MAXSIZE], double phi, double theta, double psi)
{
	double shphi0, shtheta0, shpsi0;
	double chphi0, chtheta0, chpsi0;

	shphi0   = sin( 0.5*phi );
    chphi0   = cos( 0.5*phi );
    shtheta0 = sin( 0.5*theta );
    chtheta0 = cos( 0.5*theta );
    shpsi0   = sin( 0.5*psi );
    chpsi0   = cos( 0.5*psi );
    q[0] =  chphi0*chtheta0*chpsi0 + shphi0*shtheta0*shpsi0;
    q[1] = -chphi0*shtheta0*shpsi0 + shphi0*chtheta0*chpsi0;
    q[2] =  chphi0*shtheta0*chpsi0 + shphi0*chtheta0*shpsi0;
    q[3] =  chphi0*chtheta0*shpsi0 - shphi0*shtheta0*chpsi0;

}

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
				  int n, int m)
{
	int i, j;
	double E[MAXSIZE][MAXSIZE];
	double T1[MAXSIZE][MAXSIZE];	// temp matrix 1
	double T2[MAXSIZE][MAXSIZE];	// temp matrix 2
	double TV1[MAXSIZE];			// temp vector 1
	
	// perform E = C*P*C' + R 
	MMmult(C,P,T1,m,n,n);
	transpose(C,T2,m,n);
	MMmult(T1,T2,E,m,n,m);
	MMadd(E,R,E,m,m);
	
	// perform K = P*C'*inv(E) 
	transpose(C,T2,m,n);
	MMmult(P,T2,T1,n,n,m);
	inv(E,T2,m);
	MMmult(T1,T2,K,n,m,m);
	
	// perform x = x + K*( ys - yp )
	MVmult(K,err,TV1,n,m);
	VVadd(X,TV1,X,n);
	
	
	// perform P = P - K*C*P 
	MMmult(K,C,T1,n,m,n);
	MMmult(T1,P,T2,n,n,n);
	for(i=0; i<n; ++i)
	{
		for(j=0; j<n; ++j)
		{
			P[i][j] = P[i][j] - T2[i][j];
		}
	}
	
}

/*	This will compute the current atmospheric properties given the current altitude in 
* feet above mean sea level (MSL).  The output parameters are output with pointers
* with all of the useful information.
*
*	altitude	MSL altitude (ft) (NOTE: Only valid to 36,152 ft)
*	density		density (slug/ft^3)
*	pressure	pressure (lb/ft^2)
*	temperature	temperature (degrees R)
*	sp_sound	local speed of sound (ft/s)
*/
void atmosphere(double altitude, double *density, double *pressure, double *temperature, double *sp_sound)
{
	double temp, temp1, temp2;

	// compute density first
	temp = (1 - (0.68753 - 0.003264*altitude*1e-5)*altitude*1e-5);
	*density = pow( temp, 4.256)*0.0023769;

	// compute pressure
	temp = (1-(0.68753 - 0.003264*altitude*1e-5)*altitude*1e-5);
	temp1 = pow( temp, 4.256)*0.0023769;
	temp2 = 1716.5*(1 - 0.687532*altitude*1e-5 + 0.003298*SQR(altitude*1e-5))*518.69;
	*pressure = temp1*temp2;

	// compute temp
	*temperature = (1 - 0.687532*altitude*1e-5 + 0.003298*SQR(altitude*1e-5))*518.69;

	// compute speed of sound
	*sp_sound = 1116.45*sqrt( 1 - 0.687532*altitude*1e-5 + 0.003298*SQR(altitude*1e-5));
}			

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
double line(double x, double Y1, double Y2, double X1, double X2)
{
	double retval;

	retval = (Y1 - Y2)/(X1 - X2)*x + (Y2 - (Y1 - Y2)/(X1 - X2)*X2);

	return(retval);
}
		
/*	This will produce a hysterious effect on the given value if given the
* previous value and the amount of hysterious play.  The use of this
* function is to simulate slop in linkage and gear chains.
*
*	RETURNS		new value with hysterious
*	old_val		previous value
*	current_val	current value to modify
*	play		the magnitude of play to allow (ie: +/- play)
*/
double hyst(double current_val, double old_val, double play)
{
	double retval;

	if( current_val >= old_val-play && current_val <= old_val+play )
		retval = old_val;

	if( current_val >= old_val+play )
		retval = current_val - play;

	if( current_val <= old_val-play )
		retval = current_val + play;

	return( retval );
}

