/*-----------------------------------------------------------------------------
	Global variables.
-----------------------------------------------------------------------------*/

HCORE_API extern DWORD GCRCTable[] = {
  0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419,
  0x706AF48F, 0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4,
  0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07,
  0x90BF1D91, 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
  0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 0x136C9856,
  0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
  0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4,
  0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
  0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3,
  0x45DF5C75, 0xDCD60DCF, 0xABD13D59, 0x26D930AC, 0x51DE003A,
  0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599,
  0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
  0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190,
  0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F,
  0x9FBFE4A5, 0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E,
  0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
  0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED,
  0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
  0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3,
  0xFBD44C65, 0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
  0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A,
  0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5,
  0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA, 0xBE0B1010,
  0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
  0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17,
  0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6,
  0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615,
  0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
  0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1, 0xF00F9344,
  0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
  0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A,
  0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
  0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1,
  0xA6BC5767, 0x3FB506DD, 0x48B2364B, 0xD80D2BDA, 0xAF0A1B4C,
  0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF,
  0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
  0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE,
  0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31,
  0x2CD99E8B, 0x5BDEAE1D, 0x9B64C2B0, 0xEC63F226, 0x756AA39C,
  0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
  0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B,
  0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
  0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1,
  0x18B74777, 0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
  0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45, 0xA00AE278,
  0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7,
  0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66,
  0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
  0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605,
  0xCDD70693, 0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8,
  0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B,
  0x2D02EF8D
};


/*----------------------------------------------------------------------------
	Byte order conversion.
----------------------------------------------------------------------------*/

// Bitfields.
#ifndef NEXT_BITFIELD
	#if __INTEL_BYTE_ORDER__
		#define NEXT_BITFIELD(b) ((b)<<1)
		#define FIRST_BITFIELD   (1)
		#define INTEL_ORDER(x)   (x)
	#else
		#define NEXT_BITFIELD(b) ((b)>>1)
		#define FIRST_BITFIELD   (0x80000000)
		#define INTEL_ORDER(x)   (((x)>>24) + (((x)>>8)&0xff00) + (((x)<<8)&0xff0000) + ((x)<<24))
	#endif
#endif

/*-----------------------------------------------------------------------------
	Stats.
-----------------------------------------------------------------------------*/

#if STATS
	#define STAT(x) x
#else
	#define STAT(x)
#endif

/*-----------------------------------------------------------------------------
	Global init and exit.
-----------------------------------------------------------------------------*/

HCORE_API void appInit( const TCHAR* InPackage, const TCHAR* InCmdLine, FOutputDevice* InLog, HBOOL RequireConfig );
HCORE_API void appPreExit();
HCORE_API void appExit();

/*-----------------------------------------------------------------------------
	Logging and critical errors.
-----------------------------------------------------------------------------*/

HCORE_API void appRequestExit( HBOOL Force );

HCORE_API void VARARGS appFailAssert( const ANSICHAR* Expr, const ANSICHAR* File, INT Line );
HCORE_API void VARARGS appUnwindf( const TCHAR* Fmt, ... );
HCORE_API const TCHAR* appGetSystemErrorMessage( INT Error=0 );
HCORE_API const void appDebugMessagef( const TCHAR* Fmt, ... );
#if 1
HCORE_API const void appMsgf( const TCHAR* Fmt, ... );
HCORE_API const void appGetLastError( void );
#endif

#define debugf				GLog->Process
#define appErrorf			GError->Process

#if DO_GUARD_SLOW
	#define debugfSlow		GLog->Process
	#define appErrorfSlow	GError->Process
#else
	#define debugfSlow		GNull->Process
	#define appErrorfSlow	GNull->Process
#endif

/*-----------------------------------------------------------------------------
	Misc.
-----------------------------------------------------------------------------*/

