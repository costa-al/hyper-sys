/*----------------------------------------------------------------------------
	Platform compiler definitions.
----------------------------------------------------------------------------*/

#define __WIN32__				1
#define __INTEL__				1
#define __INTEL_BYTE_ORDER__	1

/*----------------------------------------------------------------------------
	Platform specifics types and defines.
----------------------------------------------------------------------------*/

// Undo any Windows defines.
#undef BYTE
#undef WORD
#undef DWORD
#undef INT
#undef FLOAT
#undef MAXBYTE
#undef MAXWORD
#undef MAXDWORD
#undef MAXINT
#undef CDECL

// Make sure HANDLE is defined.
#ifndef _WINDOWS
	#define HANDLE void*
	#define HINSTANCE void*
	//#define HMODULE	void*
#endif

// Function type macros.
#define DLL_IMPORT	__declspec(dllimport)	/* Import function from DLL */
#define DLL_EXPORT  __declspec(dllexport)	/* Export function to DLL */
#define DLL_EXPORT_CLASS	__declspec(dllexport)	/* Export class to DLL */
#define VARARGS     __cdecl					/* Functions with variable arguments */
#define CDECL	    __cdecl					/* Standard C function */
#define STDCALL		__stdcall				/* Standard calling convention */
#define FORCEINLINE __forceinline			/* Force code to be inline */
#define ZEROARRAY                           /* Zero-length arrays in structs */

// Compiler name.
#ifdef _DEBUG
	#define COMPILER "Compiled with Visual C++ Debug"
#else
	#define COMPILER "Compiled with Visual C++"
#endif

// Unsigned base types.
typedef unsigned char		BYTE;		// 8-bit  unsigned.
typedef unsigned short		_WORD;		// 16-bit unsigned.
typedef unsigned long		DWORD;		// 32-bit unsigned.
typedef unsigned __int64	QWORD;		// 64-bit unsigned.

// Signed base types.
typedef	signed char			SBYTE;		// 8-bit  signed.
typedef signed short		SWORD;		// 16-bit signed.
typedef signed int  		INT;		// 32-bit signed.
typedef signed __int64		SQWORD;		// 64-bit signed.

// Character types.
typedef char				ANSICHAR;	// An ANSI character.
typedef unsigned short      UNICHAR;	// A unicode character.
typedef unsigned char		ANSICHARU;	// An ANSI character.
typedef unsigned short      UNICHARU;	// A unicode character.

// Other base types.
typedef signed int			HBOOL;		// Boolean 0 (false) or 1 (true).
typedef float				FLOAT;		// 32-bit IEEE floating point.
typedef double				DOUBLE;		// 64-bit IEEE double.
//typedef unsigned long       SIZE_T;     // Corresponds to C SIZE_T.

// Enum type.
typedef unsigned long       ENUM;		// For enum.

// Bitfield type.
typedef unsigned long       BITFIELD;	// For bitfields.

// Unwanted VC++ level 4 warnings to disable.
#pragma warning(disable : 4305) /* truncation from 'const double' to 'float'									*/
#pragma warning(disable : 4244) /* conversion to float, possible loss of data									*/
#pragma warning(disable : 4699) /* creating precompiled header													*/
#pragma warning(disable : 4200) /* Zero-length array item at end of structure, a VC-specific extension			*/
#pragma warning(disable : 4100) /* unreferenced formal parameter												*/
#pragma warning(disable : 4514) /* unreferenced inline function has been removed								*/
#pragma warning(disable : 4201) /* nonstandard extension used : nameless struct/union							*/
#pragma warning(disable : 4710) /* inline function not expanded													*/
#pragma warning(disable : 4702) /* unreachable code in inline expanded function									*/
#pragma warning(disable : 4711) /* function selected for autmatic inlining										*/
#pragma warning(disable : 4725) /* Pentium fdiv bug																*/
#pragma warning(disable : 4127) /* Conditional expression is constant											*/
#pragma warning(disable : 4512) /* assignment operator could not be generated									*/
#pragma warning(disable : 4530) /* C++ exception handler used, but unwind semantics are not enabled				*/
#pragma warning(disable : 4245) /* conversion from 'enum ' to 'unsigned long', signed/unsigned mismatch			*/
#pragma warning(disable : 4238) /* nonstandard extension used : class rvalue used as lvalue						*/
#pragma warning(disable : 4251) /* needs to have dll-interface to be used by clients of class					*/
#pragma warning(disable : 4275) /* non dll-interface class used as base for dll-interface class					*/
#pragma warning(disable : 4511) /* copy constructor could not be generated										*/
#pragma warning(disable : 4284) /* return type is not a UDT or reference to a UDT								*/
#pragma warning(disable : 4355) /* this used in base initializer list											*/
#pragma warning(disable : 4097) /* typedef-name '' used as synonym for class-name ''							*/
#pragma warning(disable : 4291) /* typedef-name '' used as synonym for class-name ''							*/

