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

#pragma once
#include "3DMath.h"

namespace BohgeEngine
{
	class UIAnimation
	{
		friend class UIGroup;
	private:
		bool						m_isLoop;
		bool						m_isLife;
		bool						m_isColorAnimation;
		bool						m_isPostionAnimation;
		bool						m_isScaleAnimation;
		bool						m_isFlashAnimation;
		int							m_Delay;//延迟，经过n号秒后开始
		int							m_Duration;//动画时长，毫秒
		int							m_TimeSpan;//经过时间
		float						m_AnimationPass;//动画经过的时间
		vector4f					m_clrFrom;
		vector4f					m_clrIncrease;
		vector2f					m_PosFrom;
		vector2f					m_PosIncrease;
		vector2f					m_ScaleFrom;
		vector2f					m_ScaleIncrease;
		uint						m_FlashTimeSpan;
	public:
		UIAnimation(int milisecond);
	public:
		void ColorAnimation(const Color& from, const Color& to);
		void PostionAnimation(const vector2f& from, const vector2f& to);
		void ScaleAnimation(const vector2f& from, const vector2f& to);
		void FlashAnimation(uint timespanmilli);
	public:
		BOHGE_FORCEINLINE void SetDelay(uint milisecond)
		{
			m_Delay = milisecond;
		}
		BOHGE_FORCEINLINE void Loop(bool l)
		{
			m_isLoop = l;
		}
	private:
		bool Update(uint milisecond);
		BOHGE_FORCEINLINE void Reset()
		{
			m_TimeSpan = 0;
		}
		BOHGE_FORCEINLINE Color GetColor()
		{
			float alpha = 1;
			vector4f temp(1,1,1,1);

			if( m_isFlashAnimation )
			{
				alpha = 1- ( static_cast<int>(m_TimeSpan / m_FlashTimeSpan) ) % 2;
			}
			if( m_isColorAnimation )
			{
				temp = m_clrFrom + m_clrIncrease * m_AnimationPass;
			}
			return Color(temp.m_x, temp.m_y, temp.m_z, temp.m_w * alpha );
		}
		BOHGE_FORCEINLINE vector2f GetPotstion()
		{
			return m_PosFrom + m_PosIncrease * m_AnimationPass;
		}
		BOHGE_FORCEINLINE vector2f GetScale()
		{
			return m_ScaleFrom + m_ScaleIncrease * m_AnimationPass;
		}

		BOHGE_FORCEINLINE bool isColorAnimation()
		{
			return m_isColorAnimation;
		}
		BOHGE_FORCEINLINE bool isPostionAnimation()
		{
			return m_isPostionAnimation;
		}
		BOHGE_FORCEINLINE bool isScaleAnimation()
		{
			return m_isScaleAnimation;
		}
		BOHGE_FORCEINLINE bool isFlashAnimation()
		{
			return m_isFlashAnimation;
		}
	};
}