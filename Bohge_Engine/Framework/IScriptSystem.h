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
#include "SystemSingleton.h"
#include "ScriptProperty.h"
#include "ScriptValue.h"
#include "Utility.hpp"
#include "IPlugin.h"
#include "3DMath.h"



#include <EASTL/list.h>
#include <EASTL/map.h>
#include <EASTL/set.h>


namespace BohgeEngine
{
	class Framework;
	class ClassRegister;
	class IClassResource;
	class IScriptResource;
	class IFunctionInvoker;
	class IPostEffectScriptResource;
	class BOHGE_FRAMEWORK IScriptSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR( IScriptSystem );
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IScriptSystem, IPlugin )
	public:
		typedef void* (*ScriptFunction)( void*, void* );//得到脚本函数参数的函数指针
	private:
		typedef eastl::map< uint, IFunctionInvoker* > FunctionInvokerMap;
		typedef eastl::list< IScriptResource* > IScriptResourceList;
		typedef eastl::map< uint, IScriptResource* > IScriptResourceMap;
	public:
		//注册的事件
		FunctionInvokerMap		m_ScriptFunctionMap[ScriptProperty::SLE_COUNT];
		IScriptResourceMap*		m_pLoadedSrcipt;
		IScriptResourceList*	m_pAttachList;
		IScriptResourceList*	m_pDetachList;
	public:
		IScriptSystem(void);
		virtual ~IScriptSystem(void);
	public:
		void OnFrameworkRenderEntry( float time, Framework& framework );
		void OnFrameworkLogicEntry( float time, Framework& framework );
		void OnFrameworkLogicExit( float time, Framework& framework );
		void OnBeginUpdate( float time, Framework& framework );
		void OnDoUpdate( float time, Framework& framework );
		void OnEndUpdate( float time, Framework& framework );
	private:
		virtual IClassResource* _DoLoadLogicScript( const eastl::string& path ) = 0;//加载脚本，返回脚本的handle
		virtual IClassResource* _DoLoadRenderScript( const eastl::string& source ) = 0;

		virtual IScriptResource* _DoLogicScriptFactory( ) = 0;
		virtual IScriptResource* _DoRenderScriptFactory( ) = 0;
		virtual void _DoLogicRecycleBin( IScriptResource* sc ) = 0;//释放脚本
		virtual void _DoRenderRecycleBin( IScriptResource* pe ) = 0;
	public:
		virtual bool isRegisteredClass( const eastl::string& name ) = 0;//是否注册
		virtual void RegisterClass( const ClassRegister& c ) = 0;//注册类
		virtual void SetGlobal( const eastl::string& name, Object* obj ) = 0;//设置全局变量
		virtual Object* GetGlobal( const eastl::string& name ) = 0;//取得全局变量
		virtual ScriptFunction GetScriptArgumentFunction( const Rtti* rtti  ) const = 0;
		virtual ScriptFunction GetScriptResultValueFunction( const Rtti* rtti  ) const = 0;
		virtual ScriptFunction GetScriptResultPushFunction( const Rtti* rtti  ) const = 0;
		virtual eastl::string GenerateEmptyScriptSource( const eastl::string& className ) = 0;//生成一个空的脚本文件
	public:
		virtual void RegistEvent( BohgeEngine::Framework& framework ); //注册事件
	private:
		void _DoAttach();
		void _DoDetach();
	protected:
		void _RegisterFramework();
	public:
		IClassResource* LoadScript( ScriptProperty::ScriptThread st, const eastl::string& name );
		IScriptResource* ScriptFactory( ScriptProperty::ScriptThread st );
		void RecycleBin( IScriptResource* sr );
	public:
		void AttachScriptEvent( IScriptResource* res )
		{
			m_pAttachList->push_back( res );
		}
	};
}