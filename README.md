# HYPERSYS

Engine for creating applications with a plug-in system (DLL) and build-in C++ like script engine. You can create native classes in DLL and then use them in the script engine.


## Hello World application

Example application with DLL connection to the app and run script

```C++
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
```

## Script Engine example code

Access to the DLL native class HClassTest from script frame.xs

```C++
void onMain() {

	pointer pC = new HClassTest;
	pC->Name1 = "Hello world";
	pC->Testprop1 = 32;

	print(pC->Name1);
	print(pC->Testprop1);

	print("My value = " + GetValue());

	Recurse(0);

	return;
}

void Recurse(int a) {
	a++;
	if(a<=10) {
		print(a);
		Recurse(a);
	}
}

float GetValue() {
	return 0.56777777;
}
```
