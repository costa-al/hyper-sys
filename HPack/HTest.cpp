#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <iostream.h>
#include <tchar.h>

#include "HCore.h"

HBOOL GTimestamp = true;
FLOAT GSecondsPerCycle = 0;

#include "HPackClass.h"

IMPLEMENT_PACKAGE(HPack)

const TCHAR* Description()	{ return "Pack technology test\n"; }
DWORD Version()				{ return 0x00000001; }
HBOOL AutoLoading()			{ return false; }
