//
// Enum virtual machine state.
//
enum EVMOpcode {
	VM_VAR_GET,
	VM_VAR_SET,
	VM_SVAR_GET,
	VM_SVAR_SET,
	VM_CONST,
	VM_NULL,
	VM_OBJ_NEW,
	VM_OBJ_DEL,
	VM_OBJ_GET,
	VM_OBJ_SET,
	VM_OBJ_CALL,
	VM_OBJ_ICALL,
	VM_OBJ_THIS,
	VM_SCRIPT_CALL,
	VM_SCRIPT_CONT,
	VM_JMP,
	VM_JMP_IF,
	VM_JMP_IFNOT,
	VM_STD,
	VM_RET,
	VM_WAIT,
	VM_TMP_CONT,
	VM_TMP_BREAK
};
struct FVMCommand {
    BYTE	OpCode;
	HVar	Param;
	BYTE	FileNum;
	INT		nLineNum;
	FScriptFunc *pFunc;
};
struct FNativeConst {
	FString	Name;
	HVar		Const;
};
struct FXStdOp {
	void		(*StdFunc)();
	INT			nPriority;
	HBOOL		operator == (FXStdOp Other) const {
		return StdFunc == Other.StdFunc;
	}
};
class HCORE_API HScript : public HParser {
public:
	static HTMultiMap<FString, FXStdOp>	m_NativeFuncList;
	static HTMultiMap<FString, HVar>	m_NativeConstList;

	static void	PrepareScriptSystem();
	static void	ShutdownScriptSystem();
	static HScript* AddScript(const TCHAR* pName);
	static void	AddNativeScriptFunc(const TCHAR* pName, INT nPriority, void (*StdFunc)());
	static void	AddNativeConst(const TCHAR* pName, HVar Const);

public:
	HTArray<FVMCommand>					m_ScriptCode;
	HTMultiMap<FString, FScriptFunc*>	m_Functions;
	HTMultiMap<FString, TCHAR>			m_StaticVars;

	HScript();
	~HScript();

	HBOOL	m_bIsCompiled;
	HBOOL	Compile();
	HBOOL	LoadFromFile(const TCHAR *pName);
	HBOOL	WriteCompiled(const TCHAR *pName);
	HTBuffer<TCHAR> Disassemble();

protected:
	FScriptFunc *m_pCurFunc;

	inline INT  GetStdOp(FString Str);
	inline HVar GetNativeConst(FString Str);
	inline void AddOpcodeStd(INT stdop);
	void AddOpcode(INT OpCode, HVar Par);
	void AddOpcodeGetVar(const FString &VarName);
	void AddOpcodeSetVar(const FString &VarName);

	TCHAR ParseType();
	void ParseExpression();
	INT  ParseArgs();
	void PatchLoop( INT nStart, INT nContPos ); 
	void ParseReturnStatement( HBOOL bWait = false );
	void ParseIfStatement(); 
	void ParseWhileStatement();
	void ParseForStatement(); 
	void ParseDoWhileStatement(); 
	void ParseStatement(); 
	void ParseDefsAssign();
	void ParseAssign();
	void ParseVariableDef( TCHAR Type, const TCHAR *pName, HBOOL bIsStatic );
	void ParseFunctionDef( TCHAR ReturnType, const TCHAR *pName, HBOOL bIsEvent );
};

class HCORE_API HScriptThread : public HClass {
public:
	friend class HEventDef;
	friend class HScript;

	HClass			*m_pObject;
	HTArray<HVar>	m_StaticVars;
	HTArray<HVar>	*m_pLocalVars;
	HScript			*m_pScript;
	HTMultiMap<INT, FScriptFunc*> m_ScriptEvents;

	FOutputDevice		*m_pOutDev;

	HBOOL ScenarioStarted();
	HScriptThread(HScript *pScript);
	~HScriptThread();

	HVar Execute(FString ScriptName);
	HVar ContScenario();
	const TCHAR *ScriptError(const TCHAR *pFormat, ...);

	static HScriptThread	*m_pCurThread;
	static HVar PopData();
	static void PushData(HVar* Var);

protected:
	static HTArray<HVar>	m_DataStack;
	FScriptFunc				*m_pCurFunc;
	INT						m_nCurIP;
	FScriptFunc				*m_pContFunc;
	INT						m_nContIP;

	void ExecCode();

	// Operations.
	static void StdMul();
	static void StdDiv();
	static void StdMod();
	static void StdBitAnd();
	static void StdBitOr();
	static void StdAdd();
	static void StdSub();
	static void StdLT();
	static void StdGT();
	static void StdLE();
	static void StdGE();
	static void StdEQ();
	static void StdNE();
	static void StdAND();
	static void StdOR();
	static void StdNOT();
	static void StdNEG();

	// Constructors.
	static void StdInt();
	static void StdFloat();
	static void StdVec2();
	static void StdVec3();
	static void StdQuat();

	// Functions.
	static void StdPrint();
	static void StdAbs();
	static void StdMin();
	static void StdMax();
	static void StdRand();
	static void StdSeconds();
	static void StdDist();
};

