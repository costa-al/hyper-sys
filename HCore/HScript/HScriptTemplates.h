
//
// Script container for all script types.
//
class CXEntity : public CXEntityList<CXClass>, public CXClass {
	CLASS_PROTOTYPE ( CXEntity )
	FUNC_PROTOTYPE( Add )
	FUNC_PROTOTYPE( Get )
	FUNC_PROTOTYPE( Size )
	FUNC_PROTOTYPE( Rewind )
	FUNC_PROTOTYPE( Next )
	FUNC_PROTOTYPE( Current )
	FUNC_PROTOTYPE( Clear )
};

FUNC_DECL( CXEntity, Add )
	TRANS_ARG(CXClass*, Pointer, 0);
	CXEntityList<CXClass>::Add(Pointer);
END_FUNC
FUNC_DECL( CXEntity, Get )
	TRANS_ARG(FXString, sName, 0);
	Ret = CXVar(CXEntityList<CXClass>::Get(sName));
END_FUNC
FUNC_DECL( CXEntity, Size )
	Ret = CXVar(CXEntityList<CXClass>::m_EntityList.size());
END_FUNC
FUNC_DECL( CXEntity, Rewind )
	CXEntityList<CXClass>::m_EntityList.rewind();
END_FUNC
FUNC_DECL( CXEntity, Next )
	++CXEntityList<CXClass>::m_EntityList;
END_FUNC
FUNC_DECL( CXEntity, Current )
	Ret = CXVar(*CXEntityList<CXClass>::m_EntityList);
END_FUNC
FUNC_DECL( CXEntity, Clear )
	CXEntityList<CXClass>::DeleteEntities();
END_FUNC

CXEventDef1		EV_Add		(	"Add",		"p"			);
CXEventDef1		EV_Get		(	"Get",		"s",	'p' );
CXEventDef0		EV_Size		(	"Size",		"",		'i' );
CXEventDef0		EV_Rewind	(	"Rewind"				);
CXEventDef0		EV_Next		(	"Next"					);
CXEventDef0		EV_Current	(	"Current",	"",		'p' );
CXEventDef0		EV_Clear	(	"Clear"					);

CLASS_DECLARATION( CXClass, CXEntity )
	EVENT_DECL( EV_Add,		CXEntity::Add		)
	EVENT_DECL( EV_Get,		CXEntity::Get		)
	EVENT_DECL( EV_Size,	CXEntity::Size		)
	EVENT_DECL( EV_Rewind,	CXEntity::Rewind	)
	EVENT_DECL( EV_Next,	CXEntity::Next		)
	EVENT_DECL( EV_Current,	CXEntity::Current	)
	EVENT_DECL( EV_Clear,	CXEntity::Clear		)
PROPERTIES( CXEntity )
END_CLASS

class CXName : public CXClass {
	CLASS_PROTOTYPE ( CXName )
	FUNC_PROTOTYPE( SetName )
	FUNC_PROTOTYPE( GetName )
};

FUNC_DECL( CXName, SetName )
	TRANS_ARG(FXString, Str, 0);
	CXEntitySpace::Name(Str);
END_FUNC
FUNC_DECL( CXName, GetName )
	Ret = CXVar(CXEntitySpace::Name());
END_FUNC

CXEventDef1		EV_SetName( "SetName", "s"		);
CXEventDef0		EV_GetName( "GetName", "",	's'	);

CLASS_DECLARATION( CXClass, CXName )
	EVENT_DECL( EV_GetName,	CXName::GetName )
	EVENT_DECL( EV_SetName,	CXName::SetName )
PROPERTIES( CXName )
END_CLASS



class CXClassTest : public CXName {
public:
	CLASS_PROTOTYPE( CXClassTest )

	FUNC_PROTOTYPE( Index );
	FUNC_PROTOTYPE( PrepareMap );
	FUNC_PROTOTYPE( AddActor );
	FUNC_PROTOTYPE( ProcessMap );

	FUNC_PROTOTYPE( GUI_ButtonClick );

	FUNC_PROTOTYPE( Printf );

	PROP_PROTOTYPE( INT,			Testprop1	)
	PROP_PROTOTYPE( FLOAT,			Testprop2	)
	PROP_PROTOTYPE( const TCHAR*,	Name1		)
	PROP_PROTOTYPE( FXString,		Name2		)
	PROP_PROTOTYPE( FxVector3f,		Direction	)

	CXClassTest()	{}
	~CXClassTest()	{}

};

FUNC_DECL( CXClassTest, GUI_ButtonClick )
END_FUNC
FUNC_DECL( CXClassTest, Printf )
	if(GLog) GLog->Process((const TCHAR*)Vars[0]);
END_FUNC
FUNC_DECL( CXClassTest, Index )
END_FUNC

CXEventDef0		EV_GUI_ButtonClick( "GUI_ButtonClick" );
CXEventDef0		EV_Index( "Index" );
CXEventDef1		EV_Printf( "Printf", "s" );

CLASS_DECLARATION( CXName, CXClassTest )
	EVENT_DECL( EV_GUI_ButtonClick,	CXClassTest::GUI_ButtonClick )	
	EVENT_DECL( EV_Index,	CXClassTest::Index )
	EVENT_DECL( EV_Printf,	CXClassTest::Printf )
PROPERTIES( CXClassTest )
	PROP_DECL( Testprop1 )
	PROP_DECL( Testprop2 )
	PROP_DECL( Name1 )
	PROP_DECL( Name2 )
	PROP_DECL( Direction )
END_CLASS
