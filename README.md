# Hypersys 'C++ like' script engine

Free to use a simple script engine written in C++ from scratch. Easily expandable with keywords, operations, and data types. Ability to dynamically bind plugins with native classes for use in the script code.

## Hello World application

Example application with sample DLL `HPack.dll` connection to the app and run script `frame.xs`

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

HWindowsPackage Pack;
FIPackage	*GPack = &Pack;

INT main() {

	// Connect plug-in with class 'HClassTest'
	GPack->Load("HPack.dll", true);

	// Create base object for script
	HClass *pObject = HClass::CreateClass("HClass");

	// Load & parse script
	pObject->SetScript(&HVar("frame.xs"));
	
	// Call 'main()' function from frame.xs
	pObject->ExecuteThread("main");

	// Cleaup
	delete pObject;
	GPack->Free();

	return 0;
}
```

## Script Engine example code

`frame.xs` script implementation for own script engine. Access to native class `HClassTest` from script for Hello World example code.

```C++
void main() { // pObject->ExecuteThread("main");

	pointer pC = new HClassTest; // Create class from HPack.dll
	pC->Name1 = "Hello world"; // Accessors
	pC->Testprop1 = 32;

	print(pC->Name1);
	print(pC->Testprop1);

	print("My value = " + get_value());

	recurse(0);

	return;
}

// functions example

void recurse(int a) {
	a++;
	if(a<=10) {
		print(a);
		recurse(a);
	}
}

float get_value() {
	return 0.56777777;
}
```

## DLL plug-in example code

`HClassTest` native class implementation for Hello World example code

```C++
class HClassTest : public HClass {
public:
	CLASS_PROTOTYPE( HClassTest ) // Define class for export into script engine

	FUNC_PROTOTYPE( Printf ); // Define class method for export into script engine

	PROP_PROTOTYPE( const TCHAR*, Name1 ) // The same for properties
	PROP_PROTOTYPE( INT, Testprop1 )

	HClassTest()	{}
	~HClassTest()	{}

};

FUNC_DECL( HClassTest, Printf ) // Method implementation
	printf((const TCHAR*)Vars[0]);
END_FUNC

HEventDef1 EV_Printf( "Printf", "s" );

//
// Description for export
//
CLASS_DECLARATION( HClass, HClassTest, ECF_Pack )
	EVENT_DECL( EV_Printf,	HClassTest::Printf )
PROPERTIES( HClassTest )
	PROP_DECL( Name1 )
	PROP_DECL( Testprop1 )
END_CLASS
```
