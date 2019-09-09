//#ifndef __XVECTOR3_H__
//#define __XVECTOR3_H__

#if !defined (__GNUG__) && !defined (__attribute__)
#define __attribute__(foo)
#endif

//extern "C++" {

/*----------------------------------------------------------------------------
	HTVector3.
----------------------------------------------------------------------------*/

template <class T>
struct HCORE_API HTVector3 {
  T x;
  T y;
  T z;

  HTVector3() :
    x(0), y(0), z(0) {}
  HTVector3(T _x, T _y, T _z) :
    x(_x), y(_y), z(_z) {}

	// Last modifed 19.03.05 by MaximuS --->>>
	T& operator[] (INT i) {
		if(i==0)
			return x;
		else
			if(i==1)
				return y;
			else
				return z;
	}
	// <<<---

  HTVector3<T>& operator () (T _x, T _y, T _z) {
    x = _x; y = _y; z = _z;
    return *this;
  }
  
  HTVector3<T>& operator () (const HTVector3<T>& b) {
    x = b.x;
    y = b.y;
    z = b.z;
    return *this;
  }
  
  HTVector3<T>& operator += (const HTVector3<T>& b) {
    x += b.x;
    y += b.y;
	z += b.z;
    return *this;
  }
  
  HTVector3<T>& operator -= (const HTVector3<T>& b) {
    x -= b.x;
    y -= b.y;
    z -= b.z;
    return *this;
  }
  
  HTVector3<T>& operator *= (const HTVector3<T>& b) {
    x *= b.x;
    y *= b.y;
    z *= b.z;
    return *this;
  }
  
  HTVector3<T>& operator *= (T s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }

  HTVector3<T>& operator /= (T s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
  }

  T Length () {

#ifdef DEFINED_appFastSqrt
		return (T)appFastSqrt(x*x + y*y + z*z);
#else
		return (T)sqrt(x*x + y*y + z*z);
#endif
  }
  
  T Len () {
    return Length();
  }

  void Pack(T* u) {
    x = u[0];
    y = u[1];
    z = u[2];
  }

  void Unpack(T* u) {
    u[0] = x;
    u[1] = y;
    u[2] = z;
  }
};

template struct HTVector3<FLOAT>;
template struct HTVector3<DOUBLE>;

typedef HTVector3<FLOAT> HVec3;  // FLOAT.
typedef HTVector3<DOUBLE> HVec3d; // DOUBLE.


template <class T>
HCORE_API HTBuffer<T>& operator << ( HTBuffer<T>& Ar, HTVector3<FLOAT>& V ) {
	Ar.ByteOrderSerialize( &V, sizeof(V) );
	return Ar;
}

template <class T>
HCORE_API HTBuffer<T>& operator << ( HTBuffer<T>& Ar, HTVector3<DOUBLE>& V ) {
	Ar.ByteOrderSerialize( &V, sizeof(V) );
	return Ar;
}

template <class T> inline HTVector3<T>
operator + (const HTVector3<T>& a) __attribute__ ((const));

template <class T> inline HTVector3<T>
operator + (const HTVector3<T>& a) {
  return a;
}

template <class T> inline HTVector3<T>
operator - (const HTVector3<T>& a) __attribute__ ((const));

template <class T> inline HTVector3<T>
operator - (const HTVector3<T>& a) {
  return HTVector3<T> (-a.x, -a.y, -a.z);
}

template <class T> inline HTVector3<T>
operator + (const HTVector3<T>& a, const HTVector3<T>& b) __attribute__ ((const));

template <class T> inline HTVector3<T>
operator + (const HTVector3<T>& a, const HTVector3<T>& b) {
  return HTVector3<T> (a.x+b.x, a.y+b.y, a.z+b.z);
}

template <class T> inline HTVector3<T>
operator - (const HTVector3<T>& a, const HTVector3<T>& b) __attribute__ ((const));

template <class T> inline HTVector3<T>
operator - (const HTVector3<T>& a, const HTVector3<T>& b) {
  return HTVector3<T> (a.x-b.x, a.y-b.y, a.z-b.z);
}

template <class T> inline HTVector3<T>
operator * (const HTVector3<T>& a, T s) __attribute__ ((const));

template <class T> inline HTVector3<T>
operator * (const HTVector3<T>& a, T s)
{
  return HTVector3<T> (a.x*s, a.y*s, a.z*s);
}

template <class T> inline HTVector3<T>
operator * (T s, const HTVector3<T>& a) __attribute__ ((const));