HCORE_API void* appGetDllHandle( const TCHAR* DllName );
HCORE_API void appFreeDllHandle( void* DllHandle );
HCORE_API void* appGetDllExport( void* DllHandle, const TCHAR* ExportName );
HCORE_API void* appCreateProc( const TCHAR* URL, const TCHAR* Parms , HBOOL bRealTime );
HCORE_API HBOOL appGetProcReturnCode( void* ProcHandle, INT* ReturnCode );
HCORE_API void appEnableFastMath( HBOOL Enable );
HCORE_API class FXGuid appCreateGuid();
HCORE_API void appCreateTempFilename( const TCHAR* Path, TCHAR* Result256 );
HCORE_API void appCleanFileCache();
HCORE_API HBOOL appFindPackageFile( const TCHAR* In, const FXGuid* Guid, TCHAR* Out );

/*-----------------------------------------------------------------------------
	Guard macros for call stack display.
-----------------------------------------------------------------------------*/

//
// guard/unguardf/unguard macros.
// For showing calling stack when errors occur in major functions.
// Meant to be enabled in release builds.
//
#if defined(_DEBUG) || !DO_GUARD
	#define guard(func)			{static const TCHAR __FUNC_NAME__[]=TEXT(#func);
	#define unguard				}
	#define unguardf(msg)		}
#else
	#if _MSC_VER
		#define guard(func)			{static const TCHAR __FUNC_NAME__[]=TEXT(#func); try{
		#define unguard				}catch(TCHAR*Err){throw Err;}catch(...){appUnwindf(TEXT("%s"),__FUNC_NAME__); throw;}}
		#define unguardf(msg)		}catch(TCHAR*Err){throw Err;}catch(...){appUnwindf(TEXT("%s"),__FUNC_NAME__); appUnwindf msg; throw;}}
	#else
		#define guard(func)			{static const TCHAR __FUNC_NAME__[]=TEXT(#func); \
									__Context __LOCAL_CONTEXT__; try{ \
									if(setjmp(__Context::Env)) { throw 1; } else {
		#define unguard				}}catch(char*Err){throw Err;}catch(...) \
									{appUnwindf(TEXT("%s"),__FUNC_NAME__); throw;}}
		#define unguardf(msg)		}}catch(char*Err){throw Err;}catch(...) \
									{appUnwindf(TEXT("%s"),__FUNC_NAME__); \
									appUnwindf msg; throw;}}
	#endif
#endif

//
// guardSlow/unguardfSlow/unguardSlow macros.
// For showing calling stack when errors occur in performance-critical functions.
// Meant to be disabled in release builds.
//
#if defined(_DEBUG) || !DO_GUARD || !DO_GUARD_SLOW
	#define guardSlow(func)		{
	#define unguardfSlow(msg)	}
	#define unguardSlow			}
	#define unguardfSlow(msg)	}
#else
	#define guardSlow(func)		guard(func)
	#define unguardSlow			unguard
	#define unguardfSlow(msg)	unguardf(msg)
#endif

//
// For throwing string-exceptions which safely propagate through guard/unguard.
//
HCORE_API void VARARGS appThrowf( const TCHAR* Fmt, ... );

/*-----------------------------------------------------------------------------
	Check macros for assertions.
-----------------------------------------------------------------------------*/

//
// "check" expressions are only evaluated if enabled.
// "verify" expressions are always evaluated, but only cause an error if enabled.
//
#if DO_CHECK
	#define check(expr)  {if(!(expr)) appFailAssert( #expr, __FILE__, __LINE__ );}
	#define verify(expr) {if(!(expr)) appFailAssert( #expr, __FILE__, __LINE__ );}
#else
	#define check(expr) 0
	#define verify(expr) expr
#endif

//
// Check for development only.
//
#if DO_GUARD_SLOW
	#define checkSlow(expr)  {if(!(expr)) appFailAssert( #expr, __FILE__, __LINE__ );}
	#define verifySlow(expr) {if(!(expr)) appFailAssert( #expr, __FILE__, __LINE__ );}
