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



/////////////////////////////////////////
//			3D Math class			   //
/////////////////////////////////////////


#pragma once
#include <math.h>

#include "Predefine.h"
#include "VariableType.h"
#include "BaseMath.hpp"
//-------------------------------------------------------------------------------------------------------
#define  PI 3.1415926f
//-------------------------------------------------------------------------------------------------------
template<typename T>class vector2;
template<typename T>class vector3;
template<typename T>class vector4;

typedef vector2<float> vector2f;
typedef vector2<int> vector2d;

typedef vector3<float> vector3f;
typedef vector3<int> vector3d;

typedef vector4<float> vector4f;


//注意，全部的顶点类型不能有任何的虚函数，应为顶点类型最终是通过opengl的内存指针的形式被指定出来，然后进行绘制
//这种形式要求数据在内存上是连续的，任何虚函数可能导致数据的不连续，所以不能使用虚函数


//2x2矩阵计算4x4其次矩阵的逆时使用，暂时如此
class Matrix22f
{
public:
	union//数据联合类型
	{
		struct
		{
			float a11,a12;
			float a21,a22;
		};
		float data[4];
	};
private:
	/*拷贝函数，des是目标对象，sro是输入的需要拷贝的对象*/
	inline void Copy(Matrix22f& des, const Matrix22f& sro)
	{
		des.a11 = sro.a11;
		des.a12 = sro.a12;
		des.a21 = sro.a21;
		des.a22 = sro.a22;
	}
public:
	Matrix22f()
		:a11(1.0f),
		a12(0),
		a21(0),
		a22(1.0f)
	{}
	Matrix22f(const float a11, const float a12, const float a21, const float a22)
		:a11(a11),
		a12(a12),
		a21(a21),
		a22(a22)
	{}
	Matrix22f(const Matrix22f& input)
	{
		this->Copy(*this, input);
	}
public://操作符重载
	inline Matrix22f& operator= (const Matrix22f& input)
	{
		this->Copy(*this, input);
		return *this;
	}
	inline Matrix22f operator/ (float num) const
	{
		ASSERT(false == isZero(num));
		return Matrix22f(a11/num, a12/num, a21/num, a22/num);
	}
	//inline Matrix22f& operator/= (float num)
	//{
	//	ASSERT(false == isZero(num));
	//	this->a11/=num;
	//	this->a12/=num;
	//	this->a21/=num;
	//	this->a22/=num;
	//	return *this;
	//}
	inline Matrix22f operator* (float num) const
	{
		return Matrix22f(a11*num, a12*num, a21*num, a22*num);
	}
	inline Matrix22f operator* (Matrix22f& input) const
	{
		return Matrix22f(	a11*input.a11 + a12*input.a21,   a11*input.a12 + a12*input.a22,
				 			a21*input.a11 + a22*input.a21,   a21*input.a12 + a22*input.a22);
	}
	inline Matrix22f operator- ()
	{
		return Matrix22f(-a11,-a12,-a21,-a22);
	}
	inline Matrix22f operator- (const Matrix22f& input) const
	{
		return Matrix22f(	a11-input.a11,	a12-input.a12,
							a21-input.a21,	a22-input.a22);
	}
public://接口函数
	inline float CalculateDet() const
	{
		return a11 * a22 - a12 * a21;
	}
	/*得到该矩阵的逆，如可逆返回true和逆矩阵，如果不可逆则返回false，输入矩阵不变*/
	bool GetInverse(Matrix22f& out) const
	{
		float det = this->CalculateDet();
		if(true == isZero(det))//如果矩阵的行列式为零则该矩阵没有逆，返回false，退出计算
		{
			return false;
		}
		Matrix22f Inv(a22, -a12, -a21, a11);//转置矩阵
		out = Inv / det;//结果等于转置矩阵*矩阵的行列式
		return true;	}
	/*将自己逆掉，如果不可以返回false，否则返回true*/
	bool InverseSelf()
	{
		if( true == GetInverse(*this) )
		{
			return true;
		}
		return false;
	}
};



