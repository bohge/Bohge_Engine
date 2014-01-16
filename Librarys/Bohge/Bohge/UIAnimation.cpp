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



//////////////////////////////
//			UI动画		   //
//////////////////////////////

#include "UIAnimation.h"

namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	UIAnimation::UIAnimation(int milisecond)
		:m_isColorAnimation(false),
		m_isPostionAnimation(false),
		m_isScaleAnimation(false),
		m_isFlashAnimation(false),
		m_TimeSpan(0),
		m_Duration(milisecond),
		m_Delay(0),
		m_AnimationPass(0),
		m_FlashTimeSpan(0),
		m_isLoop(false),
		m_isLife(true)
	{
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void UIAnimation::ColorAnimation(const Color& from, const Color& to)
	{
		m_isColorAnimation = true;
		m_clrFrom = from;
		m_clrIncrease = ( vector4f(to) - m_clrFrom ) / static_cast<float>(m_Duration);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void UIAnimation::PostionAnimation(const vector2f& from, const vector2f& to)
	{
		m_isPostionAnimation = true;
		m_PosFrom = from;
		m_PosIncrease = ( to - m_PosFrom ) / static_cast<float>(m_Duration);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void UIAnimation::ScaleAnimation(const vector2f& from, const vector2f& to)
	{
		m_isScaleAnimation = true;
		m_ScaleFrom = from;
		m_ScaleIncrease = ( to - from ) / static_cast<float>(m_Duration);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void UIAnimation::FlashAnimation(uint timespanmilli)
	{
		m_FlashTimeSpan = timespanmilli;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	bool UIAnimation::Update(uint milisecond)
	{
		m_TimeSpan += milisecond;
		float time = static_cast<float>(Math::GreaterZero(m_TimeSpan - m_Delay));
		m_AnimationPass = time > m_Duration ? m_Duration : time;//防止过度绘制
		m_isLife = m_TimeSpan < m_Duration + m_Delay;
		if( !m_isLife && m_isLoop )//如果动画播完，并且是循环动画
		{
			m_TimeSpan = m_Delay;
			m_isLife = true;
		}
		return m_isLife;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
}