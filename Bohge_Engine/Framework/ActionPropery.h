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
#include "IPlugin.h"


namespace BohgeEngine
{
	namespace ActionPropery
	{
		enum Constant
		{
			AC_MAX_INPUT_TOUCH = 5,
		};
		enum TouchType//触摸，鼠标输入
		{
			TT_PRESS,//按下
			TT_MOVE,//按下移动
			TT_RELEASE,//离开
		};
		enum KeyboardState
		{
			KS_LEFT_SHIFT	= 1 << 0UL,
			KS_LEFT_CTRL	= 1 << 1UL,
			KS_LEFT_ALT		= 1 << 2UL,
			KS_RIGHT_SHIFT	= 1 << 3UL,
			KS_RIGHT_CTRL	= 1 << 4UL,
			KS_RIGHT_ALT	= 1 << 5UL,
		};

		enum ActionType//游戏中的动作类型
		{
			AT_INVALID = -1,
			AT_LEFT = 0,
			AT_RIGHT,
			AT_UP,
			AT_DOWN,
			AT_OK,
			AT_CANCEL,

			AT_USER_0,
			AT_USER_1,
			AT_USER_2,
			AT_USER_3,
			AT_USER_4,
			AT_USER_5,
			AT_USER_6,
			AT_USER_7,
			AT_USER_8,
			AT_USER_9,
			AT_USER_10,
			AT_USER_11,
			AT_USER_12,
			AT_USER_13,
			AT_USER_14,
			AT_USER_15,
			AT_USER_16,
			AT_USER_17,
			AT_USER_18,
			AT_USER_19,
			AT_USER_20,
			AT_USER_21,
			AT_USER_22,
			AT_USER_23,
			AT_USER_24,
			AT_USER_26,
			AT_USER_27,
			AT_USER_28,
			AT_USER_29,
			AT_USER_30,
			AT_USER_31,
			AT_USER_32,
			AT_USER_33,
			AT_USER_34,
			AT_USER_35,
			AT_USER_36,
			AT_USER_37,
			AT_USER_38,
			AT_USER_39,
		};
	}
}
