
#if !defined (__GNUG__) && !defined (__attribute__)
#define __attribute__(foo)
#endif

/*----------------------------------------------------------------------------
	HTVector2.
----------------------------------------------------------------------------*/

template <class T>
struct HCORE_API HTVector2 {
	T x;
	T y;

	HTVector2() : x(0), y(0) {}
	HTVector2(T _x, T _y) : x(_x), y(_y) {}
	
	// Last modifed 19.11.05 by MaximuS --->>>
	T& operator[] (INT i) {
		if(i==0)
			return x;
		else
			return y;
	}
	// <<<---

	HTVector2<T>& operator () (T _x, T _y) {
		x = _x; y = _y;
		return *this;
	}
	
	HTVector2<T>& operator () (const HTVector2<T>& b) {
		x = b.x;
		y = b.y;
		return *this;
	}
	
	HTVector2<T>& operator += (const HTVector2<T>& b) {
		x += b.x;
		y += b.y;
		return *this;
	}
	
	HTVector2<T>& operator -= (const HTVector2<T>& b) {
		x -= b.x;
		y -= b.y;
		return *this;
	}
	
	HTVector2<T>& operator *= (const HTVector2<T>& b) {
		x *= b.x;
		y *= b.y;
		return *this;
	}

	HTVector2<T>& operator *= (T s) {
		x *= s;
		y *= s;
		return *this;
	}

	T Length () {
		return (T)sqrt(x*x + y*y);
	}

	T Len () {
		return Length();
	}

	void Pack(T* u) {
		x = u[0];
		y = u[1];
	}

	void Unpack(T* u) {
		u[0] = x;
		u[1] = y;
	}
};

template struct HTVector2<FLOAT>;
template struct HTVector2<DOUBLE>;

typedef HTVector2<FLOAT>	HVec2;  // FLOAT.
typedef HTVector2<DOUBLE>	HVec2d; // DOUBLE.

template <class T>
HCORE_API HTBuffer<T>& operator << ( HTBuffer<T>& Ar, HTVector2<FLOAT>& V ) {
	Ar.ByteOrderSerialize( &V, sizeof(V) );
	return Ar;
}

template <class T>
HCORE_API HTBuffer<T>& operator << ( HTBuffer<T>& Ar, HTVector2<DOUBLE>& V ) {
	Ar.ByteOrderSerialize( &V, sizeof(V) );
	return Ar;
}

template <class T> inline HTVector2<T>
operator + (const HTVector2<T>& a) __attribute__ ((const));

template <class T> inline HTVector2<T>
operator + (const HTVector2<T>& a) {
  return a;
}

template <class T> inline HTVector2<T>
operator - (const HTVector2<T>& a) __attribute__ ((const));

template <class T> inline HTVector2<T>
operator - (const HTVector2<T>& a) {
  return HTVector2<T> (-a.x, -a.y);
}

template <class T> inline HTVector2<T>
operator + (const HTVector2<T>& a, const HTVector2<T>& b) __attribute__ ((const));

template <class T> inline HTVector2<T>
operator + (const HTVector2<T>& a, const HTVector2<T>& b) {
  return HTVector2<T> (a.x+b.x, a.y+b.y);
}

template <class T> inline HTVector2<T>
operator - (const HTVector2<T>& a, const HTVector2<T>& b) __attribute__ ((const));

template <class T> inline HTVector2<T>
operator - (const HTVector2<T>& a, const HTVector2<T>& b) {
  return HTVector2<T> (a.x-b.x, a.y-b.y);
}

template <class T> inline HTVector2<T>
operator * (const HTVector2<T>& a, T s) __attribute__ ((const));

template <class T> inline HTVector2<T>
operator * (const HTVector2<T>& a, T s) {
  return HTVector2<T> (a.x*s, a.y*s);
}

template <class T> inline HTVector2<T>
operator * (T s, const HTVector2<T>& a) __attribute__ ((const));

template <class T> inline HTVector2<T>
operator * (T s, const HTVector2<T>& a)
{
  return HTVector2<T> (s*a.x, s*a.y);
}

template <class T> inline HTVector2<T>
operator * (const HTVector2<T>& a, const HTVector2<T>& b) __attribute__ ((const));

template <class T> inline HTVector2<T>
operator * (const HTVector2<T>& a, const HTVector2<T>& b) {
  return HTVector2<T> (a.x*b.x, a.y*b.y);
}

template <class T> inline T
Dot (const HTVector2<T>& a, const HTVector2<T>& b) __attribute__ ((const));

