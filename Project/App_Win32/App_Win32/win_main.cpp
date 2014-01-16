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



////--------------------------------------------------------------------------------------
// File: main.cpp
//
// Desc: Tutorial to show how to create a rendering window using EGL.
//
// Author:     QUALCOMM, Advanced Content Group - Adreno SDK
//
//               Copyright (c) 2009 QUALCOMM Incorporated. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------
//#pragma   warning   (   disable   :   4244   )
#define _WIN32_WINNT 0x0502
#ifdef OPENGL
#include <GLEW/glew.h>
#include <GLEW/wglew.h>
#else
#include <EGL/egl.h>
#endif

#include "Game.h"
#include "Log.h"
#include "FilePath.h"
#include "ShopHelper.h"

#include <stdio.h>
#include <stdlib.h>
#include "Track.h"
#include <stdio.h>
#include <windows.h>
#include <TCHAR.h>

using namespace std;
using namespace BohgeGame;
using namespace BohgeEngine;
//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
//#define FULLSCREEN
//#define HOME

//#define _4_3
//#define _3_2
#define _16_9

#ifdef HOME
#ifdef _16_9
const static int WIDTH = 960;
const static int HEIGHT = 540;
#endif
#ifdef _3_2
const static int WIDTH = 960;
const static int HEIGHT = 640;
#endif
#ifdef _4_3
const static int WIDTH = 768;
const static int HEIGHT = 576;
#endif
#else
#ifdef _16_9
const static int WIDTH = 240;
const static int HEIGHT = 135;
#endif
#ifdef _3_2
const static int WIDTH = 240;
const static int HEIGHT = 160;
#endif
#ifdef _4_3
const static int WIDTH = 240;
const static int HEIGHT = 180;
#endif
#endif

