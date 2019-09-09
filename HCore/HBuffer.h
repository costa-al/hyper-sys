
enum EBufMode {
	BUF_READ,
	BUF_WRITE,
	BUF_MOVEPOS,
	BUF_STOPPOS
};

// ------------------------------------------------------------
//	HTBuffer.
// ------------------------------------------------------------

template <class T>
class HCORE_API HTBuffer : public HTArray<T> {
protected:
	INT	m_nPos;

public:
	EBufMode m_eRWMode;
	EBufMode m_ePosMode;
	// Constructor.
	HTBuffer(EBufMode rwMode) 
		:	HTArray<T>()
		,	m_eRWMode(rwMode)
		,	m_ePosMode(BUF_MOVEPOS)
		,	m_nPos(0)
	{}
	HTBuffer(EBufMode rwMode, INT Count) 
		:	HTArray<T>(Count)
		,	m_eRWMode(rwMode)
		,	m_ePosMode(BUF_MOVEPOS)
		,	m_nPos(0)
	{}
	//~HTBuffer() { HTArray<T>::~HTArray<T>(); }
	virtual void Serialize( void* V, INT Length ) {
		guard(HTBuffer::Serialize);
		if (m_eRWMode==BUF_WRITE) {
			Add(Length);
			appMemcpy((BYTE*)m_pData+m_nPos, V, Length);
		}
		else {
			appMemcpy(V, (BYTE*)m_pData+m_nPos, Length);
		}
		if (m_eRWMode!=BUF_STOPPOS) {
			m_nPos+=Length;
		}
		unguard;
	}
	virtual INT Tell() {
		return m_nPos;
	}
	virtual HBOOL AtEnd() {
		return m_nPos>=Num();
	}
	virtual void Seek( INT InPos ) {
		check (InPos<ArrayNum);
		m_nPos = InPos;
	}
	// Hardcoded datatype routines that may not be overridden.
	HTBuffer& ByteOrderSerialize( void* V, INT Length ) {
		Serialize( V, Length );
		return *this;
	}
	// Friend archivers.
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, TCHAR& C ) {
		Ar.Serialize( &C, 1 );
		return Ar;
	}
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, INT& I ) {
		Ar.ByteOrderSerialize( &I, sizeof(I) );
		return Ar;
	}
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, HBOOL& I ) {
		Ar.ByteOrderSerialize( &I, sizeof(I) );
		return Ar;
	}
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, FLOAT& F ) {
		Ar.ByteOrderSerialize( &F, sizeof(F) );
		return Ar;
	}
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, DOUBLE& F ) {
		Ar.ByteOrderSerialize( &F, sizeof(F) );
		return Ar;
	}
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, BYTE& B ) {
		Ar.Serialize( &B, 1 );
		return Ar;
	}
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, SBYTE& B ) {
		Ar.Serialize( &B, 1 );
		return Ar;
	}
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, WORD& W ) {
		Ar.ByteOrderSerialize( &W, sizeof(W) );
		return Ar;
	}
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, SWORD& W ) {
		Ar.ByteOrderSerialize( &W, sizeof(W) );
		return Ar;
	}
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, DWORD& D ) {
		Ar.ByteOrderSerialize( &D, sizeof(D) );
		return Ar;
	}
	HCORE_API friend HTBuffer& operator << ( HTBuffer &Ar, QWORD& Q ) {
		Ar.ByteOrderSerialize( &Q, sizeof(Q) );
		return Ar;
	}
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, SQWORD& S ) {
		Ar.ByteOrderSerialize( &S, sizeof(S) );
		return Ar;
	}

	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, TCHAR *Str );
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, const TCHAR *Str );
	HCORE_API friend HTBuffer& operator <= ( HTBuffer& Ar, const TCHAR *Str );

	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, HVar& V );

	// Note: this work only if Ar-Save Other->Load.
	HCORE_API friend HTBuffer& operator << ( HTBuffer& Ar, HTBuffer& Other );

	class FIterator {
	public:
		FIterator( HTBuffer<T> *Buffer )
			:	m_pBuf( Buffer )
		{}
		HBOOL AtEnd()				{ return m_pBuf->AtEnd();			}
		void operator++()			{ m_pBuf->m_nPos += sizeof(T);		}
		void operator++(INT)		{ m_pBuf->m_nPos += sizeof(T);		}
		void operator--()			{ m_pBuf->m_nPos -= sizeof(T);		}
		void operator--(INT)		{ m_pBuf->m_nPos -= sizeof(T);		}
		void operator+=(INT Num)	{ m_pBuf->m_nPos += Num*sizeof(T);	}
		void operator-=(INT Num)	{ m_pBuf->m_nPos -= Num*sizeof(T);	}

		T* operator &()	{ return (T*)((BYTE*)m_pBuf->m_pData + m_pBuf->m_nPos);		}
		T operator *()	{ return *((T*)((BYTE*)m_pBuf->m_pData + m_pBuf->m_nPos));	}
	private:
		HTBuffer<T>* m_pBuf;
	};

	friend class FIterator;
};

