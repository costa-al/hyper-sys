#include "HScript\HParser.h"


const TCHAR *HParser::ParseError( const TCHAR *pFormat, ... ) {
	TCHAR Error[1024] = {0};
	FString ErrStr = Format("Parse error in %s(%i) : ",
		*m_Buffers[m_CurPos.nBufNum].Key,
		m_CurPos.nLineNum + 1,
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
void HParser::SetBuffer(HTBuffer<TCHAR>* pBuffer) {
	guard(HParser::SetBuffer);
	check(pBuffer);
	m_pCurBuf = pBuffer;
	unguard;
}
void HParser::GetToken(const TCHAR *pBreakStr) {
	INT i, j;
	TCHAR *pTempStr;
	TCHAR Tc[2];
	Tc[1] = '\0';

	HTBuffer<TCHAR>::FIterator It(m_pCurBuf);

	m_sToken = FString();
	m_bEof = false;

	if(pBreakStr) {
		while (!It.AtEnd()) {
			if (*It == '\n')
				m_CurPos.nLineNum++;
			if (*It == *pBreakStr) {
				Tc[0] = *It;
				m_sToken = &Tc[0];
				j=1;
				for(i=1; i<(INT)strlen(pBreakStr); i++) {
					if ((&It)[i] == pBreakStr[i]) {
						Tc[0] = *It;
						m_sToken += &Tc[0];
						j++;
					} else
						break;
					It++;
				}
				if (j == (INT)strlen(pBreakStr)) {
					It--;
					break;
				}
			}

			It++;

				/*for (i=1; i<(INT)strlen(pBreakStr); i++) {
					cout << (&It)[i] << "  ";
					if ((&It)[i] != pBreakStr[i]) break;
				}
				if (i == (INT)strlen(pBreakStr)) {
					It += i; break;
				}
			}
			Tc[0] = *It;
			m_sToken += &Tc[0]; It++;*/
		}
		m_bEof = It.AtEnd();
		return;
	}
	// Skip spaces.
	while ((!It.AtEnd()) && strchr(" \t\r\n", *It)) {
		if (*It == '\n')
			m_CurPos.nLineNum++;
		It++;
	}
	// End of the buffer.
	if (It.AtEnd()) {
		m_bEof = true;
		return;
	}
	// End of the string.
	if (*It == '\0') {
		m_bEof = true;
		return;
	}
	// Name.
	if ( (*It == '_')||(isalpha(*It)) ) {
		while( (!It.AtEnd()) && (isalpha(*It)||strchr("0123456789_",*It)) ) {
			Tc[0] = *It;
			m_sToken += &Tc[0]; It++;
		}
		m_nTokenType = TOKEN_NAME;
		return;
	}
	// Sintax.
    if (m_pSintax) {
		for(i=0;m_pSintax[i];i++) {
			pTempStr = m_pSintax[i];
			if(!strncmp(&It, pTempStr, strlen(pTempStr))) {
				It += strlen(pTempStr);
				m_sToken = pTempStr;
				m_nTokenType = TOKEN_SINTAX;
				return;
			}
		}
	}
	// Number.
	if (strchr("1234567890.", *It)) {
		while((!It.AtEnd()) && (strchr("1234567890.fFxXaAbBcCdDeEfF",*It))) {
			Tc[0] = *It;
			m_sToken += &Tc[0]; It++;
		}
		m_nTokenType = TOKEN_NUMBER;
		return;
	}
	// String.
	if (*It == '"') {
		It++; //"
		while ((!It.AtEnd())&&(*It != '"')) {
			Tc[0] = *It;
			m_sToken += &Tc[0]; It++;
		}
		m_nTokenType = TOKEN_STRING; 
		It++; //"
		return;
	}
	// Comment.
	if(!strncmp(&It,"//",2)) {
		It += 2;	m_sToken = "//";
		m_nTokenType = TOKEN_DIRECTIVE;
		return;
	}
	if(!strncmp(&It,"/*",2)) {
		It += 2;	m_sToken = "/*";
		m_nTokenType = TOKEN_DIRECTIVE;
		return;
	}
	if(!strncmp(&It,"*/",2)) {
		It += 2;	m_sToken = "*/";
		m_nTokenType = TOKEN_DIRECTIVE;
		return;
	}
	// Preprocessor directive.
	if (*It == '#') {
		if(!strncmp(&It,"#ifdef", 6)) {
			It += 6;	m_sToken = "#ifdef";
			m_nTokenType = TOKEN_DIRECTIVE;
			return;
		}
		else if(!strncmp(&It, "#ifndef", 7)) {
			It+=7;	m_sToken="#ifndef";
			m_nTokenType=TOKEN_DIRECTIVE;
			return;
		}
		else if(!strncmp(&It,"#else", 5)) {
			It += 5;	m_sToken = "#else";
			m_nTokenType = TOKEN_DIRECTIVE;
			return;
		}
		else if(!strncmp(&It,"#endif", 6)) {
			It += 6;	m_sToken = "#endif";
			m_nTokenType = TOKEN_DIRECTIVE;
			return;
		}
		else if(!strncmp(&It,"#define", 7)) {
			It += 7;	m_sToken = "#define";
			m_nTokenType = TOKEN_DIRECTIVE;
			return;
		}
		else if(!strncmp(&It,"#undef", 6)) {
			It += 6;	m_sToken = "#undef";
			m_nTokenType = TOKEN_DIRECTIVE;
			return;
		}
		else if(!strncmp(&It,"#include", 8)) {
			It += 8;	m_sToken = "#include";
			m_nTokenType = TOKEN_DIRECTIVE;
			return;
		}
	}
	Tc[0] = *It;
	m_sToken = &Tc[0]; It++;
	m_nTokenType = TOKEN_OTHER;
}
void HParser::ParseToken() {
	INT i;
	HTBuffer<TCHAR> *pBuf;
	FString		Macros;
	FParsePos		TempPos;

	do {
		GetToken();
		if (m_bEof) {
			if (m_SavedPos.Num()>0) {
				TempPos = m_SavedPos.Pop();
				if (m_CurPos.nBufNum != TempPos.nBufNum){ 
					// Delete include buffer if not used.
					for(i = 0; i < m_SavedPos.Num(); i++) 
						if (m_SavedPos(i).nBufNum == m_CurPos.nBufNum) break;
					if (i == m_SavedPos.Num()) {
						delete m_pCurBuf;
						m_Buffers[m_CurPos.nBufNum].Value = NULL;
					}
				}
				m_CurPos = TempPos;
				m_pCurBuf = m_Buffers[m_CurPos.nBufNum].Value;
				m_pCurBuf->Seek(m_CurPos.nPos);
				m_bEof = false;
				continue;
			}
			else
				break;
		}
		if (m_nTokenType == TOKEN_DIRECTIVE) {
			// Preprocessor.
			if(m_sToken == "//") {
				GetToken("\n");
			}
			else if(m_sToken == "/*")
				GetToken("*/");
			else if(m_sToken == "#ifdef") {
				GetToken();
				m_SkipStack.AddItem(IsDefine(m_sToken));
				for(i = 0; i < m_SkipStack.Num(); i++) 
					if(m_SkipStack(i) == 0) break;
				m_bNotSkip = (i == m_SkipStack.Num()) ? 1 : 0;
			}
			else if(m_sToken == "#ifndef") {
				GetToken();
				m_SkipStack.AddItem(!IsDefine(m_sToken));
				for(i = 0; i < m_SkipStack.Num(); i++) 
					if(m_SkipStack(i) == 0) break;
				m_bNotSkip = (i == m_SkipStack.Num()) ? 1 : 0;
			}
			else if(m_sToken == "#else") {
				if(m_SkipStack.Num() < 1)
					ParseError("#else is before #ifdef or #ifndef\n");
				m_SkipStack.AddItem(!m_SkipStack.Pop());
				for(i = 0; i < m_SkipStack.Num(); i++) 
					if(m_SkipStack(i) == 0) break;
				m_bNotSkip = (i == m_SkipStack.Num()) ? 1 : 0;
			}
			else if(m_sToken == "#endif") {
				if(m_SkipStack.Num() < 1)
					ParseError("#endif is before #ifdef or #ifndef\n");
				m_SkipStack.Pop();
				for(i = 0; i < m_SkipStack.Num(); i++) 
					if(m_SkipStack(i) == 0) break;
				m_bNotSkip = (i == m_SkipStack.Num()) ? 1 : 0;
			}
			else if(m_sToken == "#define") {
				GetToken();
				Macros = m_sToken;
				GetToken("\n");
				if(m_bNotSkip) {
					pBuf = m_Buffers.FindRef(m_sToken);
					if(pBuf) // Del old define.
						delete pBuf;
					pBuf = new HTBuffer<TCHAR>(BUF_WRITE, m_sToken.Len());
					pBuf->Serialize((void*)*m_sToken, m_sToken.Len());
					pBuf->m_eRWMode = BUF_READ;
					m_Buffers[Macros] = pBuf;
				}
			}
			else if(m_sToken == "#undef") {
				GetToken();
				if(m_bNotSkip) {
					pBuf = m_Buffers.FindRef(m_sToken);
					if(!pBuf)
						ParseError("#undef is before #define\n");
					else {
						delete pBuf;
						m_Buffers.Remove(*m_sToken);
					}
				}
			}
			else if(m_sToken == "#include") {
				GetToken();
				if(m_bNotSkip) {
					pBuf = m_Buffers.FindRef(m_sToken);
					if (!pBuf){
						// Load file.
						m_Buffers[m_sToken] = pBuf;
					}
					m_CurPos.nPos = m_pCurBuf->Tell();
					m_SavedPos.AddItem(m_CurPos);
					m_CurPos.nBufNum = m_Buffers.FindIndex(m_sToken);
					m_CurPos.nLineNum = 0;
					m_pCurBuf = pBuf;
					m_pCurBuf->Seek(0);
				}
			}
			continue;
		}
		if (m_nTokenType == TOKEN_NAME ){
			if(m_bNotSkip) {
				pBuf = m_Buffers.FindRef(m_sToken);
				// Define macros.
				if (pBuf) {
					m_CurPos.nPos = m_pCurBuf->Tell();
					m_SavedPos.AddItem(m_CurPos);
					m_pCurBuf = pBuf;
					m_pCurBuf->Seek(0);
					continue;
				}
			}
		}
	} while (!m_bNotSkip);
}
void HParser::ExpectToken( const TCHAR *pString ) {
	if ( m_sToken != pString ) {
		ParseError( "expected '%s', found '%s'", pString, *m_sToken );
	}
	ParseToken();
}
HBOOL HParser::CheckToken( const TCHAR *pString ) {
	if ( m_sToken != pString ) {
		return false;
	}	
	ParseToken();
	return true;
}
void HParser::ParseName( FString &Name ) {
	if ( m_nTokenType != TOKEN_NAME ) {
		ParseError( "'%s' is not a name", *m_sToken );
	}
	Name = m_sToken;
	ParseToken();
}
HBOOL HParser::IsDefine( FString Macros ) {
	return m_Buffers.FindRef(Macros) != NULL;
}


// For test.
HTBuffer<TCHAR>* ReadFile( const TCHAR* pFilename ) {
	guard(ReadFile);
	FILE* pFile = fopen(pFilename, TEXT("rb"));
	if( !pFile ) {
		return NULL;
	}
	fseek( pFile, 0, SEEK_END );
	long Length = ftell(pFile);
	fseek( pFile, 0, SEEK_SET );
	HTBuffer<TCHAR>* pBuf = new HTBuffer<TCHAR>(BUF_READ, Length);
	if( fread( pBuf->GetData(), Length, 1, pFile )!=1 ) {
		delete pBuf;
		return NULL;
	}
	fclose( pFile );
	return pBuf;
	unguard;
}
INT FileSize( const TCHAR *pFileName ) {
	guard(FileSize);
	FILE* pFile = fopen(pFileName, TEXT("rb"));
	if( !pFile ) {
		return 0;
	}
	fseek( pFile, 0, SEEK_END );
	long Length = ftell(pFile);
	fclose( pFile );
	return Length;
	unguard;
}
HBOOL WriteFile( const TCHAR* pFilename, HTBuffer<TCHAR>* pBuf, HBOOL bReplaceExisting, HBOOL bAppend ) {
	guard(WriteFile);
	if( !bReplaceExisting && FileSize(pFilename)>=0 )
		return NULL;
	const char* pMode = bAppend ? TEXT("ab") : TEXT("wb"); 
	FILE* pFile = fopen(pFilename, pMode);
	if( !pFile )
		return false;
	if( fwrite( pBuf->GetData(), pBuf->Num(), 1, pFile )!=1 ) {
		return false;
	}
	fclose( pFile );
	return true;
	unguard;
}
