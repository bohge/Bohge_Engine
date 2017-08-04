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



/////////////////////////////////////
//			类成员函数回调		   //
/////////////////////////////////////
#pragma once
#include "Predefine.h"

#include <EASTL/map.h>
#include <EASTL/list.h>



typedef void* FunctionPtr;

namespace BohgeEngine
{
	//////////////////////////
	//		无参数			//
	//////////////////////////
	#define NAME_FIX 
	#define HAVE_COMMA 
	#define EVENT_PARAM_TEMPLATE 
	#define EVENT_PARAM_TYPES_INPUTS 
	#define EVENT_PARAM_TYPES 
	#define EVENT_PARAM_INPUTS 

	#define EVENT_IMPLECATION
	#include "DelegateImplement.hpp"
	#undef EVENT_IMPLECATION
	//////////////////////////
	//		一个参数			//
	//////////////////////////
	#define NAME_FIX 1
	#define HAVE_COMMA ,
	#define EVENT_PARAM_TEMPLATE typename Arg
	#define EVENT_PARAM_TYPES_INPUTS Arg arg
	#define EVENT_PARAM_TYPES Arg
	#define EVENT_PARAM_INPUTS arg

	#define EVENT_IMPLECATION
	#include "DelegateImplement.hpp"
	#undef EVENT_IMPLECATION
	//////////////////////////
	//		两个参数			//
	//////////////////////////

	#define NAME_FIX 2
	#define HAVE_COMMA ,
	#define EVENT_PARAM_TEMPLATE typename Arg1, typename Arg2
	#define EVENT_PARAM_TYPES_INPUTS Arg1 arg1, Arg2 arg2
	#define EVENT_PARAM_TYPES Arg1, Arg2
	#define EVENT_PARAM_INPUTS arg1, arg2

	#define EVENT_IMPLECATION
	#include "DelegateImplement.hpp"
	#undef EVENT_IMPLECATION


	//////////////////////////
	//		三个参数			//
	//////////////////////////

	#define NAME_FIX 3
	#define HAVE_COMMA ,
	#define EVENT_PARAM_TEMPLATE typename Arg1, typename Arg2, typename Arg3
	#define EVENT_PARAM_TYPES_INPUTS Arg1 arg1, Arg2 arg2, Arg3 arg3
	#define EVENT_PARAM_TYPES Arg1, Arg2, Arg3
	#define EVENT_PARAM_INPUTS arg1, arg2, arg3

	#define EVENT_IMPLECATION
	#include "DelegateImplement.hpp"
	#undef EVENT_IMPLECATION


	//////////////////////////
	//		 四个参数		//
	//////////////////////////

	#define NAME_FIX 4
	#define HAVE_COMMA ,
	#define EVENT_PARAM_TEMPLATE typename Arg1, typename Arg2, typename Arg3, typename Arg4
	#define EVENT_PARAM_TYPES_INPUTS Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4
	#define EVENT_PARAM_TYPES Arg1, Arg2, Arg3, Arg4
	#define EVENT_PARAM_INPUTS arg1, arg2, arg3, arg4

	#define EVENT_IMPLECATION
	#include "DelegateImplement.hpp"
	#undef EVENT_IMPLECATION


	//////////////////////////
	//		五个参数			//
	//////////////////////////

	#define NAME_FIX 5
	#define HAVE_COMMA ,
	#define EVENT_PARAM_TEMPLATE typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5
	#define EVENT_PARAM_TYPES_INPUTS Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5
	#define EVENT_PARAM_TYPES Arg1, Arg2, Arg3, Arg4, Arg5
	#define EVENT_PARAM_INPUTS arg1, arg2, arg3, arg4, arg5

	#define EVENT_IMPLECATION
	#include "DelegateImplement.hpp"
	#undef EVENT_IMPLECATION


	//成员函数指针
	#ifdef _TRACK_MEMORY
		#define MEMBER_FUNC_PTR(x) make_bind( x, this, __FILE__, __LINE__ )
	#else
		#define MEMBER_FUNC_PTR(x) make_bind( x, this )
	#endif
}