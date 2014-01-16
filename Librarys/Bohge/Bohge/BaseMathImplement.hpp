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
#ifndef BASEMATHIMPLEMENT
#define BASEMATHIMPLEMENT
#else
#error( "Do not include this file in other files" )
#endif // BASEMATHIMPLEMENT



namespace BohgeEngine
{
	//采用neon，float特殊处理
	namespace Math
	{
		//-------------------------------------------------------------------------------------------------------
		template<typename T> 
		BOHGE_FORCEINLINE const T& GetMax( const T& a, const T& b )
		{
			return ( a > b) ? a : b;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> 
		BOHGE_FORCEINLINE const T& GetMax( const vector2<T>& v )
		{
			return GetMax( v.m_x, v.m_y );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> 
		BOHGE_FORCEINLINE const T& GetMax( const vector3<T>& v )
		{
			return GetMax( v.m_x, GetMax( v.m_y, v.m_z ) );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> 
		BOHGE_FORCEINLINE const T& GetMax( const vector4<T>& v )
		{
			return GetMax( v.m_x, GetMax( v.m_y, GetMax( v.m_z, v.m_w ) ) );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE const T& GetMin( const T& a, const T& b )
		{
			return ( a < b) ? a : b;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> 
		BOHGE_FORCEINLINE const T& GetMin( const vector2<T>& v )
		{
			return GetMin( v.m_x, v.m_y );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> 
		BOHGE_FORCEINLINE const T& GetMin( const vector3<T>& v )
		{
			return GetMin( v.m_x, GetMin( v.m_y, v.m_z ) );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> 
		BOHGE_FORCEINLINE const T& GetMin( const vector4<T>& v )
		{
			return GetMin( v.m_x, GetMin( v.m_y, GetMin( v.m_z, v.m_w ) ) );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> 
		BOHGE_FORCEINLINE vector3<T> Transform_Quat(const vector3<T>& v, const Quaternion<T>& quat)
		{
			T const a(quat.m_w * quat.m_w - quat.Vector().Dot( quat.Vector() ) );
			T const b(2 * quat.Vector().Dot( v ) );
			T const c(quat.m_w + quat.m_w);

			// Must store this, because result may alias v
			vector3<T> cross_v( quat.Vector().Cross(v) );		// q.v CROSS v

			return vector3<T>(a * v.m_x + b * quat.m_x + c * cross_v.m_x,
				a * v.m_y + b * quat.m_y + c * cross_v.m_y,
				a * v.m_z + b * quat.m_z + c * cross_v.m_z);
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE Quaternion<T> Conjugate(const Quaternion<T>& q)
		{
			return Quaternion<T>(-q.m_x, -q.m_y, -q.m_z, q.m_w);
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> Quaternion<T> MulRealQuaternion(const Quaternion<T>& rhs_real, const Quaternion<T>& lhs_real)
		{
			return rhs_real * lhs_real;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE Quaternion<T> MulDualQuaternion(Quaternion<T> const & lhs_real, Quaternion<T> const & lhs_dual, Quaternion<T> const & rhs_real, Quaternion<T> const & rhs_dual)
		{
			return lhs_real * rhs_dual + lhs_dual * rhs_real;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE Quaternion<T> YawPitchRoll(const T& yaw, const T& pitch, const T& roll)
		{
			T const angX(pitch * 0.5), angY(yaw * 0.5), angZ(roll * 0.5);
			//T sx, sy, sz;
			//T cx, cy, cz;
			//Math::SinCos(angX, sx, cx);
			//Math::SinCos(angY, sy, cy);
			//Math::SinCos(angZ, sz, cz);

			//return Quaternion<T>(
			//	sx * cy * cz + cx * sy * sz,
			//	cx * sy * cz - sx * cy * sz,
			//	cx * cy * sz - sx * sy * cz,
			//	sx * sy * sz + cx * cy * cz);

			T scx[2], scy[2], scz[2];
			Math::SinCos( angX, scx );
			Math::SinCos( angY, scy );
			Math::SinCos( angZ, scz );
			return Quaternion<T>(
				scx[0] * scy[1] * scz[1] + scx[1] * scy[0] * scz[0],
				scx[1] * scy[0] * scz[1] - scx[0] * scy[1] * scz[0],
				scx[1] * scy[1] * scz[0] - scx[0] * scy[0] * scz[1],
				scx[0] * scy[0] * scz[0] + scx[1] * scy[1] * scz[1]);
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE Quaternion<T> AxisToAxis(vector3<T> const & from, vector3<T> const & to)
		{
			vector3<T> a = from;
			vector3<T> b = to;
			a.NormalizeSelf();
			b.NormalizeSelf();
			T const cos_theta = a.Dot(b);
			if (Math::isEqual(cos_theta, T(1)))
			{
				return Quaternion<T>(0, 0, 0, 1);
				//return *this;
			}
			else
			{
				if (Math::isEqual(cos_theta, T(-1)))
				{
					return Quaternion<T>(1, 0, 0, 0);
					//return *this;
				}
				else
				{
					vector3<T> axis = Math::Cross( a, b );
					axis .NormalizeSelf();

					T const sin_theta = Math::Sqrt(1 - cos_theta * cos_theta);
					T const sin_half_theta = Math::Sqrt((1 - cos_theta) / 2);
					T const cos_half_theta = sin_theta / (2 * sin_half_theta);

					return Quaternion<T>(axis * sin_half_theta, cos_half_theta);
					//return *this;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE Quaternion<T> RotateAxis(const vector3<T>& axis, T r )
		{
			T sc[2];
			Math::SinCos( r * T(0.5), sc );
			if ( Math::isEqual( axis.Length(), T(0.0) ) )
			{
				return Quaternion<T>(sc[0], sc[0], sc[0], sc[1]);
			}
			else
			{
				vector3<T> temp = axis;
				temp.NormalizeSelf();
				return Quaternion<T>( temp * sc[0], sc[1]);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE std::pair<Quaternion<T>, Quaternion<T> > Inverse(const Quaternion<T>& real, const Quaternion<T>& dual)
		{
			T sqr_len_0 = real.Dot(real);
			T sqr_len_e = 2.0f * real.Dot(dual);
			T inv_sqr_len_0 = 1.0f / sqr_len_0;
			T inv_sqr_len_e = -sqr_len_e / (sqr_len_0 * sqr_len_0);
			Quaternion<T> conjR = Conjugate(real);
			Quaternion<T> conjD = Conjugate(dual);
			return std::make_pair( conjR * inv_sqr_len_0, conjD * inv_sqr_len_0 + conjR * inv_sqr_len_e);
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE void DualQuaternionToScrew(T& angle, T& pitch, vector3<T>& dir, vector3<T>& moment,
			Quaternion<T> const & real, Quaternion<T> const & dual)
		{
			if (abs(real.m_w) >= 1)
			{
				// pure translation

				angle = 0;
				dir = dual.Vector();

				T dir_sq_len = dir.Length();

				if (dir_sq_len > T(1e-6))
				{
					T dir_len = Math::Sqrt(dir_sq_len);
					pitch = 2 * dir_len;
					dir /= dir_len;
				}
				else
				{
					pitch = 0;
				}

				moment = vector3<T>::Zero();
			}
			else
			{ 
				angle = 2 * Math::ACos(real.m_w);

				T s = real.Vector().Length();
				if (s < T(1e-6))
				{
					dir = vector3<T>::Zero();
					pitch = 0;
					moment = vector3<T>::Zero();
				}
				else
				{
					T oos = Recip_sqrt(s);
					dir = real.Vector() * oos;

					pitch = -2 * dual.m_w * oos;

					moment = (dual.Vector() - dir * pitch * real.m_w * T(0.5)) * oos;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE std::pair<Quaternion<T>, Quaternion<T> > ScrewToDualQuaternion(T const & angle, T const & pitch, vector3<T> const & dir, vector3<T> const & moment)
		{
			//T sa, ca;
			//SinCos(angle * T(0.5), sa, ca);
			T sc[2];
			Math::SinCos( angle * T(0.5), sc );
			return std::make_pair(Quaternion<T>(dir * sc[0], sc[1] ),
				Quaternion<T>( moment * sc[0] + dir * T(0.5) * pitch * sc[1], -pitch * sc[0] * T(0.5)));
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE std::pair<Quaternion<T>, Quaternion<T> > Sclerp(Quaternion<T> const & lhs_real, Quaternion<T> const & lhs_dual, Quaternion<T> const & rhs_real, Quaternion<T> const & rhs_dual, T const & slerp)
		{
			// Make sure dot product is >= 0
			T quat_dot = lhs_real.Dot(rhs_real);
			Quaternion<T> to_sign_corrected_real = rhs_real;
			Quaternion<T> to_sign_corrected_dual = rhs_dual;
			if (quat_dot < 0)
			{
				to_sign_corrected_real = -to_sign_corrected_real;
				to_sign_corrected_dual = -to_sign_corrected_dual;
			}

			std::pair<Quaternion<T>, Quaternion<T> > dif_dq = Inverse(lhs_real, lhs_dual);
			dif_dq.second = MulDualQuaternion(dif_dq.first, dif_dq.second, to_sign_corrected_real, to_sign_corrected_dual);
			dif_dq.first = MulRealQuaternion(dif_dq.first, to_sign_corrected_real);

			T angle, pitch;
			vector3<T> direction, moment;
			DualQuaternionToScrew(angle, pitch, direction, moment, dif_dq.first, dif_dq.second);

			angle *= slerp; 
			pitch *= slerp;
			dif_dq = ScrewToDualQuaternion(angle, pitch, direction, moment);

			dif_dq.second = MulDualQuaternion(lhs_real, lhs_dual, dif_dq.first, dif_dq.second);
			dif_dq.first = MulRealQuaternion(lhs_real, dif_dq.first);

			return dif_dq;
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		vector3<T> DualQuaternionToTransform(Quaternion<T> const & real, Quaternion<T> const & dual)
		{
			Quaternion<T> qeq0 = Conjugate(real) * dual;
			return T(2.0) * vector3<T>(qeq0.m_x, qeq0.m_y, qeq0.m_z);
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		Matrix44<T> DualQuaternionToMatrix44(Quaternion<T> const & real, Quaternion<T> const & dual)
		{
			Matrix44<T> m;

			T len2 = real.Dot(real);
			T w = real.m_w, x = real.m_x, y = real.m_y, z = real.m_z;
			T t0 = dual.m_w, t1 = dual.m_x, t2 = dual.m_y, t3 = dual.m_z;

			m.a11 = w * w + x * x - y * y - z * z;
			m.a21 = 2 * x * y - 2 * w * z;
			m.a31 = 2 * x * z + 2 * w * y;
			m.a12 = 2 * x * y + 2 * w * z;
			m.a22 = w * w + y * y - x * x - z * z;
			m.a32 = 2 * y * z - 2 * w * x;
			m.a13 = 2 * x * z - 2 * w * y;
			m.a23 = 2 * y * z + 2 * w * x;
			m.a33 = w * w + z * z - x * x - y * y;

			m.a41 = -2 * t0 * x + 2 * w * t1 - 2 * t2 * z + 2 * y * t3;
			m.a42 = -2 * t0 * y + 2 * t1 * z - 2 * x * t3 + 2 * w * t2;
			m.a43 = -2 * t0 * z + 2 * x * t2 + 2 * w * t3 - 2 * t1 * y;

			m.a14 = 0;
			m.a24 = 0;
			m.a34 = 0;
			m.a44 = len2;

			m /= len2;

			return m;
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE void Decompose(vector3<T>& scale, Quaternion<T>& rot, vector3<T>& trans, Matrix44<T> const & rhs)
		{
			scale.m_x = Math::Invsqrt(rhs.a11 * rhs.a11 + rhs.a12 * rhs.a12 + rhs.a13 * rhs.a13);
			scale.m_y = Math::Invsqrt(rhs.a21 * rhs.a21 + rhs.a22 * rhs.a22 + rhs.a23 * rhs.a23);
			scale.m_z = Math::Invsqrt(rhs.a31 * rhs.a31 + rhs.a32 * rhs.a32 + rhs.a33 * rhs.a33);

			trans = vector3<T>(rhs.a41, rhs.a42, rhs.a43);

			Matrix44<T> rot_mat;
			rot_mat.a11 = rhs.a11 * scale.m_x;
			rot_mat.a12 = rhs.a12 * scale.m_x;
			rot_mat.a13 = rhs.a13 * scale.m_x;
			rot_mat.a14 = 0;
			rot_mat.a21 = rhs.a21 * scale.m_y;
			rot_mat.a22 = rhs.a22 * scale.m_y;
			rot_mat.a23 = rhs.a23 * scale.m_y;
			rot_mat.a24 = 0;
			rot_mat.a31 = rhs.a31 * scale.m_z;
			rot_mat.a32 = rhs.a32 * scale.m_z;
			rot_mat.a33 = rhs.a33 * scale.m_z;
			rot_mat.a34 = 0;
			rot_mat.a41 = 0;
			rot_mat.a42 = 0;
			rot_mat.a43 = 0;
			rot_mat.a44 = 1;
			rot = rot_mat;
		}
		//矩阵变换操作
		//---------------------------------------------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> Scaling( const vector3<T>& s ) //得到缩放矩阵
		{
			return Matrix44<T>(	s.m_x,	0,		0,		0,
				0,		s.m_y,	0,		0,
				0,		0,		s.m_z,	0,
				0,		0,		0,		1);
		}
		//---------------------------------------------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> Quaternion_to_Matrix44(Quaternion<T> const & quat) //四元数到矩阵
		{
			// calculate coefficients
			T const x2(quat.m_x + quat.m_x);
			T const y2(quat.m_y + quat.m_y);
			T const z2(quat.m_z + quat.m_z);

			T const xx2(quat.m_x * x2), xy2(quat.m_x * y2), xz2(quat.m_x * z2);
			T const yy2(quat.m_y * y2), yz2(quat.m_y * z2), zz2(quat.m_z * z2);
			T const wx2(quat.m_w * x2), wy2(quat.m_w * y2), wz2(quat.m_w * z2);

			return Matrix44<T>(
				1 - yy2 - zz2,	xy2 + wz2,		xz2 - wy2,		0,
				xy2 - wz2,		1 - xx2 - zz2,	yz2 + wx2,		0,
				xz2 + wy2,		yz2 - wx2,		1 - xx2 - yy2,	0,
				0,				0,				0,				1);
		}
		//---------------------------------------------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE Matrix33<T> Quaternion_to_Matrix33(Quaternion<T> const & quat) //四元数到矩阵
		{
			// calculate coefficients
			T const x2(quat.m_x + quat.m_x);
			T const y2(quat.m_y + quat.m_y);
			T const z2(quat.m_z + quat.m_z);

			T const xx2(quat.m_x * x2), xy2(quat.m_x * y2), xz2(quat.m_x * z2);
			T const yy2(quat.m_y * y2), yz2(quat.m_y * z2), zz2(quat.m_z * z2);
			T const wx2(quat.m_w * x2), wy2(quat.m_w * y2), wz2(quat.m_w * z2);

			return Matrix33<T>(
				1 - yy2 - zz2,	xy2 + wz2,		xz2 - wy2,
				xy2 - wz2,		1 - xx2 - zz2,	yz2 + wx2,
				xz2 + wy2,		yz2 - wx2,		1 - xx2 - yy2 );
		}
		//---------------------------------------------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> Translation(const vector3<T>& p)//等到平移矩阵
		{
			return Matrix44<T>(
				1,		0,		0,		0,
				0,		1,		0,		0,
				0,		0,		1,		0,
				p.m_x,	p.m_y,	p.m_z,	1);
		}
		//---------------------------------------------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> Transformation(vector3<T> const * scaling_center, Quaternion<T> const * scaling_rotation, vector3<T> const * scale,
			vector3<T> const * rotation_center, Quaternion<T> const * rotation, vector3<T> const * trans)//得到变化矩阵
		{
			vector3<T> psc, prc, pt;
			if (scaling_center)
			{
				psc = *scaling_center;
			}
			if (rotation_center)
			{
				prc = *rotation_center;
			}
			if (trans)
			{
				pt = *trans;
			}

			bool isbegin = false;
			Matrix44<T> r;
			Matrix44<T> m1, m2, m3, m4, m5, m6, m7;
			if (scaling_center)
			{
				m1 = Math::Translation(-psc);
				r = m1;
				isbegin = true;
			}
			if (scaling_rotation)
			{
				m4 = Quaternion_to_Matrix44(*scaling_rotation);
				m4.GetInverse( m2 );
				if( isbegin )
				{
					r *= m2;
				}
				else
				{
					isbegin = true;
					r = m2;
				}
				if ( scale && *scale != vector3<T>(1,1,1) )
				{
					m3 = Math::Scaling(*scale);
					r *= m3;
				}
				r *= m4;
			}else if ( scale && *scale != vector3<T>(1,1,1) )
			{
				m3 = Math::Scaling(*scale);
				if( isbegin )
				{
					r *= m3;
				}
				else
				{
					isbegin = true;
					r = m3;
				}
			}
			if ( scaling_center || rotation_center )
			{
				m5 = Translation(psc - prc);
				if( isbegin )
				{
					r *= m5;
				}
				else
				{
					isbegin = true;
					r = m5;
				}
			}
			if (rotation&&*rotation!=Quaternion<T>(0,0,0,1))
			{
				m6 = Quaternion_to_Matrix44(*rotation);
				if( isbegin )
				{
					r *= m6;
				}
				else
				{
					isbegin = true;
					r = m6;
				}
			}
			if ( rotation_center || trans )
			{
				m7 = Translation(prc + pt);
				if( isbegin )
				{
					r *= m7;
				}
				else
				{
					isbegin = true;
					r = m7;
				}
			}
#ifdef _DEBUG
			Matrix44<T> test = m1 * m2 * m3 * m4 * m5 * m6 * m7;
			ASSERT( test == r );
#endif
			return r;
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T, typename M>
		BOHGE_FORCEINLINE T Transform_coord(T const & v, Matrix44<M> const & mat) //移动一个坐标
		{
			vector4<typename T::value_type> temp( v );
			temp *= mat;
			T ret;
			if ( isEqual( temp.m_w, (typename T::value_type)(0) ) )
			{
				ret = T::Zero();
			}
			else
			{
				ret = temp / temp.m_w;
			}
			return ret;
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE bool isEqual(const T& lhs, const T& rhs)
		{
			T def = lhs - rhs;
			return def <= T(EPSELON) && def >= -T(EPSELON);
		}
		//-------------------------------------------------------------------------------------------------------
		/*判断该浮点是否为零*/
		template<typename T> BOHGE_FORCEINLINE bool isZero(T input)
		{
			if( input<T(EPSELON) && input > -T(EPSELON))
			{
				return true;
			}
			return false;
		}
		//-------------------------------------------------------------------------------------------------------
		/*截取浮点，使他在0.0 - 1.0 之间*/
		static BOHGE_FORCEINLINE clamp Clamp0to1( float input )
		{
			return Clamp( input, 0.0f, 1.0f );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T GreaterZero(const T& input)
		{
			return input < T(0) ? 0 : input;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		// From Quake III. But the magic number is from http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
		BOHGE_FORCEINLINE T Recip_sqrt(T number)
		{
			T const threehalfs = 1.5f;

			T x2 = number * 0.5f;
			union FNI
			{
				T f;
				int i;
			} fni;
			fni.f = number;											// evil floating point bit level hacking
			fni.i = 0x5f375a86 - (fni.i >> 1);						// what the fuck?
			fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));	// 1st iteration
			fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));		// 2nd iteration, this can be removed

			return fni.f;
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T Lerp(T const & lhs, T const & rhs, const T& s)
		{
			return lhs + (rhs - lhs) * s;
		}
		//-------------------------------------------------------------------------------------------------------
		static BOHGE_FORCEINLINE bool isPowerOfTwo( const int& n )
		{
			return ( n & (n - 1) ) == 0;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Clamp( const T& input, const T& min, const T& max )
		{
			return input < min ? min : input > max ? max : input;
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> ViewMatrix(const vector3<T>& position, const vector3<T>& lookAt, const vector3<T>& up)
		{
			vector3<T> ZAxis = position - lookAt;
			vector3<T> XAxis = up.Cross( ZAxis );
			vector3<T> YAxis = ZAxis.Cross( XAxis );
			XAxis.NormalizeSelf();
			YAxis.NormalizeSelf();
			ZAxis.NormalizeSelf();
			return Matrix44<T>(	XAxis.m_x, YAxis.m_x, ZAxis.m_x, 0.0f,
				XAxis.m_y, YAxis.m_y, ZAxis.m_y, 0.0f,
				XAxis.m_z, YAxis.m_z, ZAxis.m_z, 0.0f,
				-XAxis.Dot( position ), -YAxis.Dot( position ), -ZAxis.Dot( position ), 1.0f);
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> ProjectMatrix(const T& radian, const T& aspect, const T& near, const T& farClip)
		{
			T h = 1.0f / Math::Tan( radian * 0.5f );
			T w  = h / aspect;
			T p1 = -( farClip ) / ( farClip - near );
			T p2 = -( farClip * near ) / ( farClip - near );
			return	Matrix44<T>(  w, 0.0, 0.0, 0.0,
				0.0,   h, 0.0, 0.0,
				0.0, 0.0,  p1, -1.0f,
				0.0, 0.0,  p2, 0.0);
		}
		//-------------------------------------------------------------------------------------------------------
		static BOHGE_FORCEINLINE int Random(int begin, int end)
		{
			return rand()%(end - begin + 1) + begin;
		}
		//-------------------------------------------------------------------------------------------------------
		static BOHGE_FORCEINLINE float fRandom(float begin, float end)
		{
			float r = (float)rand()/(float)RAND_MAX;//得到 0-1的随机值
			return ( end - begin ) * r + begin;
			//int num = static_cast<int>(end * 1000.0f - begin * 1000.0f);
			//float fnum = static_cast<float>(rand()%(num)) / 1000.0f;
			//return fnum + begin;
		}


		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T Sin( const T& x )
		{
#ifdef NEON_MATH
			return (*_Neon_sinf)( (float)x );
#else
			return sin(x);
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T Cos( const T& x )
		{
#ifdef NEON_MATH
			return (*_Neon_cosf)( (float)x );
#else
			return cos(x);
#endif			
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T Tan( const T& x )
		{
#ifdef NEON_MATH
			return (*_Neon_tanf)( (float)x );
#else
			return tan(x);
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T ASin( const T& x )
		{
#ifdef NEON_MATH
			return (*_Neon_asinf)( (float)x );
#else
			return asin(x);
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T ACos( const T& x )
		{
#ifdef NEON_MATH
			return (*_Neon_acosf)( (float)x );
#else
			return acos(x);
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T ATan( const T& x )
		{
#ifdef NEON_MATH
			return (*_Neon_atanf)( (float)x );
#else
			return atan(x);
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T Pow( const T& x, const T&n )
		{
#ifdef NEON_MATH
			return (*_Neon_powf)( (float)x, (float)n );
#else
			return pow( x, n );
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T Floor( const T& x )
		{
#ifdef NEON_MATH
			return (*_Neon_floorf)( (float)x );
#else
			return floor(x);
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T Ceil( const T& x )
		{
#ifdef NEON_MATH
			return (*_Neon_ceilf)( (float)x );
#else
			return ceil(x);
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Mod(const T& x, int* y)
		{
#ifdef NEON_MATH
			return (*_Neon_modf)( (float)x, y );
#else
			T temp;
			T res = modff( x, &temp );
			*y = (int)temp;
			return res;
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Fmod(const T& x, const int& y)
		{
#ifdef NEON_MATH
			return (*_Neon_fmodf)( (float)x, y );
#else
			return fmod( x, (const T&)y );
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Abs(const T& x)
		{
#ifdef NEON_MATH
			return (*_Neon_fabsf)( x );
#else
			return x < T(0) ? -x : x;
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Sqrt(const T& x)
		{
#ifdef NEON_MATH
			return (*_Neon_sqrtf)( (float)x );
#else
			return sqrt( x );
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Exp(const T& x)
		{
#ifdef NEON_MATH
			return (*_Neon_expf)( (float)x );
#else
			return exp( x );
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Log(const T& x)
		{
#ifdef NEON_MATH
			return (*_Neon_logf)( (float)x );
#else
			return log( x );
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Log10(const T& x)
		{
#ifdef NEON_MATH
			return (*_Neon_log10f)( (float)x );
#else
			return log10( x );
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Ldexp(const T& x, const int& e )
		{
#ifdef NEON_MATH
			return (*_Neon_ldexpf)( (float)x, e );
#else
			return ldexp( x, e );
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Frexp(const T& x, int* n )
		{
#ifdef NEON_MATH
			return (*_Neon_frexpf)( (float)x, n );
#else
			return frexp( x, n );
#endif
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Invsqrt(const T& x)
		{
#ifdef NEON_MATH
			return (*_Neon_invsqrtf)( (float)x );
#else
			return T(1) / Math::Sqrt( x );
#endif
		}
		//特殊处理
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE void SinCos( const T& x, T o[2] )
		{
			o[0] = Math::Sin( x );
			o[1] = Math::Cos( x );
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE vector3<T> Cross( const vector3<T>& v1, const vector3<T>& v2 )
		{
			return vector3<T>(
				v1.m_y * v2.m_z - v1.m_z * v2.m_y,
				v1.m_z * v2.m_x - v1.m_x * v2.m_z,
				v1.m_x * v2.m_y - v1.m_y * v2.m_x );
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE void Cross( const vector3<T>& v1, const vector3<T>& v2, vector3<T>& out )
		{
			out.m_x = v1.m_y * v2.m_z - v1.m_z * v2.m_y;
			out.m_y = v1.m_z * v2.m_x - v1.m_x * v2.m_z;
			out.m_z = v1.m_x * v2.m_y - v1.m_y * v2.m_x;
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T Dot( const vector2<T>& v1, const vector2<T>& v2 )
		{
			return ( v1.m_x * v2.m_x + v1.m_y * v2.m_y );
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T Dot( const vector3<T>& v1, const vector3<T>& v2 )
		{
			return ( v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z );
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T Dot( const vector4<T>& v1, const vector4<T>& v2 )
		{
			return ( v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z + v1.m_w * v2.m_w );
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE T Dot( const Quaternion<T>& v1, const Quaternion<T>& v2 )
		{
			return ( v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z + v1.m_w * v2.m_w );
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE void Normalize( const vector2<T>& v, vector2<T>& out )
		{
			T len = v.m_x * v.m_x + v.m_y * v.m_y;
			len = Math::Invsqrt(len);
			out.m_x = v.m_x * len;
			out.m_y = v.m_y * len;
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE void Normalize( const vector3<T>& v, vector3<T>& out )
		{
			T len = v.m_x * v.m_x + v.m_y * v.m_y + v.m_z * v.m_z;
			len = Math::Invsqrt(len);
			out.m_x = v.m_x * len;
			out.m_y = v.m_y * len;
			out.m_z = v.m_z * len;
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE void Normalize( const Quaternion<T>& v, Quaternion<T>& out )
		{
			T len = v.m_x * v.m_x + v.m_y * v.m_y + v.m_z * v.m_z + v.m_w * v.m_w;
			len = Math::Invsqrt(len);
			out.m_x = v.m_x * len;
			out.m_y = v.m_y * len;
			out.m_z = v.m_z * len;
			out.m_w = v.m_w * len;
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE void Normalize( const vector4<T>& v, vector4<T>& out )
		{
			T len = v.m_x * v.m_x + v.m_y * v.m_y + v.m_z * v.m_z + v.m_w * v.m_w;
			len = Math::Invsqrt(len);
			out.m_x = v.m_x * len;
			out.m_y = v.m_y * len;
			out.m_z = v.m_z * len;
			out.m_w = v.m_w * len;
		}


#ifdef NEON_MATH
		static BOHGE_FORCEINLINE void SinCos( const float& x, float* out )
		{
			(*_Neon_sincosf)( x, out );
		}
		static BOHGE_FORCEINLINE vector3f Cross( const vector3f& v1, const vector3f& v2 )
		{
			vector3f temp;
			(*_Neon_cross3)( v1.data, v2.data, temp.data );
			return temp;
		}
		static BOHGE_FORCEINLINE void Cross( const vector3f& v1, const vector3f& v2, vector3f& out )
		{
			(*_Neon_cross3)( v1.data, v2.data, out.data );
		}
		static BOHGE_FORCEINLINE float Dot( const vector2f& v1, const vector2f& v2 )
		{
			return (*_Neon_dot2)( v1.data, v2.data);
		}
		static BOHGE_FORCEINLINE float Dot( const vector3f& v1, const vector3f& v2 )
		{
			return (*_Neon_dot3)( v1.data, v2.data);
		}
		static BOHGE_FORCEINLINE float Dot( const vector4f& v1, const vector4f& v2 )
		{
			return (*_Neon_dot4)( v1.data, v2.data);
		}
		static BOHGE_FORCEINLINE float Dot( const Quaternionf& v1, const Quaternionf& v2 )
		{
			return (*_Neon_dot4)( v1.data, v2.data);
		}
		static BOHGE_FORCEINLINE void Normalize( const vector2f& v, vector2f& out )
		{
			(*_Neon_normalize2)( v.data, out.data );
		}
		static BOHGE_FORCEINLINE void Normalize( const vector3f& v, vector3f& out )
		{
			(*_Neon_normalize3)( v.data, out.data );
		}
		static BOHGE_FORCEINLINE void Normalize( const vector4f& v, vector4f& out )
		{
			(*_Neon_normalize4)( v.data, out.data );
		}
		static BOHGE_FORCEINLINE void Normalize( const Quaternionf& v, Quaternionf& out )
		{
			(*_Neon_normalize4)( v.data, out.data );
		}
#endif
	}

}