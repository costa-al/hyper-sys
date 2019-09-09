
/*-----------------------------------------------------------------------------
	Defines.
-----------------------------------------------------------------------------*/

#ifndef WINDOW_API
#define WINDOW_API __declspec(dllimport)
#endif

#define RegSetValueExX(a,b,c,d,e,f)		TCHAR_CALL_OS(RegSetValueExW(a,b,c,d,e,f),RegSetValueExA(a,TCHAR_TO_ANSI(b),c,d,(BYTE*)TCHAR_TO_ANSI((TCHAR*)e),f))
#define RegSetValueX(a,b,c,d,e)			TCHAR_CALL_OS(RegSetValueW(a,b,c,d,e),RegSetValueA(a,TCHAR_TO_ANSI(b),c,TCHAR_TO_ANSI(d),e))
#define RegCreateKeyX(a,b,c)			TCHAR_CALL_OS(RegCreateKeyW(a,b,c),RegCreateKeyA(a,TCHAR_TO_ANSI(b),c))
#define RegQueryValueX(a,b,c,d)			TCHAR_CALL_OS(RegQueryValueW(a,b,c,d),RegQueryValueW(a,TCHAR_TO_ANSI(b),TCHAR_TO_ANSI(c),d))
#define RegOpenKeyX(a,b,c)				TCHAR_CALL_OS(RegOpenKeyW(a,b,c),RegOpenKeyA(a,TCHAR_TO_ANSI(b),c))
#define RegDeleteKeyX(a,b)				TCHAR_CALL_OS(RegDeleteKeyW(a,b),RegDeleteKeyA(a,TCHAR_TO_ANSI(b)))
#define RegDeleteValueX(a,b)			TCHAR_CALL_OS(RegDeleteValueW(a,b),RegDeleteValueA(a,TCHAR_TO_ANSI(b)))
#define RegQueryInfoKeyX(a,b)			TCHAR_CALL_OS(RegQueryInfoKeyW(a,NULL,NULL,NULL,b,NULL,NULL,NULL,NULL,NULL,NULL,NULL),RegQueryInfoKeyA(a,NULL,NULL,NULL,b,NULL,NULL,NULL,NULL,NULL,NULL,NULL))
#define RegOpenKeyExX(a,b,c,d,e)        TCHAR_CALL_OS(RegOpenKeyExW(a,b,c,d,e),RegOpenKeyExA(a,TCHAR_TO_ANSI(b),c,d,e))
#define LookupPrivilegeValueX(a,b,c)	TCHAR_CALL_OS(LookupPrivilegeValueW(a,b,c),LookupPrivilegeValueA(TCHAR_TO_ANSI(a),TCHAR_TO_ANSI(b),c))
#define GetDriveTypeX(a)				TCHAR_CALL_OS(GetDriveTypeW(a),GetDriveTypeA(TCHAR_TO_ANSI(a)))
#define GetDiskFreeSpaceX(a,b,c,d,e)	TCHAR_CALL_OS(GetDiskFreeSpaceW(a,b,c,d,e),GetDiskFreeSpaceA(TCHAR_TO_ANSI(a),b,c,d,e))
#define SetFileAttributesX(a,b)			TCHAR_CALL_OS(SetFileAttributesW(a,b),SetFileAttributesA(TCHAR_TO_ANSI(a),b))
#define DrawTextExX(a,b,c,d,e,f)		TCHAR_CALL_OS(DrawTextExW(a,b,c,d,e,f),DrawTextExA(a,const_cast<ANSICHAR*>(TCHAR_TO_ANSI(b)),c,d,e,f))
#define DrawTextX(a,b,c,d,e)			TCHAR_CALL_OS(DrawTextW(a,b,c,d,e),DrawTextA(a,TCHAR_TO_ANSI(b),c,d,e))
#define GetTextExtentPoint32X(a,b,c,d)  TCHAR_CALL_OS(GetTextExtentPoint32W(a,b,c,d),GetTextExtentPoint32A(a,TCHAR_TO_ANSI(b),c,d))
#define DefMDIChildProcX(a,b,c,d)		TCHAR_CALL_OS(DefMDIChildProcW(a,b,c,d),DefMDIChildProcA(a,b,c,d))
#define SetClassLongX(a,b,c)			TCHAR_CALL_OS(SetClassLongW(a,b,c),SetClassLongA(a,b,c))
#define GetClassLongX(a,b)				TCHAR_CALL_OS(GetClassLongW(a,b),GetClassLongA(a,b))
#define RemovePropX(a,b)				TCHAR_CALL_OS(RemovePropW(a,b),RemovePropA(a,TCHAR_TO_ANSI(b)))
#define GetPropX(a,b)					TCHAR_CALL_OS(GetPropW(a,b),GetPropA(a,TCHAR_TO_ANSI(b)))
#define SetPropX(a,b,c)					TCHAR_CALL_OS(SetPropW(a,b,c),SetPropA(a,TCHAR_TO_ANSI(b),c))
#define ShellExecuteX(a,b,c,d,e,f)      TCHAR_CALL_OS(ShellExecuteW(a,b,c,d,e,f),ShellExecuteA(a,TCHAR_TO_ANSI(b),TCHAR_TO_ANSI(c),TCHAR_TO_ANSI(d),TCHAR_TO_ANSI(e),f))
#define CreateMutexX(a,b,c)				TCHAR_CALL_OS(CreateMutexW(a,b,c),CreateMutexA(a,b,TCHAR_TO_ANSI(c)))
#define DefFrameProcX(a,b,c,d,e)		TCHAR_CALL_OS(DefFrameProcW(a,b,c,d,e),DefFrameProcA(a,b,c,d,e))
#define RegisterWindowMessageX(a)       TCHAR_CALL_OS(RegisterWindowMessageW(a),RegisterWindowMessageA(TCHAR_TO_ANSI(a)))
#define AppendMenuX(a,b,c,d)            TCHAR_CALL_OS(AppendMenuW(a,b,c,d),AppendMenuA(a,b,c,TCHAR_TO_ANSI(d)))
#define LoadLibraryX(a)					TCHAR_CALL_OS(LoadLibraryW(a),LoadLibraryA(TCHAR_TO_ANSI(a)))
#define SystemParametersInfoX(a,b,c,d)	TCHAR_CALL_OS(SystemParametersInfoW(a,b,c,d),SystemParametersInfoA(a,b,c,d))
#define DispatchMessageX(a)				TCHAR_CALL_OS(DispatchMessageW(a),DispatchMessageA(a))
#define PeekMessageX(a,b,c,d,e)			TCHAR_CALL_OS(PeekMessageW(a,b,c,d,e),PeekMessageA(a,b,c,d,e))
#define PostMessageX(a,b,c,d)			TCHAR_CALL_OS(PostMessageW(a,b,c,d),PostMessageA(a,b,c,d))
#define SendMessageX(a,b,c,d)			TCHAR_CALL_OS(SendMessageW(a,b,c,d),SendMessageA(a,b,c,d))
#define SendMessageLX(a,b,c,d)			TCHAR_CALL_OS(SendMessageW(a,b,c,(LPARAM)d),SendMessageA(a,b,c,(LPARAM)TCHAR_TO_ANSI(d)))
#define SendMessageWX(a,b,c,d)			TCHAR_CALL_OS(SendMessageW(a,b,(WPARAM)c,d),SendMessageA(a,b,(WPARAM)TCHAR_TO_ANSI(c),d))
#define DefWindowProcX(a,b,c,d)			TCHAR_CALL_OS(DefWindowProcW(a,b,c,d),DefWindowProcA(a,b,c,d))
#define CallWindowProcX(a,b,c,d,e)		TCHAR_CALL_OS(CallWindowProcW(a,b,c,d,e),CallWindowProcA(a,b,c,d,e))
#define GetWindowLongX(a,b)				TCHAR_CALL_OS(GetWindowLongW(a,b),GetWindowLongA(a,b))
#define SetWindowLongX(a,b,c)			TCHAR_CALL_OS(SetWindowLongW(a,b,c),SetWindowLongA(a,b,c))
#define LoadMenuIdX(i,n)				TCHAR_CALL_OS(LoadMenuW(i,MAKEINTRESOURCEW(n)),LoadMenuA(i,MAKEINTRESOURCEA(n)))
#define LoadCursorIdX(i,n)				TCHAR_CALL_OS(LoadCursorW(i,MAKEINTRESOURCEW(n)),LoadCursorA(i,MAKEINTRESOURCEA(n)))
#define LoadIconIdX(i,n)				TCHAR_CALL_OS(LoadIconW(i,MAKEINTRESOURCEW(n)),LoadIconA(i,MAKEINTRESOURCEA(n)))


