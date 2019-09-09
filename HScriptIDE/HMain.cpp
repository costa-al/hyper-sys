#include <windows.h>
#include <stdio.h>
#include <iostream.h>
#include <time.h>
#include <malloc.h>
#include <assert.h>
#include <tchar.h>

#include "HCore.h"

#include "HWindow.h"

#include "resource.h"

STATIC_INIT

HINSTANCE	GhInstance	= NULL;

__int64 GFreq;
DOUBLE GResolution;
HBOOL GTimestamp = true;
FLOAT GSecondsPerCycle = 0;

class HIDEEnvironment {
public:
	HWindow *m_pCurrentWindow;
	HBOOL	m_bActive;
	HBOOL	m_bReady;
};

INT Win32Run(HIDEEnvironment *pIDEEnv) {
	HBOOL bGotMsg;
	MSG  Msg;
	HACCEL hAccel = LoadAccelerators(NULL, MAKEINTRESOURCE(IDR_SCRIPT_IDE_ACCEL));
	PeekMessage(&Msg, NULL, 0U, 0U, PM_NOREMOVE);
	while(WM_QUIT != Msg.message) {
		if(pIDEEnv->m_bActive)
			bGotMsg = PeekMessage(&Msg, NULL, 0U, 0U, PM_REMOVE);
		else
			bGotMsg = GetMessage(&Msg, NULL, 0U, 0U);
		if(bGotMsg) {
	            if(0 == TranslateAccelerator((HWND)pIDEEnv->m_pCurrentWindow,
					hAccel, &Msg ))
				{
					TranslateMessage(&Msg);
					DispatchMessage(&Msg);
				}
		}
		else {
			if(pIDEEnv->m_bActive && pIDEEnv->m_bReady) {


			}
		}
	}
	return Msg.wParam;
}


HTArray<HWindow*> HWindow::_DeleteWindows;
HTArray<HWindow*> HWindow::_Windows;
INT HWindow::ModalCount = 0;

HINSTANCE hInstanceWindow;

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
{
	GhInstance = hInst;

/*	HIDEEnvironment IDEEnv;
	IDEEnv.m_bActive = true;
	IDEEnv.m_bReady = true;
	IDEEnv.m_pCurrentWindow = &MainWindow;*/

	HWindow::RegisterWindowClass("Common", CS_HREDRAW | CS_VREDRAW);

	//HWindow Window("ScriptIDE");

	//Win32Run(&IDEEnv);

	return 0;
}
