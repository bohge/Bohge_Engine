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
#ifndef BOHGE_RECT
#define BOHGE_RECT
#else
#error( "Do not include this file in other files" )
#endif // BASEMATHDECLAR
#include "Object.hpp"


namespace BohgeEngine
{

	//结构
	//------rt
	//|		|
	//|		|
	//lb----|
	template<typename T>
	class Aabrect : public Object
	{
		RTTI_TYPE_DECLEAR( Aabrect<T> );
	private:
		bool				m_isIdentitat;//是否是初始值，没有改变的
		vector2<T>			m_Center;
		vector2<T>			m_LeftBottom;
		vector2<T>			m_RightTop;
	public:
		//-------------------------------------------------------------------------------------
		Aabrect():m_isIdentitat(true)
		{
		}
		//-------------------------------------------------------------------------------------
		Aabrect(const vector2<T>& lb, const vector2<T>& rt)
			:m_LeftBottom(lb),
			m_RightTop(rt),
			m_Center( (lb+rt) / T(2) ),
			m_isIdentitat(false)
		{
		}
		//-------------------------------------------------------------------------------------
		Aabrect(const Aabrect& input)
		{
			m_Center = input.m_Center;
			m_LeftBottom = input.m_LeftBottom;
			m_RightTop = input.m_RightTop;
			m_isIdentitat = input.m_isIdentitat;
		}
		//-------------------------------------------------------------------------------------
		~Aabrect(void)
		{
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const vector2<T>& LeftBottom() const
		{
			return m_LeftBottom;
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const vector2<T>& RightTop() const
		{
			return m_RightTop;
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE Aabrect<T>& operator = ( const Aabrect<T>& r )
		{
			m_Center = r.m_Center;
			m_LeftBottom = r.m_LeftBottom;
			m_RightTop = r.m_RightTop;
			m_isIdentitat = r.m_isIdentitat;
			return *this;
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void MoveDir(const vector2<T>& dir)
		{
			m_Center += dir;
			m_LeftBottom+=dir;
			m_RightTop+=dir;
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetCenter(const vector2<T>& center)
		{
			vector2<T> dir = center - m_Center;
			m_Center = center;
			m_LeftBottom+=dir;
			m_RightTop+=dir;
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void Contains( const Aabrect& aab )
		{
			if( m_isIdentitat )
			{
				m_isIdentitat = false;
				*this = aab;
			}
			else
			{
				if( m_LeftBottom.m_x > aab.m_LeftBottom.m_x )
				{
					m_LeftBottom.m_x = aab.m_LeftBottom.m_x;
				}
				if( m_LeftBottom.m_y > aab.m_LeftBottom.m_y )
				{
					m_LeftBottom.m_y = aab.m_LeftBottom.m_y;
				}

				if( m_RightTop.m_x < aab.m_RightTop.m_x )
				{
					m_RightTop.m_x = aab.m_RightTop.m_x;
				}
				if( m_RightTop.m_y < aab.m_RightTop.m_y )
				{
					m_RightTop.m_y = aab.m_RightTop.m_y;
				}
			}
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void Contains( const vector2<T>& point )
		{
			if( m_isIdentitat )
			{
				m_isIdentitat = false;
				m_Center = point;
				m_LeftBottom = point;
				m_RightTop = point;
			}
			else
			{
				if( m_LeftBottom.m_x > point.m_x )
				{
					m_LeftBottom.m_x = point.m_x;
				}
				if( m_LeftBottom.m_y > point.m_y )
				{
					m_LeftBottom.m_y = point.m_y;
				}

				if( m_RightTop.m_x < point.m_x )
				{
					m_RightTop.m_x = point.m_x;
				}
				if( m_RightTop.m_y < point.m_y )
				{
					m_RightTop.m_y = point.m_y;
				}
				m_Center = ( m_LeftBottom + m_RightTop ) / 2;
			}
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool Intersect( const Aabrect<T>& rect ) const
		{
			return Math::Aabrect_Aabrect_Intersect( *this, rect );
		}
	public:
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool Intersect( T x, T y ) const
		{
			return Math::Aabrect_Vector2_Intersect( *this, vector2<T>(x,y) );
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool Intersect(const vector2<T>& point) const
		{
			return Math::Aabrect_Vector2_Intersect( *this, point );
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE vector2<T> GetCenter()
		{
			return m_Center;
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE vector2<T> GetSize()
		{
			return m_RightTop - m_LeftBottom;
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE vector4<T> GetRect()
		{
			return vector4<T>( m_LeftBottom.m_x, m_RightTop.m_y, m_RightTop.m_x, m_LeftBottom.m_y );
		}
	};
}