//4x4的齐次坐标系矩阵
class Matrix44f
{
public:
	union
	{
		struct 
		{
			float a11,a12,a13,a14;
			float a21,a22,a23,a24;
			float a31,a32,a33,a34;
			float a41,a42,a43,a44;
		};
		float data[16];
	};
public:
	Matrix44f();
	Matrix44f( float a11,float a12,float a13,float a14,
		float a21,float a22,float a23,float a24,
		float a31,float a32,float a33,float a34,
		float a41,float a42,float a43,float a44);
	Matrix44f(const Matrix44f& input);
private:
	bool Partitioning(Matrix44f& out);
	bool CramersRule(Matrix44f& out);
public:
	Matrix44f operator* (const Matrix44f& m44 ) const;
	Matrix44f operator* (float num) const;
	Matrix44f operator/ (float num) const;
	bool GetInverse(Matrix44f& out);
	bool InverseSelf();
public:
	inline const float* GetMatrixArray() const
	{
		return data;
	}
	static Matrix44f CreateRotateMatrix(const vector3f& vector , const float radian);
	static Matrix44f CreateMoveMatrix(const vector3f& vector , const float distance);
	/*移动至*/
	static Matrix44f MoveTo(const vector3f& position);
};



//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
class Matrix33f
{
public:
	union
	{
		struct 
		{
			float a11,a12,a13;
			float a21,a22,a23;
			float a31,a32,a33;
		};
		float data[9];
	};
public:
	Matrix33f();
	Matrix33f( float a11,float a12,float a13,
		float a21,float a22,float a23,
		float a31,float a32,float a33);
	Matrix33f(const Matrix33f& input);
	Matrix33f(const Matrix44f& input);
public:
	inline Matrix33f& YawPitchRoll(float yaw, float pitch, float roll)
	{
		float cosPit = cos(yaw);
		float sinPit = sin(yaw);
		float cosYaw = cos(pitch);
		float sinYaw = sin(pitch);
		float cosRol = cos(roll);
		float sinRol = sin(roll);
		//http://zh.wikipedia.org/wiki/%E6%97%8B%E8%BD%AC%E7%9F%A9%E9%98%B5
		//a11 = cosYaw * cosRol - cosPit * sinYaw * sinRol;
		//a12 = - sinYaw * cosPit * cosRol - cosYaw * sinRol;
		//a33 = sinYaw * sinPit;
		//a21 = cosRol * sinYaw + cosYaw * cosPit * sinRol;
		//a22 = cosYaw * cosPit * cosRol - sinYaw + sinRol;
		//a23 = - cosYaw * sinPit;
		//a31 = sinPit * sinRol;
		//a32 = cosRol * sinPit;
		//a33 = cosPit;

		//http://planning.cs.uiuc.edu/node102.html
		a11 = cosYaw * cosPit;
		a12 = cosYaw * sinPit * sinRol - sinYaw * cosRol;
		a33 = cosYaw * sinPit * cosRol + sinYaw * sinRol;
		a21 = sinYaw * cosPit;
		a22 = sinYaw * sinPit * sinRol + cosYaw * cosRol;
		a23 = sinYaw * sinPit * cosRol - cosYaw * sinRol;
		a31 = -sinPit;
		a32 = cosPit * sinRol;
		a33 = cosPit * cosRol;
		return *this;
	}
public:
	inline const float* GetMatrixArray() const
	{
		return data;
	}
};
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
template<typename T>
class vector2
{
public:
	union
	{
		struct	{ T	m_x, m_y; };
		struct	{ T	m_u, m_v; };
		T	data[2];
	};
public:
	static vector2<T>& Zero()
	{
		static vector2 zero(0,0);
		return zero;
	}
public:
	vector2():m_x(0),m_y(0){}
	vector2(T u, T v):m_x(u),m_y(v){}
	inline vector2<T>& operator= (const vector2<T>& input)
	{
		data[0] = input.data[0];
		data[1] = input.data[1];
		return *this;
	}
	inline vector2<T>& operator+= (const vector2<T>& input)
	{
		m_x+=input.m_x;
		m_y+=input.m_y;
		return *this;
	}
	inline vector2<T>& operator-= (const vector2<T>& input)
	{
		m_x-=input.m_x;
		m_y-=input.m_y;
		return *this;
	}
	inline vector2<T>& operator*= (const vector2<T>& input)
	{
		m_x*=input.m_x;
		m_y*=input.m_y;
		return *this;
	}
	inline vector2<T>& operator/= (const vector2<T>& input)
	{
		m_x/=input.m_x;
		m_y/=input.m_y;
		return *this;
	}
	inline vector2<T> operator+ (const vector2<T>& input) const
	{
		return vector2<T>( m_x+input.m_x , m_y+input.m_y );
	}
	inline vector2<T> operator- (const vector2<T>& input) const
	{
		return vector2<T>( m_x-input.m_x , m_y-input.m_y );
	}

