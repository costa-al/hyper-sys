# Hypersys Script Engine

C++ like syntaxis script engine with plug-in system (DLL). You can implement native classes in DLL and then use them in the script engine.


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

## DLL example code

HClassTest native class implementation for Hello World example code

```C++
class HClassTest : public HClass {
public:
	CLASS_PROTOTYPE( HClassTest )

	FUNC_PROTOTYPE( Printf );

	PROP_PROTOTYPE( const TCHAR*,	Name1 )
	PROP_PROTOTYPE( INT, Testprop1 )

	HClassTest()	{}
	~HClassTest()	{}

};

FUNC_DECL( HClassTest, Printf )
	printf((const TCHAR*)Vars[0]);
END_FUNC

HEventDef1 EV_Printf( "Printf", "s" );

CLASS_DECLARATION( HClass, HClassTest, ECF_Pack )
	EVENT_DECL( EV_Printf,	HClassTest::Printf )
PROPERTIES( HClassTest )
	PROP_DECL( Name1 )
	PROP_DECL( Testprop1 )
END_CLASS
```

## Script Engine example code

Script implementation for own script engine. Access to native class HClassTest from script frame.xs for Hello World example code.

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