#else
	#define checkSlow(expr)
	#define verifySlow(expr) if(expr){}
#endif


/*-----------------------------------------------------------------------------
	Timing macros.
-----------------------------------------------------------------------------*/

//
// Normal timing.
//
#define clock(Timer)   {Timer -= appCycles();}
#define unclock(Timer) {Timer += appCycles()-34;}

//
// Performance critical timing.
//
#if DO_CLOCK_SLOW
	#define clockSlow(Timer)   {Timer-=appCycles();}
	#define unclockSlow(Timer) {Timer+=appCycles();}
#else
	#define clockSlow(Timer)
	#define unclockSlow(Timer)
#endif

/*-----------------------------------------------------------------------------
	File functions.
-----------------------------------------------------------------------------*/

// File utilities.
HCORE_API const TCHAR* appFExt( const TCHAR* Filename );
HCORE_API HBOOL appUpdateFileModTime( TCHAR* Filename );

/*-----------------------------------------------------------------------------
	OS functions.
-----------------------------------------------------------------------------*/

HCORE_API const TCHAR* appCmdLine();
HCORE_API const TCHAR* appBaseDir();
HCORE_API const TCHAR* appPackage();
HCORE_API const TCHAR* appComputerName();
HCORE_API const TCHAR* appUserName();

/*-----------------------------------------------------------------------------
	Timing functions.
-----------------------------------------------------------------------------*/

#if !DEFINED_appCycles
HCORE_API DWORD appCycles();
#endif

#if !DEFINED_appSeconds
HCORE_API FTime appSeconds();
#endif

HCORE_API void appSystemTime( INT& Year, INT& Month, INT& DayOfWeek, INT& Day, INT& Hour, INT& Min, INT& Sec, INT& MSec );
HCORE_API const TCHAR* appTimestamp();
HCORE_API FTime appSecondsSlow();
HCORE_API void appSleep( FLOAT Seconds );

/*-----------------------------------------------------------------------------
	Character type functions.
-----------------------------------------------------------------------------*/