// Last modifed 18.02.05 by MaximuS --->>>
#pragma warning(disable : 4661) /* no suitable definition provided for explicit template instantiation request	*/
// <<<---

// If C++ exception handling is disabled, force guarding to be off.
#ifndef _CPPUNWIND
	#error "Bad VCC option: C++ exception handling must be enabled"
#endif

// Make sure characters are unsigned.
#ifdef _CHAR_UNSIGNED
	#error "Bad VC++ option: Characters must be signed"
#endif

// No asm if not compiling for x86.
#ifndef _M_IX86
	#undef ASM
	#define ASM 0
#endif

// Strings.
#define LINE_TERMINATOR TEXT("\r\n")
#define PATH_SEPARATOR TEXT("\\")

// DLL file extension.
#define DLLEXT TEXT(".dll")

// Pathnames.
#define PATH(s) s

// NULL.
#define NULL 0

// Package implementation.
#define IMPLEMENT_PACKAGE_PLATFORM(pkgname) \
	extern "C" {HINSTANCE hInstance;} \
	INT DLL_EXPORT STDCALL DllMain(HINSTANCE hInInstance, DWORD Reason, void* Reserved) \
	{ hInstance = hInInstance; return 1; }

// Platform support options.
#define PLATFORM_NEEDS_ARRAY_NEW 1
#define FORCE_ANSI_LOG           1

// OS unicode function calling.
#if defined(NO_UNICODE_OS_SUPPORT) || !defined(UNICODE)
	#define TCHAR_CALL_OS(funcW,funcA) (funcA)
	#define TCHAR_TO_ANSI(str) str
	#define ANSI_TO_TCHAR(str) str
#elif defined(NO_ANSI_OS_SUPPORT)
	#define TCHAR_CALL_OS(funcW,funcA) (funcW)
	#define TCHAR_TO_ANSI(str) str
	#define ANSI_TO_TCHAR(str) str
#else
	HCORE_API ANSICHAR* winToANSI(ANSICHAR* ACh, const UNICHAR* InUCh, INT Count);
	HCORE_API INT winGetSizeANSI(const UNICHAR* InUCh);
	HCORE_API UNICHAR* winToUNICODE(UNICHAR* Ch, const ANSICHAR* InUCh, INT Count);
	HCORE_API INT winGetSizeUNICODE(const ANSICHAR* InACh);
	#define TCHAR_CALL_OS(funcW,funcA) (GUnicodeOS ? (funcW) : (funcA))
	#define TCHAR_TO_ANSI(str) winToANSI((ANSICHAR*)appAlloca(winGetSizeANSI(str)),str,winGetSizeANSI(str))
	#define TCHAR_TO_OEM(str) winToOEM((ANSICHAR*)appAlloca(winGetSizeANSI(str)),str,winGetSizeANSI(str))
	#define ANSI_TO_TCHAR(str) winToUNICODE((TCHAR*)appAlloca(winGetSizeUNICODE(str)*sizeof(TCHAR)),str,winGetSizeUNICODE(str))