template <class T> HTBuffer<T>& operator << ( HTBuffer<T>& Ar, TCHAR *Str ) {
	guard(HTBuffer<T>::operator << (HTBuffer<T>& Ar, TCHAR *Str));
	INT nLen;

	if(Ar.m_eRWMode == BUF_WRITE){
		if (!Str)
			nLen = -1;
		else
			nLen = strlen(Str);
	}
	Ar << nLen;
	if(Ar.m_eRWMode == BUF_READ){
		if (Str)
			delete Str;
		if (nLen==-1)
			return Ar;
		else
			Str = new TCHAR[nLen + 1];
	}

	Ar.Serialize((void*)Str, nLen+1);

	return Ar;
	unguard;
}

template <class T> HTBuffer<T>& operator << ( HTBuffer<T>& Ar, const TCHAR *Str) {
	Ar << (TCHAR*)Str;
	return Ar;
}

template <class T> HTBuffer<T>& operator <= ( HTBuffer<T>& Ar, const TCHAR *Str ) {
	guard(CXBuffer<T>::operator <= (HTBuffer<T>& Ar, const TCHAR *Str));
	
	if(Ar.m_eRWMode == BUF_READ)
		return Ar;
	if (!Str)
		return Ar;

	Ar.Serialize((void*)Str, strlen(Str));
	return Ar;
	unguard;
}

template <class T> HTBuffer<T>& operator << ( HTBuffer<T>& Ar, HTBuffer<T>& Other ) {
	guard(HTBuffer<T>::operator << (HTBuffer<T>& Ar, HTBuffer<T>& Other));
	if((Ar.m_eRWMode == BUF_WRITE) || (Other.m_eRWMode == BUF_READ))
	{
		// Warning! Can't serialize two stream with conflict storing modes!!!
		return Ar;
	}

	Ar.Serialize(Other.m_pData, Other.Num());

	return Ar;
	unguard;
}

template <class T> HTBuffer<T>& operator << ( HTBuffer<T>& Ar, HTArray<T>& A ) {
	guard(HTBuffer<T>::operator << (HTBuffer<T>& Ar, HTArray<T>& A));
	if(sizeof(T) == 1)
	{
		// Serialize simple bytes which require no construction or destruction.
		//Ar << A.m_nArrayNum;
		if( Ar.m_eRWMode == BUF_READ )
		{
			A.m_nArrayMax = A.m_nArrayNum;
			A.Realloc( sizeof(T) );
		}
		Ar.Serialize( &A[0], A.Num() );
	}
	else if( Ar.m_eRWMode == BUF_READ )
	{
		// Load array.
		INT NewNum;
		//Ar << NewNum;
		A.Empty( NewNum );
		for( INT i=0; i<NewNum; i++ ){
			Ar << *new(A)T;
		}
	}
	else
	{
		// Save array.
		Ar << A.m_nArrayNum;
		for( INT i=0; i<A.m_nArrayNum; i++ )
			Ar << A[i];
	}
	return Ar;
	unguard;
}

/* -------------------------- End -------------------------- */
