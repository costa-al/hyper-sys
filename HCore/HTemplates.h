/*-----------------------------------------------------------------------------
	Type information.
-----------------------------------------------------------------------------*/

//
// Type information for initialization.
//
template <class T> struct FTTypeInfoBase
{
public:
	typedef const T& ConstInitType;
	static HBOOL NeedsDestructor()				{ return 1; }
	static HBOOL DefinitelyNeedsDestructor()	{ return 0; }
	static const T& ToInit( const T& In )		{ return In; }
};
template <class T> struct HTTypeInfo : public FTTypeInfoBase<T>
{
};

template <> struct HTTypeInfo<BYTE> : public FTTypeInfoBase<BYTE>
{
public:
	static HBOOL NeedsDestructor() {return 0;}
};
template <> struct HTTypeInfo<SBYTE> : public FTTypeInfoBase<SBYTE>
{
public:
	static HBOOL NeedsDestructor() {return 0;}
};
template <> struct HTTypeInfo<ANSICHAR> : public FTTypeInfoBase<ANSICHAR>
{
public:
	static HBOOL NeedsDestructor() {return 0;}
};
template <> struct HTTypeInfo<INT> : public FTTypeInfoBase<INT>
{
public:
	static HBOOL NeedsDestructor() {return 0;}
};
template <> struct HTTypeInfo<DWORD> : public FTTypeInfoBase<DWORD>
{
public:
	static HBOOL NeedsDestructor() {return 0;}
};
template <> struct HTTypeInfo<_WORD> : public FTTypeInfoBase<_WORD>
{
public:
	static HBOOL NeedsDestructor() {return 0;}
};
template <> struct HTTypeInfo<SWORD> : public FTTypeInfoBase<SWORD>
{
public:
	static HBOOL NeedsDestructor() {return 0;}
};
template <> struct HTTypeInfo<QWORD> : public FTTypeInfoBase<QWORD>
{
public:
	static HBOOL NeedsDestructor() {return 0;}
};
template <> struct HTTypeInfo<SQWORD> : public FTTypeInfoBase<SQWORD>
{
public:
	static HBOOL NeedsDestructor() {return 0;}
};

/*-----------------------------------------------------------------------------
	Standard templates.
-----------------------------------------------------------------------------*/

template< class T > inline T Abs( const T A )
{
	return (A>=(T)0) ? A : -A;
}
template< class T > inline T Sgn( const T A )
{
	return (A>0) ? 1 : ((A<0) ? -1 : 0);
}
template< class T > inline T Max( const T A, const T B )
{
	return (A>=B) ? A : B;
}
template< class T > inline T Min( const T A, const T B )
{
	return (A<=B) ? A : B;
}
template< class T > inline T Square( const T A )
{
	return A*A;
}
template< class T > inline T Clamp( const T X, const T Min, const T Max )
{
	return X<Min ? Min : X<Max ? X : Max;
}
template< class T > inline T Align( const T Ptr, INT Alignment )
{
	return (T)(((DWORD)Ptr + Alignment - 1) & ~(Alignment-1));
}
template< class T > inline void Exchange( T& A, T& B )
{
	const T Temp = A;
	A = B;
	B = Temp;
}
template< class T > T Lerp( T& A, T& B, FLOAT Alpha )
{
	return A + Alpha * (B-A);
}
inline DWORD GetTypeHash( const BYTE A )
{
	return A;
}
inline DWORD GetTypeHash( const SBYTE A )
{
	return A;
}
inline DWORD GetTypeHash( const _WORD A )
{
	return A;
}
inline DWORD GetTypeHash( const SWORD A )
{
	return A;
}
inline DWORD GetTypeHash( const INT A )
{
	return A;
}
inline DWORD GetTypeHash( const DWORD A )
{
	return A;
}
inline DWORD GetTypeHash( const QWORD A )
{
	return (DWORD)A+((DWORD)(A>>32) * 23);
}
inline DWORD GetTypeHash( const SQWORD A )
{
	return (DWORD)A+((DWORD)(A>>32) * 23);
}
inline DWORD GetTypeHash( const TCHAR* S )
{
	return appStrihash(S);
}
#define ExchangeB(A,B) { HBOOL T=A; A=B; B=T; }

/*----------------------------------------------------------------------------
	Standard macros.
----------------------------------------------------------------------------*/

// Number of elements in an array.
#define ARRAY_COUNT( array ) \
	( sizeof(array) / sizeof((array)[0]) )

// Offset of a struct member.
#define STRUCT_OFFSET( struc, member ) \
	( (INT)&((struc*)NULL)->member )

/*-----------------------------------------------------------------------------
	Dynamic array template.
-----------------------------------------------------------------------------*/

//
// Base dynamic array.
//
class HCORE_API FArray {
protected:
	FArray( INT nInNum, INT nElementSize )
	:	m_nArrayNum( nInNum )
	,	m_nArrayMax( nInNum )
	,	m_pData    ( NULL  )
	{
		Realloc( nElementSize );
	}

	void Realloc( INT nElementSize ) {
		guard(FArray::Realloc);

		if((INT)m_pData<0) m_pData = NULL;

		if(!m_nArrayMax && m_pData)
				appFree(m_pData), m_pData = NULL;

		else if(m_nArrayMax)
		{
			if(!m_pData)
				#ifdef _DEBUG
					m_pData = appMalloc(m_nArrayMax * nElementSize, "FArray");
				#else
					m_pData = appMalloc(m_nArrayMax * nElementSize);
				#endif
			else
				#ifdef _DEBUG
					m_pData = appRealloc(m_pData, m_nArrayMax * nElementSize, "FArray");
				#else
					m_pData = appRealloc(m_pData, m_nArrayMax * nElementSize);
				#endif
		}
		unguard;
	}