template <class T> inline HTVector3<T>
operator * (T s, const HTVector3<T>& a)
{
  return HTVector3<T> (s*a.x, s*a.y, s*a.z);
}

template <class T> inline HTVector3<T>
operator / (const HTVector3<T>& a, T s) __attribute__ ((const));

template <class T> inline HTVector3<T>
operator / (const HTVector3<T>& a, T s)
{
  return HTVector3<T> (a.x/s, a.y/s, a.z/s);
}

template <class T> inline HTVector3<T>
operator / (T s, const HTVector3<T>& a) __attribute__ ((const));

template <class T> inline HTVector3<T>
operator / (T s, const HTVector3<T>& a)
{
  return HTVector3<T> (s/a.x, s/a.y, s/a.z);
}
/*
template <class T> inline HTVector3<T>
operator * (const HTVector3<T>& a, const HTVector3<T>& b) __attribute__ ((const));

template <class T> inline HTVector3<T>
operator * (const HTVector3<T>& a, const HTVector3<T>& b) {
  return HTVector3<T> (a.x*b.x, a.y*b.y, a.z*b.z);
}
*/

template <class T> inline HTVector3<T>
Cross (const HTVector3<T>& u, const HTVector3<T>& v) __attribute__ ((const));

template <class T> inline HTVector3<T>
Cross (const HTVector3<T>& u, const HTVector3<T>& v) {
  return HTVector3<T> ( u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
}
// Last modifed 10.12.05 by MaximuS --->>>
template <class T> inline void
FastDot(const HTVector3<T> &A, const HTVector3<T> &B, FLOAT &RetVal) {
	// About 22 cycles on P5.
	__asm {
		mov		esi, [A]			// 1 U
		mov		edx, [B]			// 1 V
		fld		dword ptr [esi + 8] // 1 U
		fld		dword ptr [esi + 4] // 1 U
		fld		dword ptr [esi + 0] // 1 U
		fmul	dword ptr [edx + 0] // 3 U
		fxch	st(1)				// 1 V
		fmul	dword ptr [edx + 4] // 3 U
		faddp	st(1), st(0)        // 3 U
		fxch	st(1)               // 1 V
		fmul	dword ptr [edx + 8] // 3 U
		faddp	st(1), st(0)        // 3 U
		mov		edi, [RetVal]		// 1 U
		fstp    dword ptr [edi]		// 2 U
	}
}
//<<<---
template <class T> inline T
Dot (const HTVector3<T>& a, const HTVector3<T>& b) __attribute__ ((const));

template <class T> inline T
Dot (const HTVector3<T>& a, const HTVector3<T>& b) {
  return (a.x*b.x + a.y*b.y + a.z*b.z);
}

// Cross product.
template <class T> inline HTVector3<T>
operator ^ (const HTVector3<T>& u, const HTVector3<T>& v) __attribute__ ((const));

template <class T> inline HTVector3<T> 
operator ^ (const HTVector3<T>& u, const HTVector3<T>& v)
{
	return Cross(u, v);
}

// Dot product.
template <class T> inline T
operator * (const HTVector3<T>& a, const HTVector3<T>& b) __attribute__ ((const));

template <class T> inline T
operator * (const HTVector3<T>& a, const HTVector3<T>& b) {
  return Dot(a, b);
}

// Last modifed 11.12.05 by MaximuS --->>>
template <class T> inline void
FastMagnitude(HTVector3<T> &V, FLOAT &RetVal) {
	// About 91 cycles on P5.
	__asm {
		mov		eax, [V]			// 1 U
#ifdef __INTEL_BYTE_ORDER__		
		fld		dword ptr [eax+8]	// 1 U
		fld		dword ptr [eax+4]	// 1 U
		fld		dword ptr [eax+0]	// 1 U
#else
		fld		dword ptr [eax+0]	// 1 U
		fld		dword ptr [eax+4]	// 1 U
		fld		dword ptr [eax+8]	// 1 U
#endif
		fmul	st, st				// 3 U
		fxch	st(1)				// 1 V
		fmul	st, st				// 3 U
		fxch	st(2)				// 1 V
		fmul	st, st				// 3 U
		faddp	st(1), st(0)		// 3 U
		faddp	st(1), st(0)		// 3 U
		fsqrt						// 70 U NP
		mov		eax, [RetVal]		// 1 U
		fstp	dword ptr [eax]		// 2 U/V
	}
}
template <class T> inline void
FastRMagnitude(HTVector3<T> &V, FLOAT &RetVal) {
	// About 132 cycles on P5.
	__asm {
		mov		eax, [V]			// 1 U
#ifdef __INTEL_BYTE_ORDER__		
		fld		dword ptr [eax+8]	// 1 U
		fld		dword ptr [eax+4]	// 1 U
		fld		dword ptr [eax+0]	// 1 U
#else
		fld		dword ptr [eax+0]	// 1 U
		fld		dword ptr [eax+4]	// 1 U
		fld		dword ptr [eax+8]	// 1 U
#endif
		fmul	st, st				// 3 U
		fxch	st(1)				// 1 V
		fmul	st, st				// 3 U
		fxch	st(2)				// 1 V
		fmul	st, st				// 3 U
		faddp	st(1), st(0)		// 3 U
		faddp	st(1), st(0)		// 3 U
		fsqrt						// 70 U NP
		fld1						// 2 U NP
		fdivrp	st(1), st(0)		// 39 U
		mov		eax, [RetVal]		// 1 U
		fstp	dword ptr [eax]		// 2 U/V
	}
}
//<<<---

template <class T> inline FLOAT
Magnitude(HTVector3<T> vNormal)
{
#ifdef DEFINED_appFastSqrt
	return appFastSqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
#else
	return (FLOAT)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
#endif
}

template <class T> inline FLOAT
RMagnitude(HTVector3<T> vNormal)
{
	return appRSqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
}

//FastMagnitude

template <class T> inline HTVector3<T>
Normalize(HTVector3<T> vVector)
{
	//FLOAT magnitude = Magnitude(vVector);

	FLOAT rmagnitude; // = RMagnitude(vVector);

	FastRMagnitude(vVector, rmagnitude);

	//vVector.x = vVector.x / magnitude;
	//vVector.y = vVector.y / magnitude;
	//vVector.z = vVector.z / magnitude;

	vVector.x = vVector.x * rmagnitude;
	vVector.y = vVector.y * rmagnitude;
	vVector.z = vVector.z * rmagnitude;

	return vVector;										
}

#define Mag(Normal) (appFastSqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

template <class T> inline HTVector3<T>
DivideVectorByScaler(HTVector3<T> vVector1, FLOAT Scaler)
{
	HTVector3<T> vResult;

	vResult.x = vVector1.x / Scaler;
	vResult.y = vVector1.y / Scaler;
	vResult.z = vVector1.z / Scaler;

	return vResult;
}

inline FLOAT Absolute(FLOAT num)
{
	if(num < 0)
		return (0 - num);

	return num;
}

template <class T> inline HTVector3<T>
Normal(HTVector3<T> vPolygon[])
{
	HTVector3<T> vVector1 = vPolygon[2] - vPolygon[0];
	HTVector3<T> vVector2 = vPolygon[1] - vPolygon[0];

	HTVector3<T> vNormal = Cross(vVector1, vVector2);

	vNormal = Normalize(vNormal);

	return vNormal;
}

template <class T> inline FLOAT
PlaneDistance(HTVector3<T> Normal, HTVector3<T> Point)
{	
	FLOAT distance = 0;
	distance = - ((Normal.x * Point.x) + (Normal.y * Point.y) + (Normal.z * Point.z));

	return distance;
}

template <class T> inline HBOOL
IntersectedPlane(HTVector3<T> vPoly[], HTVector3<T> vLine[], HTVector3<T> &vNormal, FLOAT &originDistance)
{
	FLOAT distance1=0, distance2=0;

	vNormal = Normal(vPoly);

	originDistance = PlaneDistance(vNormal, vPoly[0]);

	distance1 = ((vNormal.x * vLine[0].x)  +					// Ax +
		         (vNormal.y * vLine[0].y)  +					// Bx +
				 (vNormal.z * vLine[0].z)) + originDistance;	// Cz + D
	
	distance2 = ((vNormal.x * vLine[1].x)  +					// Ax +
		         (vNormal.y * vLine[1].y)  +					// Bx +
				 (vNormal.z * vLine[1].z)) + originDistance;	// Cz + D

	if(distance1 * distance2 >= 0)
	   return false;

	return true;
}

template <class T> inline DOUBLE
AngleBetweenVectors(HTVector3<T> Vector1, HTVector3<T> Vector2)
{							
	DOUBLE dotProduct = Dot(Vector1, Vector2);				
	DOUBLE vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2) ;
	DOUBLE angle = acos( dotProduct / vectorsMagnitude );

	if(_isnan(angle))
		return 0;

	return( angle );
}

