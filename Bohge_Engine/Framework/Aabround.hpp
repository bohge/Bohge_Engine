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
#ifndef BOHGE_AABROUND
#define BOHGE_AABROUND
#else
#error( "Do not include this file in other files" )
#endif // BASEMATHDECLAR
#include "Object.hpp"

namespace BohgeEngine
{

	template<typename T>
	class Aabround : public Object
	{
		RTTI_TYPE_DECLEAR( Aabround<T> );
	private:
		vector2<T>			m_Center;//球中心
		T					m_Radius;//半径
		T					m_PowRadius;
	public:
		//-------------------------------------------------------------------------------------
		Aabround()
			:m_Radius(T(0)),
			m_PowRadius(T(0))
		{
		}
		//-------------------------------------------------------------------------------------
		Aabround(const vector2<T>& center, T r)
			:m_Center(center),
			m_Radius(r),
			m_PowRadius(r*r)
		{
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE Aabround<T>& operator = ( const Aabround<T>& r )
		{
			m_Center = r.m_Center;
			m_Radius = r.m_Radius;
			m_PowRadius = r.m_PowRadius;
			return *this;
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const vector2<T>& GetCenter() const
		{
			return m_Center;
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE T GetRadius() const
		{
			return m_Radius;
		}
		//-------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool Intersect( const vector2<T>& pos ) const//out_percentage是半径与点所在位置的比值
		{
			return Math::Aabround_Vector2_Intersect( *this, pos );
		}
	};
}