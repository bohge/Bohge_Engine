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
#include "Framework/ScriptUtility.h"
#include "Framework/IScriptSystem.h"
#include "PrintScriptClassInfo.h"

#include <EASTL/list.h>
#include <EASTL/vector.h>
#include <EASTL/string.h>



#define _PRINT_LUA_CLASS_INFO
namespace BohgeEngine
{
	class ClassRegister;
	class IThreadSpecific;
	class IThreadMutex;
	class IClassResource;
}
struct lua_State;
namespace LuaScriptSystemPlugin
{
	class LuaScriptSystem : public BohgeEngine::IScriptSystem
	{
	private:
		struct LuaPair
		{
			BohgeEngine::IThreadMutex*	m_pMutex;//由于Callback的存在导致有可能一些回掉在其它线程操作，这样会产生问题
			lua_State*					m_pLuaState;
			LuaPair():m_pMutex(NULL),m_pLuaState(NULL){}
		};
	private:
		typedef eastl::list< BohgeEngine::ClassRegister* > ScriptClassList;
		//typedef eastl::map< uint, eastl::string > ModuleMap;
	private:
		BohgeEngine::IThreadSpecific*		m_pSpecific;
		LuaPair					m_LogicPair;
		LuaPair					m_RenderPair;
		ScriptClassList			m_ScriptClassList;
		//ModuleMap				m_ModuleMap;
#ifdef _PRINT_LUA_CLASS_INFO
		ScriptClassMap			m_ScriptClassMap;
#endif
	public:
		LuaScriptSystem(void);
		~LuaScriptSystem(void);
	private:
		void _Clear();//清理内存
		void _SetLuaPath( lua_State* l );
		const char* _FunctionNameMapping( const eastl::string& name );
		void _DelayRegisterClasses();
		void _RegisterClassFunction( lua_State* ls, BohgeEngine::ClassRegister* c, int index, BohgeEngine::ScriptProperty::FunctionType ft, const char* name );
		void _RegisterScriptTypeConstruct( lua_State* ls, const char* cn, void* func );
		void _DoRegisterClass( lua_State* ls, const char* cn );
		void _DoRegisterInheritance( lua_State* ls, BohgeEngine::ClassRegister* sc );
		void _DoRegisterFunction( lua_State* ls, BohgeEngine::ClassRegister* sc );
		void _DoRegisterConstant( lua_State* ls, BohgeEngine::ClassRegister* sc );
		void _DoSetGlobal( lua_State* ls, const eastl::string& name, BohgeEngine::Object* obj );
		BohgeEngine::Object* _DoGetGlobal( lua_State* ls, const eastl::string& name );
	private:
		virtual BohgeEngine::IClassResource* _DoLoadLogicScript( const eastl::string& path );//加载脚本，返回脚本的handle
		virtual BohgeEngine::IClassResource* _DoLoadRenderScript( const eastl::string& source );
		virtual BohgeEngine::IScriptResource* _DoLogicScriptFactory( );
		virtual BohgeEngine::IScriptResource* _DoRenderScriptFactory( );
		virtual void _DoLogicRecycleBin( BohgeEngine::IScriptResource* sc );
		virtual void _DoRenderRecycleBin( BohgeEngine::IScriptResource* pe );
	public:
		virtual bool isRegisteredClass( const eastl::string& name );
		virtual void RegistEvent( BohgeEngine::Framework& framework ); //注册事件
		virtual void RegisterClass( const BohgeEngine::ClassRegister& c );//注册类
		virtual void SetGlobal( const eastl::string& name, BohgeEngine::Object* obj );//设置全局变量
		virtual BohgeEngine::Object* GetGlobal( const eastl::string& name );//取得全局变量
		virtual BohgeEngine::IScriptSystem::ScriptFunction GetScriptArgumentFunction( const BohgeEngine::Rtti* rtti  ) const;
		virtual BohgeEngine::IScriptSystem::ScriptFunction GetScriptResultValueFunction( const BohgeEngine::Rtti* rtti  ) const;
		virtual BohgeEngine::IScriptSystem::ScriptFunction GetScriptResultPushFunction( const BohgeEngine::Rtti* rtti  ) const;
		virtual eastl::string GenerateEmptyScriptSource( const eastl::string& className );//生成一个空的脚本文件
	public://注册的事件函数
		void OnFrameworkRenderEntry( float time, BohgeEngine::Framework& framework );
		void OnFrameworkRenderBeginUpdata( float time, BohgeEngine::Framework& framework );
		void OnFrameworkRenderExit( float time, BohgeEngine::Framework& framework );
		
		void OnFrameworkLogicEntry( float time, BohgeEngine::Framework& framework );
		void OnFrameworkLogicBeginUpdata( float time, BohgeEngine::Framework& framework );
		void OnFrameworkLogicExit( float time, BohgeEngine::Framework& framework );
	};
}
