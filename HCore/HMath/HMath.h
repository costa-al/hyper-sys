// System includies
#include <cmath>
//#include <math.h>
#include <float.h>

// Crystal math constants.
#ifndef PI
#define PI	3.14159265358979324
#endif
#ifndef PI2 // PI/2
#define PI2 1.57079632679489662
#endif

#ifndef MATH_ABS
#define MATH_ABS
inline INT ABS(INT n) {
	return n<0 ? -n : n;
}
inline FLOAT FABS(FLOAT n) {
	return n<0 ? -n : n;
}
#endif

INT appRand()		{ return rand(); }
FLOAT appFrand()	{ return (FLOAT)rand(); }

inline FLOAT RandRange(FLOAT fK0, FLOAT fK1) {
	return fK0 + appFrand()*((fK1-fK0)/RAND_MAX);
}
inline HBOOL Potencial(FLOAT fK) {
	FLOAT fR = appFrand()*(1.0f/RAND_MAX);
	if(fK>fR)
		return true;
	else
		return false;
}

#if ASM
#define DEFINED_appFastCosSin 1
inline void appFastCosSin(const FLOAT &fAngle, FLOAT &fCosA, FLOAT &fSinA) {
	__asm {
		mov		eax, [fAngle]
		fld		dword ptr [eax]
		fsincos
		mov		esi, [fCosA]
		fstp	dword ptr [esi]
		mov		edi, [fSinA]
		fstp	dword ptr [edi]
	}
}
#endif

#include "HVec2.h"
#include "HVec3.h"
#include "HQuat.h"
#include "HMatrix.h"

/*----------------------------------------------------------------------------
	HTPlane.
----------------------------------------------------------------------------*/

template <class T>
class HCORE_API HTPlane {
public:
	mutable T P[4]; // 0,1,2 - plane normal; 4 - distance plane.

	HTPlane () {
		T *p = P;
		for(int i=0; i<4; i++)
			*p++ = 0;
	}

	HTPlane (T *p) {
		T *d = P;
		for(int i=4; i; i--)
			*d++ = *p++;
	}

	T* operator() () const {
		return P;
	}
	T& operator[] (int i) const {
		return P[i];
	}

	void CalculatePlane(HTVector3<T> v[3]) {
		HTVector3<T> normal = Normal(v);
		FLOAT distance = PlaneDistance(normal, v[0]);

		PlaneNormal(normal);
		Distance(distance);
	}

	void PlaneNormal(HTVector3<T> n) {
		P[0] = n.x;
		P[1] = n.y;
		P[2] = n.z;
	}

	void Distance(T d) {
		P[3] = d;
	}

	HTVector3<T> PlaneNormal() {
		HTVector3<T> normal;
		normal.x = P[0];
		normal.y = P[1];
		normal.z = P[2];
		return normal;
	}

	T Distance() {
		return P[3];
	}
};

template class HTPlane<FLOAT>;
template class HTPlane<DOUBLE>;

typedef HTPlane<FLOAT> HPlane;
typedef HTPlane<DOUBLE> HPlaned;

inline FLOAT DegreesToRadians(FLOAT fDeg) {
	return fDeg * PI / 180.0f;
}

inline FLOAT RadiansToDegrees(FLOAT fRad) {
	return fRad * 180.0f / PI;
}

/*
 * Converts an angle&axis representation of rotations to corresponding
 * quaternion representation.
 */

template <class T> inline HTQuat<T>
AngleAxis2Quaternion (T angle, T x, T y, T z) {
	T s = (T)sin(angle/2.0);
	return HTQuat<T> ((T)cos(angle/2.0), x*s, y*s, z*s);
}

/*
 * Converts euler angles to its corresponding matrix representation, where
 * the result represents a rotation about the x-axis by the angle 'alpha'
 * followed by rotation about the y-axis by the angle 'beta' concluded by
 * rotation about the z-axis by the angle 'gamma'.
 */

template <class T> inline HTMatrix<T>
Euler2Matrix (T alpha, T beta, T gamma) {
	HTMatrix<T> M;
	T* m = M();

	T sin_alpha = (T)(sin(alpha));
	T cos_alpha = (T)(cos(alpha));
	T sin_beta = (T)(sin(beta));
	T cos_beta = (T)(cos(beta));
	T sin_gamma = (T)(sin(gamma));
	T cos_gamma = (T)(cos(gamma));

	*m = (T)(cos(beta)*cos(gamma)); m++;
	*m = (T)(cos(beta)*sin(gamma)); m++;
	*m = (T)(-sin(beta)); m++;
	*m = (T)0; m++;

	*m = (T)(cos_gamma*sin_alpha*sin_beta - cos_alpha*sin_gamma); m++;
	*m = (T)(cos_alpha*cos_gamma + sin_alpha*sin_beta*sin_gamma); m++;
	*m = (T)(cos_beta*sin_alpha); m++;
	*m = (T)0; m++;

	*m = (T)(cos_alpha*cos_gamma*sin_beta + sin_alpha*sin_gamma); m++;
	*m = (T)(cos_alpha*sin_beta*sin_gamma - cos_gamma*sin_alpha); m++;
	*m = (T)(cos_alpha*cos_beta); m++;
	*m = (T)0; m++;

	*m = (T)0; m++;
	*m = (T)0; m++;
	*m = (T)0; m++;
	*m = (T)1;

	return M;
}

