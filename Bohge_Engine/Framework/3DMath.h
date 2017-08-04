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



///////////////////////////////////////////////////
//	数学库，请引用此头文件，不要引用其它拆分的实现  //
///////////////////////////////////////////////////
#pragma once
#include "Predefine.h"
#include "BaseMathDeclaration.h"
#include "ShapeMathDeclaration.h"

#include <stdlib.h>
#include <string.h>

#ifndef __cplusplus
#error( "Do not include this file in c files" )
#endif


//注意，全部的顶点类型不能有任何的虚函数，应为顶点类型最终是通过opengl的内存指针的形式被指定出来，然后进行绘制
//这种形式要求数据在内存上是连续的，任何虚函数可能导致数据的不连续，所以不能使用虚函数
//2x2矩阵计算4x4其次矩阵的逆时使用，暂时如此
#include "Matrix22.hpp"
#include "Matrix33.hpp"
#include "Matrix44.hpp"
#include "vector1.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include "Quaternion.hpp"
#include "Color.hpp"
#include "vector2array.hpp"
#include "vector3array.hpp"
#include "vector4array.hpp"
#include "BaseMathImplement.hpp"

#include "Ray.hpp"
#include "Line.hpp"
#include "Plane.hpp"
#include "Frustum.hpp"
#include "Aabbox.hpp"
#include "Aabrect.hpp"
#include "Aabround.hpp"
#include "Aabsphere.hpp"
#include "ShapeMathImplement.hpp"


#include "Bool.h"
#include "Number.h"
#include "String.h"
#include "Function.h"
#include "Instance.h"

namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( vector1f );
	RTTI_TYPE_IMPLEMENT( vector1d );
	RTTI_TYPE_IMPLEMENT( vector1b );

	RTTI_TYPE_IMPLEMENT( vector2f );
	RTTI_TYPE_IMPLEMENT( vector2d );
	RTTI_TYPE_IMPLEMENT( vector2b );
	RTTI_TYPE_IMPLEMENT( vector2arrayf );
	RTTI_TYPE_IMPLEMENT( vector2arrayd );
	RTTI_TYPE_IMPLEMENT( vector2arrayb );

	RTTI_TYPE_IMPLEMENT( vector3f );
	RTTI_TYPE_IMPLEMENT( vector3d );
	RTTI_TYPE_IMPLEMENT( vector3b );
	RTTI_TYPE_IMPLEMENT( vector3arrayf );
	RTTI_TYPE_IMPLEMENT( vector3arrayd );
	RTTI_TYPE_IMPLEMENT( vector3arrayb );

	RTTI_TYPE_IMPLEMENT( vector4f );
	RTTI_TYPE_IMPLEMENT( vector4d );
	RTTI_TYPE_IMPLEMENT( vector4b );
	RTTI_TYPE_IMPLEMENT( vector4arrayf );
	RTTI_TYPE_IMPLEMENT( vector4arrayd );
	RTTI_TYPE_IMPLEMENT( vector4arrayb );

	RTTI_TYPE_IMPLEMENT( Matrix22f );
	RTTI_TYPE_IMPLEMENT( Matrix22b );
	RTTI_TYPE_IMPLEMENT( Matrix22d );

	RTTI_TYPE_IMPLEMENT( Matrix33f );
	RTTI_TYPE_IMPLEMENT( Matrix33b );
	RTTI_TYPE_IMPLEMENT( Matrix33d );

	RTTI_TYPE_IMPLEMENT( Matrix44f );
	RTTI_TYPE_IMPLEMENT( Matrix44b );
	RTTI_TYPE_IMPLEMENT( Matrix44d );

	RTTI_TYPE_IMPLEMENT( Quaternionf );
	RTTI_TYPE_IMPLEMENT( Quaternionb );
	RTTI_TYPE_IMPLEMENT( Quaterniond );

	RTTI_TYPE_IMPLEMENT( Rayd );
	RTTI_TYPE_IMPLEMENT( Rayf );
	RTTI_TYPE_IMPLEMENT( Rayb );

	RTTI_TYPE_IMPLEMENT( Lined );
	RTTI_TYPE_IMPLEMENT( Linef );
	RTTI_TYPE_IMPLEMENT( Lineb );

	RTTI_TYPE_IMPLEMENT( Planed );
	RTTI_TYPE_IMPLEMENT( Planef );
	RTTI_TYPE_IMPLEMENT( Planeb );

	RTTI_TYPE_IMPLEMENT( Frustumd );
	RTTI_TYPE_IMPLEMENT( Frustumf );
	RTTI_TYPE_IMPLEMENT( Frustumb );

	RTTI_TYPE_IMPLEMENT( Aabbox3dd );
	RTTI_TYPE_IMPLEMENT( Aabbox3df );
	RTTI_TYPE_IMPLEMENT( Aabbox3db );

	RTTI_TYPE_IMPLEMENT( Aabrectd );
	RTTI_TYPE_IMPLEMENT( Aabrectf );
	RTTI_TYPE_IMPLEMENT( Aabrectb );

	RTTI_TYPE_IMPLEMENT( Aabroundd );
	RTTI_TYPE_IMPLEMENT( Aabroundf );
	RTTI_TYPE_IMPLEMENT( Aabroundb );

	RTTI_TYPE_IMPLEMENT( Aabsphered );
	RTTI_TYPE_IMPLEMENT( Aabspheref );
	RTTI_TYPE_IMPLEMENT( Aabsphereb );
	namespace Math
	{
		enum DataStructType
		{
			DST_VECTOR1 = 0,
			DST_VECTOR2,
			DST_VECTOR3,
			DST_VECTOR4,
			DST_MATRIX22,
			DST_MATRIX33,
			DST_MATRIX44,
		};
	}
}