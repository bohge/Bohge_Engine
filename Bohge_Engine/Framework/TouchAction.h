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
	class BOHGE_FRAMEWORK TouchAction : public Object
	{
		RTTI_TYPE_DECLEAR( TouchAction );
	private:
		typedef eastl::vector<vector2f>		PointVector;
	private:
		ActionPropery::TouchType		m_eType;
		int				m_Count;
		PointVector		m_Points;
	public:
		TouchAction(void);
		virtual ~TouchAction(void);
	public:
		BOHGE_FORCEINLINE void Begin()
		{
			m_Count = 0;
		}
		BOHGE_FORCEINLINE void SetType( ActionPropery::TouchType t )
		{
			m_eType = t;
		}
		BOHGE_FORCEINLINE bool SetPoint( const vector2f& point )
		{
			if( m_Count < ActionPropery::AC_MAX_INPUT_TOUCH )
			{
				m_Points[m_Count] = point;
				m_Count ++;
				return true;
			}
			return false;
		}
		BOHGE_FORCEINLINE ActionPropery::TouchType GetType() const
		{
			return m_eType;
		}
		BOHGE_FORCEINLINE int GetCount() const
		{
			return m_Count;
		}
		BOHGE_FORCEINLINE const vector2f& GetPoint( int index ) const
		{
			ASSERT( index < ActionPropery::AC_MAX_INPUT_TOUCH );
			return m_Points[index];
		}
	};
}