	void* m_pData;
	INT	  m_nArrayNum;
	INT	  m_nArrayMax;

public:
	void* GetData()						{ return m_pData; }
	const void* GetData() const			{ return m_pData; }
	HBOOL IsValidIndex( INT i ) const	{ return i>=0 && i<m_nArrayNum; }
	INT Num() const {
		checkSlow(m_nArrayNum>=0);
		checkSlow(m_nArrayMax>=m_nArrayNum);
		return m_nArrayNum;
	}
	void InsertZeroed( INT nIndex, INT nCount, INT nElementSize ) {
		guardSlow(FArray::InsertZeroed);
		Insert( nIndex, nCount, nElementSize );
		appMemzero( (BYTE*)m_pData+nIndex*nElementSize, nCount*nElementSize );
		unguardSlow;
	}
	void Insert( INT nIndex, INT nCount, INT nElementSize ) {
		guardSlow(FArray::Insert);
		checkSlow(nCount>=0);
		checkSlow(m_nArrayNum>=0);
		checkSlow(m_nArrayMax>=m_nArrayNum);
		checkSlow(nIndex>=0);
		checkSlow(nIndex<=m_nArrayNum);

		INT OldNum = m_nArrayNum;
		if( (m_nArrayNum+=nCount)>m_nArrayMax )
		{
			m_nArrayMax = m_nArrayNum + 3*m_nArrayNum/8 + 32;
			Realloc( nElementSize );
		}
		appMemcpy
		(
			(BYTE*)m_pData + (nIndex+nCount )*nElementSize,
			(BYTE*)m_pData + (nIndex       )*nElementSize,
			              (OldNum-nIndex)*nElementSize
		);

		unguardSlow;
	}
	INT Add( INT nCount, INT nElementSize ) {
		guardSlow(FArray::Add);
		checkSlow(nCount>=0);
		checkSlow(m_nArrayNum>=0);
		checkSlow(m_nArrayMax>=m_nArrayNum);

		INT nIndex = m_nArrayNum;
		if( (m_nArrayNum+=nCount)>m_nArrayMax )
		{
			m_nArrayMax = m_nArrayNum + 3*m_nArrayNum/8 + 32;
			Realloc( nElementSize );
		}

		return nIndex;
		unguardSlow;
	}
	INT AddZeroed( INT nElementSize, INT n=1 ) {
		guardSlow(FArray::AddZeroed);
		INT nIndex = Add( n, nElementSize );
		appMemzero( (BYTE*)m_pData+nIndex*nElementSize, n*nElementSize );
		return nIndex;
		unguardSlow;
	}
	void Shrink( INT nElementSize ) {
		guardSlow(FArray::Shrink);
		checkSlow(m_nArrayNum>=0);
		checkSlow(m_nArrayMax>=m_nArrayNum);
		if( m_nArrayMax != m_nArrayNum )
		{
			m_nArrayMax = m_nArrayNum;
			Realloc( nElementSize );
		}
		unguardSlow;
	}
	void Empty( INT nElementSize, INT nSlack=0 ) {
		guardSlow(FArray::Empty);
		m_nArrayNum = 0;
		m_nArrayMax = nSlack;
		Realloc( nElementSize );
		unguardSlow;
	}
	FArray()
	:	m_nArrayNum( 0 )
	,	m_nArrayMax( 0 )
	,	m_pData	( NULL )
	{}
	FArray( ENoInit )
	{}
	~FArray() {
		guardSlow(FArray::~FArray);
		if( m_pData )
			appFree( m_pData );
		m_pData = NULL;
		m_nArrayNum = m_nArrayMax = 0;
		unguardSlow;
	}

	void Remove( INT nIndex, INT nCount, INT nElementSize ) {
		guard(FArray::Remove)
		check(nIndex>=0);
		check(nIndex+nCount<=m_nArrayMax);

		if (nIndex+nCount<m_nArrayNum) {
			appMemcpy(	(BYTE*)m_pData + (nIndex	  ) * nElementSize,
						(BYTE*)m_pData + (nIndex+nCount) * nElementSize,
						(m_nArrayNum - nCount) * nElementSize );
		}
		m_nArrayNum -= nCount;

		if ((m_nArrayMax-m_nArrayNum)>(6*m_nArrayNum/8 + 32)) {
			m_nArrayMax = m_nArrayNum + 3*m_nArrayNum/8 + 32;
			Realloc(nElementSize);
		}
		unguard;
	}

};

