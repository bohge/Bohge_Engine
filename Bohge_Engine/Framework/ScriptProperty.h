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
#include <EASTL/string.h>


namespace BohgeEngine
{
	namespace ScriptProperty
	{
#define BOOL_RTTI_ID (903450675)
#define NUMBER_RTTI_ID (3432659507)
#define STRING_RTTI_ID (3819627515)
#define FUNCTION_RTTI_ID (2368381682)
#define INSTANCE_RTTI_ID (455858718)

		static const char* FUNC_INITIALIZE( "Initialize" );
		static const char* FUNC_BEGIN_UPDATE( "BeginUpdate" );
		static const char* FUNC_DO_UPDATE( "DoUpdate" );
		static const char* FUNC_END_UPDATE( "EndUpdate" );
		static const char* FUNC_EXIT( "Exit" );
		static const char* FUNC_LOOPED[] = { "BeginUpdate", "DoUpdate", "EndUpdate" };

		static const char* POST_EFFECT_QUEUE( "Queue" );
		static const char* POST_EFFECT_INIT( "Initialize" );
		static const char* POST_EFFECT_PROCESS( "ProcessPostEffect" );
		static const char* POST_EFFECT_ONRESIZE( "OnResizeview" );
		//默认的重载操作符
		static const char* __CON("__construct");
		static const char* __ADD("__operator+");
		static const char* __SUB("__operator-");
		static const char* __MUL("__operator*");
		static const char* __DIV("__operator/");
		static const char* __EQU("__operator==");
		static const char* __LES("__operator<");
		static const char* __LEQ("__operator<=");

		static const char* __GlobalFrameworkName("Framework");//脚本中关键全局变量的名字

		enum FunctionType
		{
			FT_MEMBER = 0,//成员函数
			FT_STATIC,//静态函数
		};

		enum ScriptThread
		{
			ST_LOGIC = 0,
			ST_RENDER,
		};

		enum ScriptLoopEvent
		{
			SLE_BEGIN_UPDATE = 0,
			SLE_DO_UPDATE,
			SLE_END_UPDATE,

			SLE_COUNT,
		};

		struct ScriptData
		{
			eastl::string	m_Path;
			ScriptThread	m_Thread;
			ScriptData( ScriptProperty::ScriptThread st, const eastl::string& path )
				:m_Thread( st ),
				m_Path( path )
			{
			}
		};
	}
}