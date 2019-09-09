  
/* Matrix multiplication.
 */
template <class T> inline void
__glmatrix_multiply (T* a, T* b, T *c) {
	T b1, b2, b3, b4;
	for(INT i=0; i<4; i++) {
		b1 = b[i<<2]; b2 = b[1+(i<<2)]; b3 = b[2+(i<<2)]; b4 = b[3+(i<<2)];
		c[(i<<2)]   = a[0]*b1 + a[4]*b2 +  a[8]*b3 + a[12]*b4;
		c[(i<<2)+1] = a[1]*b1 + a[5]*b2 +  a[9]*b3 + a[13]*b4;
		c[(i<<2)+2] = a[2]*b1 + a[6]*b2 + a[10]*b3 + a[14]*b4;
		c[(i<<2)+3] = a[3]*b1 + a[7]*b2 + a[11]*b3 + a[15]*b4;
	}
}


/* Rotation matrix generation template, adapted from Mesa3D, the original
 * author of the function is Erich Boleyn (erich@uruk.org).
 */
template <class T> inline void
__glmatrix_rotation_matrix (T angle, T x, T y, T z, T* m) {

	T mag, s, c;
	T xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

//#ifdef DEFINED_appFastCosSin
//	appFastCosSin(PI/180.0f, (FLOAT)c, (FLOAT)s);
//#else
	s = (T)sin( angle * PI/180.0 );
	c = (T)cos( angle * PI/180.0 );
//#endif

	mag = (T)sqrt(x*x + y*y + z*z);

	if (mag == 0.0) {
		T *p = m;
		for(INT i=16; i; i--)
			*p++ = 0;
		m[0] = 1; m[5] = 1; m[10] = 1; m[15] = 1;
		return;
	}

	x /= mag;
	y /= mag;
	z /= mag;

	xx = x * x;
	yy = y * y;
	zz = z * z;
	xy = x * y;
	yz = y * z;
	zx = z * x;
	xs = x * s;
	ys = y * s;
	zs = z * s;
	one_c = 1.0 - c;

	m[0] = (one_c * xx) + c;
	m[4] = (one_c * xy) - zs;
	m[8] = (one_c * zx) + ys;
	m[12] = 0.0;

	m[1] = (one_c * xy) + zs;
	m[5] = (one_c * yy) + c;
	m[9] = (one_c * yz) - xs;
	m[13] = 0.0;

	m[2] = (one_c * zx) - ys;
	m[6] = (one_c * yz) + xs;
	m[10] = (one_c * zz) + c;
	m[14] = 0.0;

	m[3] = 0.0;
	m[7] = 0.0;
	m[11]= 0.0;
	m[15]= 1.0;
}

/*----------------------------------------------------------------------------
	HTMatrix.
----------------------------------------------------------------------------*/

template <class T>
class HCORE_API HTMatrix
{
public:
  mutable T M[16];

  HTMatrix () {
    T *p = M;
    for(INT i=0; i<16; i++)
      *p++ = 0;
  }
  HTMatrix (T *p) {
    T *d = M;
    for(INT i=16; i; i--)
      *d++ = *p++;
  }

  T* operator() () const {
    return M;
  }
  T& operator[] (INT i) const {
    return M[i];
  }