#define ID_EDIT 1
#ifdef OPENGL
HGLRC						g_hGLRC;
HDC							g_hHDC;
#endif
#ifdef ADRENO
EGLDisplay eglDisplay;
EGLSurface eglSurface;
#endif
HWND	g_hWindow;
HWND	g_hEdit;
int		maxSize;
bool	KeyDownState[256] = {false};
bool	isMouseDown = false;
static ActionManage::InputAction input;
static bool is2Point = false;
#ifdef FULLSCREEN
bool	g_isFullScreen = true;
#else
bool	g_isFullScreen = false;
#endif
//--------------------------------------------------------------------------------------
// Name: WndProc()
// Desc: The application window's message proc
//--------------------------------------------------------------------------------------
void DrawCall()
{
	//Update and render the application
	for ( int i = 0; i < 256; i ++ )
	{
		if ( KeyDownState[i] )
		{
			switch(i) 
			{
			case '1' : Game::Instance().SetKeyDown( ActionManage::KD_KEY1 ); break;
			case '2' : Game::Instance().SetKeyDown( ActionManage::KD_KEY2 ); break;
			case '3' : Game::Instance().SetKeyDown( ActionManage::KD_KEY3 ); break;
			case '4' : Game::Instance().SetKeyDown( ActionManage::KD_KEY4 ); break;
			case '5' : Game::Instance().SetKeyDown( ActionManage::KD_KEY5 ); break;
			case '6' : Game::Instance().SetKeyDown( ActionManage::KD_KEY6 ); break;
			case '7' : Game::Instance().SetKeyDown( ActionManage::KD_KEY7 ); break;
			case '8' : Game::Instance().SetKeyDown( ActionManage::KD_KEY8 ); break;
			case '9' : Game::Instance().SetKeyDown( ActionManage::KD_KEY9 ); break;
			case '0' : Game::Instance().SetKeyDown( ActionManage::KD_KEY0 ); break;
			case 'q' :
			case 'Q' : Game::Instance().SetKeyDown( ActionManage::KD_KEYQ ); break;
			case 'e' :
			case 'E' : Game::Instance().SetKeyDown( ActionManage::KD_KEYE ); break;
			case 'w' :
			case 'W' : Game::Instance().SetActionState(ActionManage::ACTION_MOVEFORWARD); break;
			case 'a' :
			case 'A' : Game::Instance().SetActionState(ActionManage::ACTION_MOVELEFT); break;
			case 's' :
			case 'S' : Game::Instance().SetActionState(ActionManage::ACTION_MOVEBACK); break;
			case 'd' :
			case 'D' : Game::Instance().SetActionState(ActionManage::ACTION_MOVELRIGHT); break;
			}

		}
	}
	if( !Game::Instance().Update() )
	{
		PostMessage(g_hWindow, WM_QUIT, 0, 0);
	}
	Game::Instance().Paint();
	// Present the scene
#ifdef ADRENO
	eglSwapBuffers( eglDisplay, eglSurface );
#endif
#ifdef OPENGL
	wglMakeCurrent( g_hHDC, g_hGLRC);
	SwapBuffers( g_hHDC );
#endif
}
std::wstring Ansi2WChar(char* pszSrc, int nLen)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if(nSize <= 0) return NULL;
	WCHAR *pwszDst = new WCHAR[nSize+1];
	if( NULL == pwszDst) return NULL;
	MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;
	if( pwszDst[0] == 0xFEFF) // skip Oxfeff
		for(int i = 0; i < nSize; i ++) 
			pwszDst[i] = pwszDst[i+1]; 
	wstring wcharString(pwszDst);
	delete pwszDst;
	return wcharString;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_COMMAND :
		{
			if (LOWORD (wParam) == ID_EDIT)
			{
				if (HIWORD (wParam) == EN_UPDATE )
				{
					if( 0 != maxSize )
					{
						int inputed = GetWindowTextLength( g_hEdit );
						if( inputed > maxSize)
						{
							SendMessage (g_hEdit, EM_UNDO, 0, 0) ;
						}
					}
					int iCount = SendMessage (g_hEdit, EM_GETLINECOUNT, 0, 0) ;
					if( iCount == 2)
					{
						int iLength = GetWindowTextLength( g_hEdit );
						iLength-=1;//去掉回车
						char* data = new char[iLength];
						if( iLength !=0 )
						{
							GetWindowText(g_hEdit, data ,iLength);
						}
						Engine::Instance().GetUIManager()->OnReceiveText( Ansi2WChar(data, iLength) );
						SAFE_DELETE_ARRAY( data );
						ShowWindow(g_hEdit, SW_HIDE);
					}
				}
			}
		}return 0;
	case WM_KEYDOWN:
		{
			KeyDownState[wParam] = true;
			switch(wParam) 
			{
			case 't':
			case 'T':
				{
					is2Point = true; 
				}break;
			}
		}return 0;
	case WM_KEYUP:
		{
			KeyDownState[wParam] = false;
			switch(wParam) 
			{
			case 't':
			case 'T':
				{
					is2Point = false;
				}break;
			case 'e':
			case 'E':
				{
					Game::Instance().SetActionState(ActionManage::ACTION_EXIT);
				}break;
			case 'q':
			case 'Q':
				{
					Game::Instance().SetActionState(ActionManage::ACTION_FALL);
				}break;
			case 'w':
			case 'W':
				{
					Game::Instance().SetActionState(ActionManage::ACTION_UP);
				}break;
			case 'a':
			case 'A':
				{
					Game::Instance().SetActionState(ActionManage::ACTION_LEFT);
				}break;
			case 's':
			case 'S':
				{
					Game::Instance().SetActionState(ActionManage::ACTION_DOWN);
				}break;
			case 'd':
			case 'D':
				{
					Game::Instance().SetActionState(ActionManage::ACTION_RIGHT);
				}break;
			case 'p':
			case 'P':
				{
					Game::Instance().SetActionState(ActionManage::ACTION_PAUSE);
				}break;
			case 'b':
			case 'B':
				{
					Game::Instance().BecomeBackground();
				}break;
			case ' ':
				{
					Game::Instance().SetActionState(ActionManage::ACTION_SWITH);
				}break;
			}
		}return 0;
	case WM_LBUTTONDOWN:
		{
			isMouseDown = true;
			//转换为opengl空间
			Engine& engine = Engine::Instance();
			float w = engine.GetDevice()->GetViewprot().m_z;
			float h = engine.GetDevice()->GetViewprot().m_w;
			int xPos = LOWORD(lParam);
			int yPos = HEIGHT - HIWORD(lParam);//转换
			w = xPos / w;
			w = 2.0 * w - 1.0;
			h = yPos / h;
			h = 2.0 * h - 1.0;
			input.Begin();
			input.SetType( ActionManage::INPUT_DOWN );
			input.SetPoint( vector2f(w,h) );
			if( is2Point )
			{
				input.SetPoint( vector2f(-w,-h) );
			}
			Game::Instance().TouchPoint( input );
		}return 0;
	case WM_MOUSEMOVE:
		{
			Engine& engine = Engine::Instance();
			float w = engine.GetDevice()->GetViewprot().m_z;
			float h = engine.GetDevice()->GetViewprot().m_w;
			int xPos = LOWORD(lParam);
			int yPos = HEIGHT - HIWORD(lParam);//转换
			w = xPos / w;
			w = 2.0 * w - 1.0;
			h = yPos / h;
			h = 2.0 * h - 1.0;
			input.Begin();
			input.SetType( ActionManage::INPUT_MOVE );
			input.SetPoint( vector2f(w,h) );
			if( is2Point )
			{
				input.SetPoint( vector2f(-w,-h) );
			}
			Game::Instance().TouchPoint( input );
		}return 0;
	case WM_LBUTTONUP:
		{
			isMouseDown = false;
			//转换为opengl空间
			Engine& engine = Engine::Instance();
			float w = engine.GetDevice()->GetViewprot().m_z;
			float h = engine.GetDevice()->GetViewprot().m_w;
			int xPos = LOWORD(lParam);
			int yPos = HEIGHT - HIWORD(lParam);//转换
			w = xPos / w;
			w = 2.0 * w - 1.0;
			h = yPos / h;
			h = 2.0 * h - 1.0;
			input.Begin();
			input.SetType( ActionManage::INPUT_UP );
			input.SetPoint( vector2f(w,h) );
			if( is2Point )
			{
				input.SetPoint( vector2f(-w,-h) );
			}
			Game::Instance().TouchPoint( input );
		}return 0;
	case WM_ERASEBKGND:
		return true;
	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
	}
	// Pass all unhandled messages to the default WndProc
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//--------------------------------------------------------------------------------------
// Name: WinMain()
// Desc: Application entry-point on the Windows platform
//--------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR *lpCmdLine, int nCmdShow )
{
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
	wc.lpszClassName	= "Game";								 // Set the class name
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
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			ASSERT(FALSE);
		}
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP|WS_CLIPSIBLINGS |	WS_CLIPCHILDREN;	// Windows Style
		//ShowCursor(FALSE);										// Hide Mouse Pointer
		SetRect( &WindowRect, 0, 0, deskX, deskY );
		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size
		// Create the parent window
		g_hWindow = CreateWindowEx(	
			dwExStyle,							// Extended Style For The Window
			"Game",							// Class Name
			"Bohge",								// Window Title
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
			"Game",							// Class Name
			"Bohge",								// Window Title
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
#ifdef ADRENO
	// Get the display
	eglDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
	if( eglDisplay == EGL_NO_DISPLAY )
		return FALSE;

	// Set our EGL API to OpenGL ES
	if( FALSE == eglBindAPI( EGL_OPENGL_ES_API ) )
		return FALSE;

	// Initialize EGL
	EGLint nMajorVersion, nMinorVersion;
	if( FALSE == eglInitialize( eglDisplay, &nMajorVersion, &nMinorVersion ) )
		return FALSE;

	// Get total number of configs
	EGLint nNumConfigs;
	if( FALSE == eglGetConfigs( eglDisplay, NULL, 0, &nNumConfigs ) )
		return FALSE;

	// Get the first config
	EGLConfig configs[10];
	if (FALSE == eglGetConfigs(eglDisplay, configs, 1, &nNumConfigs) )
		return FALSE;

	// Get the first config's Color buffer size
	EGLint nBuffSize;
	eglGetConfigAttrib(eglDisplay, configs[0], EGL_BUFFER_SIZE, &nBuffSize);

	// Test the display configuration
	EGLConfig eglConfig;

	if (nBuffSize == 32)
	{
		// Build the attibute list
		EGLint AttributeList[] = 
		{ 
			EGL_RED_SIZE,		8,
			EGL_GREEN_SIZE,	    8,
			EGL_BLUE_SIZE,	    8,
			EGL_ALPHA_SIZE,	    8,
			EGL_DEPTH_SIZE,	    24,
			EGL_STENCIL_SIZE,   8,
			EGL_SAMPLE_BUFFERS, 1,
			EGL_SAMPLES,		4,
			EGL_NONE
		};

		// Choose config based on the requested attributes
		if( FALSE == eglChooseConfig( eglDisplay, AttributeList, &eglConfig, 1, &nNumConfigs ) )
			return FALSE;
	}
	else if (nBuffSize == 16)
	{
		// Build the attibute list
		EGLint AttributeList[] = 
		{ 
			EGL_RED_SIZE,		5,
			EGL_GREEN_SIZE,	    6,
			EGL_BLUE_SIZE,	    5,
			EGL_ALPHA_SIZE,	    0,
			EGL_DEPTH_SIZE,	    24,
			EGL_STENCIL_SIZE,   8,
			EGL_SAMPLE_BUFFERS, 1,
			EGL_SAMPLES,		4,
			EGL_NONE
		};

		// Choose config based on the requested attributes
		if( FALSE == eglChooseConfig( eglDisplay, AttributeList, &eglConfig, 1, &nNumConfigs ) )
			return FALSE;
	}
	else return FALSE; // unsupported display

	// Create a window surface
	eglSurface = eglCreateWindowSurface( eglDisplay, eglConfig, g_hWindow, NULL );
	if( EGL_NO_SURFACE == eglSurface )
		return FALSE;

	// Create a OpenGL ES 2.0 rendering context
	EGLint ContextAttribList[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	EGLContext eglContext = eglCreateContext( eglDisplay, eglConfig, EGL_NO_CONTEXT, ContextAttribList );
	if( EGL_NO_CONTEXT == eglContext )
		return FALSE;

	// Make the context current
	if( FALSE == eglMakeCurrent( eglDisplay, eglSurface, eglSurface, eglContext ) )
		return FALSE;
#endif

#ifdef OPENGL
	GLuint PixelFormat; 
	static PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小 
		1,                                // 版本号 
		PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图 
		PFD_SUPPORT_OPENGL |              // 支持 OpenGL 
		PFD_DOUBLEBUFFER,                 // 双缓存模式 
		PFD_TYPE_RGBA,                    // RGBA 颜色模式 
		32,                               // 32 位颜色深度 
		0, 0, 0, 0, 0, 0,                 // 忽略颜色位 
		1,                                // 没有非透明度缓存 
		0,                                // 忽略移位位 
		0,                                // 无累加缓存 
		0, 0, 0, 0,                       // 忽略累加位 
		24,                               // 24 位深度缓存     
		8,                                // 无模板缓存 //改动阴影
		0,                                // 无辅助缓存 
		PFD_MAIN_PLANE,                   // 主层 
		0,                                // 保留 
		0, 0, 0                           // 忽略层,可见性和损毁掩模 
	};
	g_hHDC = GetDC( g_hWindow );

	// 选择合适的Device Context像素点格式
	if (!(PixelFormat=ChoosePixelFormat(g_hHDC,&pfd)))  // Did Windows Find A Matching Pixel Format?
	{       
		::MessageBox(NULL,_T("Can't Find A Suitable PixelFormat."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION );
		return 0;         // Return FALSE
	}

	// 设置新的Device Context像素点格式
	if(!SetPixelFormat(g_hHDC,PixelFormat,&pfd))   // Are We Able To Set The Pixel Format?
	{
		::MessageBox(NULL,_T("Can't Set The PixelFormat."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return 0;         // Return FALSE
	}
	// 根据当前的Device Context得到rending context
	if (!(g_hGLRC=wglCreateContext(g_hHDC))) // Are We Able To Get A Rendering Context?
	{
		//KillGLWindow();        // Reset The Display
		::MessageBox(NULL,_T("Can't Create A GL Rendering Context."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return 0;         // Return FALSE
	}
	// 设置当前Device context 的rendering context
	if(!wglMakeCurrent(g_hHDC, g_hGLRC))   // Try To Activate The Rendering Context
	{
		//KillGLWindow();        // Reset The Display
		::MessageBox(NULL,_T("Can't Activate The GL Rendering Context."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return 0;         // Return FALSE
	}

	GLenum err = glewInit(); 
	if (GLEW_OK != err) 
	{ 
		DEBUGLOG("Error: %s\n", glewGetErrorString(err)); 
	} 
	DEBUGLOG("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//if ( wglSwapIntervalEXT )//不限制帧率会导致hdr的错误
	//{
	//	wglSwapIntervalEXT(0);
	//}
#endif

	//出事话对话框
	g_hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
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
	TCHAR strFilePath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, strFilePath, MAX_PATH);
	(strrchr(strFilePath, ('\\')))[1] = 0;
	SetCurrentDirectory( strFilePath );
	string path(strFilePath);
	FILEPATH.SetRootPath(path);
	FILEPATH.SetWritePath(path);

	//初始化
	Game::Instance().GetAdmobID(AdHelper::PF_ANDROID);
	Game::Instance().Initialization( vector2d(windowsWidth, windowsHeight), vector2d(WIDTH, HEIGHT), Engine::LEL_ULTRA, false, path, "12345678901234567890123456789012");
	//假的item
	ShopHelper::Instance().isAllowIAP() = true;
	Game::Instance().CheckIapStatus();
	ShopHelper::Instance().InsertItemString( 1, L"30", L"30");
	ShopHelper::Instance().InsertItemString( 2, L"70", L"70");
	ShopHelper::Instance().InsertItemString( 5, L"200", L"200");
	ShopHelper::Instance().InsertItemString( 9, L"360", L"360");
	ShopHelper::Instance().InsertItemString( 15, L"660", L"660");
	ShopHelper::Instance().InsertItemString( 20, L"1000", L"1000");
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
			DrawCall();
		}
	}
	// Cleanup any objects created in Initialize()
	Game::Instance().Exit();
	DumpUnfreed();
	return TRUE;
}


void HideKeyBorad()
{
	ShowWindow(g_hEdit, SW_HIDE);
	Engine::Instance().GetUIManager()->OnReceiveText( L"Test" );
}
void Keyborad(const vector4f& rect, uint maxsize, const std::wstring& was, bool isNumberOnly  )
{
	maxSize = maxsize;
	Engine& engine = Engine::Instance();
	float sw = engine.GetDevice()->GetViewprot().m_z;
	float sh = engine.GetDevice()->GetViewprot().m_w;
	float x = (rect.m_x+1.0) / 2.0 * sw;
	float y = sh - (rect.m_y+1.0) / 2.0 * sh;
	float w = (rect.m_z+1.0) / 2.0 * sw;
	float h = sh - (rect.m_w+1.0) / 2.0 * sh;
	//SendMessage(g_hEdit, WM_SETTEXT)
	SendMessage(g_hEdit, WM_SETTEXT, 0, (LPARAM)L"" );
	ShowWindow(g_hEdit, SW_SHOW);
	MoveWindow(g_hEdit, x, y, w-x, h-y, TRUE );
	SetFocus(g_hEdit);
}

void RefreshItem()
{

}

void IAPBuyItem(int index)
{
	string str;
	str.resize( 2302);
	for ( int i = 0 ; i < 2302 ; i ++ )
	{
		str[i] = '0' + (i%10);
	}
	ShopHelper::Instance().PaymentTransactionStatePurchased( str );
	//ShopHelper::Instance().PaymentTransactionStateFailed();
}