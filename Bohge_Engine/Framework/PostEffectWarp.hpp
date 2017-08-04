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
#include "RenderObjectEntity.h"
#include "RenderTargetEntity.h"
#include "FunctionRegister.h"
#include "IScriptSystem.h"
#include "ClassRegister.h"
#include "ScriptValue.h"
#include "PostEffect.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	static void IPostEffect_Construct( ScriptValue* res )
	{
		res->SetValue( NEW PostEffect, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void IPostEffect_CreatePostEffect( PostEffect* pe, char* param )
	{
		pe->CreatePostEffect( param );
	}
	//-------------------------------------------------------------------------------------------------------
	static void IPostEffect_CreateRenderObject( ScriptValue* res, PostEffect* pe, char* param )
	{
		RenderObjectEntity* ro = pe->CreateRenderObject( param );
		res->SetValue( ro, &ScriptValue::NullCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void IPostEffect_CreateRenderTarget( ScriptValue* res, PostEffect* pe, float* param, vector2d* size )
	{
		RenderTargetEntity* rt = pe->CreateRenderTarget( static_cast<RenderTargetProperty::SwapTarget>( (int)*param ), *size );
		res->SetValue( rt, &ScriptValue::NullCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IPostEffect( IScriptSystem& sys )
	{
		ClassRegister sc;
		sc.ClassType( &PostEffect::RTTI() );
		{
			FunctionRegister sf;
			sf.Result( sys, &PostEffect::RTTI() );
			sf.Function( &BohgeEngine::IPostEffect_Construct, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &String::RTTI() );
			sf.Function( &BohgeEngine::IPostEffect_CreatePostEffect, "CreatePostEffect" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &RenderObjectEntity::RTTI() );
			sf.Arguments( sys, &String::RTTI() );
			sf.Function( &BohgeEngine::IPostEffect_CreateRenderObject, "CreateRenderObject" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &RenderTargetEntity::RTTI() );
			sf.Arguments( sys, &Number::RTTI(), &vector2d::RTTI() );
			sf.Function( &BohgeEngine::IPostEffect_CreateRenderTarget, "CreateRenderTarget" );
			sc.ClassFunction( sf );
		}

		sys.RegisterClass(sc);
	}
}