	//单个的数字
	inline vector2<T> operator/ (T num) const
	{
		return vector2<T>( m_x/num , m_y/num );
	}
	inline vector2<T> operator* (T num) const
	{
		return vector2<T>( m_x*num , m_y*num );
	}
	inline vector2<T> operator- (T num) const
	{
		return vector2<T>( m_x-num , m_y-num );
	}
	//逻辑
	inline bool operator == (const vector2<T>& input) const
	{
		return m_x == input.m_x && m_y == input.m_y;
	}
public:
	inline const float* GetVectorArray() const
	{
		return data;
	}
	static uint Size()
	{
		return 2;
	}
	static uint BitSize()
	{
		return 2 * sizeof(T);
	}
};

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
template<typename T>
class vector3
{
public:
	union
	{
		struct	{ T	m_x, m_y, m_z; };
		T		data[3];
	};
public:
	vector3()
	{
		data[0] = 0;
		data[1] = 0;
		data[2] = 0;
	}
	vector3(T x, T y, T z)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
	}
	vector3(const vector3& input)
	{
		data[0] = input.data[0];
		data[1] = input.data[1];
		data[2] = input.data[2];
	}
	vector3(const vector4<T>& input)
	{
		data[0] = input.data[0]/input.m_w;
		data[1] = input.data[1]/input.m_w;
		data[2] = input.data[2]/input.m_w;
	}
	/*此处有可能传进来的是自己，所以最后复制，应为如果提前复制会改变他的计算结果，重要！！！！*/
	inline void Multiplication(const Matrix33f& m33, T& x, T& y, T& z) const
	{
		T newx = m_x * m33.a11 + m_y * m33.a21 + m_z *m33.a31;
		T newy = m_x * m33.a12 + m_y * m33.a22 + m_z *m33.a32;
		T newz = m_x * m33.a13 + m_y * m33.a23 + m_z *m33.a33;
		x = newx;
		y = newy;
		z = newz;
	}
	inline T Length()
	{
		return sqrt( m_x*m_x + m_y*m_y + m_z*m_z );
	}
	inline void nomral()
	{
		float len = data[0]*data[0] + data[1]*data[1] + data[2]*data[2];
		len = sqrt(len);
		data[0] /= len;
		data[1] /= len;
		data[2] /= len;
	}
	inline vector3<T>& operator= (const vector3<T>& input)
	{
		data[0] = input.data[0];
		data[1] = input.data[1];
		data[2] = input.data[2];
		return *this;
	}
	inline vector3<T>& operator= (const vector4<T>& input)
	{
		data[0] = input.data[0];
		data[1] = input.data[1];
		data[2] = input.data[2];
		return *this;
	}
	inline vector3<T>& operator+= (const vector3<T>& v3 )
	{
		m_x += v3.m_x;
		m_y += v3.m_y;
		m_z += v3.m_z;
		return *this;
	}
	inline vector3<T>& operator*= (const T num )
	{
		m_x *= num;
		m_y *= num;
		m_z *= num;
		return *this;
	}
	inline vector3<T>& operator*= (const Matrix33f& m33 )
	{
		Multiplication(m33, m_x, m_y, m_z);
		return *this;
	}
	inline vector3<T> operator * (const Matrix33f& m33 )
	{
		T x,y,z;
		Multiplication(m33, x, y, z);
		return vector3(x,y,z);
	}
	inline vector3<T> operator- (const vector3<T>& v3 ) const
	{
		return vector3(data[0] - v3.data[0] , data[1] - v3.data[1] , data[2] - v3.data[2] );
	}
	inline vector3<T> operator+ (const vector3<T>& v3 ) const
	{
		return vector3(data[0] + v3.data[0] , data[1] + v3.data[1] , data[2] + v3.data[2] );
	}
	inline vector3<T> operator/ (const T num ) const
	{
		return vector3( data[0] / num, data[1] / num, data[2] / num );
	}
	inline vector3<T> operator* (const vector3<T>& v3 ) const
	{
		return vector3( data[0] * v3.m_x, data[1] * v3.m_y, data[2] * v3.m_z );
	}
	inline vector3<T> operator* (const T num ) const
	{
		return vector3( data[0] * num, data[1] * num, data[2] * num );
	}
	inline bool operator == (const vector3<T>& rhs ) const
	{
		return ( m_x == rhs.m_x ) & ( m_y == rhs.m_y ) & ( m_z == rhs.m_z );
	}
public:
	inline const T* GetVectorArray() const
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
};
vector3f Vector3fCross(const vector3f& v1, const vector3f& v2);
float Vector3fDot(const vector3f& v1, const vector3f& v2);

