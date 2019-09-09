// Template and inlines implementing HTQuat number support for C++

#ifdef __GNUG__
#pragma interface
#endif

#if !defined (__GNUG__) && !defined (__attribute__)
#define __attribute__(foo) /* Ignore.  */
#endif

/*----------------------------------------------------------------------------
	HTQuat.
----------------------------------------------------------------------------*/

template <class T>
class HCORE_API HTQuat
{
private:

	mutable T a0, a1, a2, a3;

public:

	// Constructor.
	HTQuat(T x0=0, T x1=0, T x2=0, T x3=0):
				a0 (x0), a1 (x1), a2 (x2), a3 (x3) { }

	// Last modifed 19.11.05 by MaximuS --->>>
	T& operator[] (INT i) {
		if(i==0)
			return a0;
		else
			if(i==1)
				return a1;
			else
				if(i==2)
					return a2;
				else
					return a3;
	}
	// <<<---

	// Function prototypes.
	HTQuat<T> operator += (const HTQuat<T>& x) {
		a0 += x.re();
		a1 += x.im1();
		a2 += x.im2();
		a3 += x.im3();

		return *this;
	}

//	HTQuat& operator -= (const HTQuat&);
//	HTQuat& operator *= (const HTQuat&);

	void re(T a) { a0 = a; }
	void im1(T a) { a1 = a; }
	void im2(T a) { a2 = a; }
	void im3(T a) { a3 = a; }

	T re () const { return a0; }
	T im1 () const { return a1; }
	T im2 () const { return a2; }
	T im3 () const { return a3; }
};

// Specializations.

template class HTQuat<FLOAT>;
template class HTQuat<DOUBLE>;
template class HTQuat<long double>;

// Aliases.

typedef HTQuat<FLOAT> HQuat;
typedef HTQuat<DOUBLE> HQuatd;
typedef HTQuat<long double> HQuatld;


// Function definitions.
/*
template <class T>
ostream& operator<<(ostream& s, const HTQuat<T>& q)
{
	return(s<<"("<<q.re()<<","<<q.im1()<<","<<q.im2()<<","<<q.im3()<<")");
}
*/

template <class T>
HCORE_API HTBuffer<T>& operator << ( HTBuffer<T>& Ar, HTQuat<FLOAT>& Q ) {
	Ar.ByteOrderSerialize( &Q, sizeof(Q) );
	return Ar;
}

template <class T>
HCORE_API HTBuffer<T>& operator << ( HTBuffer<T>& Ar, HTQuat<DOUBLE>& Q ) {
	Ar.ByteOrderSerialize( &Q, sizeof(Q) );
	return Ar;
}

