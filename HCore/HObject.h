
HTDoubleLinkList<HEvent>	GFreeEvents;
HTDoubleLinkList<HEvent>	GEventQueue;
HTDoubleLinkList<HEvent>	GActiveEvents;

static INT		GnEventTotalMemAllocated;
static INT		GnNumEvents			= 0;
static INT		GnNumFreeEvents		= 0;
static HBOOL	GbEventError		= false;
static TCHAR	GEventErrorMsg[128];

typedef HVar (HClass::*CallbackT)(HVar* Vars);
typedef HVar (HClass::*GetPropT)();
typedef void (HClass::*SetPropT)(HVar Value);

// ------------------------------------------------------------
// FScriptFunc.
// ------------------------------------------------------------

class HCORE_API FScriptFunc {
public:
	HBOOL	m_bIsEvent;
	BYTE	m_Type;
	TCHAR	*m_pName;
	BYTE	m_Formatspec[7];
	INT		m_nCodeLabel;
	HTMultiMap<FString, BYTE> m_lVars;
};

#define INITIALLY_ALLOCATED_EVENTS	8
#define EVENT_MAXARGS				8

// ------------------------------------------------------------
// HEventDef.
// ------------------------------------------------------------

class HCORE_API HEventDef {
public:
	const TCHAR					*m_pName;
	const TCHAR					*m_pFormatspec;
	TCHAR						m_ReturnType;
	INT							m_nNumArgs;
	INT							m_nEventNum;

	static HTMultiMap <FString, HEventDef*>	m_CommandList;
	static HTArray<HEventDef*>				m_TempEventDefs;

	HEventDef( const TCHAR *pCommand, const TCHAR *pFormatspec = "", TCHAR nReturnType = 0 );
	operator INT();

	static HEventDef* GetEventCommand( INT nEventNum );
	static HEventDef* FindEvent( FString FullName );

	HVar ProcessEvent(HEvent *pEvent, HBOOL bIn = false);

protected:
	virtual HVar RunInline(HEvent *pEvent, HClass *pObject, CallbackT Func) = 0; 
	HVar RunScript(HEvent *pEvent, HScriptThread *pThread, FScriptFunc *pScriptFunc);
};

inline HEventDef::operator INT() {
	return m_nEventNum;
}

inline DWORD GetTypeHash(const HEventDef &Fi) {
	return Fi.m_nEventNum;
}

inline DWORD GetTypeHash(const HEventDef *Fi) {
	return Fi->m_nEventNum;
}

HEventDef::HEventDef( const TCHAR *pCommand, const TCHAR *pFormatspec, TCHAR ReturnType) {
	guard(HEventDef::constr);
	INT i;
	HEventDef* pEvent;
	this->m_pName		= pCommand;
	this->m_pFormatspec = pFormatspec;
	this->m_ReturnType	= ReturnType;
	m_nNumArgs = strlen( pFormatspec );
	check( m_nNumArgs <= EVENT_MAXARGS );
	if ( m_nNumArgs > EVENT_MAXARGS ) {
		GbEventError = true;
		sprintf( GEventErrorMsg, "HEventDef : Too many args for '%s' pEvent.", m_pName );
		return;
	}
	m_nEventNum = m_CommandList.Num();
	for( i = 0; i < m_nEventNum; i++ ) {
		pEvent = m_CommandList[i].Value;
		if ( strcmp( pCommand, pEvent->m_pName ) == 0 ) {
			if ( strcmp( pFormatspec, pEvent->m_pFormatspec ) != 0 ) {
				GbEventError = true;
				sprintf( GEventErrorMsg, "HEventDef '%s' defined twice with differing format strings ('%s'!='%s').",
					pCommand, pFormatspec, pEvent->m_pFormatspec );
				return;
			}
			if ( pEvent->m_ReturnType != ReturnType ) {
				GbEventError = true;
				sprintf( GEventErrorMsg, "HEventDef '%s' defined twice with differing return types ('%c'!='%c').",
					pCommand, ReturnType, pEvent->m_ReturnType );
				return;
			}
			m_nEventNum = pEvent->m_nEventNum;
			return;
		}
	}
	m_CommandList.Add(m_pName, this);
	unguard;
}
HEventDef *HEventDef::GetEventCommand( INT nEventNum ) {
	return m_CommandList[nEventNum].Value;
}
HEventDef *HEventDef::FindEvent( FString Fullname ) {
	return m_CommandList.FindRef(Fullname);
}
template< class T >
struct FResponseDef {
	HEventDef	*pEvent;
	CallbackT	Response;
};
template< class T >
struct FPropertiesT {
	GetPropT GetProp;
	SetPropT SetProp;
};
template< class T >
struct FPropertiesDef {
	TCHAR*	Name;
	FPropertiesT<T> Prop;
};

