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
#include "Predefine.h"
#include "ScriptUtility.h"


#include <list>
#include <vector>
#include <string>



struct lua_State;
namespace BohgeEngine
{
	static std::string __CON("__construct");
	static std::string __ADD("__operator+");
	static std::string __SUB("__operator-");
	static std::string __MUL("__operator*");
	static std::string __DIV("__operator/");


	class Rtti;
	class RttiObject;
	class ScriptClass;
	class LuaScriptSystem
	{
	private:
		typedef std::vector< ScriptClass* > ScriptClassVector;
		typedef std::list< std::string > ModuleList;
	public:
		lua_State*				m_LuaState;
		ScriptClassVector		m_ScriptClassArray;
		ModuleList				m_ModuleList;
	public:
		LuaScriptSystem(void);
		~LuaScriptSystem(void);
	private:
		const char* _FunctionNameMapping( const std::string& name );
		void _RegisterClassFunction( ScriptClass* c, int index, const char* name );
		void _RegisterScriptTypeConstruct( const char* cn, void* func );
		void _DoRegisterClass( const char* cn );
		void _DoRegisterFunction( ScriptClass* sc );
		void _DoRegisterConstant( ScriptClass* sc );
	public:
		void Initialize();//初始化系统
		void RegisterClass( const ScriptClass& c );//注册类
		void SetGlobal( const std::string& name, RttiObject* obj );//设置全局变量
		void LoadScript( const std::string& name );//加载脚本，将脚本中的入口函数导出
	};
}
