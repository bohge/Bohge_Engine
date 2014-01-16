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



/////////////////////////////
//		Lua脚本管理	   //
/////////////////////////////
#pragma once
#include "Event.hpp"
#include <string>
#include <map>


namespace BohgeEngine
{
	class ScriptBase;
	class ScriptManager
	{
		friend class ScriptBase;
	public:
		enum SupportedClasses //目前支持的加载库
		{
			LUALIB_BASE = 0,
			LUALIB_COROUTINE,
			LUALIB_TABLE,
			LUALIB_IO,
			LUALIB_OS,
			LUALIB_STRING,
			LUALIB_BIT32,
			LUALIB_MATH,
			LUALIB_DEBUG,
			LUALIB_PACKAGE,

			//数学
			CLASS_VECTOR2D,
			CLASS_VECTOR3D,
			CLASS_VECTOR4D,
			CLASS_VECTOR2F,
			CLASS_VECTOR3F,
			CLASS_VECTOR4F,
			CLASS_VECTOR2B,
			CLASS_VECTOR3B,
			CLASS_VECTOR4B,
			CLASS_COLOR,
			CLASS_QUATERNIONF,
			CLASS_QUATERNIONB,
			CLASS_MATRIX33F,
			CLASS_MATRIX44F,
			CLASS_PLANEF,
			CLASS_FRUSTUMF,
			CLASS_RAYF,
			CLASS_AABBOXF,
			CLASS_AABRECTF,
			CLASS_AABSPHEREF,

			//相机
			CLASS_ICAMERA,
			CLASS_STATICCAMERA,
			CLASS_TRACKBALLCAMERA,
			CLASS_FPSCAMERA,

			//engine
			CLASS_ENGINE,
			CLASS_UIMANAGER,

			//ui
			CLASS_UIGROUP,
			CLASS_UIBASE,
			CLASS_UISTATIC,
			CLASS_UIBUTTON,
			CLASS_UISLIDER,
			CLASS_UILIST,
			CLASS_UIEDIT,

			LUALIB_CLASS_COUNT, //写在引擎中的全部的支持的类的计数
		};
	private:
		typedef void (*FuncPtr)( ScriptBase* s, const char* classname );//定义一个rta的函数指针
		struct ClassInfo
		{
			std::string		m_IndexName;
			std::string		m_ClassName;
			FuncPtr			m_RegFunction;
			ClassInfo( const std::string& ixn, const std::string& csn, FuncPtr ptr )
				:m_IndexName(ixn),
				m_ClassName(csn),
				m_RegFunction(ptr)
			{
			}
		};
		typedef std::map< int, ClassInfo > SupportedClassesInfo;//支持的类型数据
	private:
		SupportedClassesInfo				m_SupportedClassesInfo;
		Event<bool, const std::string& >	m_ErrorCallBack;
	private://只能ScriptBase调用
		void _SetGlobalClassesIndex( ScriptBase* s );
		void _LoadClasses(  int name, ScriptBase* s );
	public:
		ScriptManager();
		~ScriptManager();
	public:
		BOHGE_FORCEINLINE FunctionPtr OnScriptErrorEvent(FunctionPtr input)
		{
			m_ErrorCallBack.RemoveAll();
			return m_ErrorCallBack.Connect( static_cast< IFunction<bool, const std::string& >* >(input) );
		}
		BOHGE_FORCEINLINE void RemoveOnScriptErrorEvent(FunctionPtr ptr)
		{
			m_ErrorCallBack.Remove( ptr );
		}
		//注册一个新的支持的脚本类型参数，1索引，2注册名，3脚本中类名，4注册函数指针
		BOHGE_FORCEINLINE void AddSuppotrtedClassesInfo( const int& sc, const std::string& ixn, const std::string& csn, FuncPtr ptr )
		{
			m_SupportedClassesInfo.insert( std::make_pair( sc, ClassInfo( ixn, csn, ptr ) ) );
		}
	};

}