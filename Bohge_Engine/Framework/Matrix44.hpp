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
#ifndef MATRIX44
#define MATRIX44
#else
#error( "Do not include this file in other files" )
#endif
#include "Volume.hpp"




namespace BohgeEngine
{
	//4x4的齐次坐标系矩阵
	template <typename T>
	class Matrix44 : public Volume
	{
		RTTI_TYPE_DECLEAR( Matrix44 )
	public:
		union
		{
			struct 
			{
				T a11,a12,a13,a14;
				T a21,a22,a23,a24;
				T a31,a32,a33,a34;
				T a41,a42,a43,a44;
			};
			T data[16];
		};
	public:
		Matrix44<T>()
		{
			data[0] = 1.0;
			data[1] = 0;
			data[2] = 0;
			data[3] = 0;

			data[4] = 0;
			data[5] = 1.0;
			data[6] = 0;
			data[7] = 0;

			data[8] = 0;
			data[9] = 0;
			data[10] = 1.0;
			data[11] = 0;

			data[12] = 0;
			data[13] = 0;
			data[14] = 0;
			data[15] = 1.0;
		}
		Matrix44<T>( const T& a11,const T& a12,const T& a13,const T& a14,
			const T& a21,const T& a22,const T& a23,const T& a24,
			const T& a31,const T& a32,const T& a33,const T& a34,
			const T& a41,const T& a42,const T& a43,const T& a44)
		{
			data[0] = a11;
			data[1] = a12;
			data[2] = a13;
			data[3] = a14;

			data[4] = a21;
			data[5] = a22;
			data[6] = a23;
			data[7] = a24;

			data[8] = a31;
			data[9] = a32;
			data[10] = a33;
			data[11] = a34;

			data[12] = a41;
			data[13] = a42;
			data[14] = a43;
			data[15] = a44;
		}
	private:
		bool Partitioning(Matrix44<T>& out) const
		{
			Matrix22<T> P(a11,a12,
				a21,a22);
			Matrix22<T> InvP;
			P.GetInverse(InvP);

			Matrix22<T> Q(a13,a14,
				a23,a24);

			Matrix22<T> R(a31,a32,
				a41,a42);

			Matrix22<T> S(a33,a34,
				a43,a44);
			//计算此矩阵的行列式，来判断是否有此矩阵的逆矩阵
			Matrix22<T> RxInvP = R * InvP;	// R * p^-1
			Matrix22<T> RxInvPxQ = RxInvP * Q;	// R * P^-1 * Q
			Matrix22<T> S_RxInvPxQ = S - RxInvPxQ;// S - R * P^-1 * Q
			//此处该矩阵的det = |P| * |S - R * P^-1 * Q|,
			//但是本类中能走到这里说明|P|不为0，所以为了省一点点的性能，此处只判断|S - R * P^-1 * Q|
			if( true == Math::isZero(S_RxInvPxQ.CalculateDet()))
			{
				return false;//没有逆矩阵;
			}
			Matrix22<T> InvPxQ = InvP * Q;	//P^-1 * Q

			//计算逆矩阵中的成员
			Matrix22<T> ST;
			S_RxInvPxQ.GetInverse(ST);	// ST = (S - R * P^-1 * Q)^-1
			Matrix22<T> RT = - ST * RxInvP;	// RT = - ST * ( R * P^-1 )
			Matrix22<T> QT = - InvPxQ * ST;	// QT = - ( P^-1 * Q ) * ST
			Matrix22<T> PT = InvP - InvPxQ * RT;// PT = P^-1 - ( P^-1 * Q ) * RT

			out = Matrix44<T>(PT.a11, PT.a12, QT.a11, QT.a12,
				PT.a21, PT.a22, QT.a21, QT.a22,
				RT.a11, RT.a12, ST.a11, ST.a12,
				RT.a21, RT.a22, ST.a21, ST.a22);
			return true;
		}
		bool CramersRule(Matrix44<T>& out) const
		{
			T tmp[12];	//临时储存
			T Src[16];	//源矩阵的转制矩阵

			//转换矩阵 行转换到序
			for(uint i = 0 ; i < 4 ; i++ )
			{
				Src[i]		= data[i*4];
				Src[i+4]	= data[i*4+1];
				Src[i+8]	= data[i*4+2];
				Src[i+12]	= data[i*4+3];
			}
			//计算逆矩阵的头8个配对数值
			tmp[0]	= Src[10] * Src[15];
			tmp[1]	= Src[11] * Src[14];
			tmp[2]	= Src[9]  * Src[15];
			tmp[3]	= Src[11] * Src[13];
			tmp[4]	= Src[9]  * Src[14];
			tmp[5]	= Src[10] * Src[13];
			tmp[6]	= Src[8]  * Src[15];
			tmp[7]	= Src[11] * Src[12];
			tmp[8]	= Src[8]  * Src[14];
			tmp[9]	= Src[10] * Src[12];
			tmp[10]	= Src[8]  * Src[13];
			tmp[11]	= Src[9]  * Src[12];

			//计算头8位逆矩阵
			Matrix44<T> Rlt;
			Rlt.data[0]  = tmp[0]*Src[5] + tmp[3]*Src[6] + tmp[4]*Src[7];
			Rlt.data[0] -= tmp[1]*Src[5] + tmp[2]*Src[6] + tmp[5]*Src[7];
			Rlt.data[1]  = tmp[1]*Src[4] + tmp[6]*Src[6] + tmp[9]*Src[7];
			Rlt.data[1] -= tmp[0]*Src[4] + tmp[7]*Src[6] + tmp[8]*Src[7];
			Rlt.data[2]  = tmp[2]*Src[4] + tmp[7]*Src[5] + tmp[10]*Src[7];
			Rlt.data[2] -= tmp[3]*Src[4] + tmp[6]*Src[5] + tmp[11]*Src[7];
			Rlt.data[3]  = tmp[5]*Src[4] + tmp[8]*Src[5] + tmp[11]*Src[6];
			Rlt.data[3] -= tmp[4]*Src[4] + tmp[9]*Src[5] + tmp[10]*Src[6];
			Rlt.data[4]  = tmp[1]*Src[1] + tmp[2]*Src[2] + tmp[5]*Src[3];
			Rlt.data[4] -= tmp[0]*Src[1] + tmp[3]*Src[2] + tmp[4]*Src[3];
			Rlt.data[5]  = tmp[0]*Src[0] + tmp[7]*Src[2] + tmp[8]*Src[3];
			Rlt.data[5] -= tmp[1]*Src[0] + tmp[6]*Src[2] + tmp[9]*Src[3];
			Rlt.data[6]  = tmp[3]*Src[0] + tmp[6]*Src[1] + tmp[11]*Src[3];
			Rlt.data[6] -= tmp[2]*Src[0] + tmp[7]*Src[1] + tmp[10]*Src[3];
			Rlt.data[7]  = tmp[4]*Src[0] + tmp[9]*Src[1] + tmp[10]*Src[2];
			Rlt.data[7] -= tmp[5]*Src[0] + tmp[8]*Src[1] + tmp[11]*Src[2];

			/* calculate pairs for second 8 elements (cofactors) */
			tmp[0]  = Src[2]*Src[7];
			tmp[1]  = Src[3]*Src[6];
			tmp[2]  = Src[1]*Src[7];
			tmp[3]  = Src[3]*Src[5];
			tmp[4]  = Src[1]*Src[6];
			tmp[5]  = Src[2]*Src[5];
			tmp[6]  = Src[0]*Src[7];
			tmp[7]  = Src[3]*Src[4];
			tmp[8]  = Src[0]*Src[6];
			tmp[9]  = Src[2]*Src[4];
			tmp[10] = Src[0]*Src[5];
			tmp[11] = Src[1]*Src[4];
			/* calculate second 8 elements (cofactors) */
			Rlt.data[8]  = tmp[0]*Src[13] + tmp[3]*Src[14] + tmp[4]*Src[15];
			Rlt.data[8] -= tmp[1]*Src[13] + tmp[2]*Src[14] + tmp[5]*Src[15];
			Rlt.data[9]  = tmp[1]*Src[12] + tmp[6]*Src[14] + tmp[9]*Src[15];
			Rlt.data[9] -= tmp[0]*Src[12] + tmp[7]*Src[14] + tmp[8]*Src[15];
			Rlt.data[10] = tmp[2]*Src[12] + tmp[7]*Src[13] + tmp[10]*Src[15];
			Rlt.data[10]-= tmp[3]*Src[12] + tmp[6]*Src[13] + tmp[11]*Src[15];
			Rlt.data[11] = tmp[5]*Src[12] + tmp[8]*Src[13] + tmp[11]*Src[14];
			Rlt.data[11]-= tmp[4]*Src[12] + tmp[9]*Src[13] + tmp[10]*Src[14];
			Rlt.data[12] = tmp[2]*Src[10] + tmp[5]*Src[11] + tmp[1]*Src[9];
			Rlt.data[12]-= tmp[4]*Src[11] + tmp[0]*Src[9] + tmp[3]*Src[10];
			Rlt.data[13] = tmp[8]*Src[11] + tmp[0]*Src[8] + tmp[7]*Src[10];
			Rlt.data[13]-= tmp[6]*Src[10] + tmp[9]*Src[11] + tmp[1]*Src[8];
			Rlt.data[14] = tmp[6]*Src[9] + tmp[11]*Src[11] + tmp[3]*Src[8];
			Rlt.data[14]-= tmp[10]*Src[11] + tmp[2]*Src[8] + tmp[7]*Src[9];
			Rlt.data[15] = tmp[10]*Src[10] + tmp[4]*Src[8] + tmp[9]*Src[9];
			Rlt.data[15]-= tmp[8]*Src[9] + tmp[11]*Src[10] + tmp[5]*Src[8];
			/* calculate determinant */
			T det=Src[0]*Rlt.data[0]+Src[1]*Rlt.data[1]+Src[2]*Rlt.data[2]+Src[3]*Rlt.data[3];
			if( false == Math::isZero(det))
			{
				out = Rlt / det;
				return true;
			}
			return false;
		}
	public:
		BOHGE_FORCEINLINE void SetTransform( const vector3<T>& trans )
		{
			memcpy( &a41, trans.Data(), sizeof(T)*3 );
		}
		Matrix44<T> operator* (const Matrix44<T>& m44 ) const
		{
			return Matrix44<T>(	data[0]*m44.data[0] + data[1]*m44.data[4] + data[2]*m44.data[8] + data[3]*m44.data[12],
				data[0]*m44.data[1] + data[1]*m44.data[5] + data[2]*m44.data[9] + data[3]*m44.data[13],
				data[0]*m44.data[2] + data[1]*m44.data[6] + data[2]*m44.data[10] + data[3]*m44.data[14],
				data[0]*m44.data[3] + data[1]*m44.data[7] + data[2]*m44.data[11] + data[3]*m44.data[15],

				data[4]*m44.data[0] + data[5]*m44.data[4] + data[6]*m44.data[8] + data[7]*m44.data[12],
				data[4]*m44.data[1] + data[5]*m44.data[5] + data[6]*m44.data[9] + data[7]*m44.data[13],
				data[4]*m44.data[2] + data[5]*m44.data[6] + data[6]*m44.data[10] + data[7]*m44.data[14],
				data[4]*m44.data[3] + data[5]*m44.data[7] + data[6]*m44.data[11] + data[7]*m44.data[15],

				data[8]*m44.data[0] + data[9]*m44.data[4] + data[10]*m44.data[8] + data[11]*m44.data[12],
				data[8]*m44.data[1] + data[9]*m44.data[5] + data[10]*m44.data[9] + data[11]*m44.data[13],
				data[8]*m44.data[2] + data[9]*m44.data[6] + data[10]*m44.data[10] + data[11]*m44.data[14],
				data[8]*m44.data[3] + data[9]*m44.data[7] + data[10]*m44.data[11] + data[11]*m44.data[15],

				data[12]*m44.data[0] + data[13]*m44.data[4] + data[14]*m44.data[8] + data[15]*m44.data[12],
				data[12]*m44.data[1] + data[13]*m44.data[5] + data[14]*m44.data[9] + data[15]*m44.data[13],
				data[12]*m44.data[2] + data[13]*m44.data[6] + data[14]*m44.data[10] + data[15]*m44.data[14],
				data[12]*m44.data[3] + data[13]*m44.data[7] + data[14]*m44.data[11] + data[15]*m44.data[15]
			);
		}
		Matrix44<T>& operator*= (const Matrix44<T>& m44 )
		{
			*this = Matrix44<T>( data[0]*m44.data[0] + data[1]*m44.data[4] + data[2]*m44.data[8] + data[3]*m44.data[12],
				data[0]*m44.data[1] + data[1]*m44.data[5] + data[2]*m44.data[9] + data[3]*m44.data[13],
				data[0]*m44.data[2] + data[1]*m44.data[6] + data[2]*m44.data[10] + data[3]*m44.data[14],
				data[0]*m44.data[3] + data[1]*m44.data[7] + data[2]*m44.data[11] + data[3]*m44.data[15],

				data[4]*m44.data[0] + data[5]*m44.data[4] + data[6]*m44.data[8] + data[7]*m44.data[12],
				data[4]*m44.data[1] + data[5]*m44.data[5] + data[6]*m44.data[9] + data[7]*m44.data[13],
				data[4]*m44.data[2] + data[5]*m44.data[6] + data[6]*m44.data[10] + data[7]*m44.data[14],
				data[4]*m44.data[3] + data[5]*m44.data[7] + data[6]*m44.data[11] + data[7]*m44.data[15],

				data[8]*m44.data[0] + data[9]*m44.data[4] + data[10]*m44.data[8] + data[11]*m44.data[12],
				data[8]*m44.data[1] + data[9]*m44.data[5] + data[10]*m44.data[9] + data[11]*m44.data[13],
				data[8]*m44.data[2] + data[9]*m44.data[6] + data[10]*m44.data[10] + data[11]*m44.data[14],
				data[8]*m44.data[3] + data[9]*m44.data[7] + data[10]*m44.data[11] + data[11]*m44.data[15],

				data[12]*m44.data[0] + data[13]*m44.data[4] + data[14]*m44.data[8] + data[15]*m44.data[12],
				data[12]*m44.data[1] + data[13]*m44.data[5] + data[14]*m44.data[9] + data[15]*m44.data[13],
				data[12]*m44.data[2] + data[13]*m44.data[6] + data[14]*m44.data[10] + data[15]*m44.data[14],
				data[12]*m44.data[3] + data[13]*m44.data[7] + data[14]*m44.data[11] + data[15]*m44.data[15]
			);
			return *this;
		}
		BOHGE_FORCEINLINE Matrix44<T> operator/ ( T n ) const
		{
			ASSERT( false == Math::isZero(n) );
			T num = T(1.0)/n;
			return Matrix44<T>(	a11*num, a12*num, a13*num, a14*num,
				a21*num, a22*num, a23*num, a24*num,
				a31*num, a32*num, a33*num, a34*num,
				a41*num, a42*num, a43*num, a44*num);
		}
		BOHGE_FORCEINLINE Matrix44<T>& operator*= ( T num )
		{
			for ( int i = 0; i < 16; i ++ )
			{
				data[i] *= num;
			}
			return *this;
		}
		BOHGE_FORCEINLINE Matrix44<T>& operator/= ( T num )
		{
			for ( int i = 0; i < 16; i ++ )
			{
				data[i] /= num;
			}
			return *this;
		}
	public:
		friend BOHGE_FORCEINLINE Matrix44<T> operator* ( T num, const Matrix44<T>& rhs )
		{
			return Matrix44<T>(	rhs.a11*num, rhs.a12*num, rhs.a13*num, rhs.a14*num,
				rhs.a21*num, rhs.a22*num, rhs.a23*num, rhs.a24*num,
				rhs.a31*num, rhs.a32*num, rhs.a33*num, rhs.a34*num,
				rhs.a41*num, rhs.a42*num, rhs.a43*num, rhs.a44*num);
		}
		friend BOHGE_FORCEINLINE Matrix44<T> operator* ( const Matrix44<T>& rhs, T num )
		{
			return Matrix44<T>(	rhs.a11*num, rhs.a12*num, rhs.a13*num, rhs.a14*num,
				rhs.a21*num, rhs.a22*num, rhs.a23*num, rhs.a24*num,
				rhs.a31*num, rhs.a32*num, rhs.a33*num, rhs.a34*num,
				rhs.a41*num, rhs.a42*num, rhs.a43*num, rhs.a44*num);
		}
	public:
		BOHGE_FORCEINLINE bool GetInverse(Matrix44<T>& out, bool isQuick = true) const
		{
			//这个矩阵算法有些问题，在一些特殊矩阵可能导致计算出错，在摄像机这种可以旋转的东西上面还是不要用了
			//计算该矩阵的逆，首先使用partitioning方法，详情请见
			//www.freevec.org/function/inverse_matrix_4x4_using_partitioning
			if( isQuick )
			{
				if(	false == Math::isZero( a11*a22 - a12*a21 ) )//分割算法首先计算44矩阵的第一部分是否可逆,既行列式是否为零
				{
					if( true == Partitioning(out))
					{
						return true;
					}
				}
			}
			{//如果不存在逆，使用传统的标准的方法 Cramer's rule
				if(true == CramersRule(out))
				{
					return true;
				}
				return false;
			}
			return false;
		}
		BOHGE_FORCEINLINE bool InverseSelf()
		{
			return this->GetInverse(*this);
		}
		BOHGE_FORCEINLINE const Matrix44<T> Inverse() const
		{
			Matrix44<T> res = *this;
			res.InverseSelf();
			return res;
		}
		Quaternion<T> ToQuaternion() const
		{
			Quaternion<T> qut;
			T s;
			T const tr(a11 + a22 + a33);

			// check the diagonal
			if (tr > 0)
			{
				s = Math::Sqrt(tr + 1);
				qut.w() = s * T(0.5);
				s = T(0.5) / s;
				qut.x() = (a23 - a32) * s;
				qut.y() = (a31 - a13) * s;
				qut.z() = (a12 - a21) * s;
			}
			else
			{
				if ((a22 > a11) && (a33 <= a22))
				{
					s = Math::Sqrt((a22 - (a33 + a11)) + 1);

					qut.y() = s * T(0.5);

					if (!Math::isEqual<T>(s, 0))
					{
						s = T(0.5) / s;
					}

					qut.w() = (a31 - a13) * s;
					qut.z() = (a32 + a23) * s;
					qut.x() = (a12 + a21) * s;
				}
				else
				{
					if (((a22 <= a11) && (a33 > a11)) || (a33 > a22))
					{
						s = Math::Sqrt((a33 - (a11 + a22)) + 1);

						qut.z() = s * T(0.5);

						if (!Math::isEqual<T>(s, 0))
						{
							s = T(0.5) / s;
						}

						qut.w() = (a12 - a21) * s;
						qut.x() = (a13 + a31) * s;
						qut.y() = (a23 + a32) * s;
					}
					else
					{
						s = Math::Sqrt((a11 - (a22 + a33)) + 1);

						qut.x() = s * T(0.5);

						if (!Math::isEqual<T>(s, 0))
						{
							s = T(0.5) / s;
						}

						qut.w() = (a23 - a32) * s;
						qut.y() = (a21 + a12) * s;
						qut.z() = (a31 + a13) * s;
					}
				}
			}
			qut.NormalizeSelf();
			return qut;
		}
	public:
		BOHGE_FORCEINLINE bool operator== (const Matrix44<T>& m44 ) const
		{
			for ( int i = 0 ; i < 16 ; i ++ )
			{
				if( !Math::isEqual( data[i], m44.data[i] ) )
				{
					return false;
				}
			}
			return true;
		}
		BOHGE_FORCEINLINE bool operator!= (const Matrix44<T>& m44 ) const
		{
			return ! ( *this == m44 );
		}
	public:
		BOHGE_FORCEINLINE void Identitat()
		{
			memset( data, 0, 16 * sizeof( T ) );
			a11 = a22 = a33 = a44 = 1;
		}
		BOHGE_FORCEINLINE const T* GetMatrixArray() const
		{
			return data;
		}
	public:
		static Matrix44<T> CreateRotateMatrix(const vector3<T>& vector , const T radian)
		{
			//rotation matrix by any axis
			//|Nx^2(1-cosA)+cosA   ,NyNx(1-cosA)-NzsinA ,NzNx(1-cosA)+NysinA |
			//|NxNy(1-cosA)+NzsinA ,Ny^2(1-cosA)+cosA   ,NzNy(1-cosA)-NxsinA |
			//|NxNz(1-cosA)-NysinA ,NyNz(1-cosA)+NxsinA ,Nz^2(1-cosA)+cosA   |
			//T ca = cos(radian);
			//T sa = sin(radian);
			T sc[2];
			Math::SinCos( radian, sc );
			T oca = 1 - sc[1];

			vector3<T> n(vector);
			n.NormalizeSelf();

			return Matrix44<T>(	
				n.m_Data[0]*n.m_Data[0]*oca+sc[1]			,	n.m_Data[1]*n.m_Data[0]*oca+n.m_Data[2]*sc[0] ,	n.m_Data[2]*n.m_Data[0]*oca-n.m_Data[1]*sc[0],	0,
				n.m_Data[0]*n.m_Data[1]*oca-n.m_Data[2]*sc[0] ,	n.m_Data[1]*n.m_Data[1]*oca+sc[1]			,	n.m_Data[2]*n.m_Data[1]*oca+n.m_Data[0]*sc[0],	0,
				n.m_Data[0]*n.m_Data[2]*oca+n.m_Data[1]*sc[0] ,	n.m_Data[1]*n.m_Data[2]*oca-n.m_Data[0]*sc[0] ,	n.m_Data[2]*n.m_Data[2]*oca+sc[1],				0,
				0,										0,												0,											1.0);
		}
		static Matrix44<T> CreateRotateMatrixX(const T& radian)
		{
			//T sx = sin(radian);
			//T cx = cos(radian);
			T sc[2];
			Math::SinCos( radian, sc );
			return Matrix44<T>(
				1,	0,		0,		0,
				0,	sc[1],	sc[0],	0,
				0,	-sc[0],	sc[1],	0,
				0,	0,		0,		1);
		}
		static Matrix44<T> CreateRotateMatrixY(const T& radian)
		{
			//T sy = sin(radian);
			//T cy = cos(radian);
			T sc[2];
			Math::SinCos( radian, sc );
			return Matrix44<T>(
				sc[1],	0,		-sc[0],	0,
				0,		1,		0,		0,
				sc[0],	0,		sc[1],	0,
				0,		0,		0,		1);
		}
		static Matrix44<T> CreateRotateMatrixZ(const T& radian)
		{
			//T sz = sin(radian);
			//T cz = cos(radian);
			T sc[2];
			Math::SinCos( radian, sc );
			return Matrix44<T>(
				sc[1],	sc[0],	0,		0,
				-sc[0],	sc[1],	0,		0,
				0,		0,		1,		0,
				0,		0,		0,		1);
		}
		static Matrix44<T> CreateMoveMatrix(const vector3<T>& vector , const T& distance)
		{
			return Matrix44<T>(
				1.0,					0,						0,					0,
				0,						1.0,					0,					0,
				0,						0,						1.0,				0,
				vector.data[0] * distance, vector.data[1] * distance, vector.data[2] * distance, 1.0);
		}
		/*移动至*/
		static Matrix44<T> MoveTo(const vector3<T>& position)
		{
			return Matrix44<T>(1.0,			0,				0,				0,
				0,				1.0,			0,				0,
				0,				0,				1.0,			0,
				position.m_x,	position.m_y,	position.m_z,	1.0);
		}
	public:
		virtual void* Data()
		{
			return data;
		}
		virtual const void* Data() const
		{
			return data;
		}
		virtual uint Count() const
		{
			return 16;
		}
		static BOHGE_FORCEINLINE uint Size()
		{
			return 16;
		}
		static BOHGE_FORCEINLINE uint BitSize()
		{
			return 16 * sizeof(T);
		}
	};
}