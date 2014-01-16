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
//			State Manage class		   //
/////////////////////////////////////////



#include "3DMath.h"



/////////////////////////////////
//			Matrix44f		   //
/////////////////////////////////
Matrix44f::Matrix44f()
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
//-------------------------------------------------------------------------------------------------------
Matrix44f::Matrix44f(float a11,float a12,float a13,float a14,
					 float a21,float a22,float a23,float a24,
					 float a31,float a32,float a33,float a34,
					 float a41,float a42,float a43,float a44)
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
//-------------------------------------------------------------------------------------------------------
Matrix44f::Matrix44f(const Matrix44f& input)
{
	for(int i = 0 ; i < 4 ; i ++)
	{
		for( int j = 0; j < 4 ; j ++)
		{
			data[ i*4 + j ] = input.data[ i*4 + j ];
		}
	}
}
//-------------------------------------------------------------------------------------------------------
Matrix44f Matrix44f::operator* (const Matrix44f& m44 ) const
{

	return Matrix44f(	data[0]*m44.data[0] + data[1]*m44.data[4] + data[2]*m44.data[8] + data[3]*m44.data[12],
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
						data[12]*m44.data[3] + data[13]*m44.data[7] + data[14]*m44.data[11] + data[15]*m44.data[15]);
}
//-------------------------------------------------------------------------------------------------------
Matrix44f Matrix44f::operator* (float num) const
{
	return Matrix44f(	a11*num, a12*num, a13*num, a14*num,
						a21*num, a22*num, a23*num, a24*num,
						a31*num, a32*num, a33*num, a34*num,
						a41*num, a42*num, a43*num, a44*num);
}
//-------------------------------------------------------------------------------------------------------
Matrix44f Matrix44f::operator/ (float num) const
{
	ASSERT( false == isZero(num) );
	num = 1.0f/num;
	return Matrix44f(	a11*num, a12*num, a13*num, a14*num,
						a21*num, a22*num, a23*num, a24*num,
						a31*num, a32*num, a33*num, a34*num,
						a41*num, a42*num, a43*num, a44*num);
}
//-------------------------------------------------------------------------------------------------------
bool Matrix44f::Partitioning(Matrix44f& out)
{
	Matrix22f P(a11,a12,
				a21,a22);
	Matrix22f InvP;
	P.GetInverse(InvP);

	Matrix22f Q(a13,a14,
				a23,a24);
	
	Matrix22f R(a31,a32,
				a41,a42);

	Matrix22f S(a33,a34,
				a43,a44);
	//计算此矩阵的行列式，来判断是否有此矩阵的逆矩阵
	Matrix22f RxInvP = R * InvP;	// R * p^-1
	Matrix22f RxInvPxQ = RxInvP * Q;	// R * P^-1 * Q
	Matrix22f S_RxInvPxQ = S - RxInvPxQ;// S - R * P^-1 * Q
	//此处该矩阵的det = |P| * |S - R * P^-1 * Q|,
	//但是本类中能走到这里说明|P|不为0，所以为了省一点点的性能，此处只判断|S - R * P^-1 * Q|
	if( true == isZero(S_RxInvPxQ.CalculateDet()))
	{
		return false;//没有逆矩阵;
	}
	Matrix22f InvPxQ = InvP * Q;	//P^-1 * Q

	//计算逆矩阵中的成员
	Matrix22f ST;
	S_RxInvPxQ.GetInverse(ST);	// ST = (S - R * P^-1 * Q)^-1
	Matrix22f RT = - ST * RxInvP;	// RT = - ST * ( R * P^-1 )
	Matrix22f QT = - InvPxQ * ST;	// QT = - ( P^-1 * Q ) * ST
	Matrix22f PT = InvP - InvPxQ * RT;// PT = P^-1 - ( P^-1 * Q ) * RT

	out = Matrix44f(PT.a11, PT.a12, QT.a11, QT.a12,
					PT.a21, PT.a22, QT.a21, QT.a22,
					RT.a11, RT.a12, ST.a11, ST.a12,
					RT.a21, RT.a22, ST.a21, ST.a22);
	return true;
}
//-------------------------------------------------------------------------------------------------------
bool Matrix44f::CramersRule(Matrix44f& out)
{
	float tmp[12];	//临时储存
	float Src[16];	//源矩阵的转制矩阵
	
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
	Matrix44f Rlt;
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
	float det=Src[0]*Rlt.data[0]+Src[1]*Rlt.data[1]+Src[2]*Rlt.data[2]+Src[3]*Rlt.data[3];
	if( false == isZero(det))
	{
		out = Rlt / det;
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------
bool Matrix44f::GetInverse(Matrix44f& out)
{
	//计算该矩阵的逆，首先使用partitioning方法，详情请见
	//www.freevec.org/function/inverse_matrix_4x4_using_partitioning
	if(	false == isZero( a11*a22 - a12*a21 ) )//分割算法首先计算44矩阵的第一部分是否可逆,既行列式是否为零
	{
		if( true == Partitioning(out))
		{
			return true;
		}
		return false;
	}
	else//如果不存在逆，使用传统的标准的方法 Cramer's rule
	{
		if(true == CramersRule(out))
		{
			return true;
		}
		return false;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------
bool Matrix44f::InverseSelf()
{
	if(true == this->GetInverse(*this))
	{
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------
Matrix44f Matrix44f::CreateRotateMatrix(const vector3f &vector, const float radian)
{
	//rotation matrix by any axis
	//|Nx^2(1-cosA)+cosA   ,NyNx(1-cosA)-NzsinA ,NzNx(1-cosA)+NysinA |
	//|NxNy(1-cosA)+NzsinA ,Ny^2(1-cosA)+cosA   ,NzNy(1-cosA)-NxsinA |
	//|NxNz(1-cosA)-NysinA ,NyNz(1-cosA)+NxsinA ,Nz^2(1-cosA)+cosA   |
	float ca = cosf(radian);
	float sa = sinf(radian);
	float oca = 1 - ca;

	vector3f n(vector);
	n.nomral();

	return Matrix44f(	n.data[0]*n.data[0]*oca+ca       ,		n.data[1]*n.data[0]*oca+n.data[2]*sa ,	n.data[2]*n.data[0]*oca-n.data[1]*sa, 0,
						n.data[0]*n.data[1]*oca-n.data[2]*sa ,	n.data[1]*n.data[1]*oca+ca       ,		n.data[2]*n.data[1]*oca+n.data[0]*sa, 0,
						n.data[0]*n.data[2]*oca+n.data[1]*sa ,	n.data[1]*n.data[2]*oca-n.data[0]*sa ,	n.data[2]*n.data[2]*oca+ca,		0,
						0,							0,							0,						1.0);
}
//-------------------------------------------------------------------------------------------------------
Matrix44f Matrix44f::CreateMoveMatrix(const vector3f &vector, const float distance)
{
	return Matrix44f(1.0,					0,						0,					0,
					0,						1.0,					0,					0,
					0,						0,						1.0,				0,
					vector.data[0] * distance, vector.data[1] * distance, vector.data[2] * distance, 1.0);
}
//-------------------------------------------------------------------------------------------------------
Matrix44f Matrix44f::MoveTo(const vector3f& position)
{
	return Matrix44f(1.0,			0,				0,				0,
					0,				1.0,			0,				0,
					0,				0,				1.0,			0,
					position.m_x,	position.m_y,	position.m_z,	1.0);
}
//-------------------------------------------------------------------------------------------------------



/////////////////////////////////
//			Matrix33f		   //
/////////////////////////////////
Matrix33f::Matrix33f()
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
//-------------------------------------------------------------------------------------------------------
Matrix33f::Matrix33f(	float a11,float a12,float a13,
						float a21,float a22,float a23,
						float a31,float a32,float a33)
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
//-------------------------------------------------------------------------------------------------------
Matrix33f::Matrix33f(const Matrix33f& input)
{
	for(int i = 0 ; i < 3 ; i ++)
	{
		for( int j = 0; j < 3 ; j ++)
		{
			data[ i*3 + j ] = input.data[ i*3 + j ];
		}
	}
}
//-------------------------------------------------------------------------------------------------------
Matrix33f::Matrix33f(const Matrix44f& input)
{
	for(int i = 0 ; i < 3 ; i ++)
	{
		for( int j = 0; j < 3 ; j ++)
		{
			data[ i*3 + j ] = input.data[ i*4 + j ];
		}
	}
}



/////////////////////////////////
//			vector3f		   //
/////////////////////////////////
//-------------------------------------------------------------------------------------------------------
vector3f Vector3fCross(const vector3f& v1, const vector3f& v2)
{
	return vector3f(	v1.data[1] * v2.data[2] - v1.data[2] * v2.data[1],
		v1.data[2] * v2.data[0] - v1.data[0] * v2.data[2],
		v1.data[0] * v2.data[1] - v1.data[1] * v2.data[0]);
}
//-------------------------------------------------------------------------------------------------------
float Vector3fDot(const vector3f& v1, const vector3f& v2)
{
	return ( v1.data[0] * v2.data[0] + v1.data[1] * v2.data[1] + v1.data[2] * v2.data[2] );
}





Color Color::BloodRed(0.412,0.341,0.318,1.0);
Color Color::Gray(0.5,0.5,0.5,1.0);