#include <windows.h>
#include <stdio.h>
#include <iostream.h>
#include <time.h>
#include <malloc.h>
#include <assert.h>
#include <tchar.h>

#include "HCore.h"

STATIC_INIT

__int64 GFreq;
DOUBLE GResolution;
HBOOL GTimestamp = true;
FLOAT GSecondsPerCycle = 0;

HWindowsPackage Pack;
FIPackage	*GPack = &Pack;

INT main() {

	GPack->Load("HPack.dll", true);

	HClass *pObject = HClass::CreateClass("HClass");

	pObject->SetScript(&HVar("frame.xs"));
	pObject->ExecuteThread("onMain");

	delete pObject;

	GPack->Free();

	GMalloc->DumpAllocs();

	return 0;
}
