#include "HFile.h"

HCORE_API INT appStrlen (const TCHAR* p) {
	INT l=0;
	if(p) 
		while(*p) {
			l++;
			p++;
		}
	return l;
}
