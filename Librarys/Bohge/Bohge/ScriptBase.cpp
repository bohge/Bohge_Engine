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
//		Lua脚本对象	   //
/////////////////////////////
#include "ScriptBase.h"
#include "Engine.h"
#include "ScriptManager.h"
#include "Log.h"
#include "IFile.h"
#include "IOSystem.h"


using namespace std;




namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	ScriptBase::ScriptBase( )
		:m_LuaState(NULL),
		m_TableIndex(0),
		m_isLoaded(false)
#ifdef LUA_DEBUG_MODE
		,m_isError(false),
		m_OnErrorFunc(NULL)
#endif
	{
#ifdef LUA_DEBUG_MODE
		m_OnErrorFunc = MEMBER_FUNC_PTR( &ScriptBase::OnScriptError );
#endif
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	ScriptBase::~ScriptBase()
	{
		UnLoadScript();
#ifdef LUA_DEBUG_MODE
		Engine::Instance().GetScriptManager()->RemoveOnScriptErrorEvent( m_OnErrorFunc );
		SAFE_DELETE( m_OnErrorFunc );
#endif
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ScriptBase::_NewLuaState()
	{
		m_isLoaded = true;
		m_LuaState = luaL_newstate();
		if (m_LuaState == NULL)
		{
			assert(false);
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ScriptBase::_CloseLuaState()
	{
		for ( int i = 0 ; i < m_TableVector.size() ; i ++ )
		{
			SAFE_DELETE( m_TableVector[i] );
		}
		m_TableVector.clear();
		m_LoadedSet.clear();
		if (m_LuaState != NULL ) 
		{
			lua_close( m_LuaState );
		}
		m_LuaState = NULL;
		m_isLoaded = false;
#ifdef LUA_DEBUG_MODE
		m_isError = false;
#endif
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ScriptBase::DoScript( const std::string& buffer )
	{
		if ( m_isLoaded )
		{
			UnLoadScript();
		}
		_NewLuaState();
		_SetGlobalClassesIndex();
		RegClass<ScriptBase>( "ScriptBase" ); //注册一个类，实际是table
		RegClassFunc<ScriptBase>( "LoadClasses", &ScriptBase::LoadClasses );//注册table成员
		SetGlobal( "Script", this );//设置全局变量
		//加载脚本
		m_Scriptname = "Buffer";
#ifdef LUA_DEBUG_MODE
		Engine::Instance().GetScriptManager()->OnScriptErrorEvent( m_OnErrorFunc );
#endif
		lua_tinker::dostring( m_LuaState, buffer.c_str() );
#ifdef LUA_DEBUG_MODE
		Engine::Instance().GetScriptManager()->RemoveOnScriptErrorEvent( m_OnErrorFunc );
#endif
		this->CallLua<bool>( FUNCNAME(Script::FN_REGISTER) );
		_OnScriptLoaded();
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ScriptBase::LoadScript( const std::string& filename )
	{
		if ( m_isLoaded )
		{
			UnLoadScript();
		}
		_NewLuaState();
		_SetGlobalClassesIndex();
		RegClass<ScriptBase>( "ScriptBase" ); //注册一个类，实际是table
		RegClassFunc<ScriptBase>( "LoadClasses", &ScriptBase::LoadClasses );//注册table成员
		SetGlobal( "Script", this );//设置全局变量
		//加载脚本
		m_Scriptname = filename;
		string buffer;
		IFile* ruf = FILEFACTORY( m_Scriptname );
		ruf->OpenFile( IFile::AT_READ );
		buffer.resize( ruf->GetSize() );
		ruf->ReadFile( &buffer[0], ruf->GetSize() );
		ruf->CloseFile();
		FILEDESTROY( ruf );
		buffer.push_back( 0 );
#ifdef LUA_DEBUG_MODE
		Engine::Instance().GetScriptManager()->OnScriptErrorEvent( m_OnErrorFunc );
#endif
		lua_tinker::dostring( m_LuaState, buffer.c_str() );
#ifdef LUA_DEBUG_MODE
		Engine::Instance().GetScriptManager()->RemoveOnScriptErrorEvent( m_OnErrorFunc );
#endif
		this->CallLua<bool>( FUNCNAME(Script::FN_REGISTER) );
		_OnScriptLoaded();
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ScriptBase::UnLoadScript()
	{
		this->CallLua<bool>( FUNCNAME(Script::FN_CLEAR) );//清理
		_CloseLuaState();
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ScriptBase::_SetGlobalClassesIndex()
	{
		Engine::Instance().GetScriptManager()->_SetGlobalClassesIndex( this );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ScriptBase::LoadClasses( int name )
	{
		LoadedSet::iterator it = m_LoadedSet.find( name );
		if ( m_LoadedSet.end() == it )
		{
			m_LoadedSet.insert( name );
			Engine::Instance().GetScriptManager()->_LoadClasses( name, this );
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
#ifdef LUA_DEBUG_MODE
	bool ScriptBase::OnScriptError( const string& text )
	{
		DEBUGLOG( "Script %s : have Error: %s\n", m_Scriptname.c_str(), text.c_str() );
		m_isError = true;
		return true;
	}
#endif
}