void HClass::ExecuteThread(const TCHAR* EntryPoint) {
	m_pThread->Execute(EntryPoint);
}
//--------------------------------------------------------------------
//
// Process event (need declaration HScriptThread for script events).
//
HVar HEventDef::ProcessEvent(HEvent *pEvent, HBOOL bIn) {
	HClass *pObj = pEvent->m_pObject;
	FScriptFunc *pScrFunc;
	check(pObj);
	HScriptThread	*pThread = pObj->m_pThread;
	if (pThread && !bIn) {
		pScrFunc = pThread->m_ScriptEvents.FindRef(this->m_nEventNum);
		if (pScrFunc) {
			return RunScript(pEvent, pThread, pScrFunc);
		}
		else
			return HVar();
	}
	CallbackT Func = pObj->GetResponse(this);
	if(Func)
		return RunInline(pEvent, pObj, Func);
	else 
		return HVar();
}
HVar HEventDef::RunScript(HEvent *pEvent, HScriptThread *pThread, FScriptFunc *pScriptFunc) {
	for (INT i=0; i<m_nNumArgs; i++)
		pThread->PushData(&((*pEvent)[i]));
	return pThread->Execute(pScriptFunc->m_pName);
}
FUNC_DECL( HClass, SetScript )
	TRANS_ARG(const TCHAR*, pName, 0);
	HScript *pScript = HScript::AddScript(pName);
	if (!pScript) 
		return HVar(0);
	if (m_pThread)
		delete m_pThread;
	m_pThread = new HScriptThread(pScript);
	m_pThread->m_pObject = this;
	return HVar(1);
END_FUNC
FUNC_DECL( HClass, SetEventScript )
	TRANS_ARG(const TCHAR*, pEventName,	0);
	TRANS_ARG(const TCHAR*, pName,		1);
	FScriptFunc	*pFunc;
	HEventDef		*pEd;
	if (m_pThread) {
		pEd = HEventDef::FindEvent(pEventName);
		pFunc = m_pThread->m_pScript->m_Functions.FindRef(pName);
		if (pEd && pFunc) {
			
			if ((pEd->m_ReturnType == pFunc->m_Type)&&
				(strcmp( (const TCHAR*)&pFunc->m_Formatspec, pEd->m_pFormatspec ) == 0 )) 
			{
				m_pThread->m_ScriptEvents.Add(pEd->m_nEventNum, pFunc);
				return HVar(1);
			}
			else
				m_pThread->ScriptError("script event %s defined differing format.", pName);
		}
		else
			m_pThread->ScriptError("event %s or script %s not found.", pEventName, pName);
	}
	return HVar(0);
END_FUNC

//--------------------------------------------------------------------

static GLoopDepth = 0;

static TCHAR *GpPunctuation[] = {
	"&&", "||", "<=", ">=", "==", "!=",
	"+=", "-=", "*=", "/=", "++", "--",
	"::", "->",
	"~",  "*",  "/",  "%",  "-", "+", "&",  "|",
	"<",  ">", "!",
	"(",  ")",
	NULL
};

static TCHAR *GpReserveWords[] = {
	"if", "else", "do", "while", "for", "break", "continue",
	"return", "wait", "thread",
	"int", "float", "string", "vec2", "vec3", "quat", "pointer", "void"
	"new", "delete", "this",
	NULL
};

// Standart native lists.
HTMultiMap<FString, FXStdOp>  HScript::m_NativeFuncList;
HTMultiMap<FString, HVar>	HScript::m_NativeConstList;

