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

#pragma once

#include "Predefine.h"
#include "VariableType.h"

#include <list>


namespace BohgeEngine
{
	class Engine;
	class UIGroup;
}

namespace BohgeGame
{
	class IGameState;
	class StateManager
	{
		friend class Game;
	public:
		enum GameState
		{
			State_None,
			State_Logo,
			State_Main_Menu,
		};
	private:
		struct StateInfo
		{
		public:
			IGameState*				m_pStatePtr;//状态机指针
			GameState				m_eStateName;//状态机名称
			StateInfo(IGameState* ptr, GameState state)
				:m_pStatePtr(ptr),
				m_eStateName(state)

			{
			}
		};
		typedef std::list<StateInfo>			StateList;//游戏的状态机链表
	private:
		bool					m_isSwitchState;
		bool					m_isJustSwitch;
		bool					m_isLoadResouce;//更换中
		bool					m_isReplace;//直接更换还是压栈
		bool					m_isShowLoading;
		bool					m_ClearOtherState;//是否在进入前清理到全部其他状态机
		GameState				m_ePreviousState;//上一个状态
		GameState				m_eNextState;//下一个状态
		StateList				m_lStateList;//游戏状态栈
		BohgeEngine::UIGroup*	m_pLoading;
	private:
		StateManager();
		~StateManager();
	private:
		void SwitchState(BohgeEngine::Engine& engine);
		void Update(BohgeEngine::Engine& engine, uint millisecond);
		void Exit(BohgeEngine::Engine& engine);
	public:
		static StateManager& Instance()
		{
			static StateManager state;
			return state;
		}
		BOHGE_FORCEINLINE void ChangeState(GameState state, bool isReplace, bool isClearOther, bool isShowLoading)//是否被退出之前状态，是否清楚全部压栈的，是否显示loding界面
		{
			m_isShowLoading = isShowLoading;
			m_isSwitchState = true;
			m_eNextState = state;
			m_isReplace = isReplace;
			m_ClearOtherState = isClearOther;
		}
		BOHGE_FORCEINLINE GameState CurrextState()
		{
			return m_lStateList.begin()->m_eStateName;
		}
		BOHGE_FORCEINLINE IGameState* CurrentStatePtr()
		{
			return m_lStateList.begin()->m_pStatePtr;
		}
	};
}