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



/////////////////////////////
//			Plane		   //
/////////////////////////////
#ifndef BOHGE_PLANE
#define BOHGE_PLANE
#else
#error( "Do not include this file in other files" )
#endif // BASEMATHDECLAR
#include "Object.hpp"


namespace BohgeEngine
{
	template<typename T>
	class Plane : public Object//平面
	{
		RTTI_TYPE_DECLEAR( Plane<T> );
	private:
		union
		{
			struct	{ T	m_a, m_b, m_c, m_d; };
			T	data[4];
		};
	public:
		Plane()
			: m_a(T(0)),
			m_b(T(0)),
			m_c(T(0)),
			m_d(T(1))
		{
		}
		Plane(const T& a, const T& b, const T& c, const T& d)
			: m_a(a),
			m_b(b),
			m_c(c),
			m_d(d)
		{
		}
		Plane(const Plane<T>& rhs)
			: m_a(rhs.m_a),
			m_b(rhs.m_b),
			m_c(rhs.m_c),
			m_d(rhs.m_d)
		{
		}
		Plane( const vector3<T>& pos, const vector3<T>& nor )
		{
			FromPointNormal(pos, nor);
		}
		Plane( const vector3<T>& p1, const vector3<T>& p2, const vector3<T>& p3 )
		{
			FromPoints(p1, p2, p3);
		}
		BOHGE_FORCEINLINE const T& A() const
		{
			return m_a;
		}
		BOHGE_FORCEINLINE const T& B() const
		{
			return m_b;
		}
		BOHGE_FORCEINLINE const T& C() const
		{
			return m_c;
		}
		BOHGE_FORCEINLINE const T& D() const
		{
			return m_d;
		}
		//经由点和法线计算平面方程
		BOHGE_FORCEINLINE void FromPointNormal( const vector3<T>& pos, const vector3<T>& nor )
		{
			m_a = nor.m_x;
			m_b = nor.m_y;
			m_c = nor.m_z;

			m_d = - pos.Dot( nor );
		}
		BOHGE_FORCEINLINE void FromPoints( const vector3<T>& p1, const vector3<T>& p2, const vector3<T>& p3 )
		{
			vector3<T> normal = ( p2 - p1 ).Cross( p3 - p1 );
			FromPointNormal( p1, normal );
		}
		//赋值
		BOHGE_FORCEINLINE Plane& operator=(Plane const & rhs)
		{
			m_a = rhs.m_a;
			m_b = rhs.m_b;
			m_c = rhs.m_c;
			m_d = rhs.m_d;
			return *this;
		}
		BOHGE_FORCEINLINE Plane& operator=(vector4<T> const & rhs)
		{
			m_a = rhs.m_x;
			m_b = rhs.m_y;
			m_c = rhs.m_z;
			m_d = rhs.m_w;
			return *this;
		}
		//一元
		BOHGE_FORCEINLINE Plane const operator-() const
		{ 
			return Plane<T>(-m_a, -m_b, -m_c, -m_d ); 
		}
		// 取法向向量
		BOHGE_FORCEINLINE vector3<T> const Normal() const
		{
			return vector3<T>( m_a, m_b, m_c ); 
		}
		//逻辑
		BOHGE_FORCEINLINE bool operator == (const Plane<T>& rhs ) const
		{
			return ( Math::isEqual( m_a, rhs.m_a) ) & ( Math::isEqual( m_b, rhs.m_b) ) & ( Math::isEqual( m_c, rhs.m_c) ) & ( Math::isEqual( m_d, rhs.m_d) );
		}
		BOHGE_FORCEINLINE vector3<T> IntersectPoint( const Ray<T>& r ) const
		{
			return Math::Plane_Ray_IntersectPoint( *this, r );
		}
		BOHGE_FORCEINLINE T DistanceTo( const vector3<T>& p ) const
		{
			T deno =  Math::Sqrt( m_a*m_a + m_b*m_b + m_c*m_c );
			if ( Math::isEqual( deno, T(0) ) )
			{
				deno = T(0.0001);
			}
			return Math::Abs( p.Dot( this->Normal() ) ) / deno;
		}
		BOHGE_FORCEINLINE T Dot( const vector3<T>& p ) const
		{
			return Math::Dot( *this, p );
		}
		BOHGE_FORCEINLINE void NormalizeSelf()
		{
			T const inv( Math::Invsqrt( m_a*m_a+m_b*m_b+m_c*m_c ) );
			m_a *= inv;
			m_b *= inv;
			m_c *= inv;
			m_d *= inv;
		}
		BOHGE_FORCEINLINE OnSide::Volume GetSide (const vector3<T>& centre, const vector3<T>& halfSize) const
		{
			// Calculate the distance between box centre and the plane
			T dist = m_a * centre.m_x + m_b * centre.m_y + m_c * centre.m_z + m_d;

			// Calculate the maximise allows absolute distance for
			// the distance between box centre and plane
			T maxAbsDist = Math::Abs(m_a * halfSize.m_x) + Math::Abs(m_b * halfSize.m_y) + Math::Abs(m_c * halfSize.m_z);;

			if (dist < -maxAbsDist)
				return OnSide::OS_NEGATIVE_SIDE;

			if (dist > +maxAbsDist)
				return OnSide::OS_POSITIVE_SIDE;

			return OnSide::OS_BOTH_SIDE;
		}
	};
}