
class HCORE_API HWindowsPackage : public FIPackage
{
	HMODULE				m_hDll;
    INT					m_nRefs;
	TCHAR				m_DllName[128];
public:
	typedef const TCHAR*	(*PFPackInternalName)(void);
	typedef FClassDesc*		(*PFClassDesc)(const TCHAR*);
	typedef HClassDef*		(*PFClassDef)(TCHAR*);
	typedef HClassDef*		(*PFClassDefIdx)(INT);
	typedef INT				(*PFNumberClasses)(void);
	typedef DWORD			(*PFVersion)(void);
	typedef const TCHAR*	(*PFDescription)(void);
	typedef HBOOL			(*PFAutoLoading)(void);

	PFPackInternalName	m_FPackInternalName;
	PFVersion			m_FVersion;
	PFDescription		m_FDescription;
	PFAutoLoading		m_FAutoLoading;
	PFNumberClasses		m_FNumberClasses;
	PFClassDesc			m_FClassDesc;
	PFClassDef			m_FClassDef;
	PFClassDefIdx		m_FClassDefIdx;

public:
	HWindowsPackage()
		:	m_hDll(0)
		,	m_nRefs(0)
	{
		memset(m_DllName, 0, sizeof(m_DllName));
    }
	HWindowsPackage( const TCHAR* szDll )
		:	m_nRefs(0)
		,	m_hDll(0)
	{
		_tcscpy(m_DllName, szDll);
		Load(szDll, false);
	}
	HWindowsPackage( const TCHAR* szDll, HBOOL bAutoExport )
		:	m_nRefs(0)
		,	m_hDll(0)
	{
		_tcscpy(m_DllName, szDll);
		Load(szDll, bAutoExport);
	}
	HBOOL Load( const TCHAR* szDll, HBOOL bAutoExport )
	{
		_tcscpy(m_DllName, szDll);
		if(!m_hDll)
        {
			m_hDll = LoadLibrary(szDll);
		    if(m_hDll)
		    {
				m_nRefs = 1;
				m_FVersion			= (PFVersion)GetProcAddress( m_hDll,		_T("Version") );
				m_FDescription		= (PFDescription)GetProcAddress( m_hDll,	_T("Description") );
				m_FAutoLoading		= (PFAutoLoading)GetProcAddress( m_hDll,	_T("AutoLoading") );
				m_FNumberClasses	= (PFNumberClasses)GetProcAddress( m_hDll,	_T("NumberClasses") );
				m_FClassDesc		= (PFClassDesc)GetProcAddress( m_hDll,		_T("ClassDesc") );
				m_FClassDef			= (PFClassDef)GetProcAddress( m_hDll,		_T("ClassDef") );
				m_FClassDefIdx		= (PFClassDefIdx)GetProcAddress( m_hDll,	_T("ClassDefIdx") );
				if(bAutoExport)
					for(INT i=0; i<NumberClasses(); i++)
						HClass::AddClass(ClassDefIdx(i));
			} else
				return false;
        } else
			m_nRefs++;

		return true;
	}
	HBOOL Free()
	{
		if(m_hDll)
			m_nRefs--;
		if(0 == m_nRefs && m_hDll)
		{
			FreeLibrary(m_hDll);
			m_hDll = NULL;
		}
		return true;
	}
	HBOOL IsValid() { return m_nRefs > 0; }
	virtual ~HWindowsPackage() { m_nRefs = 1; Free(); }

#define PFCALL(fname) \
	if(fname) \
	return (fname)(); \
	else return NULL;

	const TCHAR* PackInternalName() { PFCALL(m_FPackInternalName) }
	INT NumberClasses()			{ PFCALL(m_FNumberClasses)	}
	const TCHAR* Description()	{ PFCALL(m_FDescription)	}
	DWORD Version()				{ PFCALL(m_FVersion)		}
	HBOOL AutoLoading()			{ PFCALL(m_FAutoLoading)	}
	FClassDesc* ClassDesc(const TCHAR *Name) {
		if(m_FClassDesc) {
			return (m_FClassDesc)(Name);
		}
		else {
			return NULL;
		}
	}
	HClassDef* ClassDef(TCHAR *Name) {
		if(m_FClassDef) {
			return (m_FClassDef)(Name);
		}
		else {
			return NULL;
		}
	}
	HClassDef* ClassDefIdx(INT nId) {
		if(m_FClassDefIdx) {
			return (m_FClassDefIdx)(nId);
		}
		else {
			return NULL;
		}
	}
};

/* -------------------------- End -------------------------- */