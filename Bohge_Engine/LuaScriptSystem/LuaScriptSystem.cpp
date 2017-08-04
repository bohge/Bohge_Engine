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
#include "Framework/FunctionRegister.h"
#include "Framework/IThreadSpecific.h"
#include "Framework/IThreadSystem.h"
#include "Framework/IThreadMutex.h"
#include "Framework/ScriptUtility.h"
#include "Framework/ClassRegister.h"
#include "Framework/IIOSystem.h"
#include "Framework/Framework.h"
#include "Framework/Utility.hpp"
#include "Framework/Rtti.hpp"
#include "Framework/IFile.h"
#include "Framework/Log.h"

#include "GetScriptArgumenFunctions.h"
#include "GetScriptResultFunction.h"
#include "LuaScriptSystem.h"
#include "ScriptCallBridge.h"
#include "LuaClassResource.h"
#include "LuaScriptHandle.h"
#include "LuaScriptConst.h"


extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lobject.h"
}

#include "lua_tinker.hpp"

#include <EASTL/vector.h>
#include <sstream>

using namespace eastl;
using namespace BohgeEngine;

namespace LuaScriptSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	LuaScriptSystem::LuaScriptSystem(void)
		:m_pSpecific(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	LuaScriptSystem::~LuaScriptSystem(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_SetLuaPath( lua_State* l )
	{
		//设置系统路径到lite路径，配合require
		const string& litepath = IIOSystem::Instance()->GetLiteappPath();
		lua_getglobal( l, "package" );
		//lua_getfield( l, -1, "path" ); // get field "path" from table at top of stack (-1)
		//string cur_path = lua_tostring( l, -1 ); // grab path string from top of stack
		//cur_path.append( ";" ); // do your path magic here
		//cur_path.append( path );
		//cur_path.append( "?.lua" );
		//lua_pop( l, 1 ); // get rid of the string on the stack we just pushed on line 5
		string cur_path;
		cur_path += litepath + "?.lua;";
		cur_path += litepath + "?.dll;";
		lua_pushstring( l, cur_path.c_str() ); // push the new one
		lua_setfield( l, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
		lua_pop( l, 1 ); // get rid of package table from top of stack
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::RegistEvent( Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_ENTRY, MEMBER_FUNC_PTR( &LuaScriptSystem::OnFrameworkRenderEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &LuaScriptSystem::OnFrameworkLogicEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_BEGIN_UPDATE, MEMBER_FUNC_PTR( &LuaScriptSystem::OnFrameworkLogicBeginUpdata ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_BEGIN_RENDER, MEMBER_FUNC_PTR( &LuaScriptSystem::OnFrameworkRenderBeginUpdata ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_EXIT, MEMBER_FUNC_PTR( &LuaScriptSystem::OnFrameworkRenderExit ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &LuaScriptSystem::OnFrameworkLogicExit ) );
		IScriptSystem::RegistEvent( framework );//首先处理跟系统，应为注册系统Type需要在跟系统注册之后
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::OnFrameworkRenderEntry( float time, BohgeEngine::Framework& framework )
	{
		m_LogicPair.m_pLuaState = luaL_newstate();
		luaL_openlibs(m_LogicPair.m_pLuaState);
		m_RenderPair.m_pLuaState = luaL_newstate();
		luaL_openlibs(m_RenderPair.m_pLuaState);
		m_pSpecific =
			static_cast< IThreadSpecific* >
			( IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_THREAD_SPECIFIC ) );
		m_pSpecific->Initialize();
		m_pSpecific->SetSpecific( &m_RenderPair );
		m_RenderPair.m_pMutex =
			static_cast< IThreadMutex* >
			( IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_MUTEX ) );
		m_LogicPair.m_pMutex =
			static_cast< IThreadMutex* >
			( IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_MUTEX ) );
		_SetLuaPath( m_RenderPair.m_pLuaState );
		_RegisterFramework();
		_DelayRegisterClasses();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::OnFrameworkLogicEntry( float time, BohgeEngine::Framework& framework )
	{
		m_pSpecific->SetSpecific( &m_LogicPair );
		_SetLuaPath( m_LogicPair.m_pLuaState );
		_RegisterFramework();
		_DelayRegisterClasses();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::OnFrameworkRenderExit( float time, BohgeEngine::Framework& framework )
	{
		lua_close( m_RenderPair.m_pLuaState );
		m_RenderPair.m_pLuaState = NULL;
		IThreadSystem::Instance()->RecycleBin( m_pSpecific );
		IThreadSystem::Instance()->RecycleBin( m_RenderPair.m_pMutex );
		IThreadSystem::Instance()->RecycleBin( m_LogicPair.m_pMutex );
#ifdef _PRINT_LUA_CLASS_INFO
		PrintScriptClassInfo( m_ScriptClassMap );
		m_ScriptClassMap.clear();
#endif
		_Clear();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::OnFrameworkLogicExit( float time, BohgeEngine::Framework& framework )
	{
		lua_close( m_LogicPair.m_pLuaState );
		m_LogicPair.m_pLuaState = NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::OnFrameworkRenderBeginUpdata( float time, BohgeEngine::Framework& framework )
	{
		//lua_gc( m_RenderPair.m_pLuaState, LUA_GCCOLLECT, 1 );ka
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::OnFrameworkLogicBeginUpdata( float time, BohgeEngine::Framework& framework )
	{
		//lua_gc( m_LogicPair.m_pLuaState, LUA_GCCOLLECT, 1 );
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_RegisterScriptTypeConstruct( lua_State* ls, const char* cn, void* func )
	{
		lua_getglobal(ls, cn );
		ASSERT(lua_istable(ls, -1));
		{
			lua_newtable(ls);
			lua_pushstring(ls, "__call");
			lua_pushcclosure(ls, (lua_CFunction)func, 0);
			lua_rawset(ls, -3);
			lua_setmetatable(ls, -2);
		}
		lua_pop(ls, 1);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_DoRegisterClass( lua_State* ls, const char* cn )
	{
		lua_newtable( ls );

		//注册类名称
		lua_pushstring( ls, "__name");
		lua_pushstring( ls, cn );
		lua_rawset( ls, -3);

		//元表
		lua_pushstring( ls, "__index");
		lua_pushcclosure( ls, lua_tinker::meta_get, 0);
		lua_rawset( ls, -3);

		lua_pushstring( ls, "__newindex");
		lua_pushcclosure( ls, lua_tinker::meta_set, 0);
		lua_rawset( ls, -3);

		lua_pushstring( ls, "__gc");
		lua_pushcclosure( ls, &ClassDestruct, 0);
		lua_rawset( ls, -3);

		//设置全局名称
		lua_setglobal( ls, cn );
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_DoRegisterInheritance( lua_State* ls, ClassRegister* sc )
	{
		if ( sc->isDriverType() )//注册继承类
		{
			lua_getglobal(ls, sc->GetClassTypeName() );
			lua_pushstring(ls, "__parent");
			lua_getglobal(ls, sc->GetBaseClassTypeName() );
			lua_rawset(ls, -3);
			lua_pop(ls, 1);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_DoRegisterFunction( lua_State* ls, ClassRegister* sc )
	{
		uint funcCount = sc->GetFunctionListCount();
		for ( int i = 0 ; i < funcCount ; ++ i )
		{
			_RegisterClassFunction( ls, sc, i, sc->GetFunctionListType( i ), _FunctionNameMapping( sc->GetFunctionListName(i) ) );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool LuaScriptSystem::isRegisteredClass( const eastl::string& name )
	{
		LuaPair* lp = m_pSpecific->GetSpecific<LuaPair>();
		lp->m_pMutex->Lock();
		lua_getglobal( lp->m_pLuaState, name.c_str() );//得到类的类型
		bool res = lua_istable( lp->m_pLuaState, -1 );
		lua_pop( lp->m_pLuaState , 1 );
		lp->m_pMutex->Unlock();
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::SetGlobal( const string& name, Object* obj )
	{
		LuaPair* lp = m_pSpecific->GetSpecific<LuaPair>();
		lp->m_pMutex->Lock();
		_DoSetGlobal( lp->m_pLuaState, name, obj );
		lp->m_pMutex->Unlock();
	}
	//-------------------------------------------------------------------------------------------------------
	Object* LuaScriptSystem::GetGlobal( const eastl::string& name )
	{
		LuaPair* lp = m_pSpecific->GetSpecific<LuaPair>();
		lp->m_pMutex->Lock();
		Object* res = _DoGetGlobal( lp->m_pLuaState, name );
		lp->m_pMutex->Unlock();
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::RegisterClass( const ClassRegister& rhs )
	{
		ClassRegister* sc = NEW ClassRegister( rhs );
		m_ScriptClassList.push_back( sc );
		//LuaPair* lp = m_pSpecific->GetSpecific<LuaPair>();//注册类在最开始调用，暂时不上锁
		//_DoRegisterClass( lp->m_pLuaState, sc->GetClassTypeName() );
		//_DoRegisterInheritance( lp->m_pLuaState, sc );
		//_DoRegisterFunction( lp->m_pLuaState, sc );
		//_DoRegisterConstant( lp->m_pLuaState, sc );
#ifdef _PRINT_LUA_CLASS_INFO
		if ( m_ScriptClassMap.end() == m_ScriptClassMap.find( sc->GetClassTypeID() ) )
		{
			m_ScriptClassMap.insert( make_pair( sc->GetClassTypeID(), sc) );
		}
#endif
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_DelayRegisterClasses()
	{
		LuaPair* lp = m_pSpecific->GetSpecific<LuaPair>();//注册类在最开始调用，暂时不上锁
		FOR_EACH( ScriptClassList, it, m_ScriptClassList )
		{
			_DoRegisterClass( lp->m_pLuaState, (*it)->GetClassTypeName() );
			_DoRegisterInheritance( lp->m_pLuaState, (*it) );
			_DoRegisterFunction( lp->m_pLuaState, (*it) );
			_DoRegisterConstant( lp->m_pLuaState, (*it) );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_Clear()
	{
		FOR_EACH( ScriptClassList, it, m_ScriptClassList )
		{
			SAFE_DELETE( *it );
		}
		m_ScriptClassList.clear();
		//m_ModuleMap.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	const char* LuaScriptSystem::_FunctionNameMapping( const eastl::string& name )
	{
		if ( name == ScriptProperty::__ADD )
		{
			return "__add";
		}
		else if( name == ScriptProperty::__SUB )
		{
			return "__sub";
		}
		else if( name == ScriptProperty::__MUL )
		{
			return "__mul";
		}
		else if( name == ScriptProperty::__DIV )
		{
			return "__div";
		}
		else if( name == ScriptProperty::__CON )
		{
			return "__call";
		}
		else if( name == ScriptProperty::__EQU )
		{
			return "__eq";
		}
		else if( name == ScriptProperty::__LES )
		{
			return "__lt";
		}
		else if( name == ScriptProperty::__LEQ )
		{
			return "__le";
		}
		return name.c_str();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_RegisterClassFunction( lua_State* ls, ClassRegister* c, int index, BohgeEngine::ScriptProperty::FunctionType ft, const char* name )
	{
		lua_CFunction func = NULL;
		bool isCon = string("__call") == name ? true : false;
		lua_getglobal(ls, c->GetClassTypeName() );//得到类的表名
		ASSERT(lua_istable(ls, -1));

		if ( isCon )lua_newtable(ls);//如果是构造需要新加元表

		lua_pushstring(ls, name );//将方法名称推入堆栈
		lua_pushlightuserdata(ls, c );//将ScriptClass信息推入堆栈
		lua_pushnumber(ls, index );//将函数在数组中的位置推入堆栈
		if ( c->isOverrideFunction( index ) )
		{
			if ( isCon || ScriptProperty::FT_STATIC == ft )
				func = &OverrideClassConstructStatic;
			else
				func = &OverrideClassFunction;
		}
		else
		{
			if ( isCon || ScriptProperty::FT_STATIC == ft )
				func = &SingleClassConstructStatic;
			else
				func = &SingleClassFunction;
		}
		lua_pushcclosure(ls, func, 2);//注册函数，通知lua函数拥有两个upvalue
		lua_rawset(ls,-3);
		
		if ( isCon )lua_setmetatable(ls, -2);

		lua_pop(ls, 1);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_DoRegisterConstant( lua_State* ls, BohgeEngine::ClassRegister* sc )
	{
		uint funcCount = sc->GetClassConstantCount();
		for ( int i = 0 ; i < funcCount ; ++ i )
		{
			CLassConstant cc = sc->GetClassConstant( i );
			lua_getglobal(ls, sc->GetClassTypeName() );//得到类的表名
			ASSERT(lua_istable(ls, -1));
			lua_pushstring(ls, cc.first.c_str() );//将名称推入堆栈

			ASSERT( BOOL_RTTI_ID == Bool::RTTI().GetTypeID() );
			ASSERT( STRING_RTTI_ID == String::RTTI().GetTypeID() );
			ASSERT( NUMBER_RTTI_ID == Number::RTTI().GetTypeID() );

			switch( cc.second->GetTypeID() )
			{
			case BOOL_RTTI_ID:
				{
					lua_pushboolean( ls, static_cast<Bool*>( cc.second )->GetData() );
				}break;
			case STRING_RTTI_ID:
				{
					lua_pushstring( ls, static_cast<String*>( cc.second )->GetData().c_str() );
				}break;
			case NUMBER_RTTI_ID:
				{
					lua_pushnumber( ls, static_cast<Number*>( cc.second )->GetData() );
				}break;
			case FUNCTION_RTTI_ID: ASSERT( false );
			default:
				{
					ScriptValue* userdata = (ScriptValue*)lua_newuserdata(ls, sizeof(ScriptValue) );
					userdata->SetValue( cc.second, &ScriptValue::NullCleaner );
				}break;
			}
			lua_rawset(ls, -3);
			lua_pop(ls, 1);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_DoSetGlobal( lua_State* ls, const eastl::string& name, BohgeEngine::Object* obj )
	{
		ScriptValue* userdata = (ScriptValue*)lua_newuserdata(ls, sizeof(ScriptValue) );
		userdata->SetValue( obj, &ScriptValue::NullCleaner );
		lua_getglobal( ls, obj->GetTypeName() );//得到类的类型
		ASSERT(lua_istable(ls, -1));
		lua_setmetatable( ls, -2 );
		lua_setglobal( ls, name.c_str() );//添加一个全局变量
	}
	//-------------------------------------------------------------------------------------------------------
	BohgeEngine::Object* LuaScriptSystem::_DoGetGlobal( lua_State* ls, const eastl::string& name )
	{
		lua_getglobal( ls, name.c_str() );
		Object* r = ((ScriptValue*)lua_touserdata(ls,-1))->GetObject();
		lua_pop( ls, 1 );
		return r;
	}
	//-------------------------------------------------------------------------------------------------------
	LuaClassResource* _DoLoad( lua_State** lua, IThreadMutex* mutex, const eastl::string& path )
	{
		LuaClassResource* sh =  NEW LuaClassResource( lua, mutex );
		IFile* scriptfile = IIOSystem::Instance()->FileFactory( path );
		eastl::string script;
		scriptfile->OpenFile( IFile::AT_READ );
		script.resize( scriptfile->GetSize() );
		scriptfile->ReadFile( &script[0], scriptfile->GetSize() );
		scriptfile->CloseFile();
		IIOSystem::Instance()->RecycleBin( scriptfile );
		sh->SetSource( script );
		string filepath = path.substr( IIOSystem::IO_SUBPATH_POS );
		sh->SetFileName( filepath );
		filepath = filepath.substr( 0, filepath.find_first_of(".") );
		sh->SetChunkName( filepath );
		sh->SetClassName( filepath.substr( filepath.find_last_of("/\\") + 1 ) );
		return sh;
	}
	IClassResource* LuaScriptSystem::_DoLoadLogicScript( const eastl::string& path )
	{
		//m_LogicPair.m_pMutex->Lock();
		LuaClassResource* sh = _DoLoad( &m_LogicPair.m_pLuaState, m_LogicPair.m_pMutex, path );
		//m_LogicPair.m_pMutex->Unlock();
		return sh;
	}
	//-------------------------------------------------------------------------------------------------------
	IClassResource* LuaScriptSystem::_DoLoadRenderScript( const eastl::string& path )
	{
		//m_RenderPair.m_pMutex->Lock();
		LuaClassResource* sh = _DoLoad( &m_RenderPair.m_pLuaState, m_RenderPair.m_pMutex, path );
		//m_RenderPair.m_pMutex->Unlock();
		return sh;
	}
	//-------------------------------------------------------------------------------------------------------
	BohgeEngine::IScriptResource* LuaScriptSystem::_DoLogicScriptFactory()
	{
		return NEW LuaScriptHandle( &m_LogicPair.m_pLuaState, m_LogicPair.m_pMutex );
	}
	//-------------------------------------------------------------------------------------------------------
	BohgeEngine::IScriptResource* LuaScriptSystem::_DoRenderScriptFactory()
	{
		return NEW LuaScriptHandle( &m_RenderPair.m_pLuaState, m_RenderPair.m_pMutex );
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_DoLogicRecycleBin( BohgeEngine::IScriptResource* sc )
	{
		SAFE_DELETE( sc );
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_DoRenderRecycleBin( BohgeEngine::IScriptResource* pe )
	{
		SAFE_DELETE( pe );
	}
	//-------------------------------------------------------------------------------------------------------
	IScriptSystem::ScriptFunction LuaScriptSystem::GetScriptArgumentFunction( const Rtti* rtti ) const
	{
		ASSERT( BOOL_RTTI_ID == Bool::RTTI().GetTypeID() );
		ASSERT( STRING_RTTI_ID == String::RTTI().GetTypeID() );
		ASSERT( NUMBER_RTTI_ID == Number::RTTI().GetTypeID() );
		ASSERT( FUNCTION_RTTI_ID == Function::RTTI().GetTypeID() );
		ASSERT( INSTANCE_RTTI_ID == Instance::RTTI().GetTypeID() );

		switch( rtti->GetTypeID() )
		{
		case BOOL_RTTI_ID: return &GetScriptParameterBool;
		case STRING_RTTI_ID:return &GetScriptParameterString;
		case NUMBER_RTTI_ID: return &GetScriptParameterNumber;
		case INSTANCE_RTTI_ID: ASSERT(false&&"can not be function type");
		case FUNCTION_RTTI_ID: return &GetScriptParameterFunction;
		default: return &GetScriptParameterClass;
		}
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	BohgeEngine::IScriptSystem::ScriptFunction LuaScriptSystem::GetScriptResultValueFunction( const BohgeEngine::Rtti* rtti ) const
	{
		ASSERT( BOOL_RTTI_ID == Bool::RTTI().GetTypeID() );
		ASSERT( STRING_RTTI_ID == String::RTTI().GetTypeID() );
		ASSERT( NUMBER_RTTI_ID == Number::RTTI().GetTypeID() );
		ASSERT( INSTANCE_RTTI_ID == Instance::RTTI().GetTypeID() );

		switch( rtti->GetTypeID() )
		{
		case BOOL_RTTI_ID: return &GetScriptResultValueBool;
		case STRING_RTTI_ID:return &GetScriptResultValueString;
		case NUMBER_RTTI_ID: return &GetScriptResultValueNumber;
		case INSTANCE_RTTI_ID: return &GetScriptResultValueInstance;
		case FUNCTION_RTTI_ID: ASSERT(false&&"can not be function type");
		default: return &GetScriptResultValueClass;
		}
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	BohgeEngine::IScriptSystem::ScriptFunction LuaScriptSystem::GetScriptResultPushFunction( const BohgeEngine::Rtti* rtti ) const
	{
		ASSERT( BOOL_RTTI_ID == Bool::RTTI().GetTypeID() );
		ASSERT( STRING_RTTI_ID == String::RTTI().GetTypeID() );
		ASSERT( NUMBER_RTTI_ID == Number::RTTI().GetTypeID() );
		ASSERT( INSTANCE_RTTI_ID == Instance::RTTI().GetTypeID() );

		switch( rtti->GetTypeID() )
		{
		case BOOL_RTTI_ID: return &GetScriptResultPushBool;
		case STRING_RTTI_ID:return &GetScriptResultPushString;
		case NUMBER_RTTI_ID: return &GetScriptResultPushNumber;
		case INSTANCE_RTTI_ID: return &GetScriptResultPushInstance;
		case FUNCTION_RTTI_ID: ASSERT(false&&"can not be function type");
		default: return &GetScriptResultPushClass;
		}
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	string LuaScriptSystem::GenerateEmptyScriptSource( const eastl::string& className )
	{
		std::stringstream ss;
		//ss<<LUA_CLASS_FACTORY<<" = String(\""<<className.c_str()<<"\");"<<std::endl<<std::endl;
		//ss<<className.c_str()<<" = {};"<<std::endl<<std::endl;
		//ss<<"function "<<className.c_str()<<"."<<ScriptProperty::FUNC_INITIALIZE<<"( timespan, framework )"<<std::endl<<std::endl<<"end"<<std::endl<<std::endl<<std::endl;
		//ss<<"function "<<className.c_str()<<"."<<ScriptProperty::FUNC_BEGIN_UPDATE<<"( timespan, framework )"<<std::endl<<std::endl<<"end"<<std::endl<<std::endl<<std::endl;
		//ss<<"function "<<className.c_str()<<"."<<ScriptProperty::FUNC_DO_UPDATE<<"( timespan, framework )"<<std::endl<<std::endl<<"end"<<std::endl<<std::endl<<std::endl;
		//ss<<"function "<<className.c_str()<<"."<<ScriptProperty::FUNC_END_UPDATE<<"( timespan, framework )"<<std::endl<<std::endl<<"end"<<std::endl<<std::endl<<std::endl;
		//ss<<"function "<<className.c_str()<<"."<<ScriptProperty::FUNC_EXIT<<"( timespan, framework )"<<std::endl<<std::endl<<"end"<<std::endl<<std::endl<<std::endl;
		return ss.str().c_str();
	}
}