  HTMatrix& Load (T *p) {
    T *d = M;
    for(INT i=16; i; i--)
      *d++ = *p++;
    return *this;
  }
  HTMatrix& Zero () {
    T *p = M;
    for(INT i=16; i; i--)
      *p++ = 0;
    return *this;
  }
  HTMatrix& Identity () {
    T *p = M;
    for(INT i=16; i; i--)
      *p++ = 0;
    M[0] = 1; M[5] = 1; M[10] = 1; M[15] = 1;
    return *this;
  }
  HTMatrix& LoadIdentity () {
    return Identity();
  }
  HTMatrix& Transpose () {
    T *u, *v;
    T a;
    for(INT i=0; i<4; i++) {
      u = v = M+i+(i<<2);
      for(INT j=3-i; j; j--) {
        u++;
        v+=4;
        a = *u;
        *u = *v;
        *v = a;
      }
    }  
    return *this;
  }
  HTMatrix& OrthoInverse () {
    T a;
    T x=M[12];
    T y=M[13];
    T z=M[14];
    a=M[1]; M[1]=M[4]; M[4]=a;
    a=M[2]; M[2]=M[8]; M[8]=a;
    a=M[6]; M[6]=M[9]; M[9]=a;
    M[12] = -(x*M[0] + y*M[4] + z*M[8]);
    M[13] = -(x*M[1] + y*M[5] + z*M[9]);
    M[14] = -(x*M[2] + y*M[6] + z*M[10]);
    return *this;
  }
  HTMatrix& Translate (T x, T y, T z) {
    M[12] = M[0] * x + M[4] * y + M[8]  * z + M[12];
    M[13] = M[1] * x + M[5] * y + M[9]  * z + M[13];
    M[14] = M[2] * x + M[6] * y + M[10] * z + M[14];
    M[15] = M[3] * x + M[7] * y + M[11] * z + M[15];
    return *this;
  }
  HTMatrix& Translate (HTVector3<T> V) {
    M[12] = M[0] * V.x + M[4] * V.y + M[8]  * V.z + M[12];
    M[13] = M[1] * V.x + M[5] * V.y + M[9]  * V.z + M[13];
    M[14] = M[2] * V.x + M[6] * V.y + M[10] * V.z + M[14];
    M[15] = M[3] * V.x + M[7] * V.y + M[11] * V.z + M[15];
    return *this;
  }
  HTMatrix& Scale (T x, T y, T z) {
    M[0]*=x; M[4]*=y;  M[8]*=z;
    M[1]*=x; M[5]*=y;  M[9]*=z;
    M[2]*=x; M[6]*=y; M[10]*=z;
    M[3]*=x; M[7]*=y; M[11]*=z;
    return *this;
  }
  HTMatrix& Rotate (T angle, T x, T y, T z) {
    T b[16];
    T c[16];
    __glmatrix_rotation_matrix(angle, x, y, z, b);
    __glmatrix_multiply(this->M, b, c);
    return Load(c);
  }
  
  HTMatrix& operator *=(const HTMatrix& B) {
    T c[16];
    __glmatrix_multiply(this->M, B(), c);
    return Load(c);
  }
  HTMatrix& operator +=(const HTMatrix& B) {
    T *d = M;
    const T *s = B.M;
    for(INT i=16; i; i--, d++, s++)
      *d += *s;
    return *this;
  }
  HTMatrix& operator -=(const HTMatrix& B) {
    T *d = M;
    const T *s = B.M;
    for(INT i=16; i; i--, d++, s++)
      *d -= *s;
    return *this;
  }
  HTMatrix& operator *=(T scalar) {
    T *d = M;
    for(INT i=16; i; i--, d++)
      *d *= scalar;
    return *this;
  }
  HTMatrix& operator /=(T scalar) {
    T *d = M;
    for(INT i=16; i; i--, d++)
      *d *= scalar;
    return *this;
  }
/*  FxVector4<T> MultVertex4f (FxVector4<T> V)
  {
    T x = V.x*M[0] + V.y*M[4] + V.z*M[8] +  V.w*M[12];
    T y = V.x*M[1] + V.y*M[5] + V.z*M[9] +  V.w*M[13];
    T z = V.x*M[2] + V.y*M[6] + V.z*M[10] + V.w*M[14];
    T w = V.x*M[3] + V.y*M[7] + V.z*M[11] + V.w*M[15];
    return FxVector4<T>(x, y, z, w);
  }
*/
  HTVector3<T> MultVertex3f (HTVector3<T> V)
  {
    T x = V.x*M[0] + V.y*M[4] + V.z*M[8] +  M[12];
    T y = V.x*M[1] + V.y*M[5] + V.z*M[9] +  M[13];
    T z = V.x*M[2] + V.y*M[6] + V.z*M[10] + M[14];
    //T w = V.x*M[3] + V.y*M[7] + V.z*M[11] + M[15];
	return HTVector3<T>(x, y, z);
  }

  void SetPosition(HTVector3<T> vP) { M[12] = vP.x; M[13] = vP.y; M[14] = vP.z; }
  HTVector3<T> GetPosition() { return HTVector3<T>(M[12], M[13], M[14]); }
  HTVector3<T> GetDirection() { return HTVector3<T>(M[8], M[9], M[10]); }
  HTVector3<T> GetRight() { return HTVector3<T>(M[0], M[1], M[2]); }

