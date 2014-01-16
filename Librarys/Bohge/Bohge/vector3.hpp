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



//////////////////////////////////////////////
//	3DMath的拆分实现，请不要单独引用该头文件	//
//////////////////////////////////////////////
#ifndef VECTOR3
#define VECTOR3
#else
#error( "Do not include this file in other files" )
#endif

namespace BohgeEngine
{
	template<typename T>
	class vector3
	{
	public:
		typedef T value_type;
	public:
		union
		{
			struct	{ T	m_x, m_y, m_z; };
			T		data[3];
		};
	public:
		BOHGE_FORCEINLINE vector3()
		{
			data[0] = 0;
			data[1] = 0;
			data[2] = 0;
		}
		BOHGE_FORCEINLINE vector3(T x, T y, T z)
		{
			data[0] = x;
			data[1] = y;
			data[2] = z;
		}
		BOHGE_FORCEINLINE vector3(const vector3& input)
		{
			data[0] = input.data[0];
			data[1] = input.data[1];
			data[2] = input.data[2];
		}
		explicit BOHGE_FORCEINLINE vector3(const vector4<T>& input)
		{
			if ( Math::isEqual( input.m_w, T(1) ) )
			{
				data[0] = input.data[0];
				data[1] = input.data[1];
				data[2] = input.data[2];
			}
			else
			{
				T invW = T(1) / input.m_w;
				data[0] = input.data[0]*invW;
				data[1] = input.data[1]*invW;
				data[2] = input.data[2]*invW;
			}
		}
	private:
		/*此处有可能传进来的是自己，所以最后复制，应为如果提前复制会改变他的计算结果，重要！！！！*/
		BOHGE_FORCEINLINE void _Multiplication(const Matrix33<T>& m33, T& x, T& y, T& z) const
		{
			T newx = m_x * m33.a11 + m_y * m33.a21 + m_z *m33.a31;
			T newy = m_x * m33.a12 + m_y * m33.a22 + m_z *m33.a32;
			T newz = m_x * m33.a13 + m_y * m33.a23 + m_z *m33.a33;
			x = newx;
			y = newy;
			z = newz;
		}
	public:
		BOHGE_FORCEINLINE T Length() const
		{
			return Math::Sqrt( m_x*m_x + m_y*m_y + m_z*m_z );
		}
		BOHGE_FORCEINLINE T LengthPow() const
		{
			return m_x*m_x + m_y*m_y + m_z*m_z;
		}
		BOHGE_FORCEINLINE vector3& NormalizeSelf()
		{
			Math::Normalize( *this, *this );
			return *this;
		}
		BOHGE_FORCEINLINE vector3 Cross( const vector3& rhs ) const
		{
			return Math::Cross<T>( *this, rhs );
		}
		BOHGE_FORCEINLINE T Dot( const vector3& rhs ) const
		{
			return Math::Dot( *this, rhs );
		}
		BOHGE_FORCEINLINE vector3 operator- ( ) const
		{
			return vector3( -m_x, -m_y, -m_z );
		}
		BOHGE_FORCEINLINE vector3& operator= (const vector3& input)
		{
			data[0] = input.data[0];
			data[1] = input.data[1];
			data[2] = input.data[2];
			return *this;
		}
		BOHGE_FORCEINLINE vector3& operator= (const vector4<T>& input)
		{
			if ( Math::isEqual( input.m_w, T(1) ) )
			{
				m_x = input.m_x;
				m_y = input.m_y;
				m_z = input.m_z;
			}
			else
			{
				T invW = T(1) / input.m_w;
				m_x = input.m_x*invW;
				m_y = input.m_y*invW;
				m_z = input.m_z*invW;
			}
			return *this;
		}
		// vector3
		BOHGE_FORCEINLINE vector3& operator+= (const vector3& v3 )
		{
			m_x += v3.m_x;
			m_y += v3.m_y;
			m_z += v3.m_z;
			return *this;
		}
		BOHGE_FORCEINLINE vector3& operator-= (const vector3& v3 )
		{
			m_x -= v3.m_x;
			m_y -= v3.m_y;
			m_z -= v3.m_z;
			return *this;
		}
		BOHGE_FORCEINLINE vector3& operator*= (const vector3& v3 )
		{
			m_x *= v3.m_x;
			m_y *= v3.m_y;
			m_z *= v3.m_z;
			return *this;
		}
		BOHGE_FORCEINLINE vector3& operator/= (const vector3& v3 )
		{
			m_x /= v3.m_x;
			m_y /= v3.m_y;
			m_z /= v3.m_z;
			return *this;
		}
		BOHGE_FORCEINLINE vector3 operator- (const vector3& v3 ) const
		{
			return vector3(data[0] - v3.data[0] , data[1] - v3.data[1] , data[2] - v3.data[2] );
		}
		BOHGE_FORCEINLINE vector3 operator+ (const vector3& v3 ) const
		{
			return vector3(data[0] + v3.data[0] , data[1] + v3.data[1] , data[2] + v3.data[2] );
		}
		BOHGE_FORCEINLINE vector3 operator/ ( const vector3& v3 ) const
		{
			return vector3( data[0] / v3.data[0] , data[1] / v3.data[1] , data[2] / v3.data[2] );
		}
		BOHGE_FORCEINLINE vector3 operator* (const vector3& v3 ) const
		{
			return vector3( data[0] * v3.m_x, data[1] * v3.m_y, data[2] * v3.m_z );
		}
		//数字
		BOHGE_FORCEINLINE vector3& operator*= (const T num )
		{
			m_x *= num;
			m_y *= num;
			m_z *= num;
			return *this;
		}
		BOHGE_FORCEINLINE vector3& operator/= (const T num )
		{
			T inv = T(1) / num;
			m_x *= inv;
			m_y *= inv;
			m_z *= inv;
			return *this;
		}
		BOHGE_FORCEINLINE vector3& operator+= (const T num )
		{
			m_x += num;
			m_y += num;
			m_z += num;
			return *this;
		}
		BOHGE_FORCEINLINE vector3& operator-= (const T num )
		{
			m_x -= num;
			m_y -= num;
			m_z -= num;
			return *this;
		}
		BOHGE_FORCEINLINE vector3 operator/ (const T num ) const
		{
			T inv = T(1) / num;
			return vector3( data[0] * inv, data[1] * inv, data[2] * inv );
		}
		BOHGE_FORCEINLINE vector3 operator* (const T num ) const
		{
			return vector3( data[0] * num, data[1] * num, data[2] * num );
		}
		BOHGE_FORCEINLINE vector3 operator- (const T num ) const
		{
			return vector3( data[0] - num, data[1] - num, data[2] - num );
		}
		BOHGE_FORCEINLINE vector3 operator+ (const T num ) const
		{
			return vector3( data[0] + num, data[1] + num, data[2] + num );
		}
		//矩阵，四元数
		BOHGE_FORCEINLINE vector3& operator*= (const Matrix44<T>& m44 )
		{
			vector4<T> local( *this );
			local *= m44;
			T inv = T(1)/local.m_w;
			m_x = local.m_x * inv;
			m_y = local.m_y * inv;
			m_z = local.m_z * inv;
			return *this;
		}
		BOHGE_FORCEINLINE vector3 operator * (const Matrix44<T>& m44 ) const
		{
			vector4<T> local( *this );
			local *= m44;
			T inv = T(1)/local.m_w;
			return vector3(
				local.m_x * inv,
				local.m_y * inv,
				local.m_z * inv);
		}
		BOHGE_FORCEINLINE vector3& operator*= (const Matrix33<T>& m33 )
		{
			_Multiplication(m33, m_x, m_y, m_z);
			return *this;
		}
		BOHGE_FORCEINLINE vector3 operator * (const Matrix33<T>& m33 ) const
		{
			T x,y,z;
			_Multiplication(m33, x, y, z);
			return vector3(x,y,z);
		}
		BOHGE_FORCEINLINE vector3 operator* (const Quaternion<T>& rhs ) const
		{
			return Math::Transform_Quat( *this, rhs );
		}
		BOHGE_FORCEINLINE vector3& operator*= (const Quaternion<T>& rhs )
		{
			*this = Math::Transform_Quat( *this, rhs );
			return *this;
		}
		//逻辑
		BOHGE_FORCEINLINE bool operator == (const vector3& rhs ) const
		{
			return ( Math::isEqual( m_x, rhs.m_x) ) & ( Math::isEqual( m_y, rhs.m_y) ) & ( Math::isEqual( m_z, rhs.m_z) );
		}
		BOHGE_FORCEINLINE bool operator != (const vector3& rhs ) const
		{
			return !( *this == rhs );
		}
		//取值
		BOHGE_FORCEINLINE T& operator[] ( int i )
		{
			return data[i];
		}
	public:
		BOHGE_FORCEINLINE const T* GetVectorArray() const
		{
			return data;
		}
		static uint Size()
		{
			return 3;
		}
		static uint BitSize()
		{
			return 3 * sizeof(T);
		}
		static vector3 Zero()
		{
			return vector3(T(0),T(0),T(0));
		}
	};
}