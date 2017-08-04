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
#include "Framework/IActionSystem.h"
#include "Framework/Framework.h"
#include "IApplication.h"
#include "ILiteapp.h"

extern BohgeApp::ILiteapp* CreateEnterPoint();
extern void DestroyLiteapp( BohgeApp::ILiteapp* app );


using namespace BohgeEngine;
namespace BohgeApp
{
	RTTI_TYPE_IMPLEMENT( IApplication );
	SYSTEM_SINGLETON_IMPLEMENT( IApplication );
	//---------------------------------------------------------------------------------------------------------
	IApplication::IApplication(void)
		:m_pLoader(NULL),
		m_pLiteapp(NULL),
		m_pNextLite(NULL)
	{
		Framework::Create();
		InitializeNeonMath();
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	IApplication::~IApplication(void)
	{
		Framework::Destroy();
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void IApplication::Initialize( void* hWindows, const vector4d& view, const vector2d& resolution, const eastl::string& root, const eastl::string& wirte )
	{
		m_pLoader = _CreateLoader();
		Config cf;
		cf.SetViewSize( view );
		cf.SetResolution( resolution );
		cf.SetRootPath( root );
		cf.SetResourcePath( wirte );
		Framework::Instance()->SetConfig( cf );
		Framework::Instance()->SetWindowsHandle( hWindows );
		m_pLiteapp = CreateEnterPoint();
		m_pLiteapp->Launch();
	}
	//---------------------------------------------------------------------------------------------------------
	void IApplication::Resizeview( const vector4d& view )
	{
		Framework::Instance()->Resizeview( view );
	}
	//---------------------------------------------------------------------------------------------------------
	void IApplication::Update()
	{
		if ( m_pNextLite )
		{
			m_pLiteapp->Exit();
			DestroyLiteapp( m_pLiteapp );
			m_pLiteapp = m_pNextLite;
			m_pLiteapp->Launch();
			m_pNextLite = NULL;
		}
		m_pLiteapp->Update();
	}
	//---------------------------------------------------------------------------------------------------------
	void IApplication::Exit()
	{
		m_pLiteapp->Exit();
		DestroyLiteapp( m_pLiteapp );
		_ReleaseLoader(m_pLoader);
	}
	//---------------------------------------------------------------------------------------------------------
	void IApplication::SwapLiteapp( ILiteapp* lite )
	{
		m_pNextLite = lite;
	}
	//---------------------------------------------------------------------------------------------------------
	void IApplication::PushTouchAction( const TouchAction& ta )
	{
		if ( IActionSystem::Instance() )
		{
			IActionSystem::Instance()->SystemTouchInput( ta );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void IApplication::PushKeyboardAction( const KeyboardAction& ka )
	{
		if ( IActionSystem::Instance() )
		{
			IActionSystem::Instance()->SystemKeyboardInput( ka );
		}
	}

}