#endif

// Bitfield alignment.
#define GCC_PACK(n)
#define GCC_ALIGN(n)

#define BIT_FLAG_MMX		23
#define BIT_FLAG_SSE		24
#define BIT_FLAG_PPRO		25
#define BIT_FLAG_SSE2		26
#define BIT_FLAG_3DNOW		31


/*----------------------------------------------------------------------------
	Globals.
----------------------------------------------------------------------------*/

// System identification.
extern "C"
{
	extern HINSTANCE		hInstance;
	extern HCORE_API HBOOL	GIsMMX;
	extern HCORE_API HBOOL	GIsSSE;
	extern HCORE_API HBOOL	GIsSSE2;
	extern HCORE_API HBOOL	GIsAMD;
	extern HCORE_API HBOOL	GIsPentiumPro;
	extern HCORE_API HBOOL	GIsKatmai;
	extern HCORE_API HBOOL	GIsK6;
	extern HCORE_API HBOOL	GIs3DNow;
	extern HCORE_API HBOOL	GTimestamp;
}

/*----------------------------------------------------------------------------
	Functions.
----------------------------------------------------------------------------*/

// Last modifed 25.10.05 by MaximuS --->>>
//
// Computes 1.0f / sqrtf(x).
//
inline FLOAT appRSqrt(FLOAT Number)
{
	long I;
	FLOAT X2, Y;
	const FLOAT Threehalfs = 1.5f;
	X2 = Number * 0.5f;
	Y  = Number;
	I  = * (long *) &Y;
	I  = 0x5f3759df - (I >> 1);
	Y  = * (FLOAT *) &I;
	Y  = Y * (Threehalfs - (X2 * Y * Y));
	return Y;
}
//<<<---

//
// Find CPU extern for work with fast math functions.
//
#if ASM
#define DEFINED_appCPUProperties
inline void appCPUProperties()
{
	DWORD dwEdx;

	// Last modifed 21.02.05 by MaximuS --->>>
	GIsMMX = false;
	GIsSSE = false;
	GIsSSE2 = false;
	GIsAMD = false;
	GIs3DNow = false;
	// <<<---

	__asm 
	{
		mov  eax, 1
		cpuid
		mov dwEdx, edx
	}

	if (dwEdx & (1<<BIT_FLAG_MMX)) 
	{
		GIsMMX = true;
	}

	if (dwEdx & (1<<BIT_FLAG_PPRO)) 
	{
		if (dwEdx & (1<<BIT_FLAG_SSE))
		{      
			__try
			{
				//__asm xorps xmm0, xmm0
				GIsSSE = true;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
			}

			if (dwEdx & (1<<BIT_FLAG_SSE2)) 
			{
				__try
				{
				//	__asm xorpd xmm0, xmm0
					GIsSSE2 = true;
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
				}
			}
		}
	}
 
	__asm 
	{
		mov eax, 0x80000000
		cpuid
		cmp eax, 0x80000000
		jc notamd
		mov eax, 0x80000001
		cpuid
		mov dwEdx, edx
		mov GIsAMD, 1
		notamd:
		mov GIsAMD, 0
	}

	if (GIsAMD)
	{
		if ((GIsMMX) && (dwEdx & (1<<BIT_FLAG_3DNOW)))
		{
			GIs3DNow = true;
		}
	}

	return;
}
#endif

//
// Round a floating point number to an integer.
// Note that (int+.5) is rounded to (int+1).
//
#if ASM
#define DEFINED_appRound 1
inline INT appRound(FLOAT F)
{
	INT I;
	__asm fld [F]
	__asm fistp [I]
	return I;
}
#endif

//
// Converts to integer equal to or less than.
//
#if ASM
#define DEFINED_appFloor 1
inline INT appFloor(FLOAT F)
{
	static FLOAT Half=0.5;
	INT I;
	__asm fld [F]
	__asm fsub [Half]
	__asm fistp [I]
	return I;
}
#endif