enum EClassFlags {
	ECF_Null		= 0x00000000,
	ECF_Native		= 0x00000001,
	ECF_Pack		= 0x00000002,
	ECF_User		= 0x00000004,
	ECF_System		= 0x00000008,
	ECF_Mod			= 0x00000010,

	ECF_Core		= ECF_Native | ECF_System,
};

// ------------------------------------------------------------
//	HClassDef.
// ------------------------------------------------------------

class HCORE_API HClassDef {
protected:
	FResponseDef<HClass>	*m_pResponses;
	FPropertiesDef<HClass>	*m_pProperties;

public:
	EClassFlags									m_ClassFlags;
	FPropertiesDef<HClass>* GetProperties() { return m_pProperties; }
	FResponseDef<HClass>* GetResponses()	{ return m_pResponses; }
	const TCHAR									*m_pClassname;
	const TCHAR									*m_pSuperclass;
	HClass										*(*m_pNewInstance)(void);
	HTMultiMap<INT, CallbackT>					m_ResponseLookup;
	HTMultiMap<FString, FPropertiesT<HClass>*>	m_PropertiesList;
	HClassDef									*m_pSuper;
	static HTMultiMap<FString, HClassDef*>		m_ClassMap;

	HClassDef(HClassDef *pOther); // Copy constructor.
	HClassDef( const TCHAR *pClassname, const TCHAR *pSuperclass,
		FResponseDef<HClass> *pResponses, FPropertiesDef<HClass> *pPropertiesDef,
		HClass* (*pNewInstance)(void), EClassFlags Flags);

	~HClassDef() {
		guard(HClassDef::destr);
		HClassDef *pNode;

		m_ClassMap.Remove(m_pClassname);

		for(HTMultiMap<FString, HClassDef*>::FIterator it(m_ClassMap); it; ++it) {
			pNode = it.Value();
			if ( pNode->m_pSuper == this )	{
				pNode->m_pSuper = NULL;
			}
		}
		Shutdown();
		unguard;
	}
	void Prepare() {
		guard(HClassDef::Prepare);
		HClassDef					*pClassDef;
		FResponseDef<HClass>		*pResponseDef;
		FPropertiesDef<HClass>		*pPropertiesDef;
		HEventDef					*pEvent;
		FString						sName;

		Shutdown();

		for(pClassDef = this; pClassDef != NULL; pClassDef = pClassDef->m_pSuper) {
			pResponseDef = pClassDef->m_pResponses;
			if(pResponseDef) {
				for(INT i = 0; pResponseDef[i].pEvent != NULL; i++)	{
					pEvent = pResponseDef[i].pEvent;
					if (!m_ResponseLookup.Find(pEvent->m_nEventNum))
						m_ResponseLookup.Add(pEvent->m_nEventNum, pResponseDef[i].Response);
				}
			}
			pPropertiesDef = pClassDef->m_pProperties;
			if (pPropertiesDef) {
				for(INT i = 0; pPropertiesDef[i].Name != NULL; i++)	{
					sName = pPropertiesDef[i].Name;
					if (!m_PropertiesList.Find(sName)) {
						m_PropertiesList.Add((*sName), &pPropertiesDef[i].Prop);
					}
				}
			}
		}
		unguard;
	}
	void Shutdown() {
		guard(HClassDef::Shutdown);
		m_PropertiesList.Empty();
		m_ResponseLookup.Empty();
		unguard;
	}
};