void HScript::PrepareScriptSystem() {
	// Native operations.
	AddNativeScriptFunc("*",		5, &HScriptThread::StdMul);
	AddNativeScriptFunc("/",		5, &HScriptThread::StdDiv);
	AddNativeScriptFunc("%",		5, &HScriptThread::StdMod);
	AddNativeScriptFunc("&",		4, &HScriptThread::StdBitAnd);
	AddNativeScriptFunc("|",		4, &HScriptThread::StdBitOr);
	AddNativeScriptFunc("+",		3, &HScriptThread::StdAdd);
	AddNativeScriptFunc("-",		3, &HScriptThread::StdSub);
	AddNativeScriptFunc("<",		2, &HScriptThread::StdLT);
	AddNativeScriptFunc(">",		2, &HScriptThread::StdGT);
	AddNativeScriptFunc("<=",		2, &HScriptThread::StdLE);
	AddNativeScriptFunc(">=",		2, &HScriptThread::StdGE);
	AddNativeScriptFunc("==",		2, &HScriptThread::StdEQ);
	AddNativeScriptFunc("!=",		2, &HScriptThread::StdNE);
	AddNativeScriptFunc("&&",		1, &HScriptThread::StdAND);
	AddNativeScriptFunc("||",		1, &HScriptThread::StdOR);
	AddNativeScriptFunc("!u",		0, &HScriptThread::StdNOT);
	AddNativeScriptFunc("-u",		0, &HScriptThread::StdNEG);
	AddNativeScriptFunc("(",		0, NULL);
	// Native constructors.
	AddNativeScriptFunc("int",		7, &HScriptThread::StdInt);	// 1
	AddNativeScriptFunc("float",	7, &HScriptThread::StdFloat);	// 1
	AddNativeScriptFunc("vec2",		7, &HScriptThread::StdVec2);	// 2
	AddNativeScriptFunc("vec3",		7, &HScriptThread::StdVec3);	// 3
	AddNativeScriptFunc("quat",		7, &HScriptThread::StdQuat);	// 4
	// Native function.
	AddNativeScriptFunc("print",	6, &HScriptThread::StdPrint);	// 1
	AddNativeScriptFunc("abs",		6, &HScriptThread::StdAbs);	// 1
	AddNativeScriptFunc("min",		6, &HScriptThread::StdMin);	// 2
	AddNativeScriptFunc("max",		6, &HScriptThread::StdMax);	// 2
	AddNativeScriptFunc("rand",		6, &HScriptThread::StdRand);	// 0
	AddNativeScriptFunc("seconds",	6, &HScriptThread::StdSeconds);	// 0
	AddNativeScriptFunc("dist",		6, &HScriptThread::StdDist);	// 2
	// Native constants.
	AddNativeConst("true",			(INT)1);
	AddNativeConst("false",			(INT)0);
	AddNativeConst("INDEX_NONE",	INDEX_NONE);
	AddNativeConst("MAX_PATH",		MAX_PATH);
	AddNativeConst("KINDA_SMALL",	0.0001f);
	AddNativeConst("SMALL",			0.00000001f);
	AddNativeConst("PI",			3.14159265358979324f);
	AddNativeConst("RAND_MAX",		RAND_MAX);
}
void HScript::ShutdownScriptSystem() {
	HScriptThread::m_DataStack.Empty();
	m_NativeFuncList.Empty();
	m_NativeConstList.Empty();
}
void HScript::AddNativeScriptFunc(const TCHAR* pName, INT nPriority, void (*StdFunc)()) {
	FXStdOp NewOp;
	NewOp.nPriority		= nPriority;
	NewOp.StdFunc		= StdFunc;
	m_NativeFuncList.Add(pName, NewOp);
}
void HScript::AddNativeConst(const TCHAR* pName, HVar Const) {
	m_NativeConstList.Add(pName, Const);
}
HScript::HScript() {
	m_pCurFunc = NULL;
	m_bIsCompiled = false;
	m_pSintax = GpPunctuation;
}
HScript* HScript::AddScript(const TCHAR *pName) {
	HScript *pScript = new HScript;
	if(pScript)
		pScript->LoadFromFile(pName);
	else
		return NULL;
	return pScript;
}
HBOOL HScript::LoadFromFile(const TCHAR *pName)
{
    m_pCurBuf = ReadFile(pName);
	m_Buffers.Add(pName, m_pCurBuf);

	// Set pos to begin.
	m_pCurBuf->Seek(0);
	m_CurPos.nBufNum = m_Buffers.FindIndex(pName);
	m_CurPos.nLineNum = 0;

	Compile();

	HTBuffer<TCHAR> DisBuf = Disassemble();
	WriteFile("dump.out", &DisBuf, true, false);

	return m_bIsCompiled;
}
HScript::~HScript() {
	m_ScriptCode.Empty();
	for(HTMultiMap<FString, FScriptFunc*>::FIterator it(m_Functions);it;++it) {
		delete it.Value()->m_pName;
		delete it.Value();
	}
	m_Functions.Empty();
}
void HScript::AddOpcode(INT OpCode, HVar Par) {
	FVMCommand NewOp;

	NewOp.FileNum	= HParser::m_CurPos.nBufNum;
	NewOp.nLineNum	= HParser::m_CurPos.nLineNum;
	NewOp.pFunc		= m_pCurFunc;
	NewOp.OpCode	= OpCode;
	NewOp.Param		= Par;

	m_ScriptCode.AddItem(NewOp);
}
void HScript::AddOpcodeGetVar(const FString &VarName) {
	INT nVarIdx;

	// NULL.
	if(VarName == "NULL") {
		AddOpcode(VM_NULL,0);
		return;
	}
	// This object.
	if(VarName == "this") {
		AddOpcode(VM_OBJ_THIS,0);
		return;
	}
	// Local variable.
	if (m_pCurFunc) {
		nVarIdx = m_pCurFunc->m_lVars.FindIndex(*VarName);
		if (nVarIdx >= 0) {
			AddOpcode(VM_VAR_GET, nVarIdx);
			return;
		}
	}
	// Static variable.
	nVarIdx = m_StaticVars.FindIndex(*VarName);
	if (nVarIdx == INDEX_NONE) {
		ParseError("Variable \"%s\" not declared\n",*VarName);
	}
	AddOpcode(VM_SVAR_GET, nVarIdx);
}
void HScript::AddOpcodeSetVar(const FString &VarName) {
	INT nVarIdx;

	// Local variable.
	if (m_pCurFunc) {
		nVarIdx = m_pCurFunc->m_lVars.FindIndex(*VarName);
		if (nVarIdx >= 0) {
			AddOpcode(VM_VAR_SET, nVarIdx);
			return;
		}
	}
	// Static variable.
	nVarIdx = m_StaticVars.FindIndex(*VarName);
	if (nVarIdx == INDEX_NONE) {
		ParseError("Variable \"%s\" not declared\n",*VarName);
	}
	else
		AddOpcode(VM_SVAR_SET, nVarIdx);
}
inline INT HScript::GetStdOp(FString Str) {
	return m_NativeFuncList.FindIndex(Str);
}
inline void HScript::AddOpcodeStd(INT StdOp) {
	AddOpcode(VM_STD, StdOp);
}
// Parse expression.
void HScript::ParseExpression() {
	HTArray<INT> StackOp;
	INT nUnaryOp = 0;
	INT nCurOp;
	INT nCurPriority, nStackPriority;
	FString pName;
	INT	nDeep = 0;

	StackOp.Empty(1024);
	while(!m_bEof) {
		if ((m_sToken == ";")||(m_sToken == ","))
			break;
		else if(m_sToken == "(") {
			nDeep++;
			StackOp.AddItem(GetStdOp(*m_sToken));
			nUnaryOp = 0;
		}
		else if(m_sToken == ")") {
			nDeep--;
			while(StackOp.Num() > 0 && StackOp.Last() != GetStdOp("(")) {
				AddOpcodeStd(StackOp.Pop());
			}
			if(StackOp.Num() > 0) 
				StackOp.Pop();
			else
				break;
		}
		// New object.
		else if(m_sToken == "new") {
			ParseToken(); // Get class name.
			ParseName(pName);
			AddOpcode(VM_OBJ_NEW, *pName);
		}
		// Object function or property.
		else if(m_sToken == "->") {
			ParseToken(); // Get member name.
			ParseName(pName);
			if( CheckToken("(") ) {
				ParseArgs();
				AddOpcode(VM_OBJ_CALL, *pName);
			} else {
				AddOpcode(VM_OBJ_GET, *pName);
			}
			continue;
		}
		else if(m_sToken == "::") {
			ParseToken(); // Get member name.
			ParseName(pName);
			if( CheckToken("(") ) {
				ParseArgs();
				AddOpcode(VM_OBJ_ICALL,*pName);
			}
			continue;
		}
		// Binary operations.
		else if(m_nTokenType == TOKEN_SINTAX) {
			if(!nDeep && m_sToken == ")")
				break;
			if(nUnaryOp == 0) {
				if(m_sToken == "+") nUnaryOp = '+';
				else if(m_sToken == "-") nUnaryOp = '-';
				else if(m_sToken == "!") nUnaryOp = '!';
				else ParseError("bad unary operation '%s'\n", *m_sToken);
			} else {
				nCurOp = GetStdOp(m_sToken);
				nCurPriority = m_NativeFuncList[nCurOp].Value.nPriority;
				while(StackOp.Num() > 0) {
					nStackPriority = m_NativeFuncList[StackOp.Last()].Value.nPriority;
					if (nCurPriority <= nStackPriority)
						AddOpcodeStd(StackOp.Pop());
					else
						break;
				}
				StackOp.AddItem(nCurOp);
				if(strchr("<>", (*m_sToken)[0])) nUnaryOp = 0;
			}
		}
		// Digits.
		else if(m_nTokenType == TOKEN_NUMBER) {
			if(strchr(*m_sToken, '.')) {
				FLOAT fValue = (FLOAT)atof(*m_sToken); // Str DEC to Float.
				if(nUnaryOp == '-')
					fValue = -fValue;
				AddOpcode(VM_CONST, fValue);
			} else {
				INT nValue;
				if((*m_sToken)[0] == '0' && ((*m_sToken)[1] == 'x' || (*m_sToken)[1] == 'X')) 
					sscanf(*m_sToken + 2,"%x", &nValue); //  Str HEX to Int.
				else
					nValue = atoi(*m_sToken); // Str DEC to Int.
				if(nUnaryOp == '-') 
					nValue = -nValue;
				else if(nUnaryOp == '!')
					nValue = !nValue;
				AddOpcode(VM_CONST, nValue);
			}
			nUnaryOp = -1;
		}
		// Strings.
		else if(m_nTokenType == TOKEN_STRING) {
			AddOpcode(VM_CONST, *m_sToken);
			nUnaryOp = -1;
		}
		// Functions and variables.
		else if( m_nTokenType == TOKEN_NAME ) {
			ParseName(pName);
			INT nIdx = m_NativeConstList.FindIndex(pName); // Check for const.
			if( nIdx != INDEX_NONE ) {
				HVar Const = m_NativeConstList.FindRef(pName);
				AddOpcode(VM_CONST, Const);
			}
			else if( CheckToken("(") ) {
				// Function or script call.
				ParseArgs();
				nCurOp = GetStdOp(pName);
				if (nCurOp != INDEX_NONE)
					AddOpcodeStd(nCurOp);
				else {
					AddOpcode(VM_SCRIPT_CALL, *pName);
				}
			}
			else {
				// Variable.
				AddOpcodeGetVar(pName);
			}
			if(nUnaryOp == '-')
				AddOpcodeStd(GetStdOp("-u"));
			else if(nUnaryOp == '!') 
				AddOpcodeStd(GetStdOp("!u"));
			nUnaryOp = -1;
			continue;
		}
		else {
			StackOp.Empty();
			ParseError("Unknown token \"%s\" in expression\n", *m_sToken);
		}
		if ((m_sToken == ";")||(m_sToken == ","))
			break;
		ParseToken();
	}
	// Add operations to the end.
	while(StackOp.Num()>0) {
		AddOpcodeStd(StackOp.Pop());
	}
}
INT HScript::ParseArgs() {
	INT nNumArg = 0;

	if ( !CheckToken( ")" ) ) {
		do {
			ParseExpression();
			nNumArg++;
		} while( CheckToken( "," ) );
		ExpectToken( ")" );
	}

	return nNumArg;
}
void HScript::PatchLoop( INT nStart, INT nContPos ) {
	INT i;
	FVMCommand *pPos;
	INT nEndPos = m_ScriptCode.Num();

	pPos = &m_ScriptCode( nStart );
	for( i = nStart; i < nEndPos; i++, pPos++ ) {
		if ( pPos->OpCode == VM_TMP_BREAK ) {
			pPos->OpCode = VM_JMP;
			pPos->Param = nEndPos;
		} else if ( pPos->OpCode == VM_TMP_CONT ) {
			pPos->OpCode = VM_JMP;
			pPos->Param = nContPos;
		}
	}
}
void HScript::ParseReturnStatement( HBOOL bWait ) {
	if ( CheckToken( ";" ) ) {
		if ( m_pCurFunc->m_Type != '@')
			ParseError( "expecting return value" );
		if (bWait)
			AddOpcode(VM_WAIT, 1);
		else
			AddOpcode(VM_RET, 1);
		return;
	}

	ParseExpression();
	ExpectToken( ";" );

	if (bWait)
		AddOpcode(VM_WAIT, 0);
	else
		AddOpcode(VM_RET, 0);
}
void HScript::ParseIfStatement() {
	INT	nEndJmp, nElseJmp;

	FString pName;

	ExpectToken( "(" );
	ParseExpression();
	ExpectToken( ")" );

	nEndJmp = m_ScriptCode.Num();
	AddOpcode(VM_JMP_IF, 0);
	m_ScriptCode(nEndJmp).Param = m_ScriptCode.Num();

	ParseStatement();

	if ( CheckToken( "else" ) ) {
		nElseJmp = m_ScriptCode.Num();
		AddOpcode(VM_JMP, 0);
		m_ScriptCode(nEndJmp).Param = m_ScriptCode.Num();
		ParseStatement();
		m_ScriptCode(nElseJmp).Param = m_ScriptCode.Num();
	} else {
		m_ScriptCode(nEndJmp).Param = m_ScriptCode.Num();
	}
}
void HScript::ParseWhileStatement() {
	INT			nBegLab;
	INT			nEndJmp;

	GLoopDepth++;

	ExpectToken( "(" );

	nBegLab = m_ScriptCode.Num();
	ParseExpression();
	ExpectToken( ")" );

	nEndJmp = m_ScriptCode.Num();
	AddOpcode(VM_JMP_IF, 0);
	ParseStatement();
	AddOpcode(VM_JMP, nBegLab);
	m_ScriptCode(nEndJmp).Param = m_ScriptCode.Num();

	// Fixup breaks and continues.
	PatchLoop( nBegLab, nBegLab );

	GLoopDepth--;
}
void HScript::ParseForStatement() {
	INT			nBegLab;
	INT			nContLab;
	INT			nProcJmp;
	INT			nEndJmp;

	GLoopDepth++;

	nBegLab = m_ScriptCode.Num();

	ExpectToken( "(" );
	
	// Init.
	if ( !CheckToken( ";" ) ) {
		ParseDefsAssign();
	}

	// Condition.
	nContLab = nBegLab = m_ScriptCode.Num();

	ParseExpression();
	ExpectToken( ";" );

	nEndJmp = m_ScriptCode.Num();
	AddOpcode(VM_JMP_IF, 0);

	// Counter.
	if ( !CheckToken( ")" ) ) {
		nProcJmp = m_ScriptCode.Num();
		AddOpcode(VM_JMP, 0);

		nContLab = m_ScriptCode.Num();
		do {
			ParseAssign();
		} while( CheckToken( "," ) );
		
		ExpectToken( ")" );

		AddOpcode(VM_JMP, nBegLab);

		m_ScriptCode(nProcJmp).Param = m_ScriptCode.Num();
	}

	ParseStatement();

	AddOpcode(VM_JMP, nContLab);

	m_ScriptCode(nEndJmp).Param = m_ScriptCode.Num();

	// Fixup breaks and continues.
	PatchLoop( nBegLab, nContLab );

	GLoopDepth--;
}
void HScript::ParseDoWhileStatement() {
	INT			nBegLab;

	GLoopDepth++;

	nBegLab = m_ScriptCode.Num();
	ParseStatement();
	ExpectToken( "while" );
	ExpectToken( "(" );
	ParseExpression();
	ExpectToken( ")" );
	ExpectToken( ";" );

	AddOpcode(VM_JMP_IFNOT, nBegLab);

	// Fixup breaks and continues.
	PatchLoop( nBegLab, nBegLab );

	GLoopDepth--;
}
void HScript::ParseStatement() {
	FString pName;

	if ( CheckToken( ";" ) ) {
		// Skip semicolons, which are harmless and ok syntax.
		return;
	}
	if ( CheckToken( "{" ) ) {
		do {
			ParseStatement();
		} while( !CheckToken( "}" ) );

		return;
	} 
	if ( CheckToken( "return" ) ) {
		ParseReturnStatement(false);
		return;
	}
	if ( CheckToken( "wait" ) ) {
		ParseReturnStatement(true);
		return;
	}
	if ( CheckToken( "if" ) ) {
		ParseIfStatement();
		return;
	}
	if ( CheckToken( "while" ) ) {
		ParseWhileStatement();
		return;
	}
	if ( CheckToken( "for" ) ) {
		ParseForStatement();
		return;
	}
	if ( CheckToken( "do" ) ) {
		ParseDoWhileStatement();
		return;
	}
	if ( CheckToken( "break" ) ) {
		ExpectToken( ";" );
		if ( !GLoopDepth ) {
			ParseError( "cannot break outside of a loop" );
		}
		AddOpcode(VM_TMP_BREAK, 0);
		return;
	}
	if ( CheckToken( "continue" ) ) {
		ExpectToken( ";" );
		if ( !GLoopDepth ) {
			ParseError( "cannot contine outside of a loop" );
		}
		AddOpcode(VM_TMP_CONT, 0);
		return;
	}
	if ( CheckToken( "thread" ) ) {
		ExpectToken( ";" );
		AddOpcode(VM_SCRIPT_CONT, 0);
		return;
	}
	if ( CheckToken( "delete" ) ) {
		ParseName( pName );
		ExpectToken( ";" );
		AddOpcodeGetVar(pName);
		AddOpcode(VM_OBJ_DEL, 0);
		return;
	}
	ParseDefsAssign();
}
TCHAR HScript::ParseType() {
	if ( CheckToken( "INT" ) )
		return 'i';
	else if ( CheckToken( "float" ) )
		return 'f';
	else if ( CheckToken( "vec3" ) )
		return 'v';
	else if ( CheckToken( "quat" ) )
		return 'q';
	else if ( CheckToken( "string" ) )
		return 's';
	else if ( CheckToken( "pointer" ) )
		return 'p';
	else if ( CheckToken( "void" ) )
		return '@';
	else
		return 0;
}
void HScript::ParseDefsAssign() {
	HBOOL bIsStatic = false;
	HBOOL bIsEvent = false;
	TCHAR Type;
	FString pName;

	if ( CheckToken( ";" ) ) {
		// Skip semicolons, which are harmless and ok syntax.
		return;
	}

	if ( CheckToken( "static" ) ) 
		bIsStatic = true;
	if ( CheckToken( "event" ) ) 
		bIsEvent = true;

 	Type = ParseType();
	if ((!Type)&&(m_pCurFunc)){
		ParseAssign();
		ExpectToken( ";" );
		return;
	}

	ParseName( pName );

	if ( CheckToken( "(" ) ) {		// Check for a function declaration.
		ParseFunctionDef( Type, *pName, bIsEvent );
	} else {
		ParseVariableDef( Type, *pName, bIsStatic ); // Check for a variable declaration.
		while( CheckToken( "," ) ) {
			ParseName( pName );
			ParseVariableDef( Type, *pName, bIsStatic );
		}
		ExpectToken( ";" );
	}
}
void HScript::ParseAssign() {
	FString pName;
	FString PropName;

	HParser::FParsePos TPos = m_CurPos;
	TPos.nPos = m_pCurBuf->Tell();

	ParseName( pName );

	HBOOL bPropFlag = false;
	if ( CheckToken( "->" ) ) {
		ParseName( PropName );
		bPropFlag = true;
	}
	if ( CheckToken( "=" ) ) {
		ParseExpression();
		if (bPropFlag){
			AddOpcodeGetVar(pName);
			AddOpcode(VM_OBJ_SET,*PropName);
		}
		else
			AddOpcodeSetVar(pName);
	}
	else if ( CheckToken( "+=" ) ) {
		AddOpcodeGetVar(pName);
		if (bPropFlag)
			AddOpcode(VM_OBJ_GET,*PropName);
		ParseExpression();
		AddOpcodeStd(GetStdOp("+"));
		if (bPropFlag){
			AddOpcodeGetVar(pName);
			AddOpcode(VM_OBJ_SET,*PropName);
		}
		else
			AddOpcodeSetVar(pName);
	}
	else if ( CheckToken( "-=" ) ) {
		AddOpcodeGetVar(pName);
		if (bPropFlag)
			AddOpcode(VM_OBJ_GET,*PropName);
		ParseExpression();
		AddOpcodeStd(GetStdOp("-"));
		if (bPropFlag){
			AddOpcodeGetVar(pName);
			AddOpcode(VM_OBJ_SET,*PropName);
		}
		else
			AddOpcodeSetVar(pName);
	}
	else if ( CheckToken( "*=" ) ) {
		AddOpcodeGetVar(pName);
		if (bPropFlag)
			AddOpcode(VM_OBJ_GET,*PropName);
		ParseExpression();
		AddOpcodeStd(GetStdOp("*"));
		if (bPropFlag){
			AddOpcodeGetVar(pName);
			AddOpcode(VM_OBJ_SET,*PropName);
		}
		else
			AddOpcodeSetVar(pName);
	}
	else if ( CheckToken( "/=" ) ) {
		AddOpcodeGetVar(pName);
		if (bPropFlag)
			AddOpcode(VM_OBJ_GET,*PropName);
		ParseExpression();
		AddOpcodeStd(GetStdOp("/"));
		if (bPropFlag){
			AddOpcodeGetVar(pName);
			AddOpcode(VM_OBJ_SET,*PropName);
		}
		else
			AddOpcodeSetVar(pName);
	}
	else if ( CheckToken( "++" ) ) {
		AddOpcodeGetVar(pName);
		if (bPropFlag)
			AddOpcode(VM_OBJ_GET,*PropName);
		AddOpcode(VM_CONST, 1);
		AddOpcodeStd(GetStdOp("+"));
		if (bPropFlag){
			AddOpcodeGetVar(pName);
			AddOpcode(VM_OBJ_SET,*PropName);
		}
		else
			AddOpcodeSetVar(pName);
	}
	else if ( CheckToken( "--" ) ) {
		AddOpcodeGetVar(pName);
		if (bPropFlag)
			AddOpcode(VM_OBJ_GET,*PropName);
		AddOpcode(VM_CONST, 1);
		AddOpcodeStd(GetStdOp("-"));
		if (bPropFlag){
			AddOpcodeGetVar(pName);
			AddOpcode(VM_OBJ_SET,*PropName);
		}
		else
			AddOpcodeSetVar(pName);
	}
	else {
		m_CurPos = TPos;
		m_pCurBuf->Seek(m_CurPos.nPos);
		m_sToken = pName;
		m_nTokenType = TOKEN_NAME;
		ParseExpression();
		return;
	}
}
void HScript::ParseVariableDef( TCHAR Type, const TCHAR *pName, HBOOL bIsStatic ) {
	if (bIsStatic) {
		if (m_StaticVars.Find(pName)) {
			ParseError( "%s static redeclared", pName );
		}
		m_StaticVars.Add(pName, Type);
	}
	else if (m_pCurFunc) {
		if (m_pCurFunc->m_lVars.Find(pName)) {
			ParseError( "%s redeclared", pName );
		}
		m_pCurFunc->m_lVars.Add(pName, Type);
	}
	else
		ParseError( "%s local variable must be in function", pName );
	if ( CheckToken( "=" ) ) {
		if (m_pCurFunc) {
			ParseExpression();
			AddOpcodeSetVar(pName);
		}
		else
			ParseError( "%s variable init must be in function", pName );
	}
}
void HScript::ParseFunctionDef( TCHAR ReturnType, const TCHAR *pName, HBOOL bIsEvent ) {
	FString		ParmName;
	INT 			nNumParms = 0;
	INT				nTypeParms;

	if ( m_pCurFunc )
		ParseError( "Functions may not be defined within other functions" );

	FScriptFunc *pFunc = new FScriptFunc;
	pFunc->m_pName = new TCHAR [strlen(pName)+1];
	strcpy((TCHAR*)(pFunc->m_pName), pName);
	pFunc->m_nCodeLabel = m_ScriptCode.Num();
	pFunc->m_Type = ReturnType;

	m_pCurFunc = pFunc;

	if ( !CheckToken( ")" ) ) {
		do {
			nTypeParms = ParseType();
			ParseName( ParmName );
			pFunc->m_Formatspec[nNumParms] = nTypeParms;
			pFunc->m_lVars.Add(*ParmName, nTypeParms);
			AddOpcodeGetVar(ParmName);
			nNumParms++;
		} while( CheckToken( "," ) );
		ExpectToken( ")" );
	}
	pFunc->m_Formatspec[nNumParms] = '\0';

	if (bIsEvent)
		pFunc->m_bIsEvent = true;
	else
		pFunc->m_bIsEvent = false;

	if (m_Functions.Find(pName))
		ParseError( "%s function redeclared", pName );

	m_Functions.Add(pName, pFunc);

	ExpectToken( "{" );
	// Parse regular statements.
	while( !CheckToken( "}" ) ) {
		ParseStatement();
	}

	AddOpcode(VM_RET, 1);
	m_pCurFunc = NULL;
}
HBOOL HScript::Compile() {
	try {
		// Read first token.
		ParseToken();
		while( !m_bEof ) {
			ParseDefsAssign( );
		}
		m_bIsCompiled = true;
	}
	catch( TCHAR *Err ) {
		ParseError("Error: %s\n", Err);
		m_bIsCompiled = false;
	}
	return m_bIsCompiled;
}
HTBuffer<TCHAR> HScript::Disassemble() {
	INT nT;
	FScriptFunc *TPFunc = NULL;
	HTBuffer<TCHAR> Buffer(BUF_WRITE);

	if (!m_bIsCompiled)
		return Buffer << "\0";

	for (HTMultiMap<FString, TCHAR>::FIterator vit(m_StaticVars); vit; ++vit){
		Buffer << vit.Value() <= " " <= *vit.Key() <= "\n";
	}
	for (HTArray<FVMCommand>::FIterator it(m_ScriptCode); it; ++it){
		if(it->pFunc) {
			if (TPFunc != it->pFunc){
				TPFunc = it->pFunc;
				Buffer << TPFunc->m_Type;
				if(!TPFunc->m_bIsEvent)
					Buffer <= " FUNC ";
				else
					Buffer <= " EVENT ";
				Buffer <= TPFunc->m_pName;
				Buffer <= "(" <= (const TCHAR*)TPFunc->m_Formatspec <= ")\n";
				for (HTMultiMap<FString, BYTE>::FIterator vit(TPFunc->m_lVars); vit; ++vit){
					Buffer << vit.Value() <= " " <= *vit.Key() <= "\n";
				}
			}
		}
		Buffer <= "\t\t";
		switch (it->OpCode){
			case VM_VAR_GET:
				Buffer <= "VAR_GET ";
				nT = (INT)it->Param;
				if (nT<0 || nT>=it->pFunc->m_lVars.Num())
					Buffer <= "_unknow_";
				else
					Buffer <= *it->pFunc->m_lVars[nT].Key;
				break;
			case VM_VAR_SET:
				Buffer <= "VAR_SET ";
				nT = (INT)it->Param;
				if (nT<0 || nT>=it->pFunc->m_lVars.Num())
					Buffer <= "_unknow_";
				else
					Buffer <= *it->pFunc->m_lVars[nT].Key;
				break;
			case VM_SVAR_GET:
				Buffer <= "SVAR_GET ";
				nT = (INT)it->Param;
				if (nT<0 || nT>=m_StaticVars.Num())
					Buffer <= "_unknow_";
				else
					Buffer <= *m_StaticVars[nT].Key;
				break;
			case VM_SVAR_SET:
				Buffer <= "SVAR_SET ";
				nT = (INT)it->Param;
				if (nT<0 || nT>=m_StaticVars.Num())
					Buffer <= "_unknow_";
				else
					Buffer <= *m_StaticVars[nT].Key;
				break;
			case VM_CONST:
				Buffer <= "CONST ";
				if (it->Param.GetType()==HVar::Int)
					Buffer <= Format("%i", (INT)it->Param);
				else if (it->Param.GetType()==HVar::Float)
					Buffer <= Format("%f", (FLOAT)it->Param);
				else if (it->Param.GetType()==HVar::String)
					Buffer <= *(FString)it->Param;
				break;
			case VM_OBJ_THIS:
				Buffer <= "OBJ_THIS";
				break;
			case VM_OBJ_NEW:
				Buffer <= "OBJ_NEW ";
				Buffer <= *(FString)it->Param;
				break;
			case VM_OBJ_DEL:
				Buffer <= "OBJ_DEL";
				break;
			case VM_OBJ_GET:
				Buffer <= "OBJ_GET ";
				Buffer <= *(FString)it->Param;
				break;
			case VM_OBJ_SET:
				Buffer <= "OBJ_SET ";
				Buffer <= *(FString)it->Param;
				break;
			case VM_OBJ_CALL:
				Buffer <= "OBJ_CALL ";
				Buffer <= *(FString)it->Param;
				break;
			case VM_OBJ_ICALL:
				Buffer <= "OBJ_ICALL ";
				Buffer <= *(FString)it->Param;
				break;
			case VM_JMP:
				Buffer <= "JMP ";
				Buffer <= Format("%i", (INT)it->Param - it.GetIndex());
				break;
			case VM_JMP_IFNOT:
				Buffer <= "JMP_IFNOT ";
				Buffer <= Format("%i", (INT)it->Param - it.GetIndex());
				break;
			case VM_JMP_IF:
				Buffer <= "JMP_IF ";
				Buffer <= Format("%i", (INT)it->Param - it.GetIndex());
				break;
			case VM_STD:
				Buffer <= "STD ";
				Buffer <= *HScript::m_NativeFuncList[(INT)it->Param].Key;
				break;
			case VM_SCRIPT_CALL:
				Buffer <= "SCRIPT_CALL ";
				Buffer <= *(FString)it->Param;
				break;
			case VM_SCRIPT_CONT:
				Buffer <= "SCRIPT_CONT ";
				break;
			case VM_RET:
				Buffer <= "RET ";
				Buffer <= Format("%i", (INT)it->Param);
				break;
			case VM_WAIT:
				Buffer <= "WAIT ";
				Buffer <= Format("%i", (INT)it->Param);
				break;
		}
		Buffer <= "\n";
	}

	Buffer << "\0";

	return Buffer;
}