inline TCHAR appToUpper( TCHAR c )
{
	return (c<'a' || c>'z') ? (c) : (TCHAR)(c+'A'-'a');
}
inline TCHAR appToLower( TCHAR c )
{
	return (c<'A' || c>'Z') ? (c) : (TCHAR)(c+'a'-'A');
}
inline HBOOL appIsAlpha( TCHAR c )
{
	return (c>='a' && c<='z') || (c>='A' && c<='Z');
}
inline HBOOL appIsDigit( TCHAR c )
{
	return c>='0' && c<='9';
}
inline HBOOL appIsAlnum( TCHAR c )
{
	return (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9');
}

/*-----------------------------------------------------------------------------
	String functions.
-----------------------------------------------------------------------------*/

HCORE_API const ANSICHAR* appToAnsi( const TCHAR* Str );
HCORE_API const UNICHAR* appToUnicode( const TCHAR* Str );
HCORE_API const TCHAR* appFromAnsi( const ANSICHAR* Str );
HCORE_API const TCHAR* appFromUnicode( const UNICHAR* Str );
HCORE_API HBOOL appIsPureAnsi( const TCHAR* Str );

HCORE_API TCHAR* appStrcpy( TCHAR* Dest, const TCHAR* Src );
HCORE_API INT appStrcpy( const TCHAR* String );
HCORE_API INT appStrlen( const TCHAR* String );
HCORE_API TCHAR* appStrstr( const TCHAR* String, const TCHAR* Find );
HCORE_API TCHAR* appStrchr( const TCHAR* String, INT c );
HCORE_API TCHAR* appStrcat( TCHAR* Dest, const TCHAR* Src );
HCORE_API INT appStrcmp( const TCHAR* String1, const TCHAR* String2 );
HCORE_API INT appStricmp( const TCHAR* String1, const TCHAR* String2 );
HCORE_API INT appStrncmp( const TCHAR* String1, const TCHAR* String2, INT Count );
HCORE_API TCHAR* appStaticString1024();
HCORE_API ANSICHAR* appAnsiStaticString1024();

HCORE_API const TCHAR* appSpc( int Num );
HCORE_API TCHAR* appStrncpy( TCHAR* Dest, const TCHAR* Src, int Max);
HCORE_API TCHAR* appStrncat( TCHAR* Dest, const TCHAR* Src, int Max);
HCORE_API TCHAR* appStrupr( TCHAR* String );
HCORE_API const TCHAR* appStrfind(const TCHAR* Str, const TCHAR* Find);
HCORE_API DWORD appStrCrc( const TCHAR* Data );
HCORE_API DWORD appStrCrcCaps( const TCHAR* Data );
HCORE_API INT appAtoi( const TCHAR* Str );
HCORE_API FLOAT appAtof( const TCHAR* Str );
HCORE_API INT appStrtoi( const TCHAR* Start, TCHAR** End, INT Base );
HCORE_API INT appStrnicmp( const TCHAR* A, const TCHAR* B, INT Count );
HCORE_API INT appSprintf( TCHAR* Dest, const TCHAR* Fmt, ... );
HCORE_API void appTrimSpaces( ANSICHAR* String);

HCORE_API HBOOL appStreq (const TCHAR* A, const TCHAR* B);
HCORE_API TCHAR* appStrcat (const TCHAR* A, const TCHAR* B);
HCORE_API TCHAR* appStrdup (const TCHAR* String);


#if _MSC_VER
	HCORE_API INT appGetVarArgs( TCHAR* Dest, INT Count, const TCHAR*& Fmt );
#else
	#include <stdio.h>
	#include <stdarg.h>
#endif

typedef int QSORT_RETURN;
typedef QSORT_RETURN(CDECL* QSORT_COMPARE)( const void* A, const void* B );
HCORE_API void appQsort( void* Base, INT Num, INT Width, QSORT_COMPARE Compare );

//
// Case insensitive string hash function.
//
inline DWORD appStrihash( const TCHAR* Data )
{
	DWORD Hash=0;
	while( *Data )
	{
		TCHAR Ch = appToUpper(*Data++);
		BYTE  B  = (BYTE)Ch;
		Hash     = ((Hash >> 8) & 0x00FFFFFF) ^ GCRCTable[(Hash ^ B) & 0x000000FF];
#if UNICODE
		B        = (BYTE)(Ch>>8);
		Hash     = ((Hash >> 8) & 0x00FFFFFF) ^ GCRCTable[(Hash ^ B) & 0x000000FF];
#endif
	}
	return Hash;
}

/*-----------------------------------------------------------------------------
	Math functions.
-----------------------------------------------------------------------------*/

HCORE_API DOUBLE appExp( DOUBLE Value );
HCORE_API DOUBLE appLoge( DOUBLE Value );
HCORE_API DOUBLE appFmod( DOUBLE A, DOUBLE B );
HCORE_API DOUBLE appSin( DOUBLE Value );
HCORE_API DOUBLE appCos( DOUBLE Value );
HCORE_API DOUBLE appAcos( DOUBLE Value );
HCORE_API DOUBLE appTan( DOUBLE Value );
HCORE_API DOUBLE appAtan( DOUBLE Value );
HCORE_API DOUBLE appAtan2( DOUBLE Y, DOUBLE X );
HCORE_API DOUBLE appSqrt( DOUBLE Value );
HCORE_API DOUBLE appPow( DOUBLE A, DOUBLE B );
HCORE_API HBOOL appIsNan( DOUBLE Value );
HCORE_API INT appRand();
HCORE_API FLOAT appFrand();
#if 1
HCORE_API FLOAT appRandRange( FLOAT Min, FLOAT Max );
HCORE_API INT appRandRange( INT Min, INT Max );
#endif

#if !DEFINED_appRound
HCORE_API INT appRound( FLOAT Value );
#endif

#if !DEFINED_appFloor
HCORE_API INT appFloor( FLOAT Value );
#endif

#if !DEFINED_appCeil
HCORE_API INT appCeil( FLOAT Value );
#endif

/*-----------------------------------------------------------------------------
	Memory functions.
-----------------------------------------------------------------------------*/

HCORE_API void* appMemmove( void* Dest, const void* Src, INT Count );
HCORE_API INT appMemcmp( const void* Buf1, const void* Buf2, INT Count );
HCORE_API HBOOL appMemIsZero( const void* V, int Count );
HCORE_API DWORD appMemCrc( const void* Data, INT Length, DWORD CRC=0 );
HCORE_API void appMemswap( void* Ptr1, void* Ptr2, DWORD Size );
HCORE_API void appMemset( void* Dest, INT C, INT Count );

#ifndef DEFINED_appMemcpy
HCORE_API void appMemcpy( void* Dest, const void* Src, INT Count );
#endif

#ifndef DEFINED_appMemzero
HCORE_API void appMemzero( void* Dest, INT Count );
#endif

/*-----------------------------------------------------------------------------
	Memory macros.
-----------------------------------------------------------------------------*/

#ifdef _DEBUG
	#define appMalloc(_Len, _Tag)			GMalloc->Malloc(_Len, _Tag)
	#define appFree(_Ptr)					GMalloc->Free(_Ptr)
	#define appRealloc(_Ptr, _Len, _Tag)	GMalloc->Realloc(_Ptr, _Len, _Tag)
#else
	#define appMalloc(_Len)					GMalloc->Malloc(_Len, "")
	#define appFree(_Ptr)					GMalloc->Free(_Ptr)
	#define appRealloc(_Ptr, _Len)			GMalloc->Realloc(_Ptr, _Len, "")
#endif

//
// C++ style memory allocation.
//

#ifndef _DEBUG
// ------------------- RELEASE MODE ---------------------------
inline void* operator new( unsigned int Size )
{
	try{ return appMalloc( Size, "" ); } 
	catch(...) { GMalloc->Init();
	try	{ return appMalloc( Size ); }
	catch(...) { return NULL; }
		return NULL;
	}
}
inline void* operator new[]( unsigned int Size )
{
	try { return appMalloc( Size, "" ); }
	catch(...) { GMalloc->Init();
	try	{ return appMalloc( Size, "" ); }
	catch(...) { return NULL; }
		return NULL;
	}
}
#else
// ------------------- DEBUG MODE ---------------------------
inline void* operator new( unsigned int Size, TCHAR *File, INT Line )
{
	try { return appMalloc(Size, Format("%s : %i", File, Line)); } 
	catch(...) { GMalloc->Init();
	try	{ return appMalloc(Size, Format("%s : %i", File, Line)); }
	catch(...) { return NULL; }
		return NULL;
	}
}

inline void* operator new[]( unsigned int Size, TCHAR *File, INT Line )
{
	try { return appMalloc(Size, Format("%s : %i", File, Line)); }
	catch(...) { GMalloc->Init();
	try	{ return appMalloc(Size, Format("%s : %i", File, Line)); } 
	catch(...) { return NULL; }
		return NULL;
	}
}

#define ___DEBUG_NEW new(__FILE__, __LINE__)
#define new ___DEBUG_NEW

#endif //_DEBUG

inline void operator delete( void* Ptr )
{
	try { appFree( Ptr ); }
	catch(...) { GMalloc->Init();
	try	{ appFree( Ptr ); }
		catch(...) {}
	}
}
inline void operator delete []( void* Ptr )
{
	try { appFree( Ptr ); }
	catch(...) { GMalloc->Init();
	try	{ appFree( Ptr ); }
	catch(...) { }
	}
}


/* -------------------------- End -------------------------- */