// Last modifed 18.10.05 by MaximuS --->>>
//
// Fast square root for floating-point values.
//
#if ASM
#define DEFINED_appFastSqrt 0
inline FLOAT appFastSqrt(FLOAT Square)
{
	FLOAT Retval;
	__asm mov eax, Square
	__asm sub eax, 0x3F800000
	__asm sar eax, 1
	__asm add eax, 0x3F800000
	__asm mov [Retval], eax
	return Retval;
}
#endif
//<<<---

//
// CPU cycles, related to GSecondsPerCycle.
//
#if ASM
#define DEFINED_appCycles 1
#pragma warning (push)
#pragma warning (disable : 4035)
#pragma warning (disable : 4715)
inline DWORD appCycles()
{
	if(GTimestamp) __asm
	{
		xor   eax,eax	          // Required so that VC++ realizes EAX is modified.
		_emit 0x0F		          // RDTSC  -  Pentium+ time stamp register to EDX:EAX.
		_emit 0x31		          // Use only 32 bits in EAX - even a Ghz cpu would have a 4+ sec period.
		xor   edx,edx	          // Required so that VC++ realizes EDX is modified.
	}
}
#pragma warning (pop)
#endif

//
// Seconds, arbitrarily based.
//
#if ASM
#define DEFINED_appSeconds 1
#pragma warning (push)
#pragma warning (disable : 4035)
extern HCORE_API FLOAT GSecondsPerCycle;
HCORE_API FTime appSecondsSlow() { return FTime(); }
inline FTime appSeconds()
{
	if( GTimestamp )
	{
		DWORD L,H;
		__asm
		{
			xor   eax,eax	// Required so that VC++ realizes EAX is modified.
			xor   edx,edx	// Required so that VC++ realizes EDX is modified.
			_emit 0x0F		// RDTSC  -  Pentium+ time stamp register to EDX:EAX.
			_emit 0x31		// Use only 32 bits in EAX - even a Ghz cpu would have a 4+ sec period.
			mov   [L],eax   // Save low value.
			mov   [H],edx   // Save high value.
		}
		return ((double)L +  4294967296.0 * (double)H) * GSecondsPerCycle;
	}
	else return appSecondsSlow();
}
#pragma warning (pop)
#endif

//
// Memory copy.
//
#if ASM
#define DEFINED_appMemcpy
inline void appMemcpy( void* Dest, const void* Src, INT Count )
{	
	__asm
	{
		mov		ecx, Count
		mov		esi, Src
		mov		edi, Dest
		mov     ebx, ecx
		shr     ecx, 2
		and     ebx, 3
		rep     movsd
		mov     ecx, ebx
		rep     movsb
	}
}
#endif

//
// Memory zero.
//
#if ASM
#define DEFINED_appMemzero
inline void appMemzero( void* Dest, INT Count )
{	
	__asm
	{
		mov		ecx, [Count]
		mov		edi, [Dest]
		xor     eax, eax
		mov		ebx, ecx
		shr		ecx, 2
		and		ebx, 3
		rep     stosd
		mov     ecx, ebx
		rep     stosb
	}
}
#endif

#if ASM3DNOW
inline void DoFemms()
{
	__asm _emit 0x0f
	__asm _emit 0x0e
}
#endif

#if ASM
#define DEFINED_appDebugBreak
inline void appDebugBreak()
{
	__asm
	{
		int 3
	}
}
#endif

extern "C" void* __cdecl _alloca(size_t);
#define appAlloca(size) _alloca((size+7)&~7)

TCHAR* Format(const TCHAR *Fmt, ...) {
	static TCHAR Str[256];
	va_list Marker;
	va_start(Marker, Fmt);
	INT nLen = vsprintf(Str, Fmt, Marker);
	va_end(Marker);
	return &Str[0];
}

/* -------------------------- End -------------------------- */