//-------------------------------------------------------------------------------------------------------
template<typename T>
class vector4
{
public:
	union
	{
		struct	{ T	m_x, m_y, m_z, m_w; };
		struct	{ T	m_r, m_g, m_b, m_a; };
		T	data[4];
	};
private:
	/*此处有可能传进来的是自己，所以最后复制，应为如果提前复制会改变他的计算结果，重要！！！！*/
	inline void Multiplication(const Matrix44f& m44, T& x, T& y, T& z, T& w) const
	{
		T newx = data[0]*m44.data[0]+data[1]*m44.data[4]+data[2]*m44.data[8]+data[3]*m44.data[12];
		T newy = data[0]*m44.data[1]+data[1]*m44.data[5]+data[2]*m44.data[9]+data[3]*m44.data[13];
		T newz = data[0]*m44.data[2]+data[1]*m44.data[6]+data[2]*m44.data[10]+data[3]*m44.data[14];
		T neww = data[0]*m44.data[3]+data[1]*m44.data[7]+data[2]*m44.data[11]+data[3]*m44.data[15];
		x = newx;
		y = newy;
		z = newz;
		w = neww;
	}
public:
	vector4():m_x(0),
		m_y(0),
		m_z(0),
		m_w(0)
	{
	}
	vector4(T x, T y, T z, T w = 1.0)
		:m_x(x),
		m_y(y),
		m_z(z),
		m_w(w)
	{
	}
	vector4(const vector4<T>& input)
		:m_x(input.m_x),
		m_y(input.m_y),
		m_z(input.m_z),
		m_w(input.m_w)
	{
	}
	vector4(const vector3<T>& input)
		:m_x(input.m_x),
		m_y(input.m_y),
		m_z(input.m_z),
		m_w(1.0)
	{
	}
public:
	inline vector4<T> operator* (const Matrix44f& m44 ) const
	{
		float x;
		float y;
		float z;
		float w;

		Multiplication(m44,x,y,z,w);

		return vector4f(x,y,z,w);
	}
	inline vector4<T> operator- (const vector4<T>& input ) const
	{
		return vector4f(m_x - input.m_x, m_y - input.m_y, m_z - input.m_z, m_w - input.m_w );
	}
	inline vector4<T> operator/ ( T input ) const
	{
		return vector4f(m_x / input, m_y / input, m_z / input, m_w / input);
	}
	vector4<T>& operator= (const vector4<T>& input )
	{
		data[0] = input.data[0];
		data[1] = input.data[1];
		data[2] = input.data[2];
		data[3] = input.data[3];
		return *this;
	}
	inline vector4<T>& operator-= ( const vector4<T>& input )
	{
		m_x -= input.m_x;
		m_y -= input.m_y;
		m_z -= input.m_z;
		m_w -= input.m_w;
		return *this;
	}
	inline vector4<T>& operator+= ( const vector4<T>& input )
	{
		m_x += input.m_x;
		m_y += input.m_y;
		m_z += input.m_z;
		m_w += input.m_w;
		return *this;
	}
	inline vector4<T>& operator*= (const Matrix44f& m44 )
	{
		Multiplication( m44, data[0], data[1], data[2], data[3]);
		return *this;
	}
	inline vector4<T>& operator/= ( T input )
	{
		m_x /= input;
		m_y /= input;
		m_z /= input;
		m_w /= input;
		return *this;
	}
	inline bool operator == (const vector4<T>& rhs ) const
	{
		return ( m_x == rhs.m_x ) & ( m_y == rhs.m_y ) & ( m_z == rhs.m_z ) & ( m_w == rhs.m_w );
	}
public:
	inline void RotateByVector (const vector3f& vector , const float radian)
	{
		*this *= Matrix44f::CreateRotateMatrix(vector,radian);
	}
	inline void MoveByDirect( const vector3f& vector, const float distance)
	{
		*this *= Matrix44f::CreateMoveMatrix(vector, distance);
	}
public:
	inline const float* GetVectorArray() const
	{
		return data;
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

//-------------------------------------------------------------------------------------------------------
class Color : public vector4f
{
public:
	Color(){}
	Color(clamp r, clamp g, clamp b, clamp a)
		:vector4f(Clamp0to1(r),
		Clamp0to1(g),
		Clamp0to1(b),
		Clamp0to1(a))
	{
	}
	Color(const Color& input)
		:vector4f(input)
	{
	}


//内置颜色
public:
	static Color BloodRed;
	static Color Gray;
};