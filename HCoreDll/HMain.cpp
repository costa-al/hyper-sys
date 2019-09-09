#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <iostream.h>
#include <tchar.h>

#include "HCore.h"

HBOOL GTimestamp = true;
FLOAT GSecondsPerCycle = 0;

IMPLEMENT_PACKAGE(HCore)

const TCHAR* Description()	{ return "Core\n"; }
DWORD Version()				{ return 0x00000001; }
HBOOL AutoLoading()			{ return true; }
