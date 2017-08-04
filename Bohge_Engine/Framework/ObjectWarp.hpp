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
#include "ObjectWarp.hpp"
#include "ScriptComponent.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void Object_GetTypeName( eastl::string* res )
	{
		*res = T::RTTI().GetTypeName();
	}
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void Object_GetTypeSize( float* res )
	{
		*res = T::RTTI().GetTypeSize();
	}
	//-------------------------------------------------------------------------------------------------------
	template< typename T >
	static void Object_Helper( IScriptSystem& sys, ClassRegister& reg )
	{
		{
			FunctionRegister sf;
			sf.Result( sys, &String::RTTI() );
			sf.SetFunctionType( ScriptProperty::FT_STATIC );
			sf.Function( &BohgeEngine::Object_GetTypeName<T>, "GetTypeName" );
			reg.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.SetFunctionType( ScriptProperty::FT_STATIC );
			sf.Function( &BohgeEngine::Object_GetTypeSize<T>, "GetTypeSize" );
			reg.ClassFunction( sf );
		}
	}
}