template <class T> inline HTVector3<T>
IntersectionPoint(HTVector3<T> vNormal, HTVector3<T> vLine[], DOUBLE distance)
{
	HTVector3<T> vPoint, vLineDir;
	DOUBLE Numerator = 0.0, Denominator = 0.0, dist = 0.0;

	vLineDir = vLine[1] - vLine[0];
	vLineDir = Normalize(vLineDir);

	Numerator = - (vNormal.x * vLine[0].x +
				   vNormal.y * vLine[0].y +
				   vNormal.z * vLine[0].z + distance);

	Denominator = Dot(vNormal, vLineDir);

	if( Denominator == 0.0)
		return vLine[0];

	dist = Numerator / Denominator;

	vPoint.x = (FLOAT)(vLine[0].x + (vLineDir.x * dist));
	vPoint.y = (FLOAT)(vLine[0].y + (vLineDir.y * dist));
	vPoint.z = (FLOAT)(vLine[0].z + (vLineDir.z * dist));

	return vPoint;
}

template <class T> inline HBOOL
InsidePolygon(HTVector3<T> vIntersection, HTVector3<T> Poly[], long verticeCount)
{
	const DOUBLE MATCH_FACTOR = 0.99f;
	DOUBLE Angle = 0.0;
	HTVector3<T> vA, vB;

	for (INT i = 0; i < verticeCount; i++)
	{	
		vA = Poly[i] - vIntersection;
		vB = Poly[(i + 1) % verticeCount] - vIntersection;

		Angle += AngleBetweenVectors(vA, vB);
	}

	if(Angle >= (MATCH_FACTOR * (2.0 * PI)) )
		return true;

	return false;
}

