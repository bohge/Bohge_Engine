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
#include "Framework/JsonReader.h"
#include "Framework/Framework.h"
#include "Framework/Log.h"
#include "DataLoader_1.h"
#include "UIWindows.h"
#include "UILayout.h"


using namespace eastl;
using namespace BohgeEngine;
namespace BGUI
{
	RTTI_TYPE_IMPLEMENT( UIWindows );
	//------------------------------------------------------------------------------
	UIWindows::UIWindows(void)
	{
	}
	//------------------------------------------------------------------------------
	UIWindows::~UIWindows(void)
	{
		Framework::Instance()->RemoveResizeviewListener( m_ReviewFunctionPtr );
	}
	//------------------------------------------------------------------------------
	void UIWindows::_Load_1( BohgeEngine::JsonReader& reader )
	{
		UIWindowsData_1 data;
		reader.Read( data );
		SetDesignRectangle( vector4d( 0, 0, data.m_Design.m_Size ) );
		SetActualRectangle( Framework::Instance()->GetConfig().GetViewSize() );//设计实际窗口尺寸
		for ( int i = 0 ; i < data.m_Source.size() ; ++ i )
		{
			UILayout* lay = NEW UILayout;
			lay->Load( data.m_Source[i] );
			m_UILayoutMap.insert( make_pair( Utility::HashCode( lay->GetName() ), lay ) );
		}
		for ( int i = 0 ; i < data.m_WindowsArray.Getsize() ; ++ i )
		{
			const Windows_1& win = data.m_WindowsArray.At<Windows_1>( i );
			vector2d pos = win.m_Position.m_Position;
			UILayoutMap::iterator lay = m_UILayoutMap.find( Utility::HashCode( win.m_WidgetName ) );
			if ( m_UILayoutMap.end() == lay )
			{
				DEBUGLOG( "Unkonw widget name %s\n", win.m_WidgetName.c_str() );
			}
			else
			{
				AttachNode( lay->second );
				lay->second->SetLocalPosition( pos );
			}
		}
	}
	//------------------------------------------------------------------------------
	void UIWindows::CreateResource( IAsyncSource* host )
	{
		SetResolution( Framework::Instance()->GetConfig().GetResolution() );
		FOR_EACH( UILayoutMap, lay, m_UILayoutMap )
		{
			lay->second->CreateResource( host );
		}
		m_ReviewFunctionPtr = Framework::Instance()->RegistResizeviewListener( MEMBER_FUNC_PTR( &UIWindows::OnResizeview ) );
	}
	//------------------------------------------------------------------------------
	void UIWindows::OnResizeview( const BohgeEngine::ViewResolution& vr )
	{
		SetActualRectangle( vr.m_NewViewSize );
		SetResolution( vr.m_NewResolution );
		FOR_EACH( UILayoutMap, lay, m_UILayoutMap )
		{
			lay->second->OnResizeview( );
		}
	}
	//------------------------------------------------------------------------------
	bool UIWindows::OnActionEvent( const BohgeEngine::TouchInfo& touch )
	{
		FOR_EACH( UILayoutMap, lay, m_UILayoutMap )
		{
			if( lay->second->OnActionEvent( touch ) )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	void UIWindows::OnResourceLoaded()
	{
		FOR_EACH( UILayoutMap, lay, m_UILayoutMap )
		{
			lay->second->OnResourceLoaded( );
		}
	}
	//------------------------------------------------------------------------------
	UILayout* UIWindows::FindLayout( const eastl::string& name )
	{
		UILayoutMap::iterator it = m_UILayoutMap.find( Utility::HashCode( name ) );
		if ( m_UILayoutMap.end() != it )
		{
			return it->second;
		}
		return NULL;
	}

}