//
// Templated dynamic array.
//
template< class T > class HTArray : public FArray {
public:
	typedef T m_ElementType;
	HTArray()
	:	FArray()
	{}
	HTArray( INT nInNum )
	:	FArray( nInNum, sizeof(T) )
	{}
	HTArray( const HTArray& Other )
	:	FArray( Other.m_nArrayNum, sizeof(T) )
	{
		guardSlow(HTArray::copyctor);
		if( HTTypeInfo<T>::NeedsDestructor() )
		{
			m_nArrayNum=0;
			//for( INT i=0; i<Other.m_nArrayNum; i++ )
			//	new(*this)T(Other(i));
			for( int i=0; i<Other.m_nArrayNum; i++ )
				AddItem(Other(i));
		}
		else if( sizeof(T)!=1 )
		{
			for( INT i=0; i<m_nArrayNum; i++ )
				(*this)(i) = Other(i);
		}
		else
		{
			appMemcpy( &(*this)(0), &Other(0), m_nArrayNum * sizeof(T) );
		}
		unguardSlow;
	}
	HTArray( ENoInit )
	: FArray( E_NoInit )
	{}
	~HTArray() {
		checkSlow(m_nArrayNum>=0);
		checkSlow(m_nArrayMax>=m_nArrayNum);
		Remove( 0, m_nArrayNum );
	}
    T& operator()( INT i ) {
		guardSlow(HTArray::operator());
		checkSlow(i>=0);
		checkSlow(i<=m_nArrayNum);
		checkSlow(m_nArrayMax>=m_nArrayNum);
		return ((T*)m_pData)[i];
		unguardSlow;
	}
	const T& operator()( INT i ) const {
		guardSlow(HTArray::operator());
		checkSlow(i>=0);
		checkSlow(i<=m_nArrayNum);
		checkSlow(m_nArrayMax>=m_nArrayNum);
		return ((T*)m_pData)[i];
		unguardSlow;
	}
	T Pop() {
		guardSlow(HTArray::Pop);
		check(m_nArrayNum>0);
		checkSlow(m_nArrayMax>=m_nArrayNum);
		T Result = ((T*)m_pData)[m_nArrayNum-1];
		Remove( m_nArrayNum-1 );
		return Result;
		unguardSlow;
	}
	T& Last( INT c=0 ) {
		guardSlow(HTArray::Last);
		check(c<m_nArrayNum);
		checkSlow(m_nArrayMax>=m_nArrayNum);
		return ((T*)m_pData)[m_nArrayNum-c-1];
		unguardSlow;
	}
	const T& Last( INT c=0 ) const {
		guardSlow(HTArray::Last);
		checkSlow(c<m_nArrayNum);
		checkSlow(m_nArrayMax>=m_nArrayNum);
		return ((T*)m_pData)[m_nArrayNum-c-1];
		unguardSlow;
	}
	void Shrink() {
		guardSlow(HTArray::Shrink);
		FArray::Shrink( sizeof(T) );
		unguardSlow;
	}
	HBOOL FindItem( const T& Item, INT& nIndex ) const {
		guardSlow(HTArray::FindItem);
		for( nIndex=0; nIndex<m_nArrayNum; nIndex++ )
			if( (*this)(nIndex)==Item )
				return 1;
		return 0;
		unguardSlow;
	}
	INT FindItemIndex( const T& Item ) const {
		guardSlow(HTArray::FindItemIndex);
		for( INT nIndex=0; nIndex<m_nArrayNum; nIndex++ )
			if( (*this)(nIndex)==Item )
				return nIndex;
		return INDEX_NONE;
		unguardSlow;
	}

	// Add, Insert, Remove, Empty interface.
	INT Add( INT n=1 ) {
		guardSlow(HTArray::Add);
		checkSlow(!HTTypeInfo<T>::DefinitelyNeedsDestructor());
		return FArray::Add( n, sizeof(T) );
		unguardSlow;
	}
	void Insert( INT nIndex, INT nCount=1 ) {
		guardSlow(HTArray::Insert);
		checkSlow(!HTTypeInfo<T>::DefinitelyNeedsDestructor());
		FArray::Insert( nIndex, nCount, sizeof(T) );
		unguardSlow;
	}
	void InsertZeroed( INT nIndex, INT nCount=1 ) {
		guardSlow(HTArray::InsertZeroed);
		checkSlow(!HTTypeInfo<T>::DefinitelyNeedsDestructor());
		FArray::InsertZeroed( nIndex, nCount, sizeof(T) );
		unguardSlow;
	}
	void Remove( INT nIndex, INT nCount=1 ) {
		guardSlow(HTArray::Remove);
		check(nIndex>=0);
		check(nIndex<=m_nArrayNum);
		check(nIndex+nCount<=m_nArrayNum);
		if( HTTypeInfo<T>::NeedsDestructor() )
			for( INT i=nIndex; i<nIndex+nCount; i++ )
				(&(*this)(i))->~T();
		FArray::Remove( nIndex, nCount, sizeof(T) );
		unguardSlow;
	}
	void Empty( INT nSlack=0 ) {
		guardSlow(HTArray::Empty);
		if( HTTypeInfo<T>::NeedsDestructor() )
			for( INT i=0; i<m_nArrayNum; i++ )
				(&(*this)(i))->~T();
		FArray::Empty( sizeof(T), nSlack );
		unguardSlow;
	}

	// Functions dependent on Add, Remove.
	HTArray& operator=( const HTArray& Other ) {
		guardSlow(HTArray::operator=);
		if( this != &Other )
		{
			Empty( Other.m_nArrayNum );
			for( INT i=0; i<Other.m_nArrayNum; i++ )
				//new( *this )T( Other(i) );
				AddItem(Other(i));
		}
		return *this;
		unguardSlow;
	}
	INT AddItem( const T& Item ) {
		guardSlow(HTArray::AddItem);
		checkSlow(!HTTypeInfo<T>::DefinitelyNeedsDestructor());
		INT nIndex=Add();
		(*this)(nIndex)=Item;
		return nIndex;
		unguardSlow;
	}
	INT AddZeroed( INT n=1 ) {
		guardSlow(HTArray::AddZeroed);
		return FArray::AddZeroed( sizeof(T), n );
		unguardSlow;
	}
	INT AddUniqueItem( const T& Item ) {
		guardSlow(HTArray::AddUniqueItem);
		checkSlow(!HTTypeInfo<T>::DefinitelyNeedsDestructor());
		for( INT nIndex=0; nIndex<m_nArrayNum; nIndex++ )
			if( (*this)(nIndex)==Item )
				return nIndex;
		return AddItem( Item );
		unguardSlow;
	}
	INT RemoveItem( const T& Item ) {
		guardSlow(HTArray::RemoveItem);
		INT OriginalNum=m_nArrayNum;
		for( INT nIndex=0; nIndex<m_nArrayNum; nIndex++ )
			if( (*this)(nIndex)==Item )
				Remove( nIndex-- );
		return OriginalNum - m_nArrayNum;
		unguardSlow;
	}

	// Iterator.
	class FIterator {
	public:
		FIterator( HTArray<T>& InArray )
			:	m_Array(InArray)
			,	m_nIndex(INDEX_NONE)
		{ ++*this; }
		void operator++()      { ++m_nIndex;														}
		void RemoveCurrent()   { m_Array.Remove(m_nIndex--);									}
		INT GetIndex()   const { return m_nIndex;												}
		operator HBOOL() const { return m_nIndex < m_Array.Num();								}
		T& operator*()   const { return m_Array(m_nIndex);										}
		T* operator->()  const { return &m_Array(m_nIndex);										}
		T& GetCurrent()  const { return m_Array( m_nIndex );									}
		T& GetPrev()     const { return m_Array( m_nIndex ? m_nIndex-1 : m_Array.Num()-1 );		}
		T& GetNext()     const { return m_Array( m_nIndex<m_Array.Num()-1 ? m_nIndex+1 : 0 );	}
	private:
		HTArray<T>& m_Array;
		INT m_nIndex;
	};
};

