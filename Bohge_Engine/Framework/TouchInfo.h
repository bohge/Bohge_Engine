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
#include "ActionPropery.h"
#include "3DMath.h"



namespace BohgeEngine
{
	class BOHGE_FRAMEWORK TouchInfo : public Object
	{
		RTTI_TYPE_DECLEAR( TouchInfo );
	private:
		Rayf						m_TouchRay[ ActionPropery::AC_MAX_INPUT_TOUCH ];
		vector2f					m_TouchPoint[ ActionPropery::AC_MAX_INPUT_TOUCH ];
		vector2f					m_LastTouch[ ActionPropery::AC_MAX_INPUT_TOUCH ];
		vector2f					m_DeltaTouch[ ActionPropery::AC_MAX_INPUT_TOUCH ];
		ActionPropery::TouchType	m_TouchType[ ActionPropery::AC_MAX_INPUT_TOUCH ];
		uint						m_PointCount;
	public:
		TouchInfo(void);
		virtual ~TouchInfo(void);
	public:
		void PushTouchPoint( ActionPropery::TouchType type, const vector2f& lp, const vector2f& np );//lp上一帧的点，np当前帧点
	public:
		BOHGE_FORCEINLINE void Clear()
		{
			m_PointCount = 0;
		}
		BOHGE_FORCEINLINE uint GetPointCount() const
		{
			return m_PointCount;
		}
		BOHGE_FORCEINLINE const vector2f& GetTouchPoint( uint index ) const
		{
			return m_TouchPoint[ index ];
		}
		BOHGE_FORCEINLINE const vector2f& GetLastTouchPoint( uint index ) const
		{
			return m_LastTouch[ index ];
		}
		BOHGE_FORCEINLINE const vector2f& GetDeltaPoint( uint index ) const
		{
			return m_DeltaTouch[ index ];
		}
		BOHGE_FORCEINLINE ActionPropery::TouchType GetTouchType( uint index ) const
		{
			return m_TouchType[ index ];
		}
		BOHGE_FORCEINLINE float GetDistance( uint index1, uint index2 ) const
		{
			return ( m_TouchPoint[index1] - m_TouchPoint[index2] ).Length();
		}
	};
}