extern WINDOW_API HINSTANCE hInstanceWindow;

// Classes.
class HWindow;
class HControl;


#define FIRST_AUTO_CONTROL 8192

/*-----------------------------------------------------------------------------
	FRect.
-----------------------------------------------------------------------------*/

struct FPoint
{
	INT X, Y;
	FPoint()
	{}
	FPoint( INT InX, INT InY )
	:	X( InX )
	,	Y( InY )
	{}
	static FPoint ZeroValue()
	{
		return FPoint(0,0);
	}
	static FPoint NoneValue()
	{
		return FPoint(INDEX_NONE,INDEX_NONE);
	}
	operator POINT*() const
	{
		return (POINT*)this;
	}
	const INT& operator()( INT i ) const
	{
		return (&X)[i];
	}
	INT& operator()( INT i )
	{
		return (&X)[i];
	}
	static INT Num()
	{
		return 2;
	}
	HBOOL operator==( const FPoint& Other ) const
	{
		return X==Other.X && Y==Other.Y;
	}
	HBOOL operator!=( const FPoint& Other ) const
	{
		return X!=Other.X || Y!=Other.Y;
	}
	FPoint& operator+=( const FPoint& Other )
	{
		X += Other.X;
		Y += Other.Y;
		return *this;
	}
	FPoint& operator-=( const FPoint& Other )
	{
		X -= Other.X;
		Y -= Other.Y;
		return *this;
	}
	FPoint operator+( const FPoint& Other ) const
	{
		return FPoint(*this) += Other;
	}
	FPoint operator-( const FPoint& Other ) const
	{
		return FPoint(*this) -= Other;
	}
};

