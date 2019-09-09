HEventDef0		EV_Remove( "<immediateremove>" );
HEventDef0		EV_SafeRemove( "Remove" );
HEventDef1		EV_SetScript( "SetScript", "s" );
HEventDef2		EV_SetEventScript( "SetEventScript", "ss", 'i' );

CLASS_DECLARATION( NULL, HObject )
	EVENT_DECL( EV_Remove,			HObject::Remove )
	EVENT_DECL( EV_SetScript,		HObject::SetScript )
	EVENT_DECL( EV_SafeRemove,		HObject::SafeRemove )
	EVENT_DECL( EV_SetEventScript,	HObject::SetEventScript )
PROPERTIES( HObject )
END_CLASS

HVar HObject::SafeRemove(HVar* Vars) {
	PostEvent(new HEvent(&EV_Remove), 0);
	return HVar(0);
}

HEventDef1		EV_SetEventDef( "SetEventDef", "s", 'i' );
HEventDef2		EV_SetEventArg( "SetEventArg", "ip" );
HEventDef2		EV_Schedule( "Schedule", "pi", 'i' );

CLASS_DECLARATION( HObject, HEvent )
	EVENT_DECL( EV_SetEventDef,	HEvent::SetEventDef )
	EVENT_DECL( EV_SetEventArg,	HEvent::SetEventArg )
	EVENT_DECL( EV_Schedule,	HEvent::Schedule )
PROPERTIES( HEvent )
END_CLASS


//--------------------------------------------------------------------
//
// Process event (need declaration HScriptThread for script events).
//
HVar HEventDef::ProcessEvent(HEvent *pEvent, HBOOL bIn) {
	return HVar(0);
}
//HVar HEventDef::RunScript(HEvent *pEvent, HScriptThread *pThread, FScriptFunc *pScriptFunc) {
//	return HVar(0);
//}
FUNC_DECL( HObject, SetScript )
	TRANS_ARG(const TCHAR*, pName, 0);
	return HVar(1);
END_FUNC
FUNC_DECL( HObject, SetEventScript )
	TRANS_ARG(const TCHAR*, pEventName,	0);
	TRANS_ARG(const TCHAR*, pName,		1);
	return HVar(0);
END_FUNC

//--------------------------------------------------------------------



class HClassTest : public HObject {
public:
	CLASS_PROTOTYPE( HClassTest )

	FUNC_PROTOTYPE( Index );
	FUNC_PROTOTYPE( Printf );

	PROP_PROTOTYPE( INT,			Testprop1	)
	PROP_PROTOTYPE( FLOAT,			Testprop2	)
	PROP_PROTOTYPE( const TCHAR*,	Name1		)
	PROP_PROTOTYPE( FString,		Name2		)
	PROP_PROTOTYPE( HVec3,			Direction	)

	HClassTest()	{}
	~HClassTest()	{}

};

FUNC_DECL( HClassTest, Printf )
	cout << (const TCHAR*)Vars[0];
END_FUNC
FUNC_DECL( HClassTest, Index )
END_FUNC

HEventDef0		EV_Index( "Index" );
HEventDef1		EV_Printf( "Printf", "s" );

CLASS_DECLARATION( HObject, HClassTest )
	EVENT_DECL( EV_Index,	HClassTest::Index )
	EVENT_DECL( EV_Printf,	HClassTest::Printf )
PROPERTIES( HClassTest )
	PROP_DECL( Direction )
	PROP_DECL( Testprop1 )
	PROP_DECL( Testprop2 )
	PROP_DECL( Name1 )
	PROP_DECL( Name2 )
END_CLASS
