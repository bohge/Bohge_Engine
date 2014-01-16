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
#ifndef MATRIX33
#define MATRIX33
#else
#error( "Do not include this file in other files" )
#endif


namespace BohgeEngine
{
	template<typename T>
	class Matrix33
	{
	public:
		union
		{
			struct 
			{
				T a11,a12,a13;
				T a21,a22,a23;
				T a31,a32,a33;
			};
			T data[9];
		};
	public:
		Matrix33<T>()
		{
			a11 = 1.0;
			a12 = 0;
			a13 = 0;

			a21 = 0;
			a22 = 1.0;
			a23 = 0;

			a31 = 0;
			a32 = 0;
			a33 = 1.0;
		}
		Matrix33<T>( const T& a11,const T& a12,const T& a13,
			const T& a21,const T& a22,const T& a23,
			const T& a31,const T& a32,const T& a33)
		{
			data[0] = a11;
			data[1] = a12;
			data[2] = a13;

			data[3] = a21;
			data[4] = a22;
			data[5] = a23;

			data[6] = a31;
			data[7] = a32;
			data[8] = a33;
		}
		Matrix33<T>(const Matrix33<T>& input)
		{
			memcpy( data, input.data, sizeof(T)*9 );
		}
		explicit Matrix33<T>(const Matrix44<T>& input)
		{
			for(int i = 0 ; i < 3 ; i ++)
			{
				for( int j = 0; j < 3 ; j ++)
				{
					data[ i*3 + j ] = input.data[ i*4 + j ];
				}
			}
		}
	public:
		BOHGE_FORCEINLINE Matrix33<T>& YawPitchRoll(const T& yaw, const T& pitch, const T& roll)
		{
			//http://planning.cs.uiuc.edu/node102.html
			//float cosA = cos(yaw);
			//float sinA = sin(yaw);
			//float cosB = cos(pitch);
			//float sinB = sin(pitch);
			//float cosY = cos(roll);
			//float sinY = sin(roll);
			//a11 = cosA * cosB;
			//a12 = cosA * sinB * sinY - sinA * cosY;
			//a33 = cosA * sinB * cosY + sinA * sinY;
			//a21 = sinA * cosB;
			//a22 = sinA * sinB * sinY + cosA * cosY;
			//a23 = sinA * sinB * cosY - cosA * sinY;
			//a31 = -sinB;
			//a32 = cosB * sinY;
			//a33 = cosB * cosY;

			////http://www.songho.ca/opengl/gl_anglestoaxes.html
			////关于右手笛卡尔坐标系的 x-, y- 和 z-轴的旋转分别叫做 pitch, yaw 和 roll 旋转
			//float cosA = cos(pitch);
			//float sinA = sin(pitch);
			//float cosB = cos(yaw);
			//float sinB = sin(yaw);
			//float cosC = cos(roll);
			//float sinC = sin(roll);
			//a11 = cosB * cosC;
			//a12 = - cosB * sinC;
			//a33 =  sinB;
			//a21 = sinA * sinB * cosC + cosA * sinC;
			//a22 = - sinA * sinB * sinC + cosA * cosC;
			//a23 = - sinA * cosB;
			//a31 = - cosA * sinB * cosC + sinA * sinC;
			//a32 = cosA * sinB * sinC + sinA * cosC;
			//a33 = cosA * cosB;

			Matrix33<T> x = Matrix33<T>::CreateRotateMatrixX(pitch);
			Matrix33<T> y = Matrix33<T>::CreateRotateMatrixY(yaw);
			Matrix33<T> z = Matrix33<T>::CreateRotateMatrixZ(roll);
			*this = z * y * x;
			return *this;
		}
		BOHGE_FORCEINLINE Matrix33<T>& operator= (const Matrix33<T>& m33 )
		{
			memcpy( data, m33.data, sizeof(T)*9 );
			return *this;
		}
		BOHGE_FORCEINLINE Matrix33<T> operator* (const Matrix33<T>& m33 ) const
		{
			return Matrix33<T>(
				a11 * m33.a11 + a12 * m33.a21 + a13 * m33.a31,
				a11 * m33.a12 + a12 * m33.a22 + a13 * m33.a32,
				a11 * m33.a13 + a12 * m33.a23 + a13 * m33.a33,

				a21 * m33.a11 + a22 * m33.a21 + a23 * m33.a31,
				a21 * m33.a12 + a22 * m33.a22 + a23 * m33.a32,
				a21 * m33.a13 + a22 * m33.a23 + a23 * m33.a33,

				a31 * m33.a11 + a32 * m33.a21 + a33 * m33.a31,
				a31 * m33.a12 + a32 * m33.a22 + a33 * m33.a32,
				a31 * m33.a13 + a32 * m33.a23 + a33 * m33.a33
				);
		}
	public:
		BOHGE_FORCEINLINE const T* GetMatrixArray() const
		{
			return data;
		}
	public:
		static Matrix33<T> CreateRotateMatrixX( const T& radian)
		{
			//T sx = sin(radian);
			//T cx = cos(radian);
			T sc[2];
			Math::SinCos( radian, sc );
			return Matrix33<T>(
				1,	0,		0,
				0,	sc[1],	sc[0],
				0,	-sc[0],	sc[1]);
		}
		static Matrix33<T> CreateRotateMatrixY( const T& radian)
		{
			//T sy = sin(radian);
			//T cy = cos(radian);
			T sc[2];
			Math::SinCos( radian, sc );
			return Matrix33<T>(
				sc[1],	0,		-sc[0],
				0,		1,		0,
				sc[0],	0,		sc[1]);
		}
		static Matrix33<T> CreateRotateMatrixZ( const T& radian)
		{
			//T sz = sin(radian);
			//T cz = cos(radian);
			T sc[2];
			Math::SinCos( radian, sc );
			return Matrix33<T>(
				sc[1],	sc[0],	0,
				-sc[0],	sc[1],	0,
				0,		0,		1);
		}
	};
}