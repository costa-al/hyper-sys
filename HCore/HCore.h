#ifndef __HCORE__
#define __HCORE__

#ifndef HCORE_API
	#define HCORE_API DLL_EXPORT
#else
	#define HCORE_API DLL_IMPORT
#endif


// Time.
#define FIXTIME 4294967296.f

/*----------------------------------------------------------------------------
	FTime.
----------------------------------------------------------------------------*/

class FTime
{
#if __GNUG__
#define TIMETYP long long
#else
#define TIMETYP __int64
#endif
public:

	        FTime      ()               {v=0;}
	        FTime      (float f)        {v=(TIMETYP)(f*FIXTIME);}
	        FTime      (double d)       {v=(TIMETYP)(d*FIXTIME);}
	float   GetFloat   ()               {return v/FIXTIME;}
	int		GetInt   ()					{return (int)(v/FIXTIME);}
	FTime   operator+  (float f) const  {return FTime(v+(TIMETYP)(f*FIXTIME));}
	float   operator-  (FTime t) const  {return (v-t.v)/FIXTIME;}
	FTime   operator*  (float f) const  {return FTime(v*f);}
	FTime   operator/  (float f) const  {return FTime(v/f);}
	FTime&  operator+= (float f)        {v=v+(TIMETYP)(f*FIXTIME); return *this;}
	FTime&  operator*= (float f)        {v=(TIMETYP)(v*f); return *this;}
	FTime&  operator/= (float f)        {v=(TIMETYP)(v/f); return *this;}
	int     operator== (FTime t)        {return v==t.v;}
	int     operator!= (FTime t)        {return v!=t.v;}
	int     operator>  (FTime t)        {return v>t.v;}
	FTime&  operator=  (const FTime& t) {v=t.v; return *this;}
private:
	FTime (TIMETYP i) {v=i;}
	TIMETYP v;
};

#if _MSC_VER
	#include "HPlatform\HWin32\HWin32.h"
#elif __GNUG__
	#include <string.h>
	#include "HPlatform\HGnugG\HGnugG.h"
#else
	#error Unknown Compiler
#endif

// Global constants.
enum {MAXBYTE		= 0xff       };
enum {MAXWORD		= 0xffffU    };
enum {MAXDWORD		= 0xffffffffU};
enum {MAXSBYTE		= 0x7f       };
enum {MAXSWORD		= 0x7fff     };
enum {MAXINT		= 0x7fffffff };
enum {INDEX_NONE	= -1         };
enum {UNICODE_BOM   = 0xfeff     };
enum ENoInit {E_NoInit = 0};

// Unicode or single byte character set mappings.
#ifdef _UNICODE
	#ifndef _TCHAR_DEFINED
		typedef UNICHAR  TCHAR;
		typedef UNICHARU TCHARU;
	#endif
	#undef TEXT
	#define TEXT(s) L##s
	#undef US
	#define US FXString(L"")
	inline TCHAR    FromAnsi   (ANSICHAR In) { return (BYTE)In;                                            }
	inline TCHAR    FromUnicode(UNICHAR In ) { return In;                                                  }
	inline ANSICHAR ToAnsi     (TCHAR In   ) { return (_WORD)In<0x100 ? (ANSICHAR)In : (ANSICHAR)MAXSBYTE; }
	inline UNICHAR  ToUnicode  (TCHAR In   ) { return In;                                                  }
#else
	#ifndef _TCHAR_DEFINED
		typedef ANSICHAR  TCHAR;
		typedef ANSICHARU TCHARU;
	#endif
	#undef TEXT
	#define TEXT(s) s
	#undef US
	#define US FXString("")
	inline TCHAR    FromAnsi   (ANSICHAR In) { return In;                              }
	inline TCHAR    FromUnicode(UNICHAR In ) { return (_WORD)In<0x100 ? In : MAXSBYTE; }
	inline ANSICHAR ToAnsi     (TCHAR In   ) { return (_WORD)In<0x100 ? In : MAXSBYTE; }
	inline UNICHAR  ToUnicode  (TCHAR In   ) { return (BYTE)In;                        }
#endif


class FClassDesc;

class HClassDef;
class HClass;
class HEvent;
class HVar;
class HEventDef;
class HScriptThread;

// Structs.
class FString;

template<class T> class HTArray;
template<class TK, class TI> class HTMap;
template<class TK, class TI> class HTMultiMap;


/*----------------------------------------------------------------------------
	FOutputDevice.
----------------------------------------------------------------------------*/

class HCORE_API FOutputDevice {
public:
	virtual void Log(const TCHAR* Str) = 0;
	virtual void Logf(const TCHAR* Str) {};
};

/*----------------------------------------------------------------------------
	FPackage.
----------------------------------------------------------------------------*/

class HCORE_API FIPackage {
public:
	FIPackage() {}
	~FIPackage() {}
	virtual HBOOL Load(const TCHAR* szPackage, HBOOL bAutoExport) = 0;
	virtual HBOOL Free() = 0;
	virtual HBOOL IsValid() = 0;
	virtual HBOOL AutoLoading() = 0;
	virtual INT	NumberClasses() = 0;
	virtual FClassDesc* ClassDesc(const TCHAR *Name) = 0;
	virtual HClassDef* ClassDef(TCHAR *Name) = 0;
	virtual HClassDef* ClassDefIdx(INT nId) = 0;
	virtual const TCHAR* Description() = 0;
	virtual DWORD Version() = 0;
};

/*----------------------------------------------------------------------------
	FMalloc.
----------------------------------------------------------------------------*/

class HCORE_API FIMalloc
{
public:
	virtual void* Malloc( DWORD Count, const TCHAR* Tag )=0;
	virtual void* Realloc( void* Original, DWORD Count, const TCHAR* Tag )=0;
	virtual void Free( void* Original )=0;
	virtual void DumpAllocs()=0;
	virtual void HeapCheck()=0;
	virtual void Init()=0;
	virtual void Exit()=0;
};

// Per module globals.
extern "C" DLL_EXPORT TCHAR GPackage[];

// Globals.
HCORE_API extern FOutputDevice		*GLog;
HCORE_API extern FIMalloc			*GMalloc;

#include "HFile.h"			// Prototypes application functions.
#include "HDecl.h"			// Declaration application functions.
#include "HBase.h"			// Core base.
#include "HTemplates.h"		// Arrays, maps, linked lists.
#include "HBuffer.h"		// Serializer.
#include "HMath\HMath.h"	// Math types and functions.
#include "HVar.h"

#include "HOutputDeviceNull.h"
#include "HOutputDeviceStdout.h"

#include "HANSIMalloc.h"	// ANSI memory manager.
#include "HDebugMalloc.h"	// Debug memory manager.


HOutputDeviceStdout LogStd;
FOutputDevice *GLog = (FOutputDevice*)&LogStd;

#ifdef _DEBUG
FMallocDebug Malloc;
#else
FMallocAnsi Malloc;
#endif
FIMalloc *GMalloc = (FIMalloc*)&Malloc;


#include "HObject.h"		// Base object.
#include "HEvent.h"			// Connect all event types.
#include "HObjDecl.h"

#include "HScript\HParser.h"
#include "HScript\HScript.h"
#include "HScript\HThread.h"


#include "HWindowsPackage.h"
#include "HInterfaces\HIMaster.h"


#endif /* __HCORE__ */

/* -------------------------- End -------------------------- */