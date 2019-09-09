

HEventDef0		EV_Remove( "<immediateremove>" );
HEventDef0		EV_SafeRemove( "Remove" );
HEventDef1		EV_SetScript( "SetScript", "s" );
HEventDef2		EV_SetEventScript( "SetEventScript", "ss", 'i' );

CLASS_DECLARATION( NULL, HClass, ECF_Core )
	EVENT_DECL( EV_Remove,			HClass::Remove )
	EVENT_DECL( EV_SetScript,		HClass::SetScript )
	EVENT_DECL( EV_SafeRemove,		HClass::SafeRemove )
	EVENT_DECL( EV_SetEventScript,	HClass::SetEventScript )
PROPERTIES( HClass )
END_CLASS

HVar HClass::SafeRemove(HVar* Vars) {
	PostEvent(new HEvent(&EV_Remove), 0);
	return HVar(0);
}

HEventDef1		EV_SetEventDef( "SetEventDef", "s", 'i' );
HEventDef2		EV_SetEventArg( "SetEventArg", "ip" );
HEventDef2		EV_Schedule( "Schedule", "pi", 'i' );

CLASS_DECLARATION( HClass, HEvent, ECF_Core )
	EVENT_DECL( EV_SetEventDef,	HEvent::SetEventDef )
	EVENT_DECL( EV_SetEventArg,	HEvent::SetEventArg )
	EVENT_DECL( EV_Schedule,	HEvent::Schedule )
PROPERTIES( HEvent )
END_CLASS

/* -------------------------- End -------------------------- */