template <class T> inline HBOOL
IntersectedPolygon(HTVector3<T> vPoly[], HTVector3<T> vLine[], INT verticeCount)
{
	HTVector3<T> vNormal;
	FLOAT originDistance = 0;

	if(!IntersectedPlane(vPoly, vLine,   vNormal,   originDistance))
		return false;

	HTVector3<T> vIntersection = IntersectionPoint(vNormal, vLine, originDistance);

	if(InsidePolygon(vIntersection, vPoly, verticeCount))
		return true;

	return false;
}

template <class T> inline FLOAT
Distance(HTVector3<T> vPoint1, HTVector3<T> vPoint2)
{
#ifdef DEFINED_appFastSqrt
	FLOAT distance = appFastSqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
						    (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +
						    (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) );
#else
	FLOAT distance = sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
						    (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +
						    (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) );
#endif
	return distance;
}

template <class T> inline HTVector3<T>
ClosestPointOnLine(HTVector3<T> vA, HTVector3<T> vB, HTVector3<T> vPoint)
{
	HTVector3<T> vVector1 = vPoint - vA;
    HTVector3<T> vVector2 = Normalize(vB - vA);

    FLOAT d = Distance(vA, vB);

    FLOAT t = Dot(vVector2, vVector1);

    if (t <= 0) 
		return vA;

    if (t >= d) 
		return vB;

    HTVector3<T> vVector3 = vVector2 * t;

	HTVector3<T> vClosestPoint = vA + vVector3;

	return vClosestPoint;
}

#define C_BEHIND		0
#define C_INTERSECTS	1
#define C_FRONT			2

template <class T> inline INT
ClassifySphere(HTVector3<T> &vCenter, 
				   HTVector3<T> &vNormal, HTVector3<T> &vPoint, FLOAT radius, FLOAT &distance)
{
	FLOAT d = (FLOAT)PlaneDistance(vNormal, vPoint);

	distance = (vNormal.x * vCenter.x + vNormal.y * vCenter.y + vNormal.z * vCenter.z + d);

	if(fabs(distance) < radius)
		return C_INTERSECTS;

	else if(distance >= radius)
		return C_FRONT;

	return C_BEHIND;
}

template <class T> inline HBOOL
SpherePolygonCollision(HTVector3<T> vPolygon[], 
							HTVector3<T> &vCenter, INT vertexCount, FLOAT radius)
{
	HTVector3<T> vNormal = Normal(vPolygon);

	FLOAT distance = 0.0f;

	INT classification = ClassifySphere(vCenter, vNormal, vPolygon[0], radius, distance);

	if(classification == C_INTERSECTS) 
	{
		HTVector3<T> vOffset = vNormal * distance;

		HTVector3<T> vPosition = vCenter - vOffset;

		if(InsidePolygon(vPosition, vPolygon, vertexCount))
			return true;
		else
		{
			if(EdgeSphereCollision(vCenter, vPolygon, vertexCount, radius))
			{
				return true;
			}
			return false;
		}
	}

	return false;
}