struct FRect
{
	FPoint Min, Max;
	FRect()
	{}
	FRect( INT X0, INT Y0, INT X1, INT Y1 )
	:	Min( X0, Y0 )
	,	Max( X1, Y1 )
	{}
	FRect( FPoint InMin, FPoint InMax )
	:	Min( InMin )
	,	Max( InMax )
	{}
	FRect( RECT R )
	:	Min( R.left, R.top )
	,	Max( R.right, R.bottom )
	{}
	operator RECT*() const
	{
		return (RECT*)this;
	}
	const FPoint& operator()( INT i ) const
	{
		return (&Min)[i];
	}
	FPoint& operator()( INT i )
	{
		return (&Min)[i];
	}
	static INT Num()
	{
		return 2;
	}
	HBOOL operator==( const FRect& Other ) const
	{
		return Min==Other.Min && Max==Other.Max;
	}
	HBOOL operator!=( const FRect& Other ) const
	{
		return Min!=Other.Min || Max!=Other.Max;
	}
	FRect Right( INT Width )
	{
		return FRect( ::Max(Min.X,Max.X-Width), Min.Y, Max.X, Max.Y );
	}
	FRect Bottom( INT Height )
	{
		return FRect( Min.X, ::Max(Min.Y,Max.Y-Height), Max.X, Max.Y );
	}
	FPoint Size()
	{
		return FPoint( Max.X-Min.X, Max.Y-Min.Y );
	}
	INT Width()
	{
		return Max.X-Min.X;
	}
	INT Height()
	{
		return Max.Y-Min.Y;
	}
	FRect& operator+=( const FPoint& P )
	{
		Min += P;
		Max += P;
		return *this;
	}
	FRect& operator-=( const FPoint& P )
	{
		Min -= P;
		Max -= P;
		return *this;
	}
	FRect operator+( const FPoint& P ) const
	{
		return FRect( Min+P, Max+P );
	}
	FRect operator-( const FPoint& P ) const
	{
		return FRect( Min-P, Max-P );
	}
	FRect operator+( const FRect& R ) const
	{
		return FRect( Min+R.Min, Max+R.Max );
	}
	FRect operator-( const FRect& R ) const
	{
		return FRect( Min-R.Min, Max-R.Max );
	}
	FRect Inner( FPoint P ) const
	{
		return FRect( Min+P, Max-P );
	}
	HBOOL Contains( FPoint P ) const
	{
		return P.X>=Min.X && P.X<Max.X && P.Y>=Min.Y && P.Y<Max.Y;
	}
};

/*-----------------------------------------------------------------------------
	FControlSnoop.
-----------------------------------------------------------------------------*/

// For forwarding interaction with a control to an object.
class WINDOW_API FControlSnoop
{
public:
	// FControlSnoop interface.
	virtual void SnoopChar( HWindow* Src, INT Char ) {}
	virtual void SnoopKeyDown( HWindow* Src, INT Char ) {}
	virtual void SnoopLeftMouseDown( HWindow* Src, FPoint P ) {}
	virtual void SnoopRightMouseDown( HWindow* Src, FPoint P ) {}
};

