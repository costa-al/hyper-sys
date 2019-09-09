
class HEventDef0 : public HEventDef {
public:
	HEventDef0( const TCHAR *pCommand, const TCHAR *pFormatspec = "", TCHAR ReturnType = 0 )
		:	HEventDef(pCommand, pFormatspec, ReturnType) {}
	HVar ProcessEvent(HClass *pObject) {
		HEvent* pEvent = new HEvent(this);
		pEvent->m_pObject = pObject;
		return HEventDef::ProcessEvent(pEvent);
	}
protected:
	typedef HVar (HClass::*EventCallbackT)(HVar*);
	virtual HVar RunInline(HEvent *pEvent, HClass *pObj, CallbackT Func) {
		HVar Var;
		Var = (pObj->*(EventCallbackT)Func)(pEvent->m_pArgs);
		return Var;
	}
};
class HEventDef1 : public HEventDef {
public:
	HEventDef1( const TCHAR *pCommand, const TCHAR *pFormatspec = "", TCHAR ReturnType = 0 )
		:	HEventDef(pCommand, pFormatspec, ReturnType) {}
	HVar ProcessEvent(HClass *pObject, HVar Arg0) {
		HEvent* pEvent = new HEvent(this);
		pEvent->m_pObject = pObject;
		HVar Var[2];
		Var[0] = HVar((INT)0);
		Var[1] = Arg0;
		pEvent->SetEventArg(Var);
		return HEventDef::ProcessEvent(pEvent);
	}
protected:
	typedef HVar ( HClass::*EventCallbackT )( HVar* );
	virtual HVar RunInline(HEvent *pEvent, HClass *pObj, CallbackT Func) {
		HVar Var;
		Var = (pObj->*(EventCallbackT)Func)(pEvent->m_pArgs);
		return Var;
	}
};
class HEventDef2 : public HEventDef {
public:
	HEventDef2( const TCHAR *pCommand, const TCHAR *pFormatspec = "", TCHAR ReturnType = 0 )
		:	HEventDef(pCommand, pFormatspec, ReturnType) {}
	HVar ProcessEvent(HClass *pObject, HVar Arg0, HVar Arg1) {
		HEvent* pEvent = new HEvent(this);
		pEvent->m_pObject = pObject;
		HVar Var[2];
		Var[0] = HVar((INT)0);
		Var[1] = Arg0;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)1);
		Var[1] = Arg1;
		pEvent->SetEventArg(Var);
		return HEventDef::ProcessEvent(pEvent);
	}
protected:
	typedef HVar ( HClass::*EventCallbackT )( HVar* );
	virtual HVar RunInline(HEvent *pEvent, HClass *pObj, CallbackT Func) {
		HVar Var;
		Var = (pObj->*(EventCallbackT)Func)(pEvent->m_pArgs);
		return Var;
	}
};
class HEventDef3 : public HEventDef {
public:
	HEventDef3( const TCHAR *pCommand, const TCHAR *pFormatspec = "", TCHAR ReturnType = 0 )
		:	HEventDef(pCommand, pFormatspec, ReturnType) {}
	HVar ProcessEvent(HClass *pObject, HVar Arg0, HVar Arg1, HVar Arg2) {
		HEvent* pEvent = new HEvent(this);
		pEvent->m_pObject = pObject;
		HVar Var[2];
		Var[0] = HVar((INT)0);
		Var[1] = Arg0;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)1);
		Var[1] = Arg1;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)2);
		Var[1] = Arg2;
		pEvent->SetEventArg(Var);
		return HEventDef::ProcessEvent(pEvent);
	}
protected:
	typedef HVar ( HClass::*EventCallbackT )( HVar* );
	virtual HVar RunInline(HEvent *pEvent, HClass *pObj, CallbackT Func) {
		HVar Var;
		Var = (pObj->*(EventCallbackT)Func)(pEvent->m_pArgs);
		return Var;
	}
};
class HEventDef4 : public HEventDef {
public:
	HEventDef4( const TCHAR *pCommand, const TCHAR *pFormatspec = "", TCHAR ReturnType = 0 )
		:	HEventDef(pCommand, pFormatspec, ReturnType) {}
	HVar ProcessEvent(HClass *pObject, HVar Arg0, HVar Arg1, HVar Arg2, HVar Arg3) {
		HEvent* pEvent = new HEvent(this);
		pEvent->m_pObject = pObject;
		HVar Var[2];
		Var[0] = HVar((INT)0);
		Var[1] = Arg0;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)1);
		Var[1] = Arg1;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)2);
		Var[1] = Arg2;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)3);
		Var[1] = Arg3;
		pEvent->SetEventArg(Var);
		return HEventDef::ProcessEvent(pEvent);
	}