template <class T> inline T
Dot (const HTVector2<T>& a, const HTVector2<T>& b) {
  return (a.x*b.x + a.y*b.y);
}


// Last modifed 9.05.05 by MaximuS --->>>
template <class T> inline FLOAT
Magnitude(HTVector2<T> vNormal)
{
	return (FLOAT)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) );
}

template <class T> inline HTVector2<T>
Normalize(HTVector2<T> vVector)
{
	FLOAT magnitude = Magnitude(vVector);				

	vVector.x = vVector.x / magnitude;
	vVector.y = vVector.y / magnitude;

	return vVector;										
}
//<<<---

template <class T> inline FLOAT
Distance(HTVector2<T> vPoint1, HTVector2<T> vPoint2)
{
	FLOAT distance = sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
						    (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) );
	return distance;
}

// Last modifed 9.02.06 by MaximuS --->>>
template <class T> inline DOUBLE
AngleBetweenVectors(HTVector2<T> Vector1, HTVector2<T> Vector2)
{							
	DOUBLE dotProduct = Dot(Vector1, Vector2);				
	DOUBLE vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2) ;
	DOUBLE angle = acos( dotProduct / vectorsMagnitude );

	if(_isnan(angle))
		return 0;

	return( angle );
}
//<<<---

// Last modifed 1.03.05 by MaximuS --->>>
#define P_RIGHT			0
#define P_LEFT			1
#define P_BEHIND		2
#define P_BEYOND		3
#define P_BETWEEN		4
#define P_ORIGIN		5
#define P_DESTINATION	6

template <class T> inline INT
Classify (const HTVector2<T>& c, const HTVector2<T>& p, const HTVector2<T>& q) {
	HTVector2<T> a = q - p;
	HTVector2<T> b = c - p;

	FLOAT s = a.x * b.y - a.y * b.x;

	if(s > 0.0f)
		return P_LEFT;
	if(s < 0.0f)
		return P_RIGHT;
	if(a.x*b.x<0.0f || a.y*b.y<0.0f)
		return P_BEHIND;
	if(a.Length() < b.Length())
		return P_BEYOND;
	if(c.x == p.x && c.y == p.y)
		return P_ORIGIN;
	if(c.x == q.x && c.y == q.y)
		return P_DESTINATION;
	return P_BETWEEN;
}
//<<<---

// Last modifed 7.02.06 by MaximuS --->>>
template <class T> inline FLOAT
DistancePointToLine(const HTVector2<T> &P, const HTVector2<T> &O, const HTVector2<T> &E) {
	//HTVector2<T> E = O + D;
	FLOAT fL = (E-O).Len();
	FLOAT fS = ((O.y-P.y)*(E.x-O.x)-(O.x-P.x)*(E.y-O.y))/(fL*fL);
	return fS*fL;
}
//<<<---

// Last modifed 19.02.06 by MaximuS --->>>
template <class T> inline FLOAT
PlaneDistance(HTVector2<T> Normal, HTVector2<T> Point)
{	
	FLOAT distance = 0;
	distance = - ((Normal.x * Point.x) + (Normal.y * Point.y));

	return distance;
}
//<<<---

template <class T> inline HTVector2<T>
ClosestPointOnLine(HTVector2<T> vA, HTVector2<T> vB, HTVector2<T> vPoint)
{
	HTVector2<T> vVector1 = vPoint - vA;
    HTVector2<T> vVector2 = Normalize(vB - vA);

    FLOAT d = Distance(vA, vB);

    FLOAT t = Dot(vVector2, vVector1);

    if (t <= 0) 
		return vA;

    if (t >= d) 
		return vB;

    HTVector2<T> vVector3 = vVector2 * t;

	HTVector2<T> vClosestPoint = vA + vVector3;

	return vClosestPoint;
}


template <class T> inline HBOOL //HTVector2<T>
PointOnLine(HTVector2<T> vA, HTVector2<T> vB, HTVector2<T> vP)
{
	FLOAT fD = (vB-vA).Len();
	FLOAT fR = ((vA.y-vP.y)*(vA.y-vB.y)-(vA.x-vP.x)*(vB.x-vA.x))/fD*fD;

	if((fR>=0.0f) && (fR<=fD))
		return true;
	return false;
}

template <class T> inline HBOOL
operator == (const HTVector2<T>& A, const HTVector2<T>& B) __attribute__ ((const));

template <class T> inline HBOOL
operator == (const HTVector2<T>& A, const HTVector2<T>& B)
{
	return A.x==B.x && A.y==B.y;
}


/* -------------------------- End -------------------------- */