class HWindow {
public:
	// Variables.
	HWND					hWnd;
	FString					PersistentName;
	WORD					ControlId, TopControlId;
	BITFIELD				Destroyed:1;
	BITFIELD				MdiChild:1;
	HWindow*				OwnerWindow;
	FControlSnoop*			Snoop;
	HTArray<class HControl*>	Controls;
	BOOL m_bShow;
	WPARAM LastwParam;	
	LPARAM LastlParam;

	// Static.
	static INT              ModalCount;
	static HTArray<HWindow*> _Windows;
	static HTArray<HWindow*> _DeleteWindows;

	// Use this procedure for modeless dialogs.
	static INT_PTR CALLBACK StaticDlgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		guard(HWindow::StaticDlgProc);
		for( INT i=0; i<_Windows.Num(); i++ )
			if( _Windows(i)->hWnd==hwndDlg )
				break;
		if( i==_Windows.Num() && uMsg==WM_INITDIALOG )
		{
			HWindow* WindowCreate = (HWindow*)lParam;
			check(WindowCreate);
			check(!WindowCreate->hWnd);
			WindowCreate->hWnd = hwndDlg;
			for( i=0; i<_Windows.Num(); i++ )
				if( _Windows(i)==WindowCreate )
					break;
			check(i<_Windows.Num());
		}
		if( i!=_Windows.Num()) // && !GIsCriticalError )
		{
			_Windows(i)->WndProc( uMsg, wParam, lParam );			
		}

		// Give up cycles.
		//
		::Sleep(0);

