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
#ifndef BASEMATHDECLAR
#define BASEMATHDECLAR
#else
#error( "Do not include this file in other files" )
#endif // BASEMATHDECLAR

#ifndef IOS
#define  NEON_MATH
#endif


#include "Predefine.h"
#include "VariableType.h"
#ifdef NEON_MATH
#include "NeonMath.h"
#else
#include <math.h>
#endif

#include <vector>

namespace BohgeEngine
{
//-------------------------------------------------------------------------------------------------------
#define EPSELON		0.00001f
#define  PI			3.141592653589793238462643383279502884f
#define  PI_D_2		1.5707963267948966192313216916395f
#define  PI_D_3		1.047197551196597746154214461093f
#define  PI_D_4		0.78539816339744830961566084581975f
#define  PI_D_5		0.6283185307179586476925286766558f
#define  PI_D_6		0.5235987755982988730771072305465f
#define  PI_D_7		0.44879895051282760549466334046843f
#define  PI_D_8		0.39269908169872415480783042290988f
#define  HIGH_PI	3.141592653589793238462643383279502884
	struct OnSide
	{
		enum Volume
		{
			OS_NEGATIVE_SIDE,
			OS_POSITIVE_SIDE,
			OS_BOTH_SIDE,
		};
	};

	struct IntersectedType
	{
		enum Volume
		{
			IT_NO,//不想交
			IT_PARTIAL,//相交一部分
			IT_FULL,//完全不想交
		};
	};
	template<typename T>class Matrix22;
	template<typename T>class Matrix33;
	template<typename T>class Matrix44;
	template<typename T>class vector2;
	template<typename T>class vector2;
	template<typename T>class vector3;
	template<typename T>class vector4;
	template<typename T>class Quaternion;

	typedef vector2<float> vector2f;
	typedef vector2<int> vector2d;
	typedef vector2<double> vector2b;

	typedef vector3<float> vector3f;
	typedef vector3<int> vector3d;
	typedef vector3<double> vector3b;

	typedef vector4<float> vector4f;
	typedef vector4<int> vector4d;
	typedef vector4<double> vector4b;

	typedef Matrix22<float> Matrix22f;
	typedef Matrix22<double> Matrix22b;

	typedef Matrix33<float> Matrix33f;
	typedef Matrix33<double> Matrix33b;

	typedef Matrix44<float> Matrix44f;
	typedef Matrix44<double> Matrix44b;

	typedef Quaternion<float> Quaternionf;
	typedef Quaternion<double> Quaternionb;

	namespace Math
	{
		template<typename T>
		BOHGE_FORCEINLINE T Clamp( const T& input, const T& min, const T& max );
		template<typename T> 
		BOHGE_FORCEINLINE const T& GetMax( const T& a, const T& b );
		template<typename T>
		BOHGE_FORCEINLINE const T& GetMin( const T& a, const T& b );
		template<typename T> 
		BOHGE_FORCEINLINE vector3<T> Transform_Quat(const vector3<T>& v, const Quaternion<T>& quat);
		template<typename T>
		BOHGE_FORCEINLINE Quaternion<T> Conjugate(const Quaternion<T>& q);
		template<typename T> 
		BOHGE_FORCEINLINE Quaternion<T> MulRealQuaternion(const Quaternion<T>& rhs_real, const Quaternion<T>& lhs_real);
		template<typename T>
		BOHGE_FORCEINLINE Quaternion<T> MulDualQuaternion(Quaternion<T> const & lhs_real, Quaternion<T> const & lhs_dual, Quaternion<T> const & rhs_real, Quaternion<T> const & rhs_dual);
		template<typename T>
		BOHGE_FORCEINLINE Quaternion<T> YawPitchRoll(const T& yaw, const T& pitch, const T& roll);
		template<typename T>
		BOHGE_FORCEINLINE Quaternion<T> AxisToAxis(vector3<T> const & from, vector3<T> const & to);
		template<typename T>
		BOHGE_FORCEINLINE Quaternion<T> RotateAxis(const vector3<T>& axis, T r );
		template<typename T>
		BOHGE_FORCEINLINE std::pair<Quaternion<T>, Quaternion<T> > Inverse(const Quaternion<T>& real, const Quaternion<T>& dual);
		template <typename T>
		BOHGE_FORCEINLINE void DualQuaternionToScrew(T& angle, T& pitch, vector3<T>& dir, vector3<T>& moment, Quaternion<T> const & real, Quaternion<T> const & dual);
		template <typename T>
		BOHGE_FORCEINLINE std::pair<Quaternion<T>, Quaternion<T> > ScrewToDualQuaternion(T const & angle, T const & pitch, vector3<T> const & dir, vector3<T> const & moment);
		template <typename T>
		BOHGE_FORCEINLINE std::pair<Quaternion<T>, Quaternion<T> > Sclerp(Quaternion<T> const & lhs_real, Quaternion<T> const & lhs_dual, Quaternion<T> const & rhs_real, Quaternion<T> const & rhs_dual, T const & slerp);
		template <typename T>
		BOHGE_FORCEINLINE void Decompose(vector3<T>& scale, Quaternion<T>& rot, vector3<T>& trans, Matrix44<T> const & rhs);
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> Scaling( const vector3<T>& s );
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> Quaternion_to_Matrix44(Quaternion<T> const & quat);
		template <typename T>
		BOHGE_FORCEINLINE Matrix33<T> Quaternion_to_Matrix33(Quaternion<T> const & quat);
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> Translation(const vector3<T>& p);
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> Transformation(vector3<T> const * scaling_center, Quaternion<T> const * scaling_rotation, vector3<T> const * scale,
			vector3<T> const * rotation_center, Quaternion<T> const * rotation, vector3<T> const * trans);//得到变化矩阵
		template <typename T, typename M>
		BOHGE_FORCEINLINE T Transform_coord(T const & v, Matrix44<M> const & mat); //移动一个坐标
		template <typename T>
		BOHGE_FORCEINLINE bool isEqual(const T& lhs, const T& rhs);
		template<typename T> 
		BOHGE_FORCEINLINE bool isZero(T input);
		template<typename T>
		BOHGE_FORCEINLINE T GreaterZero(const T& input);
		template <typename T>
		BOHGE_FORCEINLINE T Recip_sqrt(T number);
		template <typename T>
		BOHGE_FORCEINLINE T Lerp(T const & lhs, T const & rhs, const T& s);
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> ViewMatrix(const vector3<T>& position, const vector3<T>& lookAt, const vector3<T>& up);
		template <typename T>
		BOHGE_FORCEINLINE Matrix44<T> ProjectMatrix(const T& radian, const T& aspect, const T& near, const T& farClip);

