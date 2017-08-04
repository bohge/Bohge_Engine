//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////
//#pragma   warning   (   disable   :   4244   )
#define _WIN32_WINNT 0x0502

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <TCHAR.h>
#include <string.h>

#include "Framework/3DMath.h"
#include "WindowsApplication.h"

using namespace BohgeEngine;
using namespace BohgeWindows;

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
//#define FULLSCREEN
//#define HOME
#define PORTRAIT
//#define _4_3
//#define _3_2
#define _16_9

#ifdef PORTRAIT
#ifdef HOME
#ifdef _16_9
static int WIDTH = 540;
static int HEIGHT = 960;
#endif
#ifdef _3_2
static int WIDTH = 640;
static int HEIGHT = 960;
#endif
#ifdef _4_3
static int WIDTH = 576;
static int HEIGHT = 768;
#endif
#else
#ifdef _16_9
static int WIDTH = 180;
static int HEIGHT = 320;
#endif
#ifdef _3_2
static int WIDTH = 160;
static int HEIGHT = 240;
#endif
#ifdef _4_3
static int WIDTH = 180;
static int HEIGHT = 240;
#endif
#endif
#else
#ifdef HOME
#ifdef _16_9
static int WIDTH = 960;
static int HEIGHT = 540;
#endif
#ifdef _3_2
static int WIDTH = 960;
static int HEIGHT = 640;
#endif
#ifdef _4_3
static int WIDTH = 768;
static int HEIGHT = 576;
#endif
#else
#ifdef _16_9
static int WIDTH = 180;
static int HEIGHT = 320;
#endif
#ifdef _3_2
static int WIDTH = 240;
static int HEIGHT = 160;
#endif
#ifdef _4_3
static int WIDTH = 240;
static int HEIGHT = 180;
#endif
#endif
#endif

#define ID_EDIT 1
HWND	g_hWindow;
HWND	g_hEdit;
int		maxSize;
#ifdef FULLSCREEN
bool	g_isFullScreen = true;
#else
bool	g_isFullScreen = false;
#endif
//--------------------------------------------------------------------------------------
// Name: WndProc()
// Desc: The application window's message proc
//--------------------------------------------------------------------------------------