		return 0;
		unguard;
	}
	static LONG APIENTRY StaticWndProc( HWND hWnd, UINT Message, UINT wParam, LONG lParam )
	{
		guard(HWindow::StaticProc);
		for( INT i=0; i<_Windows.Num(); i++ )
			if( _Windows(i)->hWnd==hWnd )
				break;
		if( i==_Windows.Num() && (Message==WM_NCCREATE || Message==WM_INITDIALOG) )
		{
			HWindow* WindowCreate
			=	Message!=WM_NCCREATE
			?	(HWindow*)lParam
			:	(GetWindowLongX(hWnd,GWL_EXSTYLE) & WS_EX_MDICHILD)
			?	(HWindow*)((MDICREATESTRUCT*)((CREATESTRUCT*)lParam)->lpCreateParams)->lParam
			:	(HWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
			check(WindowCreate);
			check(!WindowCreate->hWnd);
			WindowCreate->hWnd = hWnd;
			for( i=0; i<_Windows.Num(); i++ )
				if( _Windows(i)==WindowCreate )
					break;
			check(i<_Windows.Num());
		}
		if( i==_Windows.Num()) // || GIsCriticalError )
		{
			// Gets through before WM_NCCREATE: WM_GETMINMAXINFO
			return DefWindowProcX( hWnd, Message, wParam, lParam );
		}
		else
		{
			return _Windows(i)->WndProc( Message, wParam, lParam );			
		}
		unguard;
	}
	static WNDPROC RegisterWindowClass( const TCHAR* Name, DWORD Style )
	{
		guard(HWindow::RegisterWindowClass);
#if UNICODE
		if( GUnicodeOS )
		{
			WNDCLASSEXW Cls;
			appMemzero( &Cls, sizeof(Cls) );
			Cls.cbSize			= sizeof(Cls);
			Cls.style			= Style;
			Cls.lpfnWndProc		= StaticWndProc;
			Cls.hInstance		= hInstanceWindow;
			Cls.hIcon			= LoadIconIdX(hInstanceWindow,(GIsEditor?IDICON_Editor:IDICON_Mainframe));
			Cls.lpszClassName	= Name;
			Cls.hIconSm			= LoadIconIdX(hInstanceWindow,(GIsEditor?IDICON_Editor:IDICON_Mainframe));
			verify(RegisterClassExW( &Cls ));
		}
		else
#endif
		{
			WNDCLASSEXA Cls;
			appMemzero( &Cls, sizeof(Cls) );
			Cls.cbSize			= sizeof(Cls);
			Cls.style			= Style;
			Cls.lpfnWndProc		= StaticWndProc;
			Cls.hInstance		= hInstanceWindow;
			Cls.hIcon			= NULL; //LoadIconIdX(hInstanceWindow,(GIsEditor?IDICON_Editor:IDICON_Mainframe));
			Cls.lpszClassName	= TCHAR_TO_ANSI(Name);
			Cls.hIconSm			= NULL; //LoadIconIdX(hInstanceWindow,(GIsEditor?IDICON_Editor:IDICON_Mainframe));
			verify(RegisterClassExA( &Cls ));
		}
		return NULL;
		unguard;
	}

	// Structors.
	HWindow( FString InPersistentName, HWindow* InOwnerWindow=NULL )
	:	hWnd				( NULL )
	,	PersistentName		( InPersistentName )
	,	ControlId			( 0 )
	,	TopControlId		( FIRST_AUTO_CONTROL )
	,	Destroyed			( 0 )
	,   MdiChild            ( 0 )
	,	OwnerWindow			( InOwnerWindow )
	,   Snoop               ( NULL )
	{}
	virtual ~HWindow()
	{
		guard(HWindow:;~HWindow);
		MaybeDestroy();
		HWindow::_DeleteWindows.RemoveItem( this );
		unguard;
	}

	// Accessors.
	FRect GetClientRect() const
	{
		RECT R;
		::GetClientRect( hWnd, &R );
		return FRect( R );
	}
	void MoveWindow( FRect R, HBOOL bRepaint )
	{
		::MoveWindow( hWnd, R.Min.X, R.Min.Y, R.Width(), R.Height(), bRepaint );
	}
	void MoveWindow( int Left, int Top, int Width, int Height, HBOOL bRepaint )
	{
		::MoveWindow( hWnd, Left, Top, Width, Height, bRepaint );
	}
	FRect GetWindowRect() const
	{
		RECT R;
		::GetWindowRect( hWnd, &R );
		return OwnerWindow ? OwnerWindow->ScreenToClient(R) : FRect(R);
	}
	FPoint ClientToScreen( const FPoint& InP )
	{
		POINT P;
		P.x = InP.X;
		P.y = InP.Y;
		::ClientToScreen( hWnd, &P );
		return FPoint( P.x, P.y );
	}
	FPoint ScreenToClient( const FPoint& InP )
	{
		POINT P;
		P.x = InP.X;
		P.y = InP.Y;
		::ScreenToClient( hWnd, &P );
		return FPoint( P.x, P.y );
	}
	FRect ClientToScreen( const FRect& InR )
	{
		return FRect( ClientToScreen(InR.Min), ClientToScreen(InR.Max) );
	}
	FRect ScreenToClient( const FRect& InR )
	{
		return FRect( ScreenToClient(InR.Min), ScreenToClient(InR.Max) );
	}
	FPoint GetCursorPos()
	{
		FPoint Mouse;
		::GetCursorPos( Mouse );
		return ScreenToClient( Mouse );
	}
	void Show( HBOOL Show )
	{
		guard(HWindow::Show);
		m_bShow = Show;
		ShowWindow( hWnd, Show ? ::IsIconic(hWnd) ? SW_RESTORE : SW_SHOW : SW_HIDE );
		unguard;
	}

	virtual const TCHAR* GetPackageName()
	{
		return TEXT("Window");
	}
	virtual void DoDestroy()
	{
		guard(HWindow::DoDestroy);
//		if( NotifyHook )
//			NotifyHook->NotifyDestroy( this );
		if( hWnd )
			DestroyWindow( *this );
		_Windows.RemoveItem( this );
		unguard;
	}
	virtual void GetWindowClassName( TCHAR* Result ) {};
	virtual LONG WndProc( UINT Message, UINT wParam, LONG lParam )
	{
		guard(HWindow::WndProc);
		try
		{
			LastwParam = wParam;
			LastlParam = lParam;

			// Message snoop.
			if( Snoop )
			{
				if( Message==WM_CHAR )
					Snoop->SnoopChar( this, wParam );
				else if( Message==WM_KEYDOWN )
					Snoop->SnoopKeyDown( this, wParam );
				else if( Message==WM_LBUTTONDOWN )
					Snoop->SnoopLeftMouseDown( this, FPoint(LOWORD(lParam),HIWORD(lParam)) );
				else if( Message==WM_RBUTTONDOWN )
					Snoop->SnoopRightMouseDown( this, FPoint(LOWORD(lParam),HIWORD(lParam)) );
			}

			// Special multi-window activation handling.
			if( !MdiChild && !ModalCount )
			{
				static HBOOL AppActive=0;
				if( Message==WM_ACTIVATEAPP )
				{
					AppActive = wParam;
					SendMessageX( hWnd, WM_NCACTIVATE, wParam, 0 );
				}
				else if( Message==WM_NCACTIVATE && AppActive && !wParam )
				{
					return 1;
				}
			}

			// Message processing.
			if( Message==WM_DESTROY )
			{
				OnDestroy();
			}
			else if( Message==WM_DRAWITEM )
			{
				DRAWITEMSTRUCT* Info = (DRAWITEMSTRUCT*)lParam;
				for( INT i=0; i<Controls.Num(); i++ )
					if( ((HWindow*)Controls(i))->hWnd==Info->hwndItem )
						{((HWindow*)Controls(i))->OnDrawItem(Info); break;}
				return 1;
			}
			else if( Message==WM_MEASUREITEM )
			{
				MEASUREITEMSTRUCT* Info = (MEASUREITEMSTRUCT*)lParam;
				for( INT i=0; i<Controls.Num(); i++ )
					if( ((HWindow*)Controls(i))->ControlId==Info->CtlID )
						{((HWindow*)Controls(i))->OnMeasureItem(Info); break;}
				return 1;
			}
			else if( Message==WM_CLOSE )
			{
				OnClose();
			}
			else if( Message==WM_CHAR )
			{
				OnChar( wParam );
			}
			else if( Message==WM_KEYDOWN )
			{
				OnKeyDown( wParam );
			}
			else if( Message==WM_PAINT )
			{
				OnPaint();
			}
			else if( Message==WM_CREATE )
			{
				OnCreate();
			}
			else if( Message==WM_TIMER )
			{
				OnTimer();
			}
			else if( Message==WM_INITDIALOG )
			{
				OnInitDialog();
			}
			else if( Message==WM_ENTERIDLE )
			{
				OnEnterIdle();
			}
			else if( Message==WM_SETFOCUS )
			{
				OnSetFocus( (HWND)wParam );
			}
			else if( Message==WM_ACTIVATE )
			{
				OnActivate( LOWORD(wParam)!=0 );
			}
			else if( Message==WM_KILLFOCUS )
			{
				OnKillFocus( (HWND)wParam );
			}
			else if( Message==WM_SIZE )
			{
				OnSize( wParam, LOWORD(lParam), HIWORD(lParam) );
			}
			else if( Message==WM_PASTE )
			{
				OnPaste();
			}
			else if( Message==WM_SHOWWINDOW )
			{
				OnShowWindow( wParam );
			}
			else if( Message==WM_COPYDATA )
			{
				OnCopyData( (HWND)wParam, (COPYDATASTRUCT*)lParam );
			}
			else if( Message==WM_CAPTURECHANGED )
			{
				OnReleaseCapture();
			}
			else if( Message==WM_MDIACTIVATE )
			{
				OnMdiActivate( (HWND)lParam==hWnd );
			}
			else if( Message==WM_MOUSEMOVE )
			{
				OnMouseMove( wParam, FPoint(LOWORD(lParam), HIWORD(lParam)) );
			}
			else if( Message==WM_LBUTTONDOWN )
			{
				OnLeftButtonDown();
			}
			else if( Message==WM_RBUTTONDOWN )
			{
				OnRightButtonDown();
			}
			else if( Message==WM_LBUTTONUP )
			{
				OnLeftButtonUp();
			}
			else if( Message==WM_RBUTTONUP )
			{
				OnRightButtonUp();
			}
			else if( Message==WM_CUT )
			{
				OnCut();
			}
			else if( Message==WM_COPY )
			{
				OnCopy();
			}
			else if( Message==WM_UNDO )
			{
				OnUndo();
			}
			else if( Message==WM_SETCURSOR )
			{
				if( OnSetCursor() )
					return 1;
			}
			else if( Message==WM_NOTIFY )
			{
				for( INT i=0; i<Controls.Num(); i++ )
					if(wParam==((HWindow*)Controls(i))->ControlId
							&& ((HWindow*)Controls(i))->InterceptControlCommand(Message,wParam,lParam) )
						return 1;
				OnCommand( wParam );
			}
			else if( Message==WM_VSCROLL )
			{
				OnVScroll( wParam, lParam );
			}
			else if( Message==WM_KEYUP)
			{
				OnKeyUp( wParam, lParam );
			}
			else if( Message==WM_COMMAND || Message==WM_HSCROLL )
			{
				for( INT i=0; i<Controls.Num(); i++ )
					if((HWND)lParam==((HWindow*)Controls(i))->hWnd
							&& ((HWindow*)Controls(i))->InterceptControlCommand(Message,wParam,lParam) )
						return 1;
				OnCommand( wParam );
			}
			else if( Message==WM_SYSCOMMAND )
			{
				if( OnSysCommand( wParam ) )
					return 1;
			}
			return CallDefaultProc( Message, wParam, lParam );
		}
		catch( const TCHAR* )
		{
			// This exception prevents the message from being routed to the default proc.
			return 0;
		}
		unguard;
	}
	virtual INT CallDefaultProc( UINT Message, UINT wParam, LONG lParam )
	{
		if( MdiChild )
			return DefMDIChildProcX( hWnd, Message, wParam, lParam );
		else
			return DefWindowProcX( hWnd, Message, wParam, lParam );
	}
	virtual HBOOL InterceptControlCommand( UINT Message, UINT wParam, LONG lParam )
	{
		return 0;
	}
	virtual FString GetText()
	{
		guard(HWindow::GetText);
		check(hWnd);
		INT Length = GetLength();
#if UNICODE
		if( GUnicode && !GUnicodeOS )
		{
			ANSICHAR* ACh = (ANSICHAR*)appAlloca((Length+1)*sizeof(ANSICHAR));
			SendMessageA( *this, WM_GETTEXT, Length+1, (LPARAM)ACh );
			return appFromAnsi(ACh);
		}
		else
#endif
		{
			TCHAR* Text = (TCHAR*)appAlloca((Length+1)*sizeof(TCHAR));
			SendMessage( *this, WM_GETTEXT, Length+1, (LPARAM)Text );
			return Text;
		}
		unguard;
	}
	virtual void SetText( const TCHAR* Text )
	{
		guard(HWindow::SetText);
		check(hWnd);
		SendMessageLX( *this, WM_SETTEXT, 0, Text );
		unguard;
	}
	virtual INT GetLength()
	{
		guard(HWindow::GetLength);
		check(hWnd);
		return SendMessageX( *this, WM_GETTEXTLENGTH, 0, 0 );
		unguard;
	}
	// HWindow notifications.
	virtual void OnCopyData( HWND hWndSender, COPYDATASTRUCT* CD )
	{}
	virtual void OnSetFocus( HWND hWndLosingFocus )
	{}
	virtual void OnKillFocus( HWND hWndGaininFocus )
	{}
	virtual void OnSize( DWORD Flags, INT NewX, INT NewY )
	{}
	virtual void OnCommand( INT Command )
	{}
	virtual int OnSysCommand( INT Command )
	{
		return 0;
	}
	virtual void OnActivate( HBOOL Active )
	{}
	virtual void OnChar( TCHAR Ch )
	{}
	virtual void OnKeyDown( TCHAR Ch )
	{}
	virtual void OnCut()
	{}
	virtual void OnCopy()
	{}
	virtual void OnPaste()
	{}
	virtual void OnShowWindow( HBOOL bShow )
	{}
	virtual void OnUndo()
	{}
	virtual void OnVScroll( WPARAM wParam, LPARAM lParam )
	{}
	virtual void OnKeyUp( WPARAM wParam, LPARAM lParam )
	{}
	virtual void OnPaint()
	{}
	virtual void OnCreate()
	{}
	virtual void OnDrawItem( DRAWITEMSTRUCT* Info )
	{}
	virtual void OnMeasureItem( MEASUREITEMSTRUCT* Info )
	{}
	virtual void OnInitDialog()
	{}
	virtual void OnEnterIdle()
	{}
	virtual void OnMouseEnter()
	{}
	virtual void OnMouseLeave()
	{}
	virtual void OnMouseHover()
	{}
	virtual void OnTimer()
	{}
	virtual void OnReleaseCapture()
	{}
	virtual void OnMdiActivate( HBOOL Active )
	{}
	virtual void OnMouseMove( DWORD Flags, FPoint Location )
	{}
	virtual void OnLeftButtonDown()
	{}
	virtual void OnRightButtonDown()
	{}
	virtual void OnLeftButtonUp()
	{}
	virtual void OnRightButtonUp()
	{}
//	virtual void OnFinishSplitterDrag( WDragInterceptor* Drag, HBOOL Success )
//	{}
	virtual INT OnSetCursor()
	{
		return 0;
	}
	virtual void OnClose()
	{
		guard(HWindow::OnClose);
		if( MdiChild )
			SendMessage( OwnerWindow->hWnd, WM_MDIDESTROY, (WPARAM)hWnd, 0 );
		else
			DestroyWindow( *this );
		unguard;
	}
	virtual void OnDestroy()
	{
		guard(HWindow::OnDestroy);
		check(hWnd);
		_Windows.RemoveItem( this );
		hWnd = NULL;
		unguard;
	}

	// HWindow functions.
	void SaveWindowPos()
	{
		guard(HWindow::SaveWindowPos);
		unguard;
	}
	void MaybeDestroy()
	{
		guard(HWindow::MaybeDestroy);
		if( !Destroyed )
		{
			Destroyed=1;
			DoDestroy();
		}
		unguard;
	}
	void _CloseWindow()
	{
		guard(HWindow::_CloseWindow);
		check(hWnd);
		DestroyWindow( *this );
		unguard;
	}
	operator HWND() const
	{
		return hWnd;
	}
	void SetFont( HFONT hFont )
	{
		guard(HWindow::SetFont);
		SendMessageX( *this, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(0,0) );
		unguard;
	}
	void PerformCreateWindowEx( DWORD dwExStyle, LPCTSTR lpWindowName, DWORD dwStyle, INT x, INT y, INT nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance )
	{
		guard(PerformCreateWindowEx);
		check(hWnd==NULL);

/*		// Retrieve remembered position.
		TCHAR Pos[256];
		if
		(	PersistentName!=NAME_None 
		&&	GConfig->GetString( TEXT("WindowPositions"), *PersistentName, Pos, ARRAY_COUNT(Pos) ) )
		{
			// Get saved position.
			Parse( Pos, TEXT("X="), x );
			Parse( Pos, TEXT("Y="), y );
			if( dwStyle & WS_SIZEBOX )
			{
				Parse( Pos, TEXT("XL="), nWidth );
				Parse( Pos, TEXT("YL="), nHeight );
			}

			// Count identical windows already opened.
			INT Count=0;
			for( INT i=0; i<_Windows.Num(); i++ )
			{
				Count += _Windows(i)->PersistentName==PersistentName;
			}
			if( Count )
			{
				// Move away.
				x += Count*16;
				y += Count*16;
			}

			// Clip size to screen.
			RECT Desktop;
			::GetWindowRect( GetDesktopWindow(), &Desktop );
			if( x+nWidth  > Desktop.right  ) x = Desktop.right  - nWidth;
			if( y+nHeight > Desktop.bottom ) y = Desktop.bottom - nHeight;
			if( x<0 )
			{
				if( dwStyle & WS_SIZEBOX )
					nWidth += x;
				x=0;
			}
			if( y<0 )
			{
				if( dwStyle & WS_SIZEBOX )
					nHeight += y;
				y=0;
			}
		}
*/
		// Create window.
		_Windows.AddItem( this );
		TCHAR ClassName[256];
		GetWindowClassName( ClassName );
		//hinstance must match window class hinstance!!
		HWND hWndCreated = TCHAR_CALL_OS(CreateWindowEx(dwExStyle,ClassName,lpWindowName,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstanceWindow,this),CreateWindowExA(dwExStyle,TCHAR_TO_ANSI(ClassName),TCHAR_TO_ANSI(lpWindowName),dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstanceWindow,this));
		//if( !hWndCreated )
		//	appErrorf( TEXT("CreateWindowEx failed: %s"), appGetSystemErrorMessage() );
		check(hWndCreated);
		check(hWndCreated==hWnd);
		unguard;
	}
	void SetRedraw( HBOOL Redraw )
	{
		guard(HWindow::SetRedraw);
		SendMessageX( *this, WM_SETREDRAW, Redraw, 0 );
		unguard;
	}
	// Used in the editor ... used to draw window edges in custom colors.
	virtual void MyDrawEdge( HDC hdc, LPRECT qrc, HBOOL bRaised )
	{
		guard(HWindow::MyDrawEdge);

		HPEN penOld, penRaised = CreatePen( PS_SOLID, 1, RGB(159,159,159) ),
			penSunken = CreatePen( PS_SOLID, 1, RGB(106,106,106) );
		HDC	hDC = GetDC( hWnd );

		RECT rc = *qrc;
		rc.right -= 1;
		rc.bottom -= 1;

		penOld = (HPEN)SelectObject( hDC, (bRaised ? penRaised : penSunken ) );
		::MoveToEx( hDC, rc.left, rc.top, NULL );
		::LineTo( hDC, rc.right, rc.top );
		::MoveToEx( hDC, rc.left, rc.top, NULL );
		::LineTo( hDC, rc.left, rc.bottom);
		SelectObject( hDC, penOld );

		penOld = (HPEN)SelectObject( hDC, (bRaised ? penSunken : penRaised ) );
		::MoveToEx( hDC, rc.right, rc.bottom, NULL );
		::LineTo( hDC, rc.right, rc.top );
		::MoveToEx( hDC, rc.right, rc.bottom, NULL );
		::LineTo( hDC, rc.left, rc.bottom );
		SelectObject( hDC, penOld );

		DeleteObject( penRaised );
		DeleteObject( penSunken );
		::ReleaseDC( hWnd, hDC );

		unguard;
	}
};
