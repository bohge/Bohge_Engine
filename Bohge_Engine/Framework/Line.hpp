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
#ifndef BOHGE_LINE
#define BOHGE_LINE
#else
#error( "Do not include this file in other files" )
#endif // BASEMATHDECLAR
#include "Object.hpp"



namespace BohgeEngine
{

	template <class T>
	class Line : public Object
	{
		RTTI_TYPE_DECLEAR( Line<T> );
	private:
		vector3<T>		m_RayOrig;
		vector3<T>		m_RayDir;
		T				m_Lenght;
	public:
		Line(){}
		Line(const vector3<T>& orig, const vector3<T>& dir, const T& len)
			:m_RayOrig(orig),
			m_RayDir(dir),
			m_Lenght(len)
		{
		}
	public:
		BOHGE_FORCEINLINE void SetOrigin( const vector3<T>& ori )
		{
			m_RayOrig = ori;
		}
		BOHGE_FORCEINLINE void SetDirection( const vector3<T>& dir )
		{
			m_RayDir = dir;
		}
		BOHGE_FORCEINLINE void SetLength( const T& len )
		{
			m_Lenght = len;
		}
		BOHGE_FORCEINLINE const vector3<T>& GetOrigin() const
		{
			return m_RayOrig;
		}
		BOHGE_FORCEINLINE const vector3<T>& GetDirection() const
		{
			return m_RayDir;
		}
		BOHGE_FORCEINLINE const T& GetLength() const
		{
			return m_Lenght;
		}
		BOHGE_FORCEINLINE bool Intersect( const Aabbox3d<T>& box ) const
		{
			return Math::Aabbox3d_Line_Intersect( box, *this );
		}
	};
}