		//非模板函数，全局静态
		static BOHGE_FORCEINLINE clamp Clamp0to1( float input );
		static BOHGE_FORCEINLINE int Random(int begin, int end);
		static BOHGE_FORCEINLINE float fRandom(float begin, float end);
		static BOHGE_FORCEINLINE bool isPowerOfTwo( const int& n );


		template <typename T>
		BOHGE_FORCEINLINE T Sin( const T& x );
		template <typename T>
		BOHGE_FORCEINLINE T Cos( const T& x );
		template <typename T>
		BOHGE_FORCEINLINE T Tan( const T& x );
		template <typename T>
		BOHGE_FORCEINLINE T ASin( const T& x );
		template <typename T>
		BOHGE_FORCEINLINE T ACos( const T& x );
		template <typename T>
		BOHGE_FORCEINLINE T ATan( const T& x );
		template <typename T>
		BOHGE_FORCEINLINE T Pow( const T& x, const T&n );
		template <typename T>
		BOHGE_FORCEINLINE T Floor( const T& x );
		template <typename T>
		BOHGE_FORCEINLINE T Ceil( const T& x );
		template <typename T>
		BOHGE_FORCEINLINE T Abs( const T& x );
		template<typename T>
		BOHGE_FORCEINLINE T Sqrt(const T& x);
		template<typename T>
		BOHGE_FORCEINLINE T Exp(const T& x);
		template<typename T>
		BOHGE_FORCEINLINE T Log(const T& x);
		template<typename T>
		BOHGE_FORCEINLINE T Log10(const T& x);
		template<typename T>
		BOHGE_FORCEINLINE T Ldexp(const T& x, const int& e );
		template<typename T>
		BOHGE_FORCEINLINE T Frexp(const T& x, int* n );
		template<typename T>
		BOHGE_FORCEINLINE T Invsqrt(const T& x);
		template<typename T>
		BOHGE_FORCEINLINE T Fmod(const T& x, const int& y);
		template<typename T>
		BOHGE_FORCEINLINE T Mod(const T& x, int* y);

		//采用neon，float特殊处理
		template <typename T>
		BOHGE_FORCEINLINE void SinCos( const T& x, T o[2] );

		template <typename T>
		BOHGE_FORCEINLINE vector3<T> Cross( const vector3<T>& v1, const vector3<T>& v2 );
		template <typename T>
		BOHGE_FORCEINLINE void Cross( const vector3<T>& v1, const vector3<T>& v2, vector3<T>& out );
		
		template <typename T>
		BOHGE_FORCEINLINE T Dot( const vector2<T>& v1, const vector2<T>& v2 );
		template <typename T>
		BOHGE_FORCEINLINE T Dot( const vector3<T>& v1, const vector3<T>& v2 );
		template <typename T>
		BOHGE_FORCEINLINE T Dot( const vector4<T>& v1, const vector4<T>& v2 );
		template <typename T>
		BOHGE_FORCEINLINE T Dot( const Quaternion<T>& v1, const Quaternion<T>& v2 );
	
		template <typename T>
		BOHGE_FORCEINLINE void Normalize( const vector2<T>& v, vector2<T>& out );
		template <typename T>
		BOHGE_FORCEINLINE void Normalize( const vector3<T>& v, vector3<T>& out );
		template <typename T>
		BOHGE_FORCEINLINE void Normalize( const vector4<T>& v, vector4<T>& out );
		template <typename T>
		BOHGE_FORCEINLINE void Normalize( const Quaternion<T>& v, Quaternion<T>& out );

#ifdef NEON_MATH
		static BOHGE_FORCEINLINE void SinCos( const float& x, float* out );
		static BOHGE_FORCEINLINE vector3f Cross( const vector3f& v1, const vector3f& v2 );
		static BOHGE_FORCEINLINE void Cross( const vector3f& v1, const vector3f& v2, vector3f& out );
		static BOHGE_FORCEINLINE float Dot( const vector2f& v1, const vector2f& v2 );
		static BOHGE_FORCEINLINE float Dot( const vector3f& v1, const vector3f& v2 );
		static BOHGE_FORCEINLINE float Dot( const vector4f& v1, const vector4f& v2 );
		static BOHGE_FORCEINLINE float Dot( const Quaternionf& v1, const Quaternionf& v2 );
		static BOHGE_FORCEINLINE void Normalize( const vector2f& v, vector2f& out );
		static BOHGE_FORCEINLINE void Normalize( const vector3f& v, vector3f& out );
		static BOHGE_FORCEINLINE void Normalize( const vector4f& v, vector4f& out );
		static BOHGE_FORCEINLINE void Normalize( const Quaternionf& v, Quaternionf& out );
#endif
	}

}

static int sqrt( const int& x )
{
	return ::sqrt( (float)x );
}