protected:
	typedef HVar ( HClass::*EventCallbackT )( HVar* );
	virtual HVar RunInline(HEvent *pEvent, HClass *pObj, CallbackT Func) {
		HVar Var;
		Var = (pObj->*(EventCallbackT)Func)(pEvent->m_pArgs);
		return Var;
	}
};
class HEventDef5 : public HEventDef {
public:
	HEventDef5( const TCHAR *pCommand, const TCHAR *pFormatspec = "", TCHAR ReturnType = 0 )
		:	HEventDef(pCommand, pFormatspec, ReturnType) {}
	HVar ProcessEvent(HClass *pObject, HVar Arg0, HVar Arg1, HVar Arg2, HVar Arg3, HVar Arg4) {
		HEvent* pEvent = new HEvent(this);
		pEvent->m_pObject = pObject;
		HVar Var[2];
		Var[0] = HVar((INT)0);
		Var[1] = Arg0;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)1);
		Var[1] = Arg1;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)2);
		Var[1] = Arg2;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)3);
		Var[1] = Arg3;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)4);
		Var[1] = Arg4;
		pEvent->SetEventArg(Var);
		return HEventDef::ProcessEvent(pEvent);
	}
protected:
	typedef HVar ( HClass::*EventCallbackT )( HVar* );
	virtual HVar RunInline(HEvent *pEvent, HClass *pObj, CallbackT Func) {
		HVar Var;
		Var = (pObj->*(EventCallbackT)Func)(pEvent->m_pArgs);
		return Var;
	}
};
class HEventDef6 : public HEventDef {
public:
	HEventDef6( const TCHAR *pCommand, const TCHAR *pFormatspec = "", TCHAR ReturnType = 0 )
		:	HEventDef(pCommand, pFormatspec, ReturnType) {}
	HVar ProcessEvent(HClass *pObject, HVar Arg0, HVar Arg1, HVar Arg2, HVar Arg3, HVar Arg4, HVar Arg5) {
		HEvent* pEvent = new HEvent(this);
		pEvent->m_pObject = pObject;
		HVar Var[2];
		Var[0] = HVar((INT)0);
		Var[1] = Arg0;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)1);
		Var[1] = Arg1;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)2);
		Var[1] = Arg2;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)3);
		Var[1] = Arg3;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)4);
		Var[1] = Arg4;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)5);
		Var[1] = Arg5;
		pEvent->SetEventArg(Var);
		return HEventDef::ProcessEvent(pEvent);
	}
protected:
	typedef HVar ( HClass::*EventCallbackT )( HVar* );
	virtual HVar RunInline(HEvent *pEvent, HClass *pObj, CallbackT Func) {
		HVar Var;
		Var = (pObj->*(EventCallbackT)Func)(pEvent->m_pArgs);
		return Var;
	}
};
class HEventDef7 : public HEventDef {
public:
	HEventDef7( const TCHAR *pCommand, const TCHAR *pFormatspec = "", TCHAR ReturnType = 0 )
		:	HEventDef(pCommand, pFormatspec, ReturnType) {}
	HVar ProcessEvent(HClass *pObject, HVar Arg0, HVar Arg1, HVar Arg2, HVar Arg3, HVar Arg4, HVar Arg5, HVar Arg6) {
		HEvent* pEvent = new HEvent(this);
		pEvent->m_pObject = pObject;
		HVar Var[2];
		Var[0] = HVar((INT)0);
		Var[1] = Arg0;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)1);
		Var[1] = Arg1;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)2);
		Var[1] = Arg2;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)3);
		Var[1] = Arg3;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)4);
		Var[1] = Arg4;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)5);
		Var[1] = Arg5;
		pEvent->SetEventArg(Var);
		Var[0] = HVar((INT)6);
		Var[1] = Arg6;
		pEvent->SetEventArg(Var);
		return HEventDef::ProcessEvent(pEvent);
	}
protected:
	typedef HVar ( HClass::*EventCallbackT )( HVar* );
	virtual HVar RunInline(HEvent *pEvent, HClass *pObj, CallbackT Func) {
		HVar Var;
		Var = (pObj->*(EventCallbackT)Func)(pEvent->m_pArgs);
		return Var;
	}
};

/* -------------------------- End -------------------------- */