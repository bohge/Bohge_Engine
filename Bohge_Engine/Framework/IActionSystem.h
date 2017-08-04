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
#pragma once
#include "SystemSingleton.h"
#include "BinaryIndex.hpp"
#include "TouchAction.h"
#include "Delegate.hpp"
#include "TouchInfo.h"
#include "IPlugin.h"


#include <EASTL/vector.h>

namespace BohgeEngine
{
	class KeyboardAction;
	class BOHGE_FRAMEWORK IActionSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IActionSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IActionSystem, IPlugin );
	private:
		enum Constant
		{
			AC_INPUT_ALLOC = 10,//初始分配的可输入的大小
			AC_SWAP_BUFF_SIZE = 2,//互换buff的尺寸
		};
	private:
		typedef eastl::vector< TouchAction > InputVector;
		typedef eastl::vector< ActionPropery::ActionType > ActionVector;
	private:
		TouchInfo		m_TouchInfo;
		InterceptDelegate1< bool, const TouchInfo& >*	m_pTouchEvent;//触摸事件
		InterceptDelegate1< bool, ActionPropery::ActionType >*	m_pActionEvent;//其他按键事件
		vector2f		m_LastPoint[ActionPropery::AC_MAX_INPUT_TOUCH];
		InputVector*	m_pTouchBuffArray;
		ActionVector*	m_pActionBuffArray;
		uint			m_TouchCount[AC_SWAP_BUFF_SIZE];//输入数量的计数
		uint			m_ActionCount[AC_SWAP_BUFF_SIZE];
		uint			m_Switch;//两个链表交替使用
		bool			m_isResponse;
	public:
		IActionSystem(void);
		virtual ~IActionSystem(void);
	private:
		virtual void _OnSystemTouchInput( const TouchAction& ta ) = 0;
		virtual void _OnSystemKeyboardInput( const KeyboardAction& ta ) = 0;
	public:
		virtual void RegistEvent( Framework& framework ); //创建系统
		virtual void RegistKeyMapping( char key, ActionPropery::ActionType at ) = 0;
	public:
		void OnFrameworkEntry( float time, Framework& framework );
		void OnFrameworkExit( float time, Framework& framework );
		void OnFrameworkBeginUpdata( float time, Framework& framework );
	protected:
		void _PushTouchIntoVector( const TouchAction& input );
		void _PushActionIntoVector( ActionPropery::ActionType at );
	public:
		void SystemTouchInput( const TouchAction& ta );
		void SystemKeyboardInput( const KeyboardAction& ta );
	public:
		BOHGE_FORCEINLINE void RegistTouchListener( InterceptDelegate1< bool, const TouchInfo& >::FunctionType* const func )
		{
			m_pTouchEvent->Connect( func );
		}
		BOHGE_FORCEINLINE void RegistActionListener( InterceptDelegate1< bool, ActionPropery::ActionType >::FunctionType* const func )
		{
			m_pActionEvent->Connect( func );
		}
	};
}
