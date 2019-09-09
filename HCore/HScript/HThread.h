
HScriptThread *HScriptThread::m_pCurThread;
HTArray<HVar> HScriptThread::m_DataStack;

HScriptThread::HScriptThread(HScript *pScript) {

	FScriptFunc		*pFunc;
	HEventDef			*pEd;

	m_pScript = pScript;
	if (!m_pScript->m_bIsCompiled)
		m_pScript->Compile();

	m_StaticVars.Empty(m_pScript->m_StaticVars.Num()+1);
	m_StaticVars.AddZeroed(m_pScript->m_StaticVars.Num()+1);

	 // BUGFIX!  * Exception PopData() when m_DataStack.Num() = 1 *
	//m_DataStack.AddItem(HVar());

	for(HTMultiMap<FString, FScriptFunc*>::FIterator it(m_pScript->m_Functions);it;++it) {
		pFunc = it.Value();
		if (pFunc->m_bIsEvent) {
			pEd = HEventDef::FindEvent(pFunc->m_pName);
			if (pEd) {
				if ((pEd->m_ReturnType != pFunc->m_Type)||
					(strcmp( (const TCHAR*)&pFunc->m_Formatspec, pEd->m_pFormatspec) != 0 )) 
				{
					ScriptError("m_pScript event %s defined differing format.", pFunc->m_pName);
				}
				else
					m_ScriptEvents.Add(pEd->m_nEventNum, pFunc);
			}
			else {
				INT nNumParms = strlen( (const TCHAR*)&pFunc->m_Formatspec );
				if (nNumParms == 0)
					pEd = new HEventDef0(pFunc->m_pName, (const TCHAR*)&pFunc->m_Formatspec, pFunc->m_Type);
				else if (nNumParms == 1)
					pEd = new HEventDef1(pFunc->m_pName, (const TCHAR*)&pFunc->m_Formatspec, pFunc->m_Type);
				else if (nNumParms == 2)
					pEd = new HEventDef2(pFunc->m_pName, (const TCHAR*)&pFunc->m_Formatspec, pFunc->m_Type);
				else if (nNumParms == 3)
					pEd = new HEventDef3(pFunc->m_pName, (const TCHAR*)&pFunc->m_Formatspec, pFunc->m_Type);
				else if (nNumParms == 4)
					pEd = new HEventDef4(pFunc->m_pName, (const TCHAR*)&pFunc->m_Formatspec, pFunc->m_Type);
				else if (nNumParms == 5)
					pEd = new HEventDef5(pFunc->m_pName, (const TCHAR*)&pFunc->m_Formatspec, pFunc->m_Type);
				else if (nNumParms == 6)
					pEd = new HEventDef6(pFunc->m_pName, (const TCHAR*)&pFunc->m_Formatspec, pFunc->m_Type);
				else if (nNumParms == 7)
					pEd = new HEventDef7(pFunc->m_pName, (const TCHAR*)&pFunc->m_Formatspec, pFunc->m_Type);

				HEventDef::m_TempEventDefs.AddItem(pEd);
				m_ScriptEvents.Add(pEd->m_nEventNum, pFunc);
			}
		}
	}

	m_pContFunc = NULL;
	m_nCurIP = 0;
}
HScriptThread::~HScriptThread() {
	m_StaticVars.Empty();
	m_ScriptEvents.Empty();
}
const TCHAR *HScriptThread::ScriptError(const TCHAR *pFormat, ...) {
	TCHAR Error[1024] = {0};
	FString ErrStr = Format("Script error in %s(%i) : ",
		*m_pScript->m_Buffers[m_pScript->m_ScriptCode(m_nCurIP).FileNum].Key,
		m_pScript->m_ScriptCode(m_nCurIP).nLineNum+1, 
		pFormat);
	va_list Arg;
	va_start(Arg, pFormat);
	vsprintf(Error, pFormat, Arg);
	va_end(Arg);
	if(GLog) {
		GLog->Log(*ErrStr);
		GLog->Log(Error);
		GLog->Log("\n");
	}
	return 0;
}
HBOOL HScriptThread::ScenarioStarted() {
	return m_pContFunc != NULL;
}
HVar HScriptThread::PopData() {
	if(m_DataStack.Num())
		return m_DataStack.Pop();
	return HVar();
}
void HScriptThread::PushData(HVar* v) {
	m_DataStack.AddItem(*v);
}
HVar HScriptThread::Execute(FString ScriptName) {
	FScriptFunc* pOldFunc = m_pCurFunc;
	m_pCurThread = this;
	m_pCurFunc = m_pScript->m_Functions.FindRef(ScriptName);

	if (!m_pCurFunc){
		ScriptError("Script function %s not found", *ScriptName);
		m_pCurFunc = pOldFunc;
		return HVar();
	}

	HTArray<HVar>  CurrentVars(m_pCurFunc->m_lVars.Num());

	INT OldIP = m_nCurIP;
	HTArray<HVar>* pOldLocalVars = m_pLocalVars;
	m_pLocalVars = &CurrentVars;

	// Trans function arguments from stack top.
	INT nNumParms = strlen( (const TCHAR*)&m_pCurFunc->m_Formatspec );
	for(INT i=0; i<nNumParms; i++)
		(*m_pLocalVars)(nNumParms-i-1) = PopData();
	for(i=nNumParms; i<m_pCurFunc->m_lVars.Num(); i++) {
		switch((m_pCurFunc->m_lVars)[i-nNumParms].Value) {
		case 'i':
			((*m_pLocalVars)(i)) = HVar(INT(0));
			break;
		case 'f':
			((*m_pLocalVars)(i)) = HVar(FLOAT(0));
			break;
		case 'v':
			((*m_pLocalVars)(i)) = HVar(HVec3());
			break;
		case 'q':
			((*m_pLocalVars)(i)) = HVar(HQuat());
			break;
		case 's':
			((*m_pLocalVars)(i)) = HVar(FString());
			break;
		default:
			(*m_pLocalVars)(i) = HVar();
		}		
	}

	m_nCurIP = m_pCurFunc->m_nCodeLabel;

	ExecCode();

	CurrentVars.Empty();
	m_pLocalVars = pOldLocalVars;
	m_nCurIP = OldIP;
	m_pCurFunc = pOldFunc;

	return PopData();
}
HVar HScriptThread::ContScenario() {
	if (!ScenarioStarted()){
		ScriptError("Scenario not started");
		return HVar();
	}

	FScriptFunc* pOldFunc = m_pCurFunc;
	m_pCurThread = this;
	m_pCurFunc = m_pContFunc;

	HTArray<HVar>  CurrentVars(m_pCurFunc->m_lVars.Num()); 

	INT OldIP = m_nCurIP;
	HTArray<HVar>* pOldLocalVars = m_pLocalVars;
	m_pLocalVars = &CurrentVars;

	for(INT i=0; i<m_pCurFunc->m_lVars.Num(); i++)
		(*m_pLocalVars)(i) = HVar();

	m_nCurIP = m_nContIP;

	ExecCode();

	CurrentVars.Empty();
	m_pLocalVars = pOldLocalVars;
	m_nCurIP = OldIP;
	m_pCurFunc = pOldFunc;

	return PopData();
}
void HScriptThread::ExecCode() {
	FVMCommand* CurCom;
	HVar TempVar;
	INT	nT;
	INT nNewIP;
	FPropertiesT<HClass> *pProp;
	HEventDef *pEvDef;
	HEvent *pEd;
	HClass *pTempCls;
	HClassDef *pClsDef;

	CurCom = &m_pScript->m_ScriptCode(m_nCurIP);
	do {
		switch (CurCom->OpCode) {
			case VM_VAR_GET:
				nT = (INT)CurCom->Param;
				if (nT<0 || nT>=m_pLocalVars->Num()) {
					PushData(&HVar());
					ScriptError("local var not found");
				}
				else {
					TempVar = (*m_pLocalVars)(nT);
					PushData(&TempVar);
				}
				break;
			case VM_VAR_SET:
				nT = (INT)CurCom->Param;
				if (nT<0 || nT>=m_pLocalVars->Num())
					ScriptError("local var not found");
				else
					(*m_pLocalVars)(nT) = PopData();
				break;
			case VM_SVAR_GET:
				nT = (INT)CurCom->Param;
				if (nT<0 || nT>=m_StaticVars.Num()) {
					PushData(&HVar());
					ScriptError("static var not found");
				}
				else
					PushData(&m_StaticVars(nT));
				break;
			case VM_SVAR_SET:
				nT = (INT)CurCom->Param;
				if (nT<0 || nT>=m_StaticVars.Num())
					ScriptError("static var not found");
				else
					m_StaticVars(nT) = PopData();
				break;
			case VM_CONST:
				PushData(&CurCom->Param);
				break;
			case VM_NULL:
				PushData(&HVar());
				break;
			case VM_OBJ_NEW:
				TempVar = HClass::CreateClass((const TCHAR*)CurCom->Param);
				if(!(HClass*)TempVar)
					ScriptError("Class %s not found", (const TCHAR*)CurCom->Param);
				PushData(&TempVar);
				break;
			case VM_OBJ_DEL:
				TempVar = PopData();
				if (TempVar.GetType() == HVar::Pointer)
					delete (void*)TempVar;
				else
					ScriptError("can`t delete non pointer var");
				break;
			case VM_OBJ_THIS:
				TempVar = m_pObject;
				PushData(&TempVar);
				break;
			case VM_OBJ_GET:
				TempVar = PopData();
				if (TempVar.GetType()==HVar::Pointer) {
					pTempCls = ((HClass*)TempVar);
					pClsDef = pTempCls->GetClassDef();
					pProp = pClsDef->m_PropertiesList.FindRef((FString)CurCom->Param);

					if (pProp) {
						TempVar = (((HClass*)TempVar)->*(pProp->GetProp))();
						PushData( &TempVar );
					}
					else {
						PushData(&HVar());
						ScriptError("property %s not found", (const TCHAR*)CurCom->Param);
					}
				}
				else  if ((TempVar.GetType()==HVar::Vec3)) {
					if ((FString)CurCom->Param == "x")
						TempVar = HVar((TempVar).GetVec3()[0]);
					else if ((FString)CurCom->Param == "y")
						TempVar = HVar((TempVar).GetVec3()[1]);
					else if ((FString)CurCom->Param == "z")
						TempVar = HVar((TempVar).GetVec3()[2]);
					else {
						TempVar = HVar();
						ScriptError("unknow vector component");
					}
					PushData( &TempVar );
				}  else if ((TempVar.GetType()==HVar::Quat)) {
					if ((FString)CurCom->Param == "x")
						TempVar = HVar((TempVar).GetQuat()[0]);
					else if ((FString)CurCom->Param == "y")
						TempVar = HVar((TempVar).GetQuat()[1]);
					else if ((FString)CurCom->Param == "z")
						TempVar = HVar((TempVar).GetQuat()[2]);
					else if ((FString)CurCom->Param == "w")
						TempVar = HVar((TempVar).GetQuat()[3]);
					else {
						TempVar = HVar();
						ScriptError("unknow vector component");
					}
					PushData( &TempVar );
				}
				else {
					PushData( &TempVar );
					ScriptError("bad type of the -> operand");
				}
				break;
			case VM_OBJ_SET:
				TempVar = PopData();
				if (TempVar.GetType()==HVar::Pointer) {
					pTempCls = ((HClass*)TempVar);
					pClsDef = pTempCls->GetClassDef();
					pProp = pClsDef->m_PropertiesList.FindRef((FString)CurCom->Param);
					if (pProp) {
						(((HClass*)TempVar)->*(pProp->SetProp))(PopData());
					}
					else
						ScriptError("property %s not found", (const TCHAR*)CurCom->Param);
				}
				else if ((TempVar.GetType()==HVar::Vec3)) {
					if ((FString)CurCom->Param == "x")
						(TempVar).GetVec3()[0] = PopData();
					else if ((FString)CurCom->Param == "y")
						(TempVar).GetVec3()[1] = PopData();
					else if ((FString)CurCom->Param == "z")
						(TempVar).GetVec3()[2] = PopData();
					else
						ScriptError("unknow vector component");
				} else if ((TempVar.GetType()==HVar::Quat)) {
					if ((FString)CurCom->Param == "x")
						(TempVar).GetQuat()[0] = PopData();
					else if ((FString)CurCom->Param == "y")
						(TempVar).GetQuat()[1] = PopData();
					else if ((FString)CurCom->Param == "z")
						(TempVar).GetQuat()[2] = PopData();
					else if ((FString)CurCom->Param == "w")
						(TempVar).GetQuat()[3] = PopData();
					else
						ScriptError("unknow vector component");
				}
				else
					ScriptError("bad type of the -> operand");
				break;
			case VM_OBJ_CALL:
			case VM_OBJ_ICALL:
				pEvDef = HEventDef::FindEvent((FString)CurCom->Param);
				if (pEvDef){
					pEd = new HEvent(pEvDef);
					for (INT i=(pEvDef->m_nNumArgs-1); i>=0; i--) {
						pEd->m_pArgs[i] = PopData();
					}
					TempVar = PopData();
					if (CurCom->OpCode == VM_OBJ_ICALL)
						TempVar = ((HClass*)TempVar)->ProcessEvent(pEd, true);
					else
						TempVar = ((HClass*)TempVar)->ProcessEvent(pEd, false);
					PushData( &TempVar );
				}
				else {
					ScriptError("event %s not found", (const TCHAR*)CurCom->Param);
					PushData(&HVar());
				}
				break;
			case VM_JMP:
				nNewIP = (INT)CurCom->Param;
				m_nCurIP = --nNewIP;
				break;
			case VM_JMP_IFNOT:
				if ((INT)PopData()) {
					nNewIP = CurCom->Param;
					m_nCurIP = --nNewIP;
				}
				break;
			case VM_JMP_IF:
				if (!(INT)PopData()) {
					nNewIP = CurCom->Param;
					m_nCurIP = --nNewIP;
				}
				break;
			case VM_STD:
				HScript::m_NativeFuncList[(INT)CurCom->Param].Value.StdFunc();
				break;
			case VM_SCRIPT_CALL:
				TempVar = Execute((FString)CurCom->Param);
				PushData(&TempVar);
				break;
			case VM_SCRIPT_CONT:
				TempVar = ContScenario();
				PushData(&TempVar);
				break;
			case VM_RET:
				if ((INT)CurCom->Param == 1)
					PushData(&HVar(0));
				if (m_pContFunc == m_pCurFunc)
					m_pContFunc = NULL;
				return;
			case VM_WAIT:
				if ((INT)CurCom->Param == 1)
					PushData(&HVar(0));
				m_pContFunc = m_pCurFunc;
				m_nContIP = m_nCurIP + 1;
				return;
		}
		CurCom = &m_pScript->m_ScriptCode(++m_nCurIP);
	} while(CurCom->OpCode != VM_RET);
}
//
// Standart operations.
//
void HScriptThread::StdAdd() {
	guardSlow(HScriptThread::StdAdd);
	HVar Res;
	HVar *pOp1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pOp2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pOp1->GetType()) {
	case HVar::Int:
		if(pOp2->GetType() == HVar::Int)
			Res = ((INT)*pOp1)+((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float)
			Res = ((INT)*pOp1)+((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the + operand\n");
		break;
	case HVar::Float:
		if(pOp2->GetType() == HVar::Int)
			Res = ((FLOAT)*pOp1)+((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float)
			Res = ((FLOAT)*pOp1)+((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the + operand\n");
		break;
	case HVar::Vec2:
		if ((pOp2->GetType() == HVar::Int)||(pOp2->GetType() == HVar::Float)) {
			FLOAT Tmp = ((FLOAT)*pOp2);
			Res = (HVec2)*pOp1 + HVec2(Tmp, Tmp);
		}
		else if(pOp2->GetType() == HVar::Vec2) 
			Res = (HVec2)*pOp1+(HVec2)*pOp2;
		else
			m_pCurThread->ScriptError("bad type of the + operand\n");
		break;
	case HVar::Vec3:
		if ((pOp2->GetType() == HVar::Int)||(pOp2->GetType() == HVar::Float)) {
			FLOAT Tmp = ((FLOAT)*pOp2);
			Res = (HVec3)*pOp1 + HVec3(Tmp, Tmp, Tmp);
		}
		else if(pOp2->GetType() == HVar::Vec3) 
			Res = (HVec3)*pOp1+(HVec3)*pOp2;
		else
			m_pCurThread->ScriptError("bad type of the + operand\n");
		break;
	case HVar::Quat:
		if ((pOp2->GetType() == HVar::Int)||(pOp2->GetType() == HVar::Float)) {
			FLOAT Tmp = ((FLOAT)*pOp2);
			Res = (HQuat)*pOp1 + HQuat(Tmp, Tmp, Tmp, Tmp);
		}
		else if(pOp2->GetType() == HVar::Quat) 
			Res = (HQuat)*pOp1+(HQuat)*pOp2;
		else
			m_pCurThread->ScriptError("bad type of the + operand\n");
		break;
	case HVar::String:
		TCHAR Buf[256];
		if(pOp2->GetType() == HVar::String) {
			Res = ((FString)*pOp1)+((FString)*pOp2);
		}
		else if(pOp2->GetType() == HVar::Int) {
			sprintf(Buf,"%d",(INT)*pOp2);
			Res = ((FString)*pOp1) + Buf;
		} else if(pOp2->GetType() == HVar::Float) {
			sprintf(Buf,"%g",(FLOAT)*pOp2);
			Res = ((FString)*pOp1) + Buf;
		}
		else
			m_pCurThread->ScriptError("bad type of the + operand\n");
		break;
	default:
		m_pCurThread->ScriptError("bad type of the + operand\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdSub() {
	guardSlow(HScriptThread::StdSub);
	HVar Res;
	HVar *pOp1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pOp2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pOp1->GetType()) {
	case HVar::Int:
		if(pOp2->GetType() == HVar::Int)
			Res = ((INT)*pOp1)-((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float)
			Res = ((INT)*pOp1)-((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the + operand\n");
		break;
	case HVar::Float:
		if(pOp2->GetType() == HVar::Int)
			Res = ((FLOAT)*pOp1)-((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float)
			Res = ((FLOAT)*pOp1)-((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the + operand\n");
		break;
	case HVar::Vec2:
		if ((pOp2->GetType() == HVar::Int)||(pOp2->GetType() == HVar::Float)) {
			FLOAT Tmp = ((FLOAT)*pOp2);
			Res = (HVec2)*pOp1-HVec2(Tmp, Tmp);
		}
		else if(pOp2->GetType() == HVar::Vec2) 
			Res = (HVec2)*pOp1-(HVec2)*pOp2;
		else
			m_pCurThread->ScriptError("bad type of the + operand\n");
		break;
	case HVar::Vec3:
		if ((pOp2->GetType() == HVar::Int)||(pOp2->GetType() == HVar::Float)) {
			FLOAT Tmp = ((FLOAT)*pOp2);
			Res = (HVec3)*pOp1-HVec3(Tmp, Tmp, Tmp);
		}
		else if(pOp2->GetType() == HVar::Vec3) 
			Res = (HVec3)*pOp1-(HVec3)*pOp2;
		else
			m_pCurThread->ScriptError("bad type of the + operand\n");
		break;
	case HVar::Quat:
		if ((pOp2->GetType() == HVar::Int)||(pOp2->GetType() == HVar::Float)) {
			FLOAT Tmp = ((FLOAT)*pOp2);
			Res = (HQuat)*pOp1-HQuat(Tmp, Tmp, Tmp, Tmp);
		}
		else if(pOp2->GetType() == HVar::Quat) 
			Res = (HQuat)*pOp1-(HQuat)*pOp2;
		else
			m_pCurThread->ScriptError("bad type of the + operand\n");
		break;
	default:
		m_pCurThread->ScriptError("bad type of the + operand\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdMul() {
	guardSlow(HScriptThread::StdMul);
	HVar Res;
	HVar *pOp1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pOp2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pOp1->GetType()) {
	case HVar::Int:
		if(pOp2->GetType() == HVar::Int)
			Res = ((INT)*pOp1)*((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float)
			Res = ((INT)*pOp1)*((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the * operand\n");
		break;
	case HVar::Float:
		if(pOp2->GetType() == HVar::Int)
			Res = ((FLOAT)*pOp1)*((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float)
			Res = ((FLOAT)*pOp1)*((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the * operand\n");
		break;
	default:
		m_pCurThread->ScriptError("bad type of the * operand\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdDiv() {
	guardSlow(HScriptThread::StdDiv);
	HVar Res;
	HVar *pOp1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pOp2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pOp1->GetType()) {
	case HVar::Int:
		if(pOp2->GetType() == HVar::Int)
			Res = ((INT)*pOp1)/((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float)
			Res = ((INT)*pOp1)/((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the / operand\n");
		break;
	case HVar::Float:
		if(pOp2->GetType() == HVar::Int)
			Res = ((FLOAT)*pOp1)/((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float)
			Res = ((FLOAT)*pOp1)/((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the / operand\n");
		break;
	default:
		m_pCurThread->ScriptError("bad type of the / operand\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdMod() {
	guardSlow(HScriptThread::StdMod);
	unguardSlow;
}
void HScriptThread::StdBitAnd() {
	guardSlow(HScriptThread::StdBitAnd);
	HVar Res;
	HVar *pOp1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pOp2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pOp1->GetType()) {
	case HVar::Int:
		if(pOp2->GetType() == HVar::Int) 
			Res = (INT)*pOp1&(INT)*pOp2;
		else
			m_pCurThread->ScriptError("bad type of the && operand\n");
		break;
	default:
		m_pCurThread->ScriptError("bad type of the && operand\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdBitOr() {
	guardSlow(HScriptThread::StdBitOr);
	unguardSlow;
}
void HScriptThread::StdLT() {
	guardSlow(HScriptThread::StdLT);
	HVar Res;
	HVar *pOp1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pOp2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pOp1->GetType()) {
	case HVar::Int:
		if(pOp2->GetType() == HVar::Int) 
			Res = ((INT)*pOp1)<((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float) 
			Res = ((INT)*pOp1)<((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the < operand\n");
		break;
	case HVar::Float:
		if(pOp2->GetType() == HVar::Int) 
			Res = ((FLOAT)*pOp1)<((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float) 
			Res = ((FLOAT)*pOp1)<((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the < operand\n");
		break;
	default:
		m_pCurThread->ScriptError("bad type of the < operand\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdGT() {
	guardSlow(HScriptThread::StdGT);
	HVar Res;
	HVar *pOp1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pOp2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pOp1->GetType()) {
	case HVar::Int:
		if(pOp2->GetType() == HVar::Int) 
			Res = ((INT)*pOp1)>((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float) 
			Res = ((INT)*pOp1)>((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the > operand\n");
		break;
	case HVar::Float:
		if(pOp2->GetType() == HVar::Int) 
			Res = ((FLOAT)*pOp1)>((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float) 
			Res = ((FLOAT)*pOp1)>((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the > operand\n");
		break;
	default:
		m_pCurThread->ScriptError("bad type of the > operand\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdLE() {
	guardSlow(HScriptThread::StdLE);
	HVar Res;
	HVar *pOp1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pOp2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pOp1->GetType()) {
	case HVar::Int:
		if(pOp2->GetType() == HVar::Int) 
			Res = ((INT)*pOp1)<=((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float) 
			Res = ((INT)*pOp1)<=((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the <= operand\n");
		break;
	case HVar::Float:
		if(pOp2->GetType() == HVar::Int) 
			Res = ((FLOAT)*pOp1)<=((INT)*pOp2);
		else if(pOp2->GetType() == HVar::Float) 
			Res = ((FLOAT)*pOp1)<=((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the <= operand\n");
		break;
	default:
		m_pCurThread->ScriptError("bad type of the <= operand\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdGE() {
	guardSlow(HScriptThread::StdGE);
	unguardSlow;
}
void HScriptThread::StdEQ() {
	guardSlow(HScriptThread::StdEQ);
	HVar Res;
	HVar *pOp1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pOp2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pOp1->GetType()) {
	case HVar::Int:
		if ((pOp2->GetType() == HVar::Int)||(pOp2->GetType() == HVar::Float))
			Res = ((INT)*pOp1)==((INT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the == operand\n");
		break;
	case HVar::Float:
		if ((pOp2->GetType() == HVar::Int)||(pOp2->GetType() == HVar::Float))
			Res = ((FLOAT)*pOp1)==((FLOAT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the == operand\n");
		break;
	case HVar::String:
		if (pOp2->GetType() == HVar::String)
			Res = ((FString)*pOp1)==((FString)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the == operand\n");
		break;
	case HVar::Pointer:
		if (pOp2->GetType() == HVar::Pointer)
			Res = ((void*)*pOp1)==((void*)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the == operand\n");
		break;
	case HVar::Void:
		Res = pOp2->GetType() == HVar::Void;
		break;
	default:
		m_pCurThread->ScriptError("bad type of the == operand\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdNE() {
	guardSlow(HScriptThread::StdNE);
	HVar Res;
	StdEQ();
	Res = !(INT)PopData();
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdAND() {
	guardSlow(HScriptThread::StdAND);
	HVar Res;
	HVar *pOp1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pOp2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pOp1->GetType()) {
	case HVar::Int:
		if(pOp2->GetType() == HVar::Int) 
			Res = ((INT)*pOp1)&&((INT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the && operand\n");
		break;
	default:
		m_pCurThread->ScriptError("bad type of the && operand\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdOR() {
	guardSlow(HScriptThread::StdOR);
	HVar Res;
	HVar *pOp1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pOp2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pOp1->GetType()) {
	case HVar::Int:
		if(pOp2->GetType() == HVar::Int) 
			Res = ((INT)*pOp1)||((INT)*pOp2);
		else
			m_pCurThread->ScriptError("bad type of the || operand\n");
		break;
	default:
		m_pCurThread->ScriptError("bad type of the || operand\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdNOT() {
	guardSlow(HScriptThread::StdNOT);
	HVar Res =	PopData();
	if (Res.GetType() == HVar::Int)
		Res = !(INT)Res;
	else
		m_pCurThread->ScriptError("bad type of the ! operand\n");
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdNEG() {
	guardSlow(HScriptThread::StdNEG);
	HVar Res =	PopData();
	switch (Res.GetType()) {
	case HVar::Int:
		Res = -(INT)Res;
		break;
	case HVar::Float:
		Res = -(FLOAT)Res;
		break;
	case HVar::Vec2:
		Res = -(HVec2)Res;
		break;
	case HVar::Vec3:
		Res = -(HVec3)Res;
		break;
	case HVar::Quat:
		Res = -(HQuat)Res;
		break;
	default:
		m_pCurThread->ScriptError("bad type of the unary - operand\n");
	}
	PushData(&Res);
	unguardSlow;
}
//
// Constructors.
//
void HScriptThread::StdInt() {
	guardSlow(HScriptThread::StdInt);
	HVar Res = PopData();
	Res = HVar((INT)Res);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdFloat() {
	guardSlow(HScriptThread::StdFloat);
	HVar Res = PopData();
	Res = HVar((FLOAT)Res);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdVec2() {
	guardSlow(HScriptThread::StdVec2);
	HVar Res;
	HVar *pArg1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pArg2 = &m_DataStack(m_DataStack.Num()-1);
	Res = HVar(HVec2((FLOAT)*pArg1, (FLOAT)*pArg2));
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdVec3() {
	guardSlow(HScriptThread::StdVec3);
	HVar Res;
	HVar *pArg1 = &m_DataStack(m_DataStack.Num()-3);
	HVar *pArg2 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pArg3 = &m_DataStack(m_DataStack.Num()-1);
	Res = HVar(HVec3((FLOAT)*pArg1, (FLOAT)*pArg2, (FLOAT)*pArg3));
	m_DataStack.Remove(m_DataStack.Num()-3, 3);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdQuat() {
	guardSlow(HScriptThread::StdQuat);
	HVar Res;
	HVar *pArg1 = &m_DataStack(m_DataStack.Num()-4);
	HVar *pArg2 = &m_DataStack(m_DataStack.Num()-3);
	HVar *pArg3 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pArg4 = &m_DataStack(m_DataStack.Num()-1);
	Res = HVar(HQuat((FLOAT)*pArg1, (FLOAT)*pArg2, (FLOAT)*pArg3, (FLOAT)*pArg4));
	m_DataStack.Remove(m_DataStack.Num()-4, 4);
	PushData(&Res);
	unguardSlow;
}
//
// Standart functions.
//
void HScriptThread::StdPrint() {
	guardSlow(HScriptThread::StdPrint);
	HVar Res =	PopData();
	if(GLog) GLog->Log((const TCHAR*)Res);
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdAbs() {
	guardSlow(HScriptThread::StdAbs);
	HVar Res =	PopData();
	Res = (INT)Res < 0 ? HVar(-(INT)Res) : Res;
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdMin() {
	guardSlow(HScriptThread::StdMin);
	unguardSlow;
}
void HScriptThread::StdMax() {
	guardSlow(HScriptThread::StdMax);
	unguardSlow;
}
void HScriptThread::StdRand() {
	guardSlow(HScriptThread::StdRand);
	HVar Res = HVar((INT)appRand());
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdSeconds() {
	guardSlow(HScriptThread::StdSeconds);
	HVar Res = HVar(appSeconds().GetFloat());
	PushData(&Res);
	unguardSlow;
}
void HScriptThread::StdDist() {
	guardSlow(HScriptThread::StdDist);
	HVar Res;
	HVar *pArg1 = &m_DataStack(m_DataStack.Num()-2);
	HVar *pArg2 = &m_DataStack(m_DataStack.Num()-1);
	switch (pArg1->GetType()) {
	case HVar::Vec3:
		if(pArg2->GetType() == HVar::Vec3) 
			Res = HVar((FLOAT)(Distance((HVec3)*pArg1, (HVec3)*pArg2)));
		else
			m_pCurThread->ScriptError("bad type of the dist argument\n");
		break;
	default:
		m_pCurThread->ScriptError("bad type of the dist argument\n");
	}
	m_DataStack.Remove(m_DataStack.Num()-2, 2);
	PushData(&Res);
	unguardSlow;
}
