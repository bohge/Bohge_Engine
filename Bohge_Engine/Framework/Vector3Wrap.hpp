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
	static void vector3_Construct( ScriptValue* res )
	{
		res->SetValue( NEW vector3f(), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Construct_P3( ScriptValue* res, float* f1, float* f2, float* f3 )
	{
		res->SetValue( NEW vector3f( *f1, *f2, *f3 ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Add_vec( ScriptValue* res, vector3f* v1, vector3f* v2 )
	{
		vector3f* vec2 = NEW vector3f( *v1 + *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Add_num( ScriptValue* res, vector3f* v1, float* v2 )
	{
		vector3f* vec2 = NEW vector3f( *v1 + *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Sub_vec( ScriptValue* res, vector3f* v1, vector3f* v2 )
	{
		vector3f* vec2 = NEW vector3f( *v1 - *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Sub_num( ScriptValue* res, vector3f* v1, float* v2 )
	{
		vector3f* vec2 = NEW vector3f( *v1 - *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Mul_vec( ScriptValue* res, vector3f* v1, vector3f* v2 )
	{
		vector3f* vec2 = NEW vector3f( *v1 * *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Mul_num( ScriptValue* res, vector3f* v1, float* v2 )
	{
		vector3f* vec2 = NEW vector3f( *v1 * *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Mul_mat33( ScriptValue* res, vector3f* v1, Matrix33f* m33 )
	{
		vector3f* vec2 = NEW vector3f( *v1 * *m33 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Mul_mat44( ScriptValue* res, vector3f* v1, Matrix44f* m44 )
	{
		vector3f* vec2 = NEW vector3f( *v1 * *m44 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Mul_qua( ScriptValue* res, vector3f* v1, Quaternionf* qua )
	{
		vector3f* vec2 = NEW vector3f( *v1 * *qua );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Div_vec( ScriptValue* res, vector3f* v1, vector3f* v2 )
	{
		vector3f* vec2 = NEW vector3f( *v1 / *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Div_num( ScriptValue* res, vector3f* v1, float* v2 )
	{
		vector3f* vec2 = NEW vector3f( *v1 / *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Equal( bool* res, vector3f* v1, vector3f* v2 )
	{
		*res = *v1 == *v2;
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Dot( float* res, vector3f* v1, vector3f* v2 )
	{
		*res = v1->Dot( *v2 );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_NormalizeSelf( vector3f* v1 )
	{
		v1->NormalizeSelf();
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Normalize( ScriptValue* res, vector3f* v1 )
	{
		res->SetValue( NEW vector3f( v1->Normalize() ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Length( float* res, vector3f* v1 )
	{
		*res = v1->Length();
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_LengthPow( float* res, vector3f* v1 )
	{
		*res = v1->LengthPow();
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_X( float* res, vector3f* v1 )
	{
		*res = v1->m_x;
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Y( float* res, vector3f* v1 )
	{
		*res = v1->m_y;
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector3_Z( float* res, vector3f* v1 )
	{
		*res = v1->m_z;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_vector3( IScriptSystem& sys )
	{
		ClassRegister sc;
		sc.ClassType( &vector3f::RTTI() );

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector3_X, "x" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector3_Y, "y" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector3_Z, "z" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Function( &BohgeEngine::vector3_Construct, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &Number::RTTI(), &Number::RTTI(), &Number::RTTI() );
			sf.Function( &BohgeEngine::vector3_Construct_P3, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &vector3f::RTTI() );
			sf.Function( &BohgeEngine::vector3_Add_vec, ScriptProperty::__ADD );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector3_Add_num, ScriptProperty::__ADD );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &vector3f::RTTI() );
			sf.Function( &BohgeEngine::vector3_Sub_vec, ScriptProperty::__SUB );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector3_Sub_num, ScriptProperty::__SUB );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &vector3f::RTTI() );
			sf.Function( &BohgeEngine::vector3_Mul_vec, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &Matrix33f::RTTI() );
			sf.Function( &BohgeEngine::vector3_Mul_mat33, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &Matrix44f::RTTI() );
			sf.Function( &BohgeEngine::vector3_Mul_mat44, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &Quaternionf::RTTI() );
			sf.Function( &BohgeEngine::vector3_Mul_qua, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector3_Mul_num, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &vector3f::RTTI() );
			sf.Function( &BohgeEngine::vector3_Div_vec, ScriptProperty::__DIV );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector3_Div_num, ScriptProperty::__DIV );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Bool::RTTI() );
			sf.Arguments( sys, &vector3f::RTTI() );
			sf.Function( &BohgeEngine::vector3_Equal, ScriptProperty::__EQU );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Arguments( sys, &vector3f::RTTI() );
			sf.Function( &BohgeEngine::vector3_Dot, "Dot" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Function( &BohgeEngine::vector3_NormalizeSelf, "NormalizeSelf" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector3f::RTTI() );
			sf.Function( &BohgeEngine::vector3_Normalize, "Normalize" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector3_Length, "Length" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector3_LengthPow, "LengthPow" );
			sc.ClassFunction( sf );
		}

		sys.RegisterClass(sc);
	}
}