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



/////////////////////////////////////////
//			State Manage class		   //
/////////////////////////////////////////

#include "Predefine.h"
#include "Engine.h"
#include "StateManager.h"
#include "ActionManager.h"
#include "Log.h"
#include "UIManager.h"
#include "GameResourceList.h"
#include "StateInterface.h"
#include "StateMainMenu.h"
#include "StateLogo.h"

using namespace BohgeEngine;

namespace BohgeGame
{
	StateManager::StateManager ()
		:m_isSwitchState(true),	//游戏初始化，走第一个state
		m_isReplace(true),//替换
		m_ePreviousState(State_None),
		m_eNextState(State_Logo),
		m_isShowLoading(false),
		m_isLoadResouce(false),
		m_isJustSwitch(false),
		m_ClearOtherState(false)
	{
		m_pLoading = Engine::Instance().GetUIManager()->CreateUIGroup( UIFileName[ui_Loading] );
		m_pLoading->SetShow(false);
	}
	//-------------------------------------------------------------------
	StateManager::~StateManager ()
	{
		for (StateList::iterator it = m_lStateList.begin();
			it != m_lStateList.end();
			it++)
		{
			SAFE_DELETE(it->m_pStatePtr);
		}
	}
	//-------------------------------------------------------------------
	void StateManager::Update (Engine& engine, uint millisecond)
	{
		if( m_isJustSwitch )
		{
			m_isJustSwitch = false;
			millisecond = 17;
		}
		if( m_isLoadResouce )
		{
			m_isLoadResouce = false;
			m_isJustSwitch = true;
			m_lStateList.begin()->m_pStatePtr->LoadResource(engine);
			m_lStateList.begin()->m_pStatePtr->OnEnter(engine, m_ePreviousState );
			m_isShowLoading = false;
			m_pLoading->SetShow(false);
			engine.GetActionManager()->SetResponse(true);//开始保存按键buffer
		}
		if ( !m_lStateList.empty() )
		{
			m_lStateList.begin()->m_pStatePtr->Update(*this,engine, millisecond);
		}
		if( true == m_isSwitchState)
		{
			SwitchState(engine);
		}
	}
	//-------------------------------------------------------------------
	void StateManager::Exit(Engine& engine)
	{
		m_lStateList.begin()->m_pStatePtr->OnLeave(engine);//通知栈首的状态退出
		for (StateList::iterator it = m_lStateList.begin();
			it != m_lStateList.end();
			it++)
		{
			it->m_pStatePtr->ReleaseResource(engine);
			SAFE_DELETE( it->m_pStatePtr );
		}
		m_lStateList.clear();
		Engine::Instance().GetUIManager()->DeleteUIGroup( m_pLoading );
	}
	//-------------------------------------------------------------------
	void StateManager::SwitchState(Engine& engine)
	{
		engine.GetActionManager()->SetResponse(false);//状态切换停止保存案件buffer
		DEBUGLOG("State Change to %d!\n", static_cast<int>(m_eNextState));
		if( ! m_lStateList.empty() )
		{
			m_lStateList.begin()->m_pStatePtr->OnLeave(engine);//通知久的状态机需要推出了
			m_ePreviousState = m_lStateList.begin()->m_eStateName;
		}
		//新建新状态
		bool isExist = false;
		StateList::iterator it;
		if( !m_lStateList.empty() )
		{
			if ( m_eNextState != m_lStateList.begin()->m_eStateName )//切换自身被认为是重置
			{
				for(it = ++(m_lStateList.begin());
					it != m_lStateList.end();
					it++)
				{
					if( it->m_eStateName == m_eNextState )
					{
						isExist = true;
						break;
					}
				}
			}
		}
		//如果更换状态，删除旧的状态
		if( true == m_isReplace )
		{
			if( ! m_lStateList.empty() )
			{
				for ( StateList::iterator it = m_lStateList.begin();
					it != m_lStateList.end() ;
				 )
				{
					if( m_ClearOtherState )
					{
						if ( m_eNextState != it->m_eStateName )
						{
							it->m_pStatePtr->ReleaseResource(engine);
							SAFE_DELETE( it->m_pStatePtr );
							it = m_lStateList.erase( it );
						}
						else
						{
							it ++;
						}
					}
					else
					{
						if( m_ePreviousState == it->m_eStateName )//删除需要替换掉的状态机
						{
							it->m_pStatePtr->ReleaseResource(engine);
							SAFE_DELETE( it->m_pStatePtr );
							it = m_lStateList.erase( it );
							break;
						}
						it ++;
					}
				}
			}
		}
		//设置新的
		IGameState* state;
		if( isExist )
		{
			m_isLoadResouce = false; //状态如果在堆栈中这里不需要再次读取，所以为false
			state = it->m_pStatePtr;
			state->OnEnter(engine, m_ePreviousState );

			//一下执行从新放到栈首的操作
			StateInfo newInfo(state, it->m_eStateName );
			m_lStateList.erase(it);
			m_lStateList.push_front(newInfo);
		}
		else
		{
			switch(m_eNextState)
			{
			case State_None:
				{
					ASSERT(false);
				}break;
			case State_Logo:
				{
					state = NEW StateLogo();
				}break;
			case State_Main_Menu:
				{
					state = NEW StateMainMenu();
				}break;
			}
			StateInfo newInfo(state, m_eNextState);
			//新状态压住栈首
			//newState->LoadResource(engine);
			if( m_isShowLoading )
			{
				m_pLoading->SetShow(true);
				m_pLoading->TopMost();
				m_isLoadResouce = true;
			}
			else
			{
				m_isJustSwitch = true;
				state->LoadResource(engine);
				state->OnEnter(engine, m_ePreviousState );
			}
			m_lStateList.push_front(newInfo);
		}

		m_isSwitchState = false;
		m_isReplace = false;
		m_ClearOtherState = false;
		m_eNextState = State_None;
		engine.GetActionManager()->SetResponse(true);//回复
	}
}