// --------------------------------------------
//	Array operator news.
// --------------------------------------------

#ifdef _DEBUG
	#undef new
	template <class T> void* operator new( unsigned int Size, HTArray<T>& Array)
	#define new ___DEBUG_NEW
#else
	template <class T> void* operator new( unsigned int Size, HTArray<T>& Array)
#endif
{
	guard(HTArray<T>::operator new);
	int Index = Array.Add();
	return &Array(Index);
	unguard;
}
#ifdef _DEBUG
	#undef new
	template <class T> void* operator new( unsigned int Size, HTArray<T>& Array, INT Index)
	#define new ___DEBUG_NEW
#else
	template <class T> void* operator new( unsigned int Size, HTArray<T>& Array, INT Index)
#endif
{
	guard(HTArray<T>::operator new);
	Array.Insert(Index);
	return &Array(Index);
	unguard;
}

/*-----------------------------------------------------------------------------
	Dynamic strings.
-----------------------------------------------------------------------------*/

//
// A dynamically sizeable string.
//
class HCORE_API FString : protected HTArray<TCHAR> {
public:
	FString()
	: HTArray<TCHAR>()
	{}
	FString( const FString& Other )
	: HTArray<TCHAR>( Other.m_nArrayNum )
	{
		if( m_nArrayNum )
			appMemcpy( &(*this)(0), &Other(0), m_nArrayNum*sizeof(TCHAR) );
	}
	FString( const TCHAR* In )
	: HTArray<TCHAR>( *In ? (appStrlen(In)+1) : 0 )
	{
		if( m_nArrayNum )
			appMemcpy( &(*this)(0), In, m_nArrayNum*sizeof(TCHAR) );
	}
	FString( ENoInit )
	: HTArray<TCHAR>( E_NoInit )
	{}
	explicit FString( BYTE   Arg, INT Digits=1 );
	explicit FString( SBYTE  Arg, INT Digits=1 );
	explicit FString( _WORD  Arg, INT Digits=1 );
	explicit FString( SWORD  Arg, INT Digits=1 );
	explicit FString( INT    Arg, INT Digits=1 );
	explicit FString( DWORD  Arg, INT Digits=1 );
	explicit FString( FLOAT  Arg, INT Digits=1, INT RightDigits=0, HBOOL LeadZero=1 );
	FString& operator=( const TCHAR* Other ) {
		if( &(*this)(0)!=Other )
		{
			m_nArrayNum = m_nArrayMax = *Other ? appStrlen(Other)+1 : 0;
			Realloc( sizeof(TCHAR) );
			if( m_nArrayNum )
				appMemcpy( &(*this)(0), Other, m_nArrayNum*sizeof(TCHAR) );
		}
		return *this;
	}
	FString& operator=( const FString& Other ) {
		if( this != &Other )
		{
			m_nArrayNum = m_nArrayMax = Other.Num();
			Realloc( sizeof(TCHAR) );
			if( m_nArrayNum )
				appMemcpy( &(*this)(0), *Other, m_nArrayNum*sizeof(TCHAR) );
		}
		return *this;
	}
	~FString() {
		HTArray<TCHAR>::Empty();		
	}
	void Empty() {
		HTArray<TCHAR>::Empty();
	}
	void Shrink() {
		HTArray<TCHAR>::Shrink();
	}
	const TCHAR* operator*() const {
		return Num() ? &(*this)(0) : TEXT("");
	}
	operator HBOOL() const {
		return Num()!=0;
	}
	HTArray<TCHAR>& GetCharArray() {
		//warning: Operations on the HTArray<CHAR> can be unsafe, such as adding
		// non-terminating 0's or removing the terminating zero.
		return (HTArray<TCHAR>&)*this;
	}
	FString& operator+=( const TCHAR* Str ) {
		if( m_nArrayNum )
		{
			INT nIndex = m_nArrayNum-1;
			Add( appStrlen(Str) );
			strcpy( &(*this)(nIndex), Str );
		}
		else if( *Str )
		{
			Add( appStrlen(Str)+1 );
			strcpy( &(*this)(0), Str );
		}
		return *this;
	}
	FString& operator+=( const FString& Str ) {
		return operator+=( *Str );
	}
	FString operator+( const TCHAR* Str ) {
		return FString( *this ) += Str;
	}
	FString operator+( const FString& Str ) {
		return operator+( *Str );
	}
	FString& operator*=( const TCHAR* Str ) {
		if( m_nArrayNum>1 && (*this)(m_nArrayNum-2)!=PATH_SEPARATOR[0] )
			*this += PATH_SEPARATOR;
		return *this += Str;
	}
	FString& operator*=( const FString& Str ) {
		return operator*=( *Str );
	}
	FString operator*( const TCHAR* Str ) const {
		return FString( *this ) *= Str;
	}
	FString operator*( const FString& Str ) const {
		return operator*( *Str );
	}
	HBOOL operator<=( const TCHAR* Other ) const {
		return !(stricmp( **this, Other ) > 0);
	}
	HBOOL operator<( const TCHAR* Other ) const {
		return stricmp( **this, Other ) < 0;
	}
	HBOOL operator>=( const TCHAR* Other ) const {
		return !(stricmp( **this, Other ) < 0);
	}
	HBOOL operator>( const TCHAR* Other ) const {
		return stricmp( **this, Other ) > 0;
	}
	HBOOL operator==( const TCHAR* Other ) const {
		return stricmp( **this, Other )==0;
	}
	HBOOL operator==( const FString& Other ) const {
		return stricmp( **this, *Other )==0;
	}
	HBOOL operator!=( const TCHAR* Other ) const {
		return stricmp( **this, Other )!=0;
	}
	HBOOL operator!=( const FString& Other ) const {
		return stricmp( **this, *Other )!=0;
	}

