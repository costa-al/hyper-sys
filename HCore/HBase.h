/*----------------------------------------------------------------------------
	Core types.
----------------------------------------------------------------------------*/

#define SUID_IID( A, B ) \
	extern "C" { DWORD GGuidA = A; DWORD GGuidB = B; }

//
// System unique identifier.
//
class HCORE_API FSuid
{
public:
	DWORD A,B;
	FSuid()
	{}
	FSuid(DWORD InA, DWORD InB)
	: A(InA), B(InB)
	{}
	friend HBOOL operator==(const FSuid& X, const FSuid& Y)
		{return X.A==Y.A && X.B==Y.B;}
	friend HBOOL operator!=(const FSuid& X, const FSuid& Y)
		{return X.A!=Y.A || X.B!=Y.B;}
	TCHAR* String() const
	{
//		TCHAR* Result = appStaticString1024();
//		appSprintf(Result, TEXT("%08X%08X%08X%08X"), A, B, C, D);
//		return Result;
		return 0;
	}
};
inline INT CompareGuids(FSuid* A, FSuid* B)
{
	INT Diff;
	Diff = A->A-B->A; if(Diff) return Diff;
	Diff = A->B-B->B; if(Diff) return Diff;
	return 0;
}

//
// COM IUnknown interface.
//
class HCORE_API FIUnknown
{
public:
	virtual DWORD STDCALL QueryInterface(const FSuid& RefIID, void** InterfacePtr) {return 0;}
	virtual DWORD STDCALL AddRef() {return 0;}
	virtual DWORD STDCALL Release() {return 0;}
};

//
// Package class descriptor.
//
class HCORE_API FClassDesc {
public:
	virtual INT				IsPublic()						= 0;
	virtual void*			Create(HBOOL bLoading = false)	= 0;
	virtual const TCHAR*	ClassName()						= 0;
	virtual FSuid			SuperClassID()					= 0;
	virtual FSuid			ClassID()						= 0;
	virtual const TCHAR*	Category()						= 0;
};
/*
			for(INT i=0; i<GNumberClasses; i++) { \
				if(!strcmp(DescKeeper[0]->ClassName(), Name)) \
					return DescKeeper[0]; \
			} \
*/
// Define the package portable functions.
#define PACKAGE_PORTABLE \
	extern "C" { \
		DLL_EXPORT	const TCHAR* PackInternalName() { return &GPackage[0]; } \
		DLL_EXPORT	INT NumberClasses() { return HClassDef::m_ClassMap.Num(); } \
		DLL_EXPORT  FClassDesc* ClassDesc(TCHAR *Name) { \
		return DescKeeper[0]; \
		} \
		DLL_EXPORT	HClassDef* ClassDef(TCHAR *Name) { \
		return HClassDef::m_ClassMap.FindRef(Name); \
		} \
		DLL_EXPORT	HClassDef* ClassDefIdx(INT nId) { \
		return HClassDef::m_ClassMap[nId].Value; \
		} \
		DLL_EXPORT	const TCHAR* Description(); \
		DLL_EXPORT	DWORD Version(); \
		DLL_EXPORT	HBOOL AutoLoading(); \
	};

#define STATIC_INIT \
	class HStatic { public: HStatic() { HClass::PrepareClassSystem(); \
	HEvent::PrepareEventList(); HScript::PrepareScriptSystem(); } \
	~HStatic() { HScript::ShutdownScriptSystem(); \
	HEvent::ShutdownEventList(); HClass::ShutdownClassSystem(); } \
	}; static HStatic Static; \

// Define the package of the current DLL being compiled.
#if _MSC_VER
	#define IMPLEMENT_PACKAGE(pkg) \
		extern "C" DLL_EXPORT TCHAR GPackage[]; \
		DLL_EXPORT TCHAR GPackage[] = TEXT(#pkg); \
		INT GNumberClasses = 0; \
		FClassDesc *DescKeeper[8]; \
		STATIC_INIT \
		IMPLEMENT_PACKAGE_PLATFORM(pkg) \
		PACKAGE_PORTABLE
#else
	#define IMPLEMENT_PACKAGE(pkg) \
	extern "C" DLL_EXPORT {TCHAR GPackage[] = TEXT(#pkg);} \
		INT	GNumberClasses = 0; \
		FClassDesc *DescKeeper[8]; \
		STATIC_INIT \
		IMPLEMENT_PACKAGE_PLATFORM(pkg) \
		PACKAGE_PORTABLE
#endif

#define DECLARE_CLASS_DESCRIPTOR(classname, classsuid, supersuid, category) \
	class HDesc##classname : public FClassDesc { \
	public: HDesc##classname() { \
	DescKeeper[GNumberClasses] = (FClassDesc*)this; GNumberClasses++; } \
	INT				IsPublic()		{ return 1; } \
	void*			Create(HBOOL bLoading = false) { return new classname(); } \
	const TCHAR*	ClassName()		{ return TEXT(#classname); } \
	FSuid			SuperClassID()	{ return supersuid; } \
	FSuid			ClassID()		{ return classsuid; } \
	const TCHAR*	Category()		{ return TEXT(#category); } \
	}; \
	static HDesc##classname Desc##classname;

/* -------------------------- End -------------------------- */