// ************************* EXTERN ***************************
HTMultiMap<FString, HEventDef*>	HEventDef::m_CommandList;
HTArray<HEventDef*>				HEventDef::m_TempEventDefs;
// Global information`s all system classes.
HTMultiMap<FString, HClassDef*> HClassDef::m_ClassMap;
// ************************************************************

//
// Documentation function.
//
template <class T> void PrintDocumentation(HTBuffer<T>& Buf, HClassDef* Cd) {
	guard(PrintDocumentation);
	cout << Format("\n------------------------------\n%s : public %s {\n", Cd->m_pClassname, Cd->m_pSuperclass);
	FString sName;
	HEventDef *pEvent;
	FPropertiesDef<HClass> *pPropertiesDef;
	FResponseDef<HClass> *pResponseDef;

	cout << "Responses:\n";
	pResponseDef = Cd->GetResponses();
	if(pResponseDef) {
		for(INT i = 0; pResponseDef[i].pEvent != NULL; i++)	{
			pEvent = pResponseDef[i].pEvent;
			if (Cd->m_ResponseLookup.Find(pEvent->m_nEventNum))
				cout << "\t" << pEvent->m_ReturnType << ' ' <<
				pEvent->m_pName << "(" << pEvent->m_pFormatspec << ")\n";
		}
	}

	cout << "Properties:\n";
	pPropertiesDef = Cd->GetProperties();
	if(pPropertiesDef) {
		for(INT i = 0; pPropertiesDef[i].Name != NULL; i++)	{
			sName = pPropertiesDef[i].Name;
			if (Cd->m_PropertiesList.Find(sName)) {
				cout << "\t" << *sName << "\n";
			}
		}
	}

	cout << Format("}\n------------------------------\n");
	unguard;
}
template <class T> void PrintClassesInfo(HTBuffer<T>& Buf) {
	guard(PrintClassesInfo);
	HClassDef *pNode;
	for(HTMultiMap<FString, HClassDef*>::FIterator it(HClassDef::m_ClassMap); it; ++it) {
		pNode = it.Value();
		if (pNode)
			PrintDocumentation(Buf, pNode);
	}
	unguard;
}

// ------------------------------------------------------------
//	HClass.
// ------------------------------------------------------------