	INT Len() const {
		return Num() ? Num()-1 : 0;
	}
	FString Left( INT nCount ) const {
		return FString( Clamp(nCount,0,Len()), **this );
	}
	FString LeftChop( INT nCount ) const {
		return FString( Clamp(Len()-nCount,0,Len()), **this );
	}
	FString bRight( INT nCount ) const {
		return FString( **this + Len()-Clamp(nCount,0,Len()) );
	}
	FString Mid( INT nStart, INT nCount=MAXINT ) const {
		DWORD End = nStart+nCount;
		nStart    = Clamp( (DWORD)nStart, (DWORD)0,     (DWORD)Len() );
		End      = Clamp( (DWORD)End,   (DWORD)nStart, (DWORD)Len() );
		return FString( End-nStart, **this + nStart );
	}
	INT InStr( const TCHAR* pSubStr, HBOOL bRight=0 ) const {
		if( !bRight )
		{
			TCHAR* Tmp = strstr(**this,pSubStr);
			return Tmp ? (Tmp-**this) : -1;
		}
		else
		{
			for( INT i=Len()-1; i>=0; i-- )
			{
				INT j;
				for( j=0; pSubStr[j]; j++ )
					if( (*this)(i+j)!=pSubStr[j] )
						break;
				if( !pSubStr[j] )
					return i;
			}
			return -1;
		}
	}
	INT InStr( const FString& pSubStr, HBOOL bRight=0 ) const {
		return InStr( *pSubStr, bRight );
	}
	HBOOL Split( const FString& InS, FString* LeftS, FString* RightS, HBOOL bRight=0 ) const {
		INT InPos = InStr(InS,bRight);
		if( InPos<0 )
			return 0;
		if( LeftS )
			*LeftS = Left(InPos);
		if( RightS )
			*RightS = Mid(InPos+InS.Len());
		return 1;
	}
	FString Caps() const {
		FString New( **this );
		for( INT i=0; i<m_nArrayNum; i++ )
			New(i) = appToUpper(New(i));
		return New;
	}
	FString Locs() const {
		FString New( **this );
		for( INT i=0; i<m_nArrayNum; i++ )
			New(i) = appToLower(New(i));
		return New;
	}
	FString LeftPad( INT nChCount );
	FString RightPad( INT nChCount );
	static FString Printf( const TCHAR* Fmt, ... );
	static FString Chr( TCHAR Ch );
	friend struct FXStringNoInit;
private:
	FString( INT nInCount, const TCHAR* pInSrc )
	:	HTArray<TCHAR>( nInCount ? nInCount+1 : 0 )
	{
		if( m_nArrayNum )
			strncpy( &(*this)(0), pInSrc, nInCount+1 );
	}
};
struct HCORE_API FXStringNoInit : public FString {
	FXStringNoInit()
	: FString( E_NoInit )
	{}
	FXStringNoInit& operator=( const TCHAR* Other )
	{
		if( &(*this)(0)!=Other )
		{
			m_nArrayNum = m_nArrayMax = *Other ? appStrlen(Other)+1 : 0;
			Realloc( sizeof(TCHAR) );
			if( m_nArrayNum )
				appMemcpy( &(*this)(0), Other, m_nArrayNum*sizeof(TCHAR) );
		}
		return *this;
	}
	FXStringNoInit& operator=( const FString& Other )
	{
		if( this != &Other )
		{
			m_nArrayNum = m_nArrayMax = Other.Num();
			Realloc( sizeof(TCHAR) );
			if( m_nArrayNum )
				appMemcpy( &(*this)(0), *Other, m_nArrayNum*sizeof(TCHAR) );
		}
		return *this;
	}
};
inline DWORD GetTypeHash( const FString& S ) {
	return appStrihash(*S);
}
template <> struct HTTypeInfo<FString> : public FTTypeInfoBase<FString> {
	typedef const TCHAR* ConstInitType;
	static const TCHAR* ToInit( const FString& In )	{ return *In;	}
	static HBOOL DefinitelyNeedsDestructor()			{ return 0;		}
};

