#include <windows.h>
#include <stdio.h>
#include <iostream.h>
#include <time.h>
#include <malloc.h>
#include <assert.h>
#include <tchar.h>

#include "HCore.h

__int64 GFreq;
DOUBLE GResolution;
HBOOL GTimestamp = true;
FLOAT GSecondsPerCycle = 0;

INT main() {

	HClass::PrepareClassSystem();
	HEvent::PrepareEventList();
	HScript::PrepareScriptSystem();

	HWindowsPackage Pack;
	Pack.Load("HPack.dll");

	if(Pack.IsValid()) {
		HClass::AddClass(Pack.ClassDef("HClassTest")); // From HPack.dll.
	}

	HClass *pObject = HClass::CreateClass("HClass");

	pObject->SetScript(&HVar("frame.xs"));
	pObject->ExecuteThread("onMain");

	delete pObject;

	Pack.Free();

	//GMalloc->DumpAllocs();

	HScript::ShutdownScriptSystem();
	HEvent::ShutdownEventList();
	HClass::ShutdownClassSystem();

	GMalloc->DumpAllocs();

	return 0;
}
