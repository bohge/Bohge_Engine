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
	static void Matrix44_Construct( ScriptValue* res )
	{
		res->SetValue( NEW Matrix44f(), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix44_SetTransform( Matrix44f* self, vector3f* v )
	{
		self->SetTransform( *v );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix44_Mul_mat44( ScriptValue* res, Matrix44f* v1, Matrix44f* v2 )
	{
		Matrix44f* mat44 = NEW Matrix44f( *v1 * *v2 );
		res->SetValue( mat44, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix44_GetInverse( ScriptValue* res, Matrix44f* v1 )
	{
		Matrix44f* mat44 = NEW Matrix44f;
		v1->GetInverse( *mat44 );
		res->SetValue( mat44, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix44_InverseSelf( Matrix44f* v1 )
	{
		v1->InverseSelf();
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix44_CreateRotateMatrix( ScriptValue* res, vector3f* axis, float* f1 )
	{
		res->SetValue( NEW Matrix44f( Matrix44f::CreateRotateMatrix( *axis, *f1) ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix44_CreateRotateMatrixX( ScriptValue* res, float* f1 )
	{
		res->SetValue( NEW Matrix44f( Matrix44f::CreateRotateMatrixX(*f1) ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix44_CreateRotateMatrixY( ScriptValue* res, float* f1 )
	{
		res->SetValue( NEW Matrix44f( Matrix44f::CreateRotateMatrixY(*f1) ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Matrix44_CreateRotateMatrixZ( ScriptValue* res, float* f1 )
	{
		res->SetValue( NEW Matrix44f( Matrix44f::CreateRotateMatrixZ(*f1) ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Matrix44( IScriptSystem& sys )
	{
		ClassRegister sc;
		sc.ClassType( &Matrix44f::RTTI() );
		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix44f::RTTI() );
			sf.Function( &BohgeEngine::Matrix44_Construct, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Arguments( sys, &vector3f::RTTI() );
			sf.Function( &BohgeEngine::Matrix44_SetTransform, "SetTransform" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Arguments( sys, &Matrix44f::RTTI() );
			sf.Function( &BohgeEngine::Matrix44_InverseSelf, "InverseSelf" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix44f::RTTI() );
			sf.Function( &BohgeEngine::Matrix44_GetInverse, "GetInverse" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix44f::RTTI() );
			sf.Arguments( sys, &Matrix44f::RTTI() );
			sf.Function( &BohgeEngine::Matrix44_Mul_mat44, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix44f::RTTI() );
			sf.Arguments( sys, &vector3f::RTTI(), &Number::RTTI() );
			sf.SetFunctionType( ScriptProperty::FT_STATIC );
			sf.Function( &BohgeEngine::Matrix44_CreateRotateMatrix, "CreateRotateMatrix" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix44f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.SetFunctionType( ScriptProperty::FT_STATIC );
			sf.Function( &BohgeEngine::Matrix44_CreateRotateMatrixX, "CreateRotateMatrixX" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix44f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.SetFunctionType( ScriptProperty::FT_STATIC );
			sf.Function( &BohgeEngine::Matrix44_CreateRotateMatrixY, "CreateRotateMatrixY" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Matrix44f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.SetFunctionType( ScriptProperty::FT_STATIC );
			sf.Function( &BohgeEngine::Matrix44_CreateRotateMatrixZ, "CreateRotateMatrixZ" );
			sc.ClassFunction( sf );
		}
		sys.RegisterClass(sc);
	}
}