WindowsApplication* g_pApp;
bool isDragMove = false;
bool isPress = false;
KeyboardAction g_KeyboardAction;
TouchAction g_TouchAction;
bool	KeyDownState[256] = {false};
vector2f ToGameTouchPoint( WPARAM lParam )
{
	float w = WIDTH;
	float h = HEIGHT;
	int xPos = LOWORD(lParam);
	int yPos = HEIGHT - HIWORD(lParam);//转换
	w = xPos / w;
	w = 2.0 * w - 1.0;
	h = yPos / h;
	h = 2.0 * h - 1.0;
	return vector2f( w, h );
}
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			KeyDownState[wParam] = true;
			return true;
		}
	case WM_KEYUP:
		{
			KeyDownState[wParam] = false;
			return true;
		}
	case WM_LBUTTONDOWN:
		{
			isPress = true;
			g_TouchAction.Begin();
			g_TouchAction.SetType( ActionPropery::TT_PRESS );
			g_TouchAction.SetPoint( ToGameTouchPoint( lParam ) );
			g_pApp->PushTouchAction( g_TouchAction );
		}return 0;
	case WM_MOUSEMOVE:
		{
			if ( isPress )
			{
				g_TouchAction.Begin();
				g_TouchAction.SetType( ActionPropery::TT_MOVE );
				g_TouchAction.SetPoint( ToGameTouchPoint( lParam ) );
				g_pApp->PushTouchAction( g_TouchAction );
			}
		}return 0;
	case WM_LBUTTONUP:
		{
			isPress = false;
			g_TouchAction.Begin();
			g_TouchAction.SetType( ActionPropery::TT_RELEASE );
			g_TouchAction.SetPoint( ToGameTouchPoint( lParam ) );
			g_pApp->PushTouchAction( g_TouchAction );
		}return 0;
	case WM_ENTERSIZEMOVE:
		{
			isDragMove = true;
			return true;
		}
	case WM_SIZE:
		{
			uint w = LOWORD(lParam);
			uint h = HIWORD(lParam);
			if ( WIDTH != w || HEIGHT != h )
			{
				WIDTH = LOWORD(lParam);
				HEIGHT = HIWORD(lParam);
				if ( !isDragMove )//最大化
				{
					g_pApp->Resizeview( vector4d(0,0,WIDTH,HEIGHT) );
				}
			}
			return true;
		}
	case WM_EXITSIZEMOVE:
		{
			isDragMove = false;
			g_pApp->Resizeview( vector4d(0,0,WIDTH,HEIGHT) );
			return true;
		}
	case WM_ERASEBKGND:
		{
			return true;
		}
	case WM_CLOSE:
		{
			PostQuitMessage( 0 );
			return 0;
		}
	}
	// Pass all unhandled messages to the default WndProc
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//--------------------------------------------------------------------------------------
// Name: WinMain()
// Desc: Application entry-point on the Windows platform
//--------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	MemoryTrack::Initialize();
	g_pApp = NEW WindowsApplication;
	// Create the application window
	// Register the window class
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Window style
	wc.lpfnWndProc		= (WNDPROC)WndProc;                    // WndProc message handler
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.hInstance		= GetModuleHandle( NULL );              // Instance
	wc.lpszClassName	= _T("Game");							 // Set the class name
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );       // Cursor
	if( FALSE == RegisterClass(&wc) )
		return FALSE;

	int deskX = GetSystemMetrics ( SM_CXSCREEN ); 
	int deskY = GetSystemMetrics ( SM_CYSCREEN ); 
	int windowsWidth, windowsHeight;
	if ( g_isFullScreen )												// Attempt Fullscreen Mode?
	{
		windowsWidth = deskX;
		windowsHeight = deskY;
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= deskX;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= deskY;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= 32;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP|WS_CLIPSIBLINGS |	WS_CLIPCHILDREN;	// Windows Style
		//ShowCursor(FALSE);										// Hide Mouse Pointer
		SetRect( &WindowRect, 0, 0, deskX, deskY );
		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size
		// Create the parent window
		g_hWindow = CreateWindowEx(	
			dwExStyle,							// Extended Style For The Window
			_T("Game"),							// Class Name
			_T("Bohge"),						// Window Title
			dwStyle ,							// Defined Window Style
			//WS_CLIPSIBLINGS |					// Required Window Style
			//WS_CLIPCHILDREN,					// Required Window Style
			0, 0,								// Position
			(WindowRect.right-WindowRect.left), // Width
			(WindowRect.bottom-WindowRect.top),        // Height
			NULL,								// No Parent Window
			NULL,								// No Menu
			GetModuleHandle( NULL ),							// Instance
			NULL);								// Dont Pass Anything To WM_CREATE

	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS |	WS_CLIPCHILDREN;// Windows Style
		deskX -= WIDTH;
		deskX /= 2;
		deskY -= HEIGHT;
		deskY /= 2;
		SetRect( &WindowRect, deskX, deskY, deskX+WIDTH, deskY+HEIGHT );
		windowsWidth = WIDTH;
		windowsHeight = HEIGHT;
		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size
		// Create the parent window
		g_hWindow = CreateWindowEx(	
			dwExStyle,							// Extended Style For The Window
			_T("Game"),							// Class Name
			_T("Bohge"),						// Window Title
			dwStyle ,							// Defined Window Style
			//WS_CLIPSIBLINGS |					// Required Window Style
			//WS_CLIPCHILDREN,					// Required Window Style
			50 + WindowRect.left, 50 + WindowRect.top, // Position
			(WindowRect.right-WindowRect.left),        // Width
			(WindowRect.bottom-WindowRect.top),        // Height
			NULL,								// No Parent Window
			NULL,								// No Menu
			GetModuleHandle( NULL ),							// Instance
			NULL);								// Dont Pass Anything To WM_CREATE

	}

	if( NULL == g_hWindow )
		return FALSE;

	ShowWindow( g_hWindow, SW_SHOW );
	SetForegroundWindow( g_hWindow );
	SetFocus( g_hWindow );

	//出事话对话框
	g_hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,
		_T("EDIT"),
		_T(""),
		WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
		0,
		0,
		0,
		0,
		g_hWindow,
		(HMENU) ID_EDIT,
		GetModuleHandle(NULL),
		NULL);
	ShowWindow(g_hEdit, SW_HIDE);

	//游戏相关代码的开始
	//set work path
	char strFilePath[MAX_PATH + 1]={0};
	GetModuleFileNameA(NULL, strFilePath, MAX_PATH);
	(strrchr(strFilePath, ('\\')))[1] = 0;
	SetCurrentDirectoryA( strFilePath );
	eastl::string path(strFilePath);
	//加载插件
	g_pApp->Initialize( &g_hWindow, vector4d(0,0,windowsWidth, windowsHeight), vector2d(windowsWidth, windowsHeight), path, path );

	// Run the main loop until the user closes the window
	while ( true ) 
	{
		MSG mssg;
		if (PeekMessage( &mssg, NULL, 0, 0, PM_REMOVE)) 
		{
			// dispatch the message
			if( mssg.message == WM_QUIT )
			{
				break;
			}
			else
			{
				TranslateMessage(&mssg);
				DispatchMessage(&mssg);

			}
		} 
		else 
		{
			for ( int i = 0; i < 256; i ++ )
			{
				if ( KeyDownState[i] )
				{
					g_KeyboardAction.SetKeyboard( i );
					g_pApp->PushKeyboardAction( g_KeyboardAction );
				}
			}
			g_pApp->Update();
		}
	}
	// Cleanup any objects created in Initialize()
	g_pApp->Exit();
	SAFE_DELETE( g_pApp );
	MemoryTrack::DumpUnfreed();
	return TRUE;
}