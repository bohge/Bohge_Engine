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
#include "FunctionRegister.h"
#include "IScriptSystem.h"
#include "ClassRegister.h"
#include "ScriptValue.h"
#include "IMetadata.h"
#include "ScriptComponent.h"
#include "ObjectWarp.hpp"

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void IAsyncEntity_CreateResource( T* entity )
	{
		entity->CreateResource();
	}
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void IAsyncEntity_SetPriority( T* entity, float* param )
	{
		entity->SetPriority( *param );
	}
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void IAsyncEntity_GetPriority( float* res, T* entity )
	{
		*res = entity->GetPriority();
	}
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void IAsyncEntity_SetJobType( T* entity, float* param )
	{
		entity->SetJobType( static_cast<IJob::JobType>( (int)*param ) );
	}
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void IAsyncEntity_GetJobType( float* res, T* entity )
	{
		*res = entity->GetJobType();
	}
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void IAsyncEntity_GetHashCode( float* res, T* entity )
	{
		*res = entity->GetHashCode();
	}
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void IAsyncEntity_isReady( bool* res, T* entity )
	{
		*res = entity->isReady();
	}
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void IAsyncEntity_PushMetadata( T* entity, PathMetadata* data )
	{
		entity->PushMetadata( *data );
	}
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void IAsyncEntity_SetScriptLoadedEvent( T* entity, IFunctionInvoker* f )
	{
		entity->SetScriptLoadedEvent( f );
	}
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void IAsyncEntity_Helper( IScriptSystem& sys, ClassRegister& reg )
	{
		{
			FunctionRegister sf;
			sf.Function( &BohgeEngine::IAsyncEntity_CreateResource<T>, "CreateResource" );
			reg.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &PathMetadata::RTTI() );
			sf.Function( &BohgeEngine::IAsyncEntity_PushMetadata<T>, "PushMetadata" );
			reg.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::IAsyncEntity_SetPriority<T>, "SetPriority" );
			reg.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::IAsyncEntity_GetPriority<T>, "GetPriority" );
			reg.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::IAsyncEntity_SetJobType<T>, "SetJobType" );
			reg.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::IAsyncEntity_GetJobType<T>, "GetJobType" );
			reg.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::IAsyncEntity_GetHashCode<T>, "GetHashCode" );
			reg.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Bool::RTTI() );
			sf.Function( &BohgeEngine::IAsyncEntity_isReady<T>, "isReady" );
			reg.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &IFunctionInvoker::RTTI() );
			sf.Function( &BohgeEngine::IAsyncEntity_SetScriptLoadedEvent<T>, "SetScriptLoadedEvent" );
			reg.ClassFunction( sf );
		}
		Object_Helper<T>( sys, reg );
	}
}