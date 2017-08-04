////////////////////////////////////////////////////////////////////////////////////////
////
////						The Bohge Engine License (BEL)
////
////	Copyright (c) 2011-2014 Peng Zhao
////
////	Permission is hereby granted, free of charge, to any person obtaining a copy
////	of this software and associated documentation files (the "Software"), to deal
////	in the Software without restriction, including without limitation the rights
////	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
////	copies of the Software, and to permit persons to whom the Software is
////	furnished to do so, subject to the following conditions:
////
////	The above copyright notice and this permission notice shall be included in 
////	all copies or substantial portions of the Software. And the logo of 
////	Bohge Engine shall be displayed full screen for more than 3 seconds 
////	when the software is started. Copyright holders are allowed to develop 
////	game edit based on Bohge Engine, The edit must be released under the MIT 
////	open source license if it is going to be published. In no event shall 
////	copyright holders be prohibited from using any code of Bohge Engine 
////	to develop any other analogous game engines.
////
////	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
////	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
////	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
////	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
////	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
////	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
////
////////////////////////////////////////////////////////////////////////////////////////
//#pragma once
//#include "FunctionRegister.h"
//#include "IScriptSystem.h"
//#include "ClassRegister.h"
//#include "ScriptValue.h"
//#include "TextureEntity.h"
//
//
//namespace BohgeEngine
//{
//	//-------------------------------------------------------------------------------------------------------
//	void TextureEntity_Construct( ScriptValue* res )//构造
//	{
//		res->SetValue( NEW TextureEntity(), ScriptValue::SafeDeleteCleaner );
//	}
//	//-------------------------------------------------------------------------------------------------------
//	void vector2_Dot( ScriptValue* res, T* v1, T* v2 )//带返回值
//	{
//		res->SetValue( NEW P( v1->Dot( *v2 ) ), false );
//	}
//	//-------------------------------------------------------------------------------------------------------
//	void vector2_NormalizeSelf( T* v1 )//不带返回值
//	{
//		v1->NormalizeSelf();
//	}
//	//-------------------------------------------------------------------------------------------------------
//	static void Register_( IScriptSystem& sys )
//	{
//		ClassRegister sc;
//		sc.ClassType( &T::RTTI() );
//
//		{
//			FunctionRegister sf;
//			sf.Result( sys, &T::RTTI() );
//			sf.Function( &BohgeEngine::_Construct, ScriptProperty::__CON );
//			sc.ClassFunction( sf );
//		}
//
//		{
//			FunctionRegister sf;
//			sf.Result( sys, &P::RTTI() );
//			sf.Arguments( sys, &T::RTTI() );
//			sf.Function( &BohgeEngine::vector2_Dot<T,P>, "Dot" );
//			sc.ClassFunction( sf );
//		}
//
//		{
//			FunctionRegister sf;
//			sf.Function( &BohgeEngine::vector2_NormalizeSelf<T>, "NormalizeSelf" );
//			sc.ClassFunction( sf );
//		}
//		sys.RegisterClass(sc);
//	}
//}