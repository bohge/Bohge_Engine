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
#include "RttiObject.hpp"
#include "Rtti.hpp"

namespace BohgeEngine
{
	template <typename T>
	class Matrix22 : public RttiObject
	{
		RTTI_TYPE_DECLEAR( Matrix22<T> )
	public:
		union//数据联合类型
		{
			struct
			{
				T a11,a12;
				T a21,a22;
			};
			T data[4];
		};
	private:
		/*拷贝函数，des是目标对象，sro是输入的需要拷贝的对象*/
		BOHGE_FORCEINLINE void Copy(Matrix22<T>& des, const Matrix22<T>& sro)
		{
			des.a11 = sro.a11;
			des.a12 = sro.a12;
			des.a21 = sro.a21;
			des.a22 = sro.a22;
		}
	public:
		BOHGE_FORCEINLINE Matrix22<T>()
			:a11(T(1.0)),
			a12(T(0)),
			a21(T(0)),
			a22(T(1.0))
		{}
		BOHGE_FORCEINLINE Matrix22<T>(const T& a11, const T& a12, const T& a21, const T& a22)
			:a11(a11),
			a12(a12),
			a21(a21),
			a22(a22)
		{}
		BOHGE_FORCEINLINE Matrix22<T>(const Matrix22<T>& input)
		{
			this->Copy(*this, input);
		}
	public://操作符重载
		BOHGE_FORCEINLINE Matrix22<T>& operator= (const Matrix22<T>& input)
		{
			this->Copy(*this, input);
			return *this;
		}
		//BOHGE_FORCEINLINE Matrix22f& operator/= (float num)
		//{
		//	ASSERT(false == isZero(num));
		//	this->a11/=num;
		//	this->a12/=num;
		//	this->a21/=num;
		//	this->a22/=num;
		//	return *this;
		//}
		BOHGE_FORCEINLINE Matrix22<T> operator* (const Matrix22<T>& input) const
		{
			return Matrix22<T>(	a11*input.a11 + a12*input.a21,   a11*input.a12 + a12*input.a22,
				a21*input.a11 + a22*input.a21,   a21*input.a12 + a22*input.a22);
		}
		BOHGE_FORCEINLINE Matrix22<T> operator/ (T num)
		{
			ASSERT(false == Math::isZero(num));
			return Matrix22<T>(a11/num, a12/num, a21/num, a22/num);
		}
		BOHGE_FORCEINLINE Matrix22<T> operator- ()
		{
			return Matrix22<T>(-a11,-a12,-a21,-a22);
		}
		BOHGE_FORCEINLINE Matrix22<T> operator- (const Matrix22<T>& input) const
		{
			return Matrix22<T>(	a11-input.a11,	a12-input.a12,
				a21-input.a21,	a22-input.a22);
		}
	public:
		friend BOHGE_FORCEINLINE Matrix22<T> operator* ( T lhs, const Matrix22<T>& rhs)
		{
			return  Matrix22<T>(rhs.a11*lhs, rhs.a12*lhs, rhs.a21*lhs, rhs.a22*lhs);
		}
		friend BOHGE_FORCEINLINE Matrix22<T> operator* ( const Matrix22<T>& rhs, T lhs )
		{
			return  Matrix22<T>(rhs.a11*lhs, rhs.a12*lhs, rhs.a21*lhs, rhs.a22*lhs);
		}
	public://接口函数
		BOHGE_FORCEINLINE void FromRadian( T r )//旋转一定弧度
		{
			//T cosr = cos( r );
			//T sinr = sin( r );
			//a11 = cosr;
			//a12 = sinr;
			//a21 = -sinr;
			//a22 = cosr;
			T sc[2];
			Math::SinCos( r, sc );
			a11 = sc[1];
			a12 = sc[0];
			a21 = -sc[0];
			a22 = sc[1];
		}
		BOHGE_FORCEINLINE T CalculateDet() const
		{
			return a11 * a22 - a12 * a21;
		}
		/*得到该矩阵的逆，如可逆返回true和逆矩阵，如果不可逆则返回false，输入矩阵不变*/
		BOHGE_FORCEINLINE bool GetInverse(Matrix22<T>& out) const
		{
			T det = this->CalculateDet();
			if( 0.0 == det)//如果矩阵的行列式为零则该矩阵没有逆，返回false，退出计算
			{
				return false;
			}
			Matrix22<T> Inv(a22, -a12, -a21, a11);//转置矩阵
			out = Inv / det;//结果等于转置矩阵*矩阵的行列式
			return true;	
		}
		/*将自己逆掉，如果不可以返回false，否则返回true*/
		BOHGE_FORCEINLINE bool InverseSelf()
		{
			if( true == GetInverse(*this) )
			{
				return true;
			}
			return false;
		}
	};

	RTTI_TYPE_IMPLEMENT( Matrix22<int> );
	RTTI_TYPE_IMPLEMENT( Matrix22<float> );
	RTTI_TYPE_IMPLEMENT( Matrix22<double> );
}