template <class T> inline T
re (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline T
re (const HTQuat<T>& x)
{
	return x.re ();
}

template <class T> inline T
im1 (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline T
im1 (const HTQuat<T>& x)
{
	return x.im1 ();
}

template <class T> inline T
im2 (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline T
im2 (const HTQuat<T>& x)
{
	return x.im2 ();
}

template <class T> inline T
im3 (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline T
im3 (const HTQuat<T>& x)
{
	return x.im3 ();
}


template <class T> inline HTQuat<T>
operator + (const HTQuat<T>& x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator + (const HTQuat<T>& x, const HTQuat<T>& y)
{
	return HTQuat<T> ( re(x)+re(y), im1(x)+im1(y),
								im2(x)+im2(y), im3(x)+im3(y));
}

template <class T> inline HTQuat<T>
operator + (const HTQuat<T>& x, T y) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator + (const HTQuat<T>& x, T y)
{
	return HTQuat<T> (re(x)+y, im1(x), im2(x), im3(x));
}

template <class T> inline HTQuat<T>
operator + (T x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator + (T x, const HTQuat<T>& y)
{
	return HTQuat<T> (x + re(y), im1(y), im2(y), im3(y));
}

template <class T> inline HTQuat<T>
operator - (const HTQuat<T>& x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator - (const HTQuat<T>& x, const HTQuat<T>& y)
{
	return HTQuat<T> ( re(x)-re(y), im1(x)-im1(y),
								im2(x)-im2(y), im3(x)-im3(y));
}

template <class T> inline HTQuat<T>
operator - (const HTQuat<T>& x, T y) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator - (const HTQuat<T>& x, T y)
{
	return HTQuat<T> (re(x) - y, im1 (x), im2 (x), im3 (x));
}

template <class T> inline HTQuat<T>
operator - (T x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator - (T x, const HTQuat<T>& y)
{
	return HTQuat<T> (x - re (y), - im1 (y), - im2 (y), - im3 (y));
}

template <class T> inline HTQuat<T>
operator * (const HTQuat<T>& x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator * (const HTQuat<T>& x, const HTQuat<T>& y)
{
	return HTQuat<T>
             (re(x)*re(y) - im1(x)*im1(y) - im2(x)*im2(y) - im3(x)*im3(y), 
              re(x)*im1(y) + im1(x)*re(y) + im2(x)*im3(y) - im3(x)*im2(y), 
              re(x)*im2(y) + im2(x)*re(y) + im3(x)*im1(y) - im1(x)*im3(y), 
              re(x)*im3(y) + im3(x)*re(y) + im1(x)*im2(y) - im2(x)*im1(y) );
}

template <class T> inline HTQuat<T>
operator * (const HTQuat<T>& x, T y) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator * (const HTQuat<T>& x, T y)
{
	return HTQuat<T> (re (x) * y, im1 (x) * y, im2 (x) * y, im3 (x) * y);
}

template <class T> inline HTQuat<T>
operator * (T x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator * (T x, const HTQuat<T>& y)
{
	return HTQuat<T> (x * re (y), x * im1 (y), x * im2 (y), x * im3 (y));
}

template <class T> HTQuat<T>
operator / (const HTQuat<T>& x, T y) __attribute__ ((const));

template <class T> HTQuat<T>
operator / (const HTQuat<T>& x, T y)
{
	return HTQuat<T> (re (x) / y, im1 (x) / y, im2 (x) / y, im3 (x) / y);
}

template <class T> inline HTQuat<T>
operator + (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator + (const HTQuat<T>& x)
{
	return x;
}

template <class T> inline HTQuat<T>
operator - (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator - (const HTQuat<T>& x)
{
  return HTQuat<T> (-re (x), -im1 (x), -im2 (x), -im3 (x));
}


// Last modifed 5.02.05 by MaximuS --->>>

template <class T> inline HTQuat<T>
operator ~ (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator ~ (const HTQuat<T>& x)
{
	return HTQuat<T> (re (x), -im1 (x), -im2 (x), -im3 (x));
}

template <class T> inline HTQuat<T>
operator * (const HTQuat<T>& q, const HTVector3<T>& v) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator * (const HTQuat<T>& q, const HTVector3<T>& v)
{
	return	HTQuat<T>(-(im1 (q)*v.x + im2 (q)*v.y + im3 (q)*v.z),
							re (q)*v.x + im2 (q)*v.z - im3 (q)*v.y,
							re (q)*v.y + im3 (q)*v.x - im1 (q)*v.z,
							re (q)*v.z + im1 (q)*v.y - im2 (q)*v.x);
}

template <class T> inline HTQuat<T>
operator * (const HTVector3<T>& v, const HTQuat<T>& q) __attribute__ ((const));

template <class T> inline HTQuat<T>
operator * (const HTVector3<T>& v, const HTQuat<T>& q)
{
	return	HTQuat<T>(-(im1 (q)*v.x + im2 (q)*v.y + im3 (q)*v.z),
							re (q)*v.x + im3 (q)*v.y - im2 (q)*v.z,
							re (q)*v.y + im1 (q)*v.z - im3 (q)*v.x,
							re (q)*v.z + im2 (q)*v.x - im1 (q)*v.y);
}

template <class T> inline HTQuat<T>
QRotate (const HTQuat<T>& q1, const HTQuat<T>& q2) __attribute__ ((const));

template <class T> inline HTQuat<T>
QRotate (const HTQuat<T>& q1, const HTQuat<T>& q2)
{
	return	q1*q2*(~q1);
}

template <class T> inline HTVector3<T>
QVRotate (const HTQuat<T>& q, const HTVector3<T>& v) __attribute__ ((const));

template <class T> inline HTVector3<T>
QVRotate (const HTQuat<T>& q, const HTVector3<T>& v)
{
	HTQuat<T> t;

	t = q*v*(~q);

	return	HTVector3<T>(im1(t), im2(t), im3(t));
}
// <<<---


template <class T> inline HBOOL
operator == (const HTQuat<T>& x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline HBOOL
operator == (const HTQuat<T>& x, const HTQuat<T>& y)
{
	return re(x)==re(y) && im1(x)==im1(y) && im2(x)==im2(y) && im3(x)==im3(y);
}

template <class T> inline HBOOL
operator == (const HTQuat<T>& x, T y) __attribute__ ((const));

template <class T> inline HBOOL
operator == (const HTQuat<T>& x, T y)
{
	return re(x)==y && im1(x) == 0 && im2(x) == 0 && im3(x) == 0;
}

template <class T> inline HBOOL
operator == (T x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline HBOOL
operator == (T x, const HTQuat<T>& y)
{
	return x==re(y) && im1(y)==0 && im2(y)==0 && im3(y)==0;
}

template <class T> inline HBOOL
operator != (const HTQuat<T>& x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline HBOOL
operator != (const HTQuat<T>& x, const HTQuat<T>& y)
{
	return re(x)!=re(y) || im1(x)!=im1(y) || im2(x)!=im2(y) || im3(x)!=im3(y);
}

template <class T> inline HBOOL
operator != (const HTQuat<T>& x, T y) __attribute__ ((const));

template <class T> inline HBOOL
operator != (const HTQuat<T>& x, T y)
{
	return re(x)!=y || im1(x)!=0 || im2(x)!=0 || im3(x)!=0;
}

template <class T> inline HBOOL
operator != (T x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline HBOOL
operator != (T x, const HTQuat<T>& y)
{
	return x!=re(y) || im1(y)!=0 || im2(y)!=0 || im3(y)!=0;
}

template <class T> inline T
abs (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline T
abs (const HTQuat<T>& x)
{
	return sqrt(re(x)*re(x) + im1(x)*im1(x) + im2(x)*im2(x) + im3(x)*im3(x));
}

template <class T> inline T
arg (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline T
arg (const HTQuat<T>& x)
{
	return atan2(sqrt(im1(x)*im1(x) + im2(x)*im2(x) + im3(x)*im3(x)), re(x));
}

template <class T> inline HTQuat<T>
conj (const HTQuat<T>& x)  __attribute__ ((const));

template <class T> inline HTQuat<T>
conj (const HTQuat<T>& x) 
{
	return HTQuat<T> (re(x), -im1(x), -im2(x), -im3(x));
}

template <class T> inline T
norm (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline T
norm (const HTQuat<T>& x)
{
	return re(x)*re(x) + im1(x)*im1(x) + im2(x)*im2(x) + im3(x)*im3(x);
}

template <class T> inline HTQuat<T>
NormalizeQuaternion (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline HTQuat<T>
NormalizeQuaternion (const HTQuat<T>& x)
{
	FLOAT mag = abs(x);
	return HTQuat<T> (re(x) / mag, im1(x) / mag, im2(x) / mag, im3(x) / mag);
}

template <class T> inline T
dot (const HTQuat<T>& x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline T
dot (const HTQuat<T>& x, const HTQuat<T>& y)
{
	return re(x)*re(y) + im1(x)*im1(y) + im2(x)*im2(y) + im3(x)*im3(y);
}

template <class T> inline HTQuat<T>
slerp (T t, const HTQuat<T>& x, const HTQuat<T>& y) __attribute__ ((const));

template <class T> inline HTQuat<T>
slerp (T t, const HTQuat<T>& x, const HTQuat<T>& y)
{
	DOUBLE a = acos(dot(x, y));
	DOUBLE sa = sin(a);
	if(sa>=(1e-06))
		return x*(T)(sin((1.0-t)*a)/sin(a)) + y*(T)(sin(t*a)/sin(a));
	return x*(T)(1.0-t) + y*(T)(t);
}



template <class T> inline HTQuat<T>
exp (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline HTQuat<T>
exp (const HTQuat<T>& x)
{
	T angle = sqrt(im1(x)*im1(x) + im2(x)*im2(x) + im3(x)*im3(x));
	T cs = cos(angle);
	T sn = sin(angle);

	if ( fabs(sn) >= (1e-06) ) {
		T coeff = sn/angle;
		return HTQuat<T> (cs, coeff*im1(x), coeff*im2(x), coeff*im3(x));
	}
	return HTQuat<T> (cs, im1(x), im2(x), im3(x));
}

template <class T> inline HTQuat<T>
log (const HTQuat<T>& x) __attribute__ ((const));

template <class T> inline HTQuat<T>
log (const HTQuat<T>& x)
{
	if(fabs(re(x))<1)
	{
		T angle = acos(re(x));
		T sn = sin(angle);
		if(fabs(sn)>=(1e-06))
		{
			T coeff = angle/sn;
			return HTQuat<T>(0, coeff*im1(x), coeff*im2(x), coeff*im3(x));
		}
	}
	return HTQuat<T> (0, im1(x), im2(x), im3(x));
}

template <class T> inline HTQuat<T>
ShortestArc(const HTVector3<T> &from, const HTVector3<T> &to) __attribute__ ((const));

template <class T> inline HTQuat<T>
ShortestArc(const HTVector3<T> &from, const HTVector3<T> &to)
{
	HTVector3<T> c( Cross(from, to) );
	FLOAT tw = (from.x+from.y+from.z)/(to.x+to.y+to.z);
    HTQuat<T> qQ( c.x, c.y, c.z, tw );
    qQ = NormalizeQuaternion(qQ);    // if "from" or "to" not unit, normalize quat

	T w = qQ.im3();
	w += 1.0f;      // reducing angle to halfangle
	qQ.im3(w);
    if( w <= 0.001f ) // angle close to PI
    {
        if( ( from.z*from.z ) > ( from.x*from.x ) )
				qQ = HTQuat<T>(0 , from.z, -from.y, w);
        else 
		qQ = HTQuat<T>(from.y, -from.x, 0, w);
    }
    qQ = NormalizeQuaternion(qQ);

	return qQ;
}


//} // extern "C++"

//#endif

/* -------------------------- End -------------------------- */