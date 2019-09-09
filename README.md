# Hypersys 'C++ like' script engine

Free to use a simple script engine written in C++ from scratch. Easily expandable with keywords, operators, and data types. Ability to dynamically bind plugins with native classes for use in the script code.

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

## frame.xs example script

Some functions examples and access to native class `HClassTest` (HPack.dll) from script for Hello World example code.

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

## HPack.dll plug-in example code

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

## Dump for frame.xs after parsing & compilation

```
@ FUNC onMain()
p pC
		OBJ_NEW HClassTest
		VAR_SET pC
		CONST Hello world
		VAR_GET pC
		OBJ_SET Name1
		CONST 32
		VAR_GET pC
		OBJ_SET Testprop1
		VAR_GET pC
		OBJ_GET Name1
		STD print
		VAR_GET pC
		OBJ_GET Testprop1
		STD print
		CONST My value = 
		SCRIPT_CALL GetValue
		STD +
		STD print
		CONST 0
		SCRIPT_CALL Recurse
		RET 1
		RET 1
@ FUNC Recurse(i)
i a
		VAR_GET a
		VAR_GET a
		CONST 1
		STD +
		VAR_SET a
		VAR_GET a
		CONST 10
		STD <=
		JMP_IF 5
		VAR_GET a
		STD print
		VAR_GET a
		SCRIPT_CALL Recurse
		RET 1
f FUNC GetValue()
		CONST 0.567778
		RET 0
		RET 1
```
