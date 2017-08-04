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
#include "3DMath.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	static void Matrix33_Construct( ScriptValue* res )
	{
		res->SetValue( NEW Matrix33f(), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix33_Construct_P9( ScriptValue* res, float* f1, float* f2, float* f3, float* f4, float* f5, float* f6, float* f7, float* f8, float* f9 )
	{
		res->SetValue( NEW Matrix33f( *f1, *f2, *f3, *f4, *f5, *f6, *f7, *f8, *f9 ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix33_YawPitchRoll( Matrix33f* self, float* y, float* p, float* r )
	{
		self->YawPitchRoll( *y, *p, *r );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix33_CreateRotateMatrixX( ScriptValue* res, float* f1 )
	{
		res->SetValue( NEW Matrix33f( Matrix33f::CreateRotateMatrixX(*f1) ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix33_CreateRotateMatrixY( ScriptValue* res, float* f1 )
	{
		res->SetValue( NEW Matrix33f( Matrix33f::CreateRotateMatrixY(*f1) ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix33_CreateRotateMatrixZ( ScriptValue* res, float* f1 )
	{
		res->SetValue( NEW Matrix33f( Matrix33f::CreateRotateMatrixZ(*f1) ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix33_Mul_mat( ScriptValue* res, Matrix33f* v1, Matrix33f* v2 )
	{
		Matrix33f* mat33 = NEW Matrix33f( *v1 * *v2 );
		res->SetValue( mat33, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Matrix33( IScriptSystem& sys )
	{
		ClassRegister sc;
		sc.ClassType( &Matrix33f::RTTI() );
		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix33f::RTTI() );
			sf.Function( &BohgeEngine::Matrix33_Construct, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix33f::RTTI() );
			sf.Arguments( sys, &Number::RTTI(), &Number::RTTI(), &Number::RTTI(), &Number::RTTI(), &Number::RTTI(), &Number::RTTI(), &Number::RTTI(), &Number::RTTI(), &Number::RTTI() );
			sf.Function( &BohgeEngine::Matrix33_Construct_P9, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::Matrix33_YawPitchRoll, "YawPitchRoll" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix33f::RTTI() );
			sf.Arguments( sys, &Matrix33f::RTTI() );
			sf.Function( &BohgeEngine::Matrix33_Mul_mat, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix33f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.SetFunctionType( ScriptProperty::FT_STATIC );
			sf.Function( &BohgeEngine::Matrix33_CreateRotateMatrixX, "CreateRotateMatrixX" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix33f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.SetFunctionType( ScriptProperty::FT_STATIC );
			sf.Function( &BohgeEngine::Matrix33_CreateRotateMatrixY, "CreateRotateMatrixY" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix33f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.SetFunctionType( ScriptProperty::FT_STATIC );
			sf.Function( &BohgeEngine::Matrix33_CreateRotateMatrixZ, "CreateRotateMatrixZ" );
			sc.ClassFunction( sf );
		}
		sys.RegisterClass(sc);
	}
}