/*
 * Converts a rotation matrix to the corresponding unit quaternion.
 * The matrix argument must not represent nothing more than a rotation,
 * except for translation data which will be left unnoticed.
 */

template <class T> inline HTQuat<T>
Matrix2Quaternion (const HTMatrix<T>& mat) {
	T* m = mat();
	T s, x, y, z;
	s = (T)(-0.5*sqrt(m[0] + m[5] + m[10] + m[15]));
	T c = (T)(0.25/s);
	x = (m[6] - m[9])*c;
	y = (m[8] - m[2])*c;
	z = (m[1] - m[4])*c;
	return HTQuat<T> (s, x, y, z);
}

/*
 * Converts an unit quaternion to the corresponding rotation matrix.
 */

template <class T> inline HTMatrix<T>
Quaternion2Matrix(const HTQuat<T>& q) {
	T w = re(q);
	T x = im1(q);
	T y = im2(q);
	T z = im3(q);

	T wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
	x2 = x + x; y2 = y + y; 
	z2 = z + z;
	xx = x * x2;   xy = x * y2;   xz = x * z2;
	yy = y * y2;   yz = y * z2;   zz = z * z2;
	wx = w * x2;   wy = w * y2;   wz = w * z2;

	HTMatrix<T> m;
	m[0] = 1.0 - (yy + zz); m[4] = xy - wz;         m[8] = xz + wy;          m[12] = 0.0;
	m[1] = xy + wz;         m[5] = 1.0 - (xx + zz); m[9] = yz - wx;          m[13] = 0.0;
	m[2] = xz - wy;         m[6] = yz + wx;         m[10] = 1.0 - (xx + yy); m[14] = 0.0;
	m[3] = 0.0;             m[7] = 0.0;             m[11] = 0.0;             m[15] = 1.0;

	return m; 
}


/*
 * Linear matrix interpolation. The input matrices must not represent
 * anything more than a rotation and a translation. The result is a matrix
 * representing the rotation and the translation on the way from 'm1' to 'm2'
 * according to the time parameter 't' which should lie between [0,1]
 * boundaries. Internally, the matrices are converted to the corresponding
 * quaternions, then they are interpolated through the slerp function and
 * finally the result is converted back to a matrix, where the translation
 * is computed using linear interpolation on the translation vectors from
 * the original two matrices.
 */

template <class T> inline HTMatrix<T>
Mslerp (T t, const HTMatrix<T>& m1,
                  const HTMatrix<T>& m2)
{
	HTQuat<T> q = slerp(t, Matrix2Quaternion(m1), Matrix2Quaternion(m2));
	HTMatrix<T> m = Quaternion2Matrix(q);

	m[12] = m1[12] + t*(m2[12]-m1[12]);
	m[13] = m1[13] + t*(m2[13]-m1[13]);
	m[14] = m1[14] + t*(m2[14]-m1[14]);

	return m;
}

// Last modifed 6.10.05 by MaximuS --->>>
template <class T> inline HTVector3<T>
BezierCurve(HTVector3<T> vP1, HTVector3<T> vP2, HTVector3<T> vP3,
			HTVector3<T> vP4, T dT)
{
	T Var1, Var2, Var3;
    HTVector3<T> vPoint = HTVector3<T>(0.0f, 0.0f, 0.0f);

    Var1 = 1 - dT;
    Var2 = Var1 * Var1 * Var1;
    Var3 = dT * dT * dT;

    vPoint.x = Var2*vP1.x + 3*dT*Var1*Var1*vP2.x + 3*dT*dT*Var1*vP3.x + Var3*vP4.x;
    vPoint.y = Var2*vP1.y + 3*dT*Var1*Var1*vP2.y + 3*dT*dT*Var1*vP3.y + Var3*vP4.y;
    vPoint.z = Var2*vP1.z + 3*dT*Var1*Var1*vP2.z + 3*dT*dT*Var1*vP3.z + Var3*vP4.z;

    return vPoint;
}
//<<<---

/* -------------------------- End -------------------------- */