/*----------------------------------------------------------------------------
	FTMapBase.
----------------------------------------------------------------------------*/

//
// Maps unique keys to values.
//
template< class TK, class TI > class FTMapBase
{
protected:
	class FPair
	{
	public:
		INT HashNext;
		TK Key;
		TI Value;
		FPair( HTTypeInfo<TK>::ConstInitType InKey, HTTypeInfo<TI>::ConstInitType InValue )
		:	Key( InKey )
		,	Value( InValue )
		{}
		FPair()
		{}
	};
	void Rehash()
	{
		guardSlow(FTMapBase::Rehash);
		checkSlow(!(nHashCount&(nHashCount-1)));
		checkSlow(nHashCount>=8);
		INT* pNewHash = new INT[nHashCount];
		{for( INT i=0; i<nHashCount; i++ )
		{
			pNewHash[i] = INDEX_NONE;
		}}
		{for( INT i=0; i<Pairs.Num(); i++ )
		{
			FPair& Pair    = Pairs(i);
			INT    nHash   = (GetTypeHash(Pair.Key) & (nHashCount-1));
			Pair.HashNext  = pNewHash[nHash];
			pNewHash[nHash] = i;
		}}
		if( pHash )
			delete pHash;
		pHash = pNewHash;
		unguardSlow;
	}
	void Relax()
	{
		guardSlow(FTMapBase::Relax);
		while( nHashCount>Pairs.Num()*2+8 )
			nHashCount /= 2;
		Rehash();
		unguardSlow;
	}
	TI& Add( HTTypeInfo<TK>::ConstInitType InKey, HTTypeInfo<TI>::ConstInitType InValue )
	{
		guardSlow(FTMapBase::Add);
		#ifdef _DEBUG
			#undef new
		#endif
		FPair& Pair   = *new(Pairs)FPair( InKey, InValue );
		#ifdef _DEBUG
			#define new ___DEBUG_NEW
		#endif

		INT    nHash  = (GetTypeHash(Pair.Key) & (nHashCount-1));
		Pair.HashNext = pHash[nHash];
		pHash[nHash]   = Pairs.Num()-1;
		if( nHashCount*2+8 < Pairs.Num() )
		{
			nHashCount *= 2;
			Rehash();
		}
		return Pair.Value;
		unguardSlow;
	}
	HTArray<FPair> Pairs;
	INT* pHash;
	INT nHashCount;
public:
	FPair& operator [] (INT i) {
		return Pairs(i);
	}

	FTMapBase()
	:	pHash( NULL )
	,	nHashCount( 8 )
	{
		guardSlow(FTMapBase::FTMapBase);
		Rehash();
		unguardSlow;
	}
	FTMapBase( const FTMapBase& Other )
	:	Pairs( Other.Pairs )
	,	nHashCount( Other.nHashCount )
	,	pHash( NULL )
	{
		guardSlow(FTMapBase::FTMapBase copy);
		Rehash();
		unguardSlow;
	}
	~FTMapBase()
	{
		guardSlow(FTMapBase::~FTMapBase);
		if( pHash )
			delete pHash;
		pHash = NULL;
		nHashCount = 0;
		unguardSlow;
	}
	FTMapBase& operator=( const FTMapBase& Other )
	{
		guardSlow(FTMapBase::operator=);
		Pairs     = Other.Pairs;
		nHashCount = Other.nHashCount;
		Rehash();
		return *this;
		unguardSlow;
	}
	void Empty()
	{
		guardSlow(FTMapBase::Empty);
		checkSlow(!(nHashCount&(nHashCount-1)));
		Pairs.Empty();
		nHashCount = 8;
		Rehash();
		unguardSlow;
	}
	TI& Set( HTTypeInfo<TK>::ConstInitType InKey, HTTypeInfo<TI>::ConstInitType InValue )
	{
		guardSlow(HTMap::Set);
		for( INT i=pHash[(GetTypeHash(InKey) & (nHashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==InKey )
				{Pairs(i).Value=InValue; return Pairs(i).Value;}
		return Add( InKey, InValue );
		unguardSlow;
	}
	TI& operator [] ( const TK& InKey )
	{
		guard(FTMapBase::[]);
		TI* Pair = Find(InKey);
		return *Pair;
		unguard;
	}
	TI& operator [] ( const TK* InKey )
	{
		guard(FTMapBase::[]);
		TI* Pair = Find(InKey);
		return *Pair;
		unguard;
	}
	INT Remove( HTTypeInfo<TK>::ConstInitType InKey )
	{
		guardSlow(FTMapBase::Remove);
		INT nCount=0;
		for( INT i=Pairs.Num()-1; i>=0; i-- )
			if( Pairs(i).Key==InKey )
				{Pairs.Remove(i); nCount++;}
		if( nCount )
			Relax();
		return nCount;
		unguardSlow;
	}
	TI* Find( const TK& Key )
	{
		guardSlow(FTMapBase::Find);
		for( INT i=pHash[(GetTypeHash(Key) & (nHashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key )
				return &Pairs(i).Value;
		return NULL;
		unguardSlow;
	}
	TI FindRef( const TK& Key )
	{
		guardSlow(FTMapBase::Find);
		for( INT i=pHash[(GetTypeHash(Key) & (nHashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key )
				return Pairs(i).Value;
		return NULL;
		unguardSlow;
	}
	const TI* Find( const TK& Key ) const
	{
		guardSlow(FTMapBase::Find);
		for( INT i=pHash[(GetTypeHash(Key) & (nHashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key )
				return &Pairs(i).Value;
		return NULL;
		unguardSlow;
	}
	INT FindIndex( const TK& Key )
	{
		guardSlow(FTMapBase::FindIndex);
		for( INT i=pHash[(GetTypeHash(Key) & (nHashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key )
				return i;
		return INDEX_NONE;
		unguardSlow;
	}
	const INT FindIndex( const TK& Key ) const
	{
		guardSlow(FTMapBase::FindIndex);
		for( INT i=pHash[(GetTypeHash(Key) & (nHashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key )
				return i;
		return INDEX_NONE;
		unguardSlow;
	}
	void Dump( FOutputDevice& Ar )
	{
		guard(FTMapBase::Dump);
		Ar.Logf( TEXT("FTMapBase: %i items, %i hash slots"), Pairs.Num(), nHashCount );
		for( INT i=0; i<nHashCount; i++ )
		{
			INT c=0;
			for( INT j=pHash[i]; j!=INDEX_NONE; j=Pairs(j).HashNext )
				c++;
			Ar.Logf( TEXT("   pHash[%i] = %i"), i, c );
		}
		unguard;
	}
	class FIterator
	{
	public:
		FIterator( FTMapBase& InMap )
			:	Pairs( InMap.Pairs )
			,	nIndex( 0 )
		{}
		void operator++()			{ ++nIndex;						}
		void RemoveCurrent()		{ Pairs.Remove(nIndex--);		}
		operator HBOOL() const		{ return nIndex<Pairs.Num();	}
		TK& Key() const				{ return Pairs(nIndex).Key;		}
		TI& Value() const			{ return Pairs(nIndex).Value;	}

		TI operator*() const		{ return Pairs(nIndex).Value;	}
		FPair* operator->() const	{ return &Pairs(nIndex);		}

	private:
		HTArray<FPair>& Pairs;
		INT nIndex;
	};
	friend class FIterator;
};
template< class TK, class TI > class HTMap : public FTMapBase<TK,TI>
{
public:
	HTMap& operator=( const HTMap& Other )
	{
		FTMapBase<TK,TI>::operator=( Other );
		return *this;
	}
	INT Num()
	{
		guardSlow(HTMap::Num);
		return Pairs.Num();
		unguardSlow;
	}
};
template< class TK, class TI > class HTMultiMap : public FTMapBase<TK,TI>
{
public:
	HTMultiMap& operator=( const HTMultiMap& Other )
	{
		FTMapBase<TK,TI>::operator=( Other );
		return *this;
	}
	void MultiFind( const TK& Key, HTArray<TI>& Values ) 
	{
		guardSlow(HTMap::MultiFind);
		for( INT i=pHash[(GetTypeHash(Key) & (nHashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key )
				new(Values)TI(Pairs(i).Value);
		unguardSlow;
	}
	TI& Add( HTTypeInfo<TK>::ConstInitType InKey, HTTypeInfo<TI>::ConstInitType InValue )
	{
		return FTMapBase<TK,TI>::Add( InKey, InValue );
	}
	TI& AddUnique( HTTypeInfo<TK>::ConstInitType InKey, HTTypeInfo<TI>::ConstInitType InValue )
	{
		for( INT i=pHash[(GetTypeHash(InKey) & (nHashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==InKey && Pairs(i).Value==InValue )
				return Pairs(i).Value;
		return Add( InKey, InValue );
	}
	INT RemovePair( HTTypeInfo<TK>::ConstInitType InKey, HTTypeInfo<TI>::ConstInitType InValue )
	{
		guardSlow(HTMap::Remove);
		INT nCount=0;
		for( INT i=Pairs.Num()-1; i>=0; i-- )
			if( Pairs(i).Key==InKey && Pairs(i).Value==InValue )
				{Pairs.Remove(i); nCount++;}
		if( nCount )
			Relax();
		return nCount;
		unguardSlow;
	}
	TI* FindPair( const TK& Key, const TK& Value )
	{
		guardSlow(HTMap::Find);
		for( INT i=pHash[(GetTypeHash(Key) & (nHashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key && Pairs(i).Value==Value )
				return &Pairs(i).Value;
		return NULL;
		unguardSlow;
	}
	INT Num()
	{
		guardSlow(HTMultiMap::Num);
		return Pairs.Num();
		unguardSlow;
	}
};

// ------------------------------------------------------------
//	Double Linked List.
// ------------------------------------------------------------
template <class T> class HTDoubleLinkList {
public:
	struct FLink {
		T* m_pPrevList;
		T* m_pNextList;
	};

	T	*m_pRoot;

	HTDoubleLinkList()		{ m_pRoot=NULL;	}
	~HTDoubleLinkList()	{ Free();		}

	// Empty list.
	void Free()
	{
		T *pPos = m_pRoot;
		T *pTemp;

		while (pPos)
		{
			pTemp = pPos;
			pPos = (T*)pPos->m_pNextList;
			delete pTemp;
		}
		m_pRoot = NULL;
	}

	// Unlink a node from list.
	void Unlink(T *pNode)
	{
		if(pNode->m_pPrevList)
		{
			pNode->m_pPrevList->m_pNextList = pNode->m_pNextList;
		}
		if(pNode->m_pNextList)
		{
			pNode->m_pNextList->m_pPrevList = pNode->m_pPrevList;
		}
		if(pNode==m_pRoot)
		{
			if(!pNode->m_pPrevList)
				m_pRoot = pNode->m_pNextList;
			else
				m_pRoot = pNode->m_pPrevList;
		}
		pNode->m_pPrevList = NULL;
		pNode->m_pNextList = NULL;
	}

	// Link node to top of list.
	void LinkAtTop(T *pNewNode)
	{
		if (m_pRoot)
			m_pRoot->m_pPrevList = (T*)pNewNode;

		pNewNode->m_pNextList = (T*)m_pRoot;
		pNewNode->m_pPrevList = NULL;
		m_pRoot = pNewNode;
	}

	// Link new node before root.
	void InsertBefore(T *pRootNode, T *pNewNode)
	{
		pNewNode->m_pNextList = pRootNode;                  			
		pNewNode->m_pPrevList = pRootNode->m_pPrevList;                	
		if (pRootNode->m_pPrevList)
			pRootNode->m_pPrevList->m_pNextList = pNewNode;                	
		pRootNode->m_pPrevList = pNewNode; 
		if (m_pRoot == pRootNode)
			m_pRoot = pNewNode;
	}

	// Link node to end of list.
	void LinkAtEnd(T *pNewNode)
	{
		T *pPos = m_pRoot;

		if(pPos)
		{
			while(pPos->m_pNextList)
				pPos = (T*)pPos->m_pNextList;
        
			pNewNode->m_pPrevList = pPos;
			pPos->m_pNextList = pNewNode;
		} else {
			m_pRoot = pNewNode;
			pNewNode->m_pPrevList = NULL;
		}
		pNewNode->m_pNextList = NULL;
	}

	HBOOL IsEmpty()
	{
		return m_pRoot == NULL;
	}

	class FIterator
	{
	private:
		T *m_pItem;
	public:
		FIterator(T *pStartItem)
			:	m_pItem(pStartItem)
		{}
		operator HBOOL()		{ return m_pItem != NULL; }
		void operator++()		{ if(m_pItem) m_pItem=m_pItem->m_pNextList; }
		void operator++(INT)	{ if(m_pItem) m_pItem=m_pItem->m_pNextList; }
		void operator--()		{ if(m_pItem) m_pItem=m_pItem->m_pPrevList; }
		void operator--(INT)	{ if(m_pItem) m_pItem=m_pItem->m_pPrevList; }
		T* operator*()   const	{ return m_pItem; }
		T* operator->()  const	{ return m_pItem; }
	};
};
/*
// ------------------------------------------------------------
//	Single Linked List.
// ------------------------------------------------------------
template <class T> class HTSingleLinkList {
public:
	struct FLink {
		T* m_pNextList;
	};

	T	*m_pRoot;

	HTSingleLinkList()			{ m_pRoot=NULL;	}
	~HTSingleLinkList()		{ Free();		}

	// Empty list.
	void Free()
	{
		T *pPos = m_pRoot;
		T *pTemp;

		while (pPos)
		{
			pTemp = pPos;
			pPos = (T*)pPos->m_pNextList;
			delete pTemp;
		}
		m_pRoot = NULL;
	}

	// Link node to list.
	void Link(T *pNewNode)
	{
		pNewNode->m_pNextList = (T*) m_pRoot;
		m_pRoot = (T*)pNewNode;
	}

	// Unlink node.
	void Unlink(T *pNode)
	{
		T *pPos = m_pRoot;

		while(pPos && pPos->m_pNextList!=pNode)
			pPos = (T*)pPos->m_pNextList;

		if(pPos)
			pPos->Next = pNode->m_pNextList;
	}

	HBOOL IsEmpty()
	{
		return m_pRoot == NULL;
	}

	class FIterator
	{
	private:
		T *m_pItem;
	public:
		FIterator(T *pStartItem)
			:	m_pItem(pStartItem)
		{}
		operator HBOOL()		{ return (HBOOL)m_pItem; }
		void operator++()		{ if(m_pItem) m_pItem=m_pItem->m_pNextList; }
		void operator++(INT)	{ if(m_pItem) m_pItem=m_pItem->m_pNextList; }
		T* operator*()   const	{ return m_pItem; }
		T* operator->()  const	{ return m_pItem; }
	};

	friend class FIterator;
};
*/

//
// Simple double-linked list template.
//
template< class T > class HTDoubleLinkedList : public T
{
public:
	HTDoubleLinkedList* Next;
	HTDoubleLinkedList** PrevLink;
	void Unlink()
	{
		if( Next )
			Next->PrevLink = PrevLink;
		*PrevLink = Next;
	}
	void Link( HTDoubleLinkedList*& Before )
	{
		if( Before )
			Before->PrevLink = &Next;
		Next     = Before;
		PrevLink = &Before;
		Before   = this;
	}
};

/* -------------------------- End -------------------------- */