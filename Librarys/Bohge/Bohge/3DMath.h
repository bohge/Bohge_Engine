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


#include <stdlib.h>
#include <string.h>


//注意，全部的顶点类型不能有任何的虚函数，应为顶点类型最终是通过opengl的内存指针的形式被指定出来，然后进行绘制
//这种形式要求数据在内存上是连续的，任何虚函数可能导致数据的不连续，所以不能使用虚函数
//2x2矩阵计算4x4其次矩阵的逆时使用，暂时如此
#include "Matrix22.hpp"
#include "Matrix33.hpp"
#include "Matrix44.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include "Quaternion.hpp"
#include "BaseMathImplement.hpp"

namespace BohgeEngine
{
	class Color : public vector4f
	{
	public:
		BOHGE_FORCEINLINE Color(){}
		BOHGE_FORCEINLINE Color(clamp r, clamp g, clamp b, clamp a)
			:vector4f(r,g,b,a)
		{
		}
		BOHGE_FORCEINLINE Color(const Color& input)
			:vector4f(input)
		{
		}
		//内置颜色
	public:
		static const Color BloodRed;
		static const Color Gray;
		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Blue;
		static const Color Green;
	};

}

