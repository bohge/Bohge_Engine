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
#ifndef QUATERNION
#define QUATERNION
#else
#error( "Do not include this file in other files" )
#endif
#include "Volume.hpp"



namespace BohgeEngine
{
	template<typename T>
	class Quaternion : public Volume
	{
		RTTI_TYPE_DECLEAR( Quaternion )
	public:
		typedef T value_type;
	public:
		union
		{
			struct	{ T	m_x, m_y, m_z, m_w; };
			T	m_Data[4];
		};
	public:
		BOHGE_FORCEINLINE T& x()
		{
			return m_x;
		}
		BOHGE_FORCEINLINE T& y()
		{
			return m_y;
		}
		BOHGE_FORCEINLINE T& z()
		{
			return m_z;
		}
		BOHGE_FORCEINLINE T& w()
		{
			return m_w;
		}	
	private:
		BOHGE_FORCEINLINE Quaternion mul(Quaternion const & lhs, Quaternion const & rhs) const
		{
			return Quaternion(
				lhs.m_x * rhs.m_w - lhs.m_y * rhs.m_z + lhs.m_z * rhs.m_y + lhs.m_w * rhs.m_x,
				lhs.m_x * rhs.m_z + lhs.m_y * rhs.m_w - lhs.m_z * rhs.m_x + lhs.m_w * rhs.m_y,
				lhs.m_y * rhs.m_x - lhs.m_x * rhs.m_y + lhs.m_z * rhs.m_w + lhs.m_w * rhs.m_z,
				lhs.m_w * rhs.m_w - lhs.m_x * rhs.m_x - lhs.m_y * rhs.m_y - lhs.m_z * rhs.m_z);
			//return Quaternion(
			//	lhs.m_w * rhs.m_x + lhs.m_x * rhs.m_w + lhs.m_y * rhs.m_z - lhs.m_z * rhs.m_y,
			//	lhs.m_w * rhs.m_y + lhs.m_y * rhs.m_w + lhs.m_z * rhs.m_x - lhs.m_x * rhs.m_z,
			//	lhs.m_w * rhs.m_z + lhs.m_z * rhs.m_w + lhs.m_x * rhs.m_y - lhs.m_y * rhs.m_x,
			//	lhs.m_w * rhs.m_w - lhs.m_w * rhs.m_x - lhs.m_y * rhs.m_y - lhs.m_z * rhs.m_z);
		}
	public:
		BOHGE_FORCEINLINE Quaternion()
			: m_x(T(0)),
			m_y(T(0)),
			m_z(T(0)),
			m_w(T(1))
		{
		}
		BOHGE_FORCEINLINE Quaternion(const T& x, const T& y, const T& z, const T& w)
			: m_x(x),
			m_y(y),
			m_z(z),
			m_w(w)
		{
		}
		BOHGE_FORCEINLINE Quaternion(const vector3<T>& vec, const T& s)
		{
			this->x() = vec.m_x;
			this->y() = vec.m_y;
			this->z() = vec.m_z;
			this->w() = s;
		}
		BOHGE_FORCEINLINE void Identity()
		{
			this->x() = 0;
			this->y() = 0;
			this->z() = 0;
			this->w() = 1;
		}
		BOHGE_FORCEINLINE Quaternion operator- () const
		{
			return Quaternion( -m_x, -m_y, -m_z, -m_w );
		}
		//与四元数
		BOHGE_FORCEINLINE Quaternion operator+ (const Quaternion& input ) const
		{
			return Quaternion(m_x + input.m_x, m_y + input.m_y, m_z + input.m_z, m_w + input.m_w );
		}
		BOHGE_FORCEINLINE Quaternion operator- (const Quaternion& input ) const
		{
			return Quaternion(m_x - input.m_x, m_y - input.m_y, m_z - input.m_z, m_w - input.m_w );
		}
		BOHGE_FORCEINLINE Quaternion operator* ( const Quaternion& input ) const
		{
			return mul(*this, input);
		}
		BOHGE_FORCEINLINE Quaternion& operator+= (const Quaternion& input )
		{
			m_x += input.m_x;
			m_y += input.m_y;
			m_z += input.m_z;
			m_w += input.m_w;
			return *this;
		}
		BOHGE_FORCEINLINE Quaternion& operator-= (const Quaternion& input )
		{
			m_x -= input.m_x;
			m_y -= input.m_y;
			m_z -= input.m_z;
			m_w -= input.m_w;
			return *this;
		}
		BOHGE_FORCEINLINE Quaternion& operator*= ( const Quaternion& input )
		{
			*this = mul(*this, input);
			return *this;
		}
		//单个数字
		friend BOHGE_FORCEINLINE Quaternion<T> operator* ( T rhs, const Quaternion<T>& lhs )
		{
			return Quaternion<T>( lhs.m_x*rhs, lhs.m_y*rhs, lhs.m_z*rhs, lhs.m_w*rhs );
		}
		friend BOHGE_FORCEINLINE Quaternion<T> operator* ( const Quaternion<T>& lhs, T rhs )
		{
			return Quaternion<T>( lhs.m_x*rhs, lhs.m_y*rhs, lhs.m_z*rhs, lhs.m_w*rhs );
		}
		BOHGE_FORCEINLINE Quaternion operator/ ( T rhs ) const
		{
			T inv = T(1) / rhs;
			return Quaternion( m_x*inv, m_y*inv, m_z*inv, m_w*inv );
		}
		BOHGE_FORCEINLINE Quaternion& operator*= ( T rhs )
		{
			m_x *= rhs;
			m_y *= rhs;
			m_z *= rhs;
			m_w *= rhs;
			return *this;
		}
		BOHGE_FORCEINLINE Quaternion& operator/= ( T rhs )
		{
			T inv = T(1) / rhs;
			m_x *= inv;
			m_y *= inv;
			m_z *= inv;
			m_w *= inv;
			return *this;
		}
		//逻辑
		BOHGE_FORCEINLINE bool operator== (const Quaternion<T>& rhs ) const
		{
			if ( m_x == rhs.m_x
				&& m_y == rhs.m_y
				&& m_z == rhs.m_z
				&& m_w == rhs.m_w
				)
			{
				return true;
			}
			return false;
		}
		BOHGE_FORCEINLINE bool operator!= (const Quaternion<T>& rhs ) const
		{
			return !this->operator ==( rhs );
		}
	public:
		BOHGE_FORCEINLINE vector3<T> Vector() const
		{
			return vector3<T>(m_x, m_y, m_z);
		}
		BOHGE_FORCEINLINE const Quaternion<T>& NormalizeSelf()
		{
			Math::Normalize( *this, *this );
			return *this;
		}
		BOHGE_FORCEINLINE const T Dot(  const Quaternion<T>& rhs ) const
		{
			return Math::Dot( *this, rhs );;
		}
		BOHGE_FORCEINLINE void YawPitchRoll( T yaw, T pitch, T roll)
		{
			T const angX(pitch * 0.5), angY(yaw * 0.5), angZ(roll * 0.5);
			//T sx, sy, sz;
			//T cx, cy, cz;
			//Math::SinCos(angX, sx, cx);
			//Math::SinCos(angY, sy, cy);
			//Math::SinCos(angZ, sz, cz);

			//*this = Quaternion(
			//	sx * cy * cz + cx * sy * sz,
			//	cx * sy * cz - sx * cy * sz,
			//	cx * cy * sz - sx * sy * cz,
			//	sx * sy * sz + cx * cy * cz);

			T scx[2], scy[2], scz[2];
			Math::SinCos( angX, scx );
			Math::SinCos( angY, scy );
			Math::SinCos( angZ, scz );
			*this = Quaternion(
				scx[0] * scy[1] * scz[1] + scx[1] * scy[0] * scz[0],
				scx[1] * scy[0] * scz[1] - scx[0] * scy[1] * scz[0],
				scx[1] * scy[1] * scz[0] - scx[0] * scy[0] * scz[1],
				scx[0] * scy[0] * scz[0] + scx[1] * scy[1] * scz[1]);
		}
		BOHGE_FORCEINLINE void RotateXYZ( T x, T y, T z)
		{
			YawPitchRoll( y, x, z);
		}
		BOHGE_FORCEINLINE void AxisToAxis(vector3<T> const & from, vector3<T> const & to)
		{
			vector3<T> a = from;
			vector3<T> b = to;
			a.NormalizeSelf();
			b.NormalizeSelf();
			T const cos_theta = a.Dot(b);
			if (Math::isEqual(cos_theta, T(1)))
			{
				*this = Quaternion<T>(0, 0, 0, 1);
				//return *this;
			}
			else
			{
				if (Math::isEqual(cos_theta, T(-1)))
				{
					*this = Quaternion<T>(1, 0, 0, 0);
					//return *this;
				}
				else
				{
					vector3<T> axis = a.Cross(b).NormalizeSelf();

					T const sin_theta = Math::Sqrt(1 - cos_theta * cos_theta);
					T const sin_half_theta = Math::Sqrt((1 - cos_theta) / 2);
					T const cos_half_theta = sin_theta / (2 * sin_half_theta);

					*this = Quaternion<T>(axis * sin_half_theta, cos_half_theta);
					//return *this;
				}
			}
		}
		BOHGE_FORCEINLINE void RotateAxis(const vector3<T>& axis, T r )
		{
			//T sa, ca;
			//Math::SinCos(r * T(0.5), sa, ca);
			T sc[2];
			Math::SinCos( r * T(0.5), sc );
			if ( Math::isEqual( axis.Length(), T(0.0) ) )
			{
				m_x = sc[0];
				m_y = sc[0];
				m_z = sc[0];
				m_w = sc[1];
			}
			else
			{
				vector3<T> temp = axis;
				temp.NormalizeSelf();
				temp *= sc[0];
				//*this = Quaternion<T>( temp * sc[0], sc[1] );
				m_x = temp.m_x;
				m_y = temp.m_y;
				m_z = temp.m_z;
				m_w = sc[1];
			}
		}
		BOHGE_FORCEINLINE T Length( ) const
		{
			return Math::Sqrt( Math::Dot( *this, *this ) );
		}
		BOHGE_FORCEINLINE T LengthPow() const
		{
			return Math::Dot( *this, *this );
		}
		BOHGE_FORCEINLINE void GetInverse( Quaternion<T>& out ) const
		{
			T const inv(T(1) /  this->Length() );
			out.m_x = -this->m_x * inv;
			out.m_y = -this->m_y * inv;
			out.m_z = -this->m_z * inv;
			out.m_w =  this->m_w * inv;
		}
		BOHGE_FORCEINLINE void InverseSelf()
		{
			GetInverse( *this );
		}
		BOHGE_FORCEINLINE Matrix44<T> ToMatrix44 ( ) const
		{
			return Math::Quaternion_to_Matrix44( *this );
		}
		BOHGE_FORCEINLINE Matrix33<T> ToMatrix33 ( ) const
		{
			return Math::Quaternion_to_Matrix33( *this );
		}
		BOHGE_FORCEINLINE T& operator[] ( int i )
		{
			return m_Data[i];
		}
	public:
		virtual void* Data()
		{
			return m_Data;
		}
		virtual const void* Data() const
		{
			return m_Data;
		}
		virtual uint Count() const
		{
			return 4;
		}
		static uint Size()
		{
			return 4;
		}
		static uint BitSize()
		{
			return 4 * sizeof(T);
		}
	};
}