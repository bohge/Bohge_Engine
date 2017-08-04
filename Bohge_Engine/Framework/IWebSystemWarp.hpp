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
#include "IWebSystem.h"
#include "IPostHandle.h"
#include "IDownloadHandle.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	static void IWebSystem_Cleaner( Object* rec, Object* obj )
	{
		static_cast< IWebSystem* >(rec)->RecycleBin( static_cast<IBaseHandle*>( obj ) );
	}
	//-------------------------------------------------------------------------------------------------------
	static void IWebSystem_WebFactory_PostHandle( ScriptValue* res, IWebSystem* self )
	{
		res->SetValue( self->WebFactory( IWebProperty::JT_POST ), self, &IWebSystem_Cleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void IWebSystem_WebFactory_DownloadHandle( ScriptValue* res, IWebSystem* self )
	{
		res->SetValue( self->WebFactory( IWebProperty::JT_DOWNLOAD ), self, &IWebSystem_Cleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IWebSystem( IScriptSystem& sys )
	{
		ClassRegister sc;
		sc.ClassType( &IWebSystem::RTTI() );

		{
			FunctionRegister sf;
			sf.Result( sys, &IPostHandle::RTTI() );
			sf.Function( &BohgeEngine::IWebSystem_WebFactory_PostHandle, "PostHandleFactory" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &IDownloadHandle::RTTI() );
			sf.Function( &BohgeEngine::IWebSystem_WebFactory_DownloadHandle, "DownloadHandleFactory" );
			sc.ClassFunction( sf );
		}
		sys.RegisterClass(sc);


		//×¢²áIWebProperty
		{
			ClassRegister sc;
			sc.ClassType( &IWebProperty::RTTI() );
			sc.ClassConstant( IWebProperty::HS_CONTINUE, "HS_CONTINUE" );
			sc.ClassConstant( IWebProperty::HS_SWITCH_PROTOCOLS, "HS_SWITCH_PROTOCOLS" );
			sc.ClassConstant( IWebProperty::HS_OK, "HS_OK" );
			sc.ClassConstant( IWebProperty::HS_CREATED, "HS_CREATED" );
			sc.ClassConstant( IWebProperty::HS_ACCEPTED	, "HS_ACCEPTED" );
			sc.ClassConstant( IWebProperty::HS_NO_CONTENT, "HS_NO_CONTENT" );
			sc.ClassConstant( IWebProperty::HS_RESET_CONTENT, "HS_RESET_CONTENT" );
			sc.ClassConstant( IWebProperty::HS_PARTIAL_CONTENT, "HS_PARTIAL_CONTENT" );
			sc.ClassConstant( IWebProperty::HS_MULTIPLE_CHOICES, "HS_MULTIPLE_CHOICES" );
			sc.ClassConstant( IWebProperty::HS_MOVED_PERMANENTLY, "HS_MOVED_PERMANENTLY" );
			sc.ClassConstant( IWebProperty::HS_MOVE_TEMPORARILY, "HS_MOVE_TEMPORARILY" );
			sc.ClassConstant( IWebProperty::HS_SEE_OTHER, "HS_SEE_OTHER" );
			sc.ClassConstant( IWebProperty::HS_NOT_MODIFIED, "HS_NOT_MODIFIED" );
			sc.ClassConstant( IWebProperty::HS_USE_PROXY, "HS_USE_PROXY" );
			sc.ClassConstant( IWebProperty::HS_SWITCH_PROXY, "HS_SWITCH_PROXY" );
			sc.ClassConstant( IWebProperty::HS_TEMP_REDIRECT, "HS_TEMP_REDIRECT" );
			sc.ClassConstant( IWebProperty::HS_BAD_REQUEST, "HS_BAD_REQUEST" );
			sc.ClassConstant( IWebProperty::HS_UNAUTHORIZED, "HS_UNAUTHORIZED" );
			sc.ClassConstant( IWebProperty::HS_PAYMENT_REQUIRED, "HS_PAYMENT_REQUIRED" );
			sc.ClassConstant( IWebProperty::HS_FORBIDDEN, "HS_FORBIDDEN" );
			sc.ClassConstant( IWebProperty::HS_NOT_FOUND, "HS_NOT_FOUND" );
			sc.ClassConstant( IWebProperty::HS_NOT_ALLOWED, "HS_NOT_ALLOWED" );
			sc.ClassConstant( IWebProperty::HS_NOT_ACCEPTABLE, "HS_NOT_ACCEPTABLE" );
			sc.ClassConstant( IWebProperty::HS_AUTHENTICATION, "HS_AUTHENTICATION" );
			sc.ClassConstant( IWebProperty::HS_REQUEST_TIMEOUT, "HS_REQUEST_TIMEOUT" );
			sc.ClassConstant( IWebProperty::HS_CONFLICT, "HS_CONFLICT" );
			sc.ClassConstant( IWebProperty::HS_GONE, "HS_GONE" );
			sc.ClassConstant( IWebProperty::HS_LENGTH_REQUIRED, "HS_LENGTH_REQUIRED" );
			sc.ClassConstant( IWebProperty::HS_PRECONDITION_FAILED, "HS_PRECONDITION_FAILED" );
			sc.ClassConstant( IWebProperty::HS_INTERNAL_SERVER_ERROR, "HS_INTERNAL_SERVER_ERROR" );
			sc.ClassConstant( IWebProperty::HS_NOT_IMPLEMENTED, "HS_NOT_IMPLEMENTED" );
			sc.ClassConstant( IWebProperty::HS_BAD_GATEWAY, "HS_BAD_GATEWAY" );
			sc.ClassConstant( IWebProperty::HS_SERVICE_UNAVAILABLE, "HS_SERVICE_UNAVAILABLE" );
			sc.ClassConstant( IWebProperty::HS_GATEWAY_TIMEOUT, "HS_GATEWAY_TIMEOUT" );
			sc.ClassConstant( IWebProperty::HS_UNPARSEABLE_RESPONSE_HEADERS, "HS_UNPARSEABLE_RESPONSE_HEADERS" );
			sc.ClassConstant( IWebProperty::HS_CACHED, "HS_CACHED" );
			sys.RegisterClass(sc);
		}
	}
}