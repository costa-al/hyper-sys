HCORE_API HBOOL appStreq (const TCHAR* p, const TCHAR* q) {
	if(p==q) return true;
	if(!(p&&q)) return false;
	while(*p==*q) {
		if(*p==0) return true;
		p++;
		q++;
	}
	return false;
}

HCORE_API INT appStrlen (const TCHAR* p) {
	INT l=0;
	if(p) 
		while(*p) {
			l++;
			p++;
		}
	return l;
}
HCORE_API TCHAR* appStrdup (const TCHAR* p) {
	if(!p) {
		return 0;
	}
	else {
		INT l = appStrlen(p);
		TCHAR* string = new TCHAR[l+1];
		TCHAR* s = string;
		for(INT i=l+1; i; i--) {
			*s = *p;
			p++;
			s++;
		}
		return string;
	}
}
HCORE_API TCHAR* appStrcat (const TCHAR* A, const TCHAR* B) {
	if(!A)
		return appStrdup(B);
	if(!B)
		return appStrdup(A);
	INT la = appStrlen(A);
	INT lb = appStrlen(B);
	TCHAR* string = new TCHAR[la+lb+1];
	TCHAR* s = string;
	for(INT i=la; i; i--) {
		*s = *A;
		A++;
		s++;
	}
	for(i=lb+1; i; i--) {
		*s = *B;
		B++;
		s++;
	}
	return string;
}
HCORE_API INT appStrcmp(TCHAR *A, TCHAR *B) {
	if(!A||!B)
		return 1;
	INT nL2 = appStrlen(B);
	INT nTL1=0, nTL2=0;
	HBOOL bBreak=false;
	while(*A) {
		if(!bBreak) {
			if(*A==*B) {
				nTL2++;
				B++;
			}
			else
				bBreak = true;
		}
		nTL1++;
		A++;
	}
	if(nTL1==nTL2)
		return 0;
	if((nTL2==nL2) && (nTL2<nTL1))
		return -1;
	else
		return 1;
}

/* -------------------------- End -------------------------- */