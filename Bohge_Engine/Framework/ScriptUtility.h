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
#include "VariableType.h"
#include "Predefine.h"

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/map.h>

namespace BohgeEngine
{
	class Object;
	class FunctionRegister;
	typedef eastl::vector< FunctionRegister* > FunctionArray;//同名且函数参数相同的函数数组
	typedef eastl::map< uint, FunctionArray > OverlappedMap;//同名函数的重载函数列表
	typedef eastl::vector< eastl::pair< eastl::string, OverlappedMap > > ClassFuncitonArray;//函数列表
	typedef eastl::pair< eastl::string, Object* > CLassConstant;
	typedef eastl::vector< CLassConstant > ClassConstantArray;

	//计算函数ID
	BOHGE_FORCEINLINE uint CaculateFunctionID()
	{
		return 0;
	}
	BOHGE_FORCEINLINE uint CaculateFunctionID( uint id1 )
	{
		return 1 + id1;
	}
	BOHGE_FORCEINLINE uint CaculateFunctionID( uint id1, uint id2 )
	{
		return 2 + id1 + id2;
	}
	BOHGE_FORCEINLINE uint CaculateFunctionID( uint id1, uint id2, uint id3 )
	{
		return 3 + id1 + id2 + id3;
	}
	BOHGE_FORCEINLINE uint CaculateFunctionID( uint id1, uint id2, uint id3, uint id4 )
	{
		return 4 + id1 + id2 + id3 + id4;
	}
	BOHGE_FORCEINLINE uint CaculateFunctionID( uint id1, uint id2, uint id3, uint id4, uint id5 )
	{
		return 5 + id1 + id2 + id3 + id4 + id5;
	}
	BOHGE_FORCEINLINE uint CaculateFunctionID( uint id1, uint id2, uint id3, uint id4, uint id5, uint id6 )
	{
		return 6 + id1 + id2 + id3 + id4 + id5 + id6;
	}
	BOHGE_FORCEINLINE uint CaculateFunctionID( uint id1, uint id2, uint id3, uint id4, uint id5, uint id6, uint id7 )
	{
		return 7 + id1 + id2 + id3 + id4 + id5 + id6 + id7;
	}
	BOHGE_FORCEINLINE uint CaculateFunctionID( uint id1, uint id2, uint id3, uint id4, uint id5, uint id6, uint id7, uint id8 )
	{
		return 8 + id1 + id2 + id3 + id4 + id5 + id6 + id7 + id8;
	}
	BOHGE_FORCEINLINE uint CaculateFunctionID( uint id1, uint id2, uint id3, uint id4, uint id5, uint id6, uint id7, uint id8, uint id9 )
	{
		return 9 + id1 + id2 + id3 + id4 + id5 + id6 + id7 + id8 + id9;
	}
	BOHGE_FORCEINLINE uint CaculateFunctionID( uint id1, uint id2, uint id3, uint id4, uint id5, uint id6, uint id7, uint id8, uint id9, uint id10 )
	{
		return 10 + id1 + id2 + id3 + id4 + id5 + id6 + id7 + id8 + id9 + id10;
	}
}