//
// Declaration macroces.
//
#define CLASS_DECLARATION( nameofsuperclass, nameofclass, flags )		\
	HClassDef nameofclass::m_ClassInfo							\
	(															\
		#nameofclass, #nameofsuperclass,						\
		( FResponseDef<HClass>* )nameofclass::m_Responses,		\
		( FPropertiesDef<HClass>* )nameofclass::m_Properties,	\
		nameofclass::_newInstance,								\
		flags													\
	);															\
	HClass *nameofclass::_newInstance()							\
	{															\
		return new nameofclass;									\
	}															\
	HClassDef *nameofclass::GetClassDef()						\
	{															\
		return &( nameofclass::m_ClassInfo );					\
	}															\
	FResponseDef<HClass> nameofclass::m_Responses[] = {

#define EVENT_DECL( event, function )	{ &( event ), ( HVar ( HClass::* )( HVar* Vars ) )( &function ) },

#define PROPERTIES( nameofclass )								\
	{ NULL, NULL } };											\
	FPropertiesDef<HClass> nameofclass::m_Properties[] = {

#define PROP_DECL( propname )									\
	{ #propname ,												\
	{(GetPropT)(&Get##propname), (SetPropT)(&Set##propname)}},

#define END_CLASS				{ NULL, { NULL, NULL }} };

#define FUNC_DECL( nameofclass, funcname )						\
	HVar nameofclass::funcname(HVar* Vars) {					\
	guardSlow(nameofclass::funcname);							\
	checkSlow(Vars);											\
	HVar Ret;

#define END_FUNC												\
	return Ret;													\
	unguardSlow;												\
	}

#define TRANS_ARG( argtype, argname, argnum )					\
		argtype argname = (argtype)Vars[argnum];

//
// Prototype macroces.
//
#define CLASS_PROTOTYPE( nameofclass )							\
	public:														\
	static	HClassDef						m_ClassInfo;		\
	static	HClass							*_newInstance();	\
	virtual	HClassDef						*GetClassDef();		\
	static	FResponseDef<HClass>			m_Responses[];		\
	static	FPropertiesDef<HClass>			m_Properties[];

#define PROP_PROTOTYPE( proptype, propname )					\
	proptype propname;											\
	HVar Get##propname()										\
	{															\
		return (HVar)propname;									\
	}															\
	void Set##propname(HVar value)								\
	{															\
		propname = (proptype)value;								\
	}															

#define FUNC_PROTOTYPE( funcname )	HVar funcname (HVar* Vars);

// ------------------------------------------------------------

class HCORE_API HClass {
public:
	CLASS_PROTOTYPE ( HClass )

	FUNC_PROTOTYPE ( Remove )
	FUNC_PROTOTYPE ( SafeRemove )
	FUNC_PROTOTYPE ( SetEventScript )
	FUNC_PROTOTYPE ( SetScript )

	HScriptThread		*m_pThread;

public:

	static void			PrepareClassSystem();
	static void			ShutdownClassSystem();
	static HClass*		CreateClass( const TCHAR *Name );
	static HClassDef*	GetClass( const TCHAR *Name );
	static HClassDef*	AddClass(HClassDef *Cd);

	const TCHAR*		GetClassname();
	const TCHAR*		GetSuperclass();
	HClass*				NewInstance();

	void				ExecuteThread(const TCHAR *EntryName);

	CallbackT			GetResponse( HEventDef *pEd );
	HVar				ProcessEvent( HEvent *pEvent, HBOOL bIn = false );
	HBOOL				PostEvent( HEvent *pEvent, INT nTime );
	void				CancelEvents(HEventDef *pEd );

	HClass()
		:	m_pThread(NULL)
	{}
	~HClass()	{} // delete m_pThread;
};

HClassDef::HClassDef( HClassDef *pOther ) {
	HClassDef *pNode;
	FPropertiesDef<HClass> *pPropertiesDef = pOther->GetProperties();
	FResponseDef<HClass> *pResponseDef = pOther->GetResponses();
	this->m_ClassFlags = pOther->m_ClassFlags;
	this->m_pClassname = appStrdup(pOther->m_pClassname);
	this->m_pSuperclass = appStrdup(pOther->m_pSuperclass);
	this->m_pSuper = HClass::GetClass(m_pSuperclass);
	this->m_ResponseLookup.Empty();
	this->m_PropertiesList.Empty();
	this->m_pNewInstance	= pOther->m_pNewInstance;
	INT Count = 1;
	while(pPropertiesDef[Count-1].Name != NULL) Count++;
	this->m_pProperties = new FPropertiesDef<HClass>[Count];
	appMemcpy(m_pProperties, pPropertiesDef, Count*sizeof(FPropertiesDef<HClass>));
	Count = 1;
	while(pResponseDef[Count-1].pEvent != NULL) Count++;
	this->m_pResponses = new FResponseDef<HClass>[Count];	
	appMemcpy(m_pResponses, pResponseDef, Count*sizeof(FResponseDef<HClass>));
	// Check if any subclasses were initialized before their superclass.
	for(HTMultiMap<FString, HClassDef*>::FIterator it(m_ClassMap); it; ++it) {
		pNode = it.Value();
		if((pNode->m_pSuper == NULL) && (!_stricmp(pNode->m_pSuperclass, this->m_pClassname)) &&
			(_stricmp(pNode->m_pClassname, "HClass")))
		{
			pNode->m_pSuper	= this;
		}
	}

	Prepare();

	m_ClassMap.Add(m_pClassname, this);
}

//
// HClassDef constructor.
//
HClassDef::HClassDef( const TCHAR *pClassname, const TCHAR *pSuperclass,
		FResponseDef<HClass> *pResponses, FPropertiesDef<HClass> *pPropertiesDef,
		HClass* (*pNewInstance)(void), EClassFlags Flags)
{
	guard(HClassDef::constr);
	HClassDef *pNode;

	this->m_ClassFlags		= Flags;
	this->m_pClassname		= pClassname;
	this->m_pSuperclass		= pSuperclass;
	this->m_pResponses		= pResponses;
	this->m_pProperties		= pPropertiesDef;
	this->m_ResponseLookup.Empty();
	this->m_PropertiesList.Empty();
	this->m_pNewInstance	= pNewInstance;
	this->m_pSuper			= HClass::GetClass(pSuperclass);

	// Check if any subclasses were initialized before their superclass.
	for(HTMultiMap<FString, HClassDef*>::FIterator it(m_ClassMap); it; ++it) {
		pNode = it.Value();
		if((pNode->m_pSuper == NULL) && (!_stricmp(pNode->m_pSuperclass, this->m_pClassname)) &&
			(_stricmp(pNode->m_pClassname, "HClass")))
		{
			pNode->m_pSuper	= this;
		}
	}

	m_ClassMap.Add(pClassname, this);
	unguard;
}

void HClass::PrepareClassSystem() {
	guard(HClass::PrepareClassSystem);
	for(HTMultiMap<FString, HClassDef*>::FIterator it(HClassDef::m_ClassMap); it; ++it)
		it.Value()->Prepare();
	unguard;
}

void HClass::ShutdownClassSystem() {
	guard(HClass::ShutdownClassSystem);
	for(HTMultiMap<FString, HClassDef*>::FIterator it(HClassDef::m_ClassMap); it; ++it)
		it.Value()->Shutdown();
	unguard;
}

HClass* HClass::CreateClass( const TCHAR *Name ) {
	guard(HClass::CreateClass);
	HClassDef *Cd = GetClass(Name);
	if(Cd)
		return Cd->m_pNewInstance();
	return NULL;
	unguard;
}

HClassDef* HClass::GetClass( const TCHAR *Name ) {
	guard(HClass::GetClass);
	return HClassDef::m_ClassMap.FindRef(Name);
	unguard;
}

HClassDef* HClass::AddClass(HClassDef *Cd) {
	guard(HClass::AddClass);
	if(!HClassDef::m_ClassMap.FindRef(Cd->m_pClassname)) {
		HClassDef *pClassDef = new HClassDef(Cd);
		return pClassDef;
	}
	return NULL;
	unguard;
}

const TCHAR* HClass::GetClassname() {
	guard(HClass::GetClassname);
	HClassDef *Cls;
	Cls = GetClassDef();
	return Cls->m_pClassname;
	unguard;
}
const TCHAR* HClass::GetSuperclass() {
	guard(HClass::GetSuperclass);
	HClassDef *Cls;
	Cls = GetClassDef();
	return Cls->m_pSuperclass;
	unguard;
}
HClass* HClass::NewInstance() {
	guard(HClass::NewInstance);
	HClassDef *Cls;
	Cls = GetClassDef();
	return Cls->m_pNewInstance();
	unguard;
}
HVar HClass::Remove(HVar* Vars) {
	delete this;
	return HVar(0);
}

// ------------------------------------------------------------
//	HEvent.
// ------------------------------------------------------------

#define EVENT_COMMAND(eventcom)	new HEvent(&eventcom)
#define EVENT_NAME(eventpoint, eventname) eventpoint = new HEvent(HEventDef::FindEvent(TEXT(eventname)));

class HCORE_API HEvent : public HClass {
public:
	CLASS_PROTOTYPE ( HEvent );

	FUNC_PROTOTYPE( SetEventDef );
	FUNC_PROTOTYPE( SetEventArg );
	FUNC_PROTOTYPE( Schedule );

private:
	friend class HClass;
	friend class HEventDef;

public:
	HEvent			*m_pPrevList;			
	HEvent			*m_pNextList;

	HEventDef		*m_pEventdef;
	HVar			*m_pArgs;
	HClass			*m_pObject;
	INT				m_nTime;

	#ifdef _DEBUG
		#undef new
		void* operator new( size_t Size, char *File, int);
		#define new ___DEBUG_NEW
	#else
		void* operator new( size_t Size );
	#endif

	void			operator delete( void* );

	static void		CancelEvents( HClass *pObj, HEventDef *pEvdef = NULL );
	static void		ClearEventList();
	static void		PrepareEventList();
	static void		ShutdownEventList();
	static void		ProcessServiceEvents();

	static HBOOL	m_bEventSystemStarted;

	HEvent();
	HEvent(HEventDef *pEvent);
	virtual ~HEvent();

						operator	INT();
	HVar&				operator [] (INT nIndex);
};

HBOOL	HEvent::m_bEventSystemStarted;

static HEvent *RealAllocateEvent() {
	guardSlow(HEvent::RealAllocateEvent);
	HEvent *pEvent;
	pEvent = (HEvent*)new TCHAR[sizeof(HEvent)];
	GnEventTotalMemAllocated += sizeof(HEvent);
	return pEvent;
	unguardSlow;
}
static void RealDeallocateEvent( HEvent* pEvent ) {
	guardSlow(HEvent::RealDeallocateEvent);
	delete[]((void*)pEvent);
	GnEventTotalMemAllocated -= sizeof(HEvent);
	unguardSlow;
}

#ifdef _DEBUG
	#undef new
	void* HEvent::operator new( size_t Size, TCHAR *File, INT)
	#define new ___DEBUG_NEW
#else
	void * HEvent::operator new( size_t Size )
#endif
{
	HEvent * pNewEvent;
	check(sizeof(HEvent) == Size);
	if (GFreeEvents.IsEmpty()) {
		check(0);
		pNewEvent = RealAllocateEvent();
	} else {
		pNewEvent = GFreeEvents.m_pRoot;
		GFreeEvents.Unlink(pNewEvent);
		GnNumFreeEvents--;
	}
	GActiveEvents.LinkAtTop(pNewEvent);
	return pNewEvent;
}
void HEvent::operator delete( void *Ptr ) {
	HEvent *pEvent;
	pEvent = (HEvent*)Ptr;
	GFreeEvents.LinkAtTop(pEvent);
	GnNumFreeEvents++;
}
inline HEvent::operator INT() {
	return m_pEventdef->m_nEventNum;
}
inline HVar& HEvent::operator []( INT nIndex ) {
	check(nIndex < m_pEventdef->m_nNumArgs);
	return m_pArgs[nIndex];
}
HEvent::HEvent()
	:	m_pEventdef(NULL)
	,	m_pArgs(NULL)
{}
HEvent::HEvent( HEventDef *pEv ) {
	if (!pEv){
		HEvent();
		return;
	}
	m_pEventdef = pEv;
	if (pEv->m_nNumArgs>0)
		m_pArgs = new HVar[pEv->m_nNumArgs];
	else
		m_pArgs = NULL;
}
HEvent::~HEvent() {
	if (m_pArgs)
		delete[] m_pArgs;
}
FUNC_DECL( HEvent, SetEventDef )
	const TCHAR* pEvName = (const TCHAR*)Vars[0];

	m_pEventdef = HEventDef::FindEvent(pEvName);
	if (m_pEventdef) {
		if (m_pArgs)
			delete[] m_pArgs;
		if (m_pEventdef->m_nNumArgs>0)
			m_pArgs = new HVar[m_pEventdef->m_nNumArgs];
		else
			m_pArgs = NULL;
		return HVar(1);
	}
	return HVar(0);
END_FUNC
FUNC_DECL( HEvent, SetEventArg )
	INT nNumArg = (INT)Vars[0];
	HVar Arg = Vars[1];
	m_pArgs[nNumArg] = Arg;
END_FUNC
FUNC_DECL( HEvent, Schedule )
	HClass *pObj = (HClass*)Vars[0];
	INT nTime = (INT)Vars[1];
	if(!m_bEventSystemStarted)
		return HVar(0);
	check(pObj);
	this->m_pObject=pObj;
	this->m_nTime=appSeconds().GetInt() + nTime;
	GActiveEvents.Unlink(this);
	HTDoubleLinkList<HEvent>::FIterator it(GEventQueue.m_pRoot);
	while((*it)&&(this->m_nTime>=it->m_nTime)) it++;
	if (*it)
		GEventQueue.InsertBefore(*it, this);
	else
		GEventQueue.LinkAtEnd(this);
	return HVar(1);
END_FUNC
void HEvent::CancelEvents( HClass *pObj, HEventDef *pEv ) {
	HEvent *pEvent;
	if (!m_bEventSystemStarted)
		return;
	HTDoubleLinkList<HEvent>::FIterator it(GEventQueue.m_pRoot);
	while (it) {
		pEvent = *it; it++;
		if( pEvent->m_pObject==pObj ) {
			if( !pEv || ( pEv==pEvent->m_pEventdef ) ) {
				GEventQueue.Unlink(pEvent);
				delete pEvent;
			}
		}
	}
}
void HEvent::ClearEventList() {
	HEvent *pEvent;
	GEventQueue.Free();
	GActiveEvents.Free();
	while(!GFreeEvents.IsEmpty())
	{
		pEvent = GFreeEvents.m_pRoot;
		GFreeEvents.Unlink(pEvent);
		RealDeallocateEvent(pEvent);
	}
	GnNumEvents		= 0;
	GnNumFreeEvents = 0;
}
void HEvent::ProcessServiceEvents() {
	HEvent *pEvent;
	INT		nNum = 0;
	HVar	Var;
	while(!GEventQueue.IsEmpty())
	{
		pEvent = GEventQueue.m_pRoot;
		check(pEvent);
		if (pEvent->m_nTime > appSeconds().GetInt())
			break;
		GEventQueue.Unlink(pEvent);
		GnNumEvents--;
		GActiveEvents.LinkAtTop(pEvent);
		pEvent->m_pEventdef->ProcessEvent(pEvent);
		GActiveEvents.Unlink(pEvent);
		delete pEvent;
		nNum++;
		if (nNum > 10000) {
		    break;
		}
	}
}
void HEvent::PrepareEventList() {
	HEvent *pNewEvent;
	GnEventTotalMemAllocated = 0;
	for(INT i = 0; i < INITIALLY_ALLOCATED_EVENTS; i++) {
		pNewEvent = RealAllocateEvent();
		GFreeEvents.LinkAtTop(pNewEvent);
		GnNumFreeEvents++;
	}
	m_bEventSystemStarted = true;
}
void HEvent::ShutdownEventList() {
	ClearEventList();
	check(GnEventTotalMemAllocated == 0);
	HEventDef::m_CommandList.Empty();
	for (HTArray<HEventDef*>::FIterator it(HEventDef::m_TempEventDefs); it; ++it)
		delete *it;
	HEventDef::m_TempEventDefs.Empty();
	m_bEventSystemStarted = false;
}
CallbackT HClass::GetResponse( HEventDef *pEv ) {
	return GetClassDef()->m_ResponseLookup.FindRef(pEv->m_nEventNum);
}
HVar HClass::ProcessEvent(HEvent *pEvent, HBOOL bIn) {
    pEvent->m_pObject = this ;
	return pEvent->m_pEventdef->ProcessEvent(pEvent, bIn);
}
HBOOL HClass::PostEvent(HEvent *pEv, INT nTime) {
	if (!HEvent::m_bEventSystemStarted) {
		return false;
    }
	HVar *pArgs = new HVar[2];
	pArgs[0] = HVar(this);
	pArgs[1] = HVar(nTime);
	return pEv->Schedule(pArgs);
}
void HClass::CancelEvents(HEventDef *pEv) {
	HEvent::CancelEvents(this, pEv);
}

/* -------------------------- End -------------------------- */