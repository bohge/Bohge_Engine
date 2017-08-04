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
#include "IFunctionInvoker.h"
#include "ScriptComponent.h"
#include "ScriptMetadata.h"
#include "IScriptSystem.h"
#include "ScriptEntity.h"
#include "Node.h"

namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( ScriptComponent )
	//---------------------------------------------------------------------------------------------------------
	ScriptComponent::ScriptComponent( void )
	:m_pScriptEntity( NEW ScriptEntity )
	{

	}
	//---------------------------------------------------------------------------------------------------------
	ScriptComponent::~ScriptComponent( void )
	{
		SAFE_DELETE( m_pScriptEntity );
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptComponent::_DoOnAttachNode()
	{
		m_pScriptEntity->ConnectLoadedEvent( MEMBER_FUNC_PTR(&ScriptComponent::OnScriptLoaded) );
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptComponent::OnRootNodeChange()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptComponent::Release()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptComponent::SetScriptPath( const eastl::string& path )
	{
		m_pScriptEntity->PushMetadata( ScriptMetadata( ScriptProperty::ST_LOGIC, path ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptComponent::SetJobType( IJob::JobType jt )
	{
		m_pScriptEntity->SetJobType( jt );
	}
	//---------------------------------------------------------------------------------------------------------
	IFunctionInvoker* ScriptComponent::SignFunciton( const eastl::string& name, uint argCount )
	{
		return m_pScriptEntity->SignFunciton( name, argCount );
	}
	//---------------------------------------------------------------------------------------------------------
	IFunctionInvoker* ScriptComponent::SignFunciton( int userdata, uint argCount )
	{
		return m_pScriptEntity->SignFunciton( userdata, argCount );
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptComponent::UnsignFunction( IFunctionInvoker* func )
	{
		m_pScriptEntity->UnsignFunction( func );
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptComponent::SetMember( const char* name, const Object* obj )
	{
		m_pScriptEntity->SetMember( name, obj );
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptComponent::OnScriptLoaded( const SmartPtr<IAsyncSource>& source )
	{
		SetMember( "HostNode", _HostNode() );
		m_pScriptEntity->AsLogicScript();
	}
	//---------------------------------------------------------------------------------------------------------
	const void* ScriptComponent::GetMember( const char* name )
	{
		return m_pScriptEntity->GetMember( name );
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptComponent::DependenceSource( IAsyncSource* source )
	{
		m_pScriptEntity->DependenceSource( source );
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptComponent::ConnectLoadedEvent( Delegate1<void, const SmartPtr<IAsyncSource>& >::FunctionType* ptr )
	{
		m_pScriptEntity->ConnectLoadedEvent( ptr );
	}

}