template <class T> inline HBOOL
EdgeSphereCollision(HTVector3<T> &vCenter, 
						 HTVector3<T> vPolygon[], INT vertexCount, FLOAT radius)
{
	HTVector3<T> vPoint;

	for(INT i = 0; i < vertexCount; i++)
	{
		vPoint = ClosestPointOnLine(vPolygon[i], vPolygon[(i + 1) % vertexCount], vCenter);

		FLOAT distance = Distance(vPoint, vCenter);

		if(distance < radius)
			return true;
	}

	return false;
}

template <class T> inline HTVector3<T>
GetCollisionOffset(HTVector3<T> &vNormal, FLOAT radius, FLOAT distance)
{
	HTVector3<T> vOffset = HTVector3<T>(0, 0, 0);

	//if(distance > 0)
	//{
		FLOAT distanceOver = radius - distance;
		vOffset = vNormal * distanceOver;
		//vOffset = vNormal * (distanceOver/4);
	//} else {
	//	FLOAT distanceOver = radius + distance;
		//vOffset = vNormal * (-distanceOver);
	//	vOffset = vNormal * (-distanceOver/4);
	//}

	return vOffset;
}

template <class T> inline void
RotateVectorByMat( HTVector3<T> *pResult, const HTVector3<T> *vVector, FLOAT *pMat )
{
	pResult->x = (pMat[0] * vVector->x) + (pMat[4] * vVector->y) + (pMat[8]  * vVector->z);
	pResult->y = (pMat[1] * vVector->x) + (pMat[5] * vVector->y) + (pMat[9]  * vVector->z);
	pResult->z = (pMat[2] * vVector->x) + (pMat[6] * vVector->y) + (pMat[10] * vVector->z);
}

template <class T> inline HTVector3<T>
ProjectVector(HTVector3<T> v, HTVector3<T> vOnto)
{
	HTVector3<T> vProjected;

	//vProjected = v - Cross(vOnto, Cross(v, vOnto));
	vProjected = v; //vOnto * Dot(v, vOnto);	

	FLOAT fS = Dot(v, vOnto)/Dot(vOnto, vOnto);
	vProjected *= fS;

	return vProjected;
}

template <class T> inline HTVector3<T>
ProjectPlane(HTVector3<T> v, HTVector3<T> v1,
 HTVector3<T> v2)
{
	HTVector3<T> vResult, vProject1, vProject2;

	vProject1 = ProjectVector(v, v1);
	vProject2 = ProjectVector(v, v2);

	vResult = vProject1 + vProject2;

	return vResult;
}

template <class T> inline HTVector3<T>
ReflectVector(HTVector3<T> vVel, HTVector3<T> n)
{
	HTVector3<T> vN = n*Dot(n, vVel);
	HTVector3<T> vT = vVel - vN;
	HTVector3<T> vResult = vT - vN;

	return vResult;
}

// Last modifed 19.03.05 by MaximuS --->>>
template <class T> inline void
GetRightUp(HTVector3<T> n, HTVector3<T> &right, HTVector3<T> &up)
{
	HTVector3<T> vAxis[3] = 
	{
		HTVector3<T>(1.0f, 0.0f, 0.0f),
		HTVector3<T>(0.0f, 1.0f, 0.0f),
		HTVector3<T>(0.0f, 0.0f, 1.0f)
	};

	HTVector3<T> fn(FABS(n.x), FABS(n.y), FABS(n.z));

	INT major = 0;

	// Find the major axis.
	if (fn[1] > fn[major])
      major = 1;
	if (fn[2] > fn[major])
      major = 2;

	// Build right vector by hand.
	if (fn[0] == 1 || fn[1] == 1 || fn[2] == 1)
	{
		if (major == 0 && n[0] > 0.0f)
			right = HVec3(0, 0, -1);
		else if (major == 0)
			right = HVec3(0, 0, 1);
		if (major == 1 || (major == 2 && n[2] > 0.0f))
			right = HVec3(1, 0, 0);
		if (major == 2 && n[2] < 0.0f)
			right = HVec3(-1, 0, 0);
	}
	else
		right = Cross(vAxis[major], n);

	up = Cross(n, right);
	right = Normalize(right);
	up = Normalize(up);
}
// <<<---

// Last modifed 13.02.06 by MaximuS --->>>
template <class T> inline HBOOL
operator == (const HTVector3<T>& A, const HTVector3<T>& B) __attribute__ ((const));

template <class T> inline HBOOL
operator == (const HTVector3<T>& A, const HTVector3<T>& B)
{
	return A.x==B.x && A.y==B.y && A.z==B.z;
}
// <<<---

//} // extern "C++"


//#endif

/* -------------------------- End -------------------------- */