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
#include "IActionSystem.h"
#include "Framework.h"
#include "Log.h"




namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( IActionSystem );
	SYSTEM_SINGLETON_IMPLEMENT( IActionSystem );
	//------------------------------------------------------------------------------------------------------
	IActionSystem::IActionSystem( void )
		:m_Switch(0),
		m_pTouchEvent(NULL),
		m_pActionEvent(NULL),
		m_pTouchBuffArray(NULL),
		m_pActionBuffArray(NULL),
		m_isResponse(true)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//------------------------------------------------------------------------------------------------------
	IActionSystem::~IActionSystem( void )
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//------------------------------------------------------------------------------------------------------
	void IActionSystem::RegistEvent( Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &IActionSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &IActionSystem::OnFrameworkExit ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_BEGIN_UPDATE, MEMBER_FUNC_PTR( &IActionSystem::OnFrameworkBeginUpdata ) );
	}
	//------------------------------------------------------------------------------------------------------
	void IActionSystem::OnFrameworkEntry( float time, Framework& framework )
	{
		m_pTouchEvent = NEW InterceptDelegate1< bool, const TouchInfo& >;
		m_pActionEvent = NEW InterceptDelegate1< bool, ActionPropery::ActionType >;
		m_pTouchBuffArray = NEW InputVector[AC_SWAP_BUFF_SIZE];
		m_pActionBuffArray = NEW ActionVector[AC_SWAP_BUFF_SIZE];
		for ( int i = 0 ; i < AC_SWAP_BUFF_SIZE ; ++ i )
		{
			m_TouchCount[i] = 0;
			m_ActionCount[i] = 0;
			m_pTouchBuffArray[i].resize( AC_INPUT_ALLOC );
			m_pActionBuffArray[i].resize( AC_INPUT_ALLOC );
		}
	}
	//------------------------------------------------------------------------------------------------------
	void IActionSystem::OnFrameworkExit( float time, Framework& framework )
	{
		SAFE_DELETE( m_pTouchEvent );
		SAFE_DELETE( m_pActionEvent );
		SAFE_DELETE_ARRAY( m_pTouchBuffArray );
		SAFE_DELETE_ARRAY( m_pActionBuffArray );
	}
	//------------------------------------------------------------------------------------------------------
	void IActionSystem::OnFrameworkBeginUpdata( float time, Framework& framework )
	{
		{//处理按键输入
			for ( int i = 0 ; i < m_ActionCount[m_Switch] ; ++ i )
			{
				m_pActionEvent->Multicast( m_pActionBuffArray[m_Switch][i] );
			}
		}
		{//处理触摸或者鼠标等点输入
			for ( int i = 0 ; i < m_TouchCount[m_Switch] ; ++ i )
			{
				m_TouchInfo.Clear();
				for ( int j = 0 ; j < m_pTouchBuffArray[m_Switch][i].GetCount() ; ++ j )
				{
					const vector2f& point = m_pTouchBuffArray[m_Switch][j].GetPoint( j );
					ActionPropery::TouchType type = m_pTouchBuffArray[m_Switch][j].GetType();
					switch( type )
					{
					case ActionPropery::TT_PRESS:
						{
							m_TouchInfo.PushTouchPoint( type, m_LastPoint[j], point );
							m_LastPoint[j] = point;
						}break;
					case ActionPropery::TT_MOVE:
						{
							if( Math::Abs( m_LastPoint[j].m_x - point.m_x ) > 0.05
								|| Math::Abs( m_LastPoint[j].m_y - point.m_y ) > 0.05 )
							{
								m_TouchInfo.PushTouchPoint( type, m_LastPoint[j], point );
								m_LastPoint[j] = point;
							}
						}break;
					case ActionPropery::TT_RELEASE:
						{
							m_TouchInfo.PushTouchPoint( type, m_LastPoint[j], point );
							m_LastPoint[j] = vector2f(-2, -2);
						}break;
					}
				}
				if ( m_TouchInfo.GetPointCount() > 0 )
				{
					m_pTouchEvent->Multicast( m_TouchInfo );
				}
			}
		}
		m_TouchCount[m_Switch] = 0;
		m_ActionCount[m_Switch] = 0;
		m_Switch = !m_Switch;
	}
	//------------------------------------------------------------------------------------------------------
	void IActionSystem::_PushTouchIntoVector( const TouchAction& input )
	{
		if ( m_isResponse )
		{
			if( m_TouchCount[m_Switch] >= m_pTouchBuffArray[m_Switch].size() )
			{
				DEBUGLOG("More touch input buffer\n",1);
				m_pTouchBuffArray[m_Switch].resize( m_pTouchBuffArray[m_Switch].size() + AC_INPUT_ALLOC );
			}
			m_pTouchBuffArray[m_Switch][m_TouchCount[m_Switch]] = input;
			++ m_TouchCount[m_Switch];
		}
	}
	//------------------------------------------------------------------------------------------------------
	void IActionSystem::_PushActionIntoVector( ActionPropery::ActionType at )
	{
		if ( m_isResponse )
		{
			if( m_ActionCount[m_Switch] >= m_pActionBuffArray[m_Switch].size() )
			{
				DEBUGLOG("More action input buffer\n",1);
				m_pActionBuffArray[m_Switch].resize( m_pActionBuffArray[m_Switch].size() + AC_INPUT_ALLOC );
			}
			m_pActionBuffArray[m_Switch][m_ActionCount[m_Switch]] = at;
			++ m_ActionCount[m_Switch];
		}
	}
	//------------------------------------------------------------------------------------------------------
	void IActionSystem::SystemTouchInput( const TouchAction& ta )
	{
		_OnSystemTouchInput( ta );
	}
	//------------------------------------------------------------------------------------------------------
	void IActionSystem::SystemKeyboardInput( const KeyboardAction& ta )
	{
		_OnSystemKeyboardInput( ta );
	}

}
