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
#include "IBaseHandle.h"
#include "ScriptComponent.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_SetUsername( IBaseHandle* self, char* name )
	{
		self->SetUsername( name );
	}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_SetPassword( IBaseHandle* self, char* pass )
	{
		self->SetPassword( pass );
	}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_SetUrl( IBaseHandle* self, char* url )
	{
		self->SetUrl( url );
	}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_SetThreadCount( IBaseHandle* self, float* c )
	{
		self->SetThreadCount( *c );
	}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_Start( IBaseHandle* self )
	{
		self->Start();
	}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_Pause( IBaseHandle* self )
	{
		self->Pause();
	}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_Stop( IBaseHandle* self )
	{
		self->Stop();
	}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_GetThreadCount( float* res, IBaseHandle* self )
	{
		*res = self->GetThreadCount();
	}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_GetUsername( eastl::string* res, IBaseHandle* self )
	{
		*res = self->GetUsername();
	}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_GetPassword( eastl::string* res, IBaseHandle* self )
	{
		*res = self->GetPassword();
	}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_GetUrl( eastl::string* res, IBaseHandle* self )
	{
		*res = self->GetUrl();
	}
	////-------------------------------------------------------------------------------------------------------
	//static void IBaseHandle_GetState( float* res, IBaseHandle* self )
	//{
	//	*res = self->GetState();
	//}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_GetStatusCode( float* res, IBaseHandle* self )
	{
		*res = self->GetStatusCode();
	}
	//-------------------------------------------------------------------------------------------------------
	static void IBaseHandle_SetFinishScriptCallback( IBaseHandle* self, IFunctionInvoker* f  )
	{
		self->SetFinishScriptCallback( f );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IBaseHandle( IScriptSystem& sys )
	{
		ClassRegister sc;
		sc.ClassType( &IBaseHandle::RTTI() );

		{
			FunctionRegister sf;
			sf.Arguments( sys, &String::RTTI() );
			sf.Function( &BohgeEngine::IBaseHandle_SetUsername, "SetUsername" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &String::RTTI() );
			sf.Function( &BohgeEngine::IBaseHandle_SetPassword, "SetPassword" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &String::RTTI() );
			sf.Function( &BohgeEngine::IBaseHandle_SetUrl, "SetUrl" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::IBaseHandle_SetThreadCount, "SetThreadCount" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Function( &BohgeEngine::IBaseHandle_Start, "Start" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Function( &BohgeEngine::IBaseHandle_Pause, "Pause" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Function( &BohgeEngine::IBaseHandle_Stop, "Stop" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::IBaseHandle_GetThreadCount, "GetThreadCount" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &String::RTTI() );
			sf.Function( &BohgeEngine::IBaseHandle_GetUsername, "GetUsername" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &String::RTTI() );
			sf.Function( &BohgeEngine::IBaseHandle_GetPassword, "GetPassword" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &String::RTTI() );
			sf.Function( &BohgeEngine::IBaseHandle_GetUrl, "GetUrl" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::IBaseHandle_GetStatusCode, "GetStatusCode" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Arguments( sys, &IFunctionInvoker::RTTI() );
			sf.Function( &BohgeEngine::IBaseHandle_SetFinishScriptCallback, "SetFinishCallback" );
			sc.ClassFunction( sf );
		}

		sys.RegisterClass(sc);
	}
}