  void MatrixLookAtLH (HTVector3<T> pEye, HTVector3<T> pAt,
			HTVector3<T> pUp )
  {
    HTVector3<T> XAxis, YAxis, ZAxis;

	ZAxis = pAt - pEye;

    // Normalize the z basis vector.
	ZAxis = -Normalize(ZAxis);

    // Compute the orthogonal axes from the cross product of the gaze 
    // and the pUp vector.
	XAxis = Cross(pUp, ZAxis);
	XAxis = Normalize(XAxis);
	YAxis = Cross(ZAxis, XAxis);
	//YAxis = pUp;

    // Start building the matrix. The first three rows contain the 
    // basis vectors used to rotate the view to point at the look-at 
    // point. The fourth row contains the translation values. 
    // Rotations are still about the eyepoint.
	M[0] = XAxis.x; M[4] = XAxis.y; M[8] = XAxis.z; M[12] = -Dot(XAxis, pEye);
	M[1] = YAxis.x; M[5] = YAxis.y; M[9] = YAxis.z; M[13] = -Dot(YAxis, pEye);
	M[2] = ZAxis.x; M[6] = ZAxis.y; M[10] = ZAxis.z; M[14] = -Dot(ZAxis, pEye);
	M[3] = 0.0f; M[7] = 0.0f; M[11] = 0.0f; M[15] = 1.0f;
  }
};

template class HTMatrix<FLOAT>;
template class HTMatrix<DOUBLE>;
typedef HTMatrix<FLOAT> HMatrix;
typedef HTMatrix<DOUBLE> HMatrixd;

// Inlines.
/*
template <class T> inline ostream&
operator<<(ostream& s, const HTMatrix<T>& A)
{
  T *p = A();
  for(INT i=4; i; i--) {
    for(INT j=4; j; j--, p+=4)
      s << (FLOAT)(*p) << " ";
    p-=15;
    s << endl;
  }
  return s;
}
*/
template <class T> inline HTMatrix<T>
operator + (const HTMatrix<T>& A)
{
  return A;
}

template <class T> inline HTMatrix<T>
operator - (const HTMatrix<T>& A)
{
  HTMatrix<T> nA;
  T *d = nA();
  T *s = A();
  for(INT i=16; i; i--)
    *d++ = -(*s++);
  return nA;
}

template <class T> inline HTMatrix<T>
operator + (const HTMatrix<T>& A, const HTMatrix<T>& B)
{
  HTMatrix<T> C;
  T *d = C();
  T *sA = A();
  T *sB = B();
  for(INT i=16; i; i--, d++, sA++, sB++)
    *d = *sA + *sB;
  return C;
}

template <class T> inline HTMatrix<T>
operator - (const HTMatrix<T>& A, const HTMatrix<T>& B)
{
  HTMatrix<T> C;
  T *d = C();
  T *sA = A();
  T *sB = B();
  for(INT i=16; i; i--, d++, sA++, sB++)
    *d = *sA - *sB;
  return C;
}

template <class T> inline HTMatrix<T>
operator * (const HTMatrix<T>& A, const HTMatrix<T>& B)
{
  HTMatrix<T> C;
  __glmatrix_multiply(A(), B(), C());
  return C;
}

template <class T> inline HTMatrix<T>
operator * (const HTMatrix<T>& A, const T& scalar)
{
  HTMatrix<T> C;
  T *d = C();
  T *s = A();
  for(INT i=16; i; i--, d++, s++)
    *d = (*s)*scalar;
  return C;
}

template <class T> inline HTMatrix<T>
operator * (const T& scalar, const HTMatrix<T>& A)
{
  HTMatrix<T> C;
  T *d = C();
  T *s = A();
  for(INT i=16; i; i--, d++, s++)
    *d = scalar*(*s);
  return C;
}

template <class T> inline HTMatrix<T>
operator / (const HTMatrix<T>& A, const T& scalar)
{
  HTMatrix<T> C;
  T *d = C();
  T *s = A();
  for(INT i=16; i; i--, d++, s++)
    *d = (*s)/scalar;
  return C;
}

template <class T> inline T
norm (const HTMatrix<T>& A)
{
	T acc = 0;
	T *s = A();
	for(INT i=16; i; i--, s++)
		acc += (*s)*(*s);
	return sqrt(acc);
}

//} // extern "C++"

//#endif

/* -------------------------- End -------------------------- */