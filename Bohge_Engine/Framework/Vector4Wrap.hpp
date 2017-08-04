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
	static void vector4_Construct( ScriptValue* res )
	{
		res->SetValue( NEW vector4f(), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Construct_P4( ScriptValue* res, float* f1, float* f2, float* f3, float* f4 )
	{
		res->SetValue( NEW vector4f( *f1, *f2, *f3, *f4 ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Add_vec( ScriptValue* res, vector4f* v1, vector4f* v2 )
	{
		vector4f* vec2 = NEW vector4f( *v1 + *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Add_num( ScriptValue* res, vector4f* v1, float* v2 )
	{
		vector4f* vec2 = NEW vector4f( *v1 + *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Sub_vec( ScriptValue* res, vector4f* v1, vector4f* v2 )
	{
		vector4f* vec2 = NEW vector4f( *v1 - *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Sub_num( ScriptValue* res, vector4f* v1, float* v2 )
	{
		vector4f* vec2 = NEW vector4f( *v1 - *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Mul_vec( ScriptValue* res, vector4f* v1, vector4f* v2 )
	{
		vector4f* vec2 = NEW vector4f( *v1 * *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Mul_num( ScriptValue* res, vector4f* v1, float* v2 )
	{
		vector4f* vec2 = NEW vector4f( *v1 * *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Mul_M44( ScriptValue* res, vector4f* v1, Matrix44f* m )
	{
		vector4f* vec2 = NEW vector4f( *v1 * *m );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Mul_QUA( ScriptValue* res, vector4f* v1, Quaternionf* q )
	{
		vector4f* vec2 = NEW vector4f( *v1 * *q );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Div_vec( ScriptValue* res, vector4f* v1, vector4f* v2 )
	{
		vector4f* vec2 = NEW vector4f( *v1 / *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Div_num( ScriptValue* res, vector4f* v1, float* v2 )
	{
		vector4f* vec2 = NEW vector4f( *v1 / *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Equal( bool* res, vector4f* v1, vector4f* v2 )
	{
		*res = *v1 == *v2;
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_NormalizeSelf( vector4f* v1 )
	{
		v1->NormalizeSelf();
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Normalize( ScriptValue* res, vector4f* v1 )
	{
		res->SetValue( NEW vector4f( v1->Normalize() ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Length( float* res, vector4f* v1 )
	{
		*res = v1->Length();
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_LengthPow( float* res, vector4f* v1 )
	{
		*res = v1->LengthPow();
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_X( float* res, vector4f* v1 )
	{
		*res = v1->m_x;
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Y( float* res, vector4f* v1 )
	{
		*res = v1->m_y;
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_Z( float* res, vector4f* v1 )
	{
		*res = v1->m_z;
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector4_W( float* res, vector4f* v1 )
	{
		*res = v1->m_w;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_vector4( IScriptSystem& sys )
	{
		ClassRegister sc;
		sc.ClassType( &vector4f::RTTI() );

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector4_X, "x" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector4_Y, "y" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector4_Z, "z" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector4_W, "w" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Function( &BohgeEngine::vector4_Construct, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Arguments( sys, &Number::RTTI(), &Number::RTTI(), &Number::RTTI(), &Number::RTTI() );
			sf.Function( &BohgeEngine::vector4_Construct_P4, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Arguments( sys, &vector4f::RTTI() );
			sf.Function( &BohgeEngine::vector4_Add_vec, ScriptProperty::__ADD );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector4_Add_num, ScriptProperty::__ADD );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Arguments( sys, &vector4f::RTTI() );
			sf.Function( &BohgeEngine::vector4_Sub_vec, ScriptProperty::__SUB );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector4_Sub_num, ScriptProperty::__SUB );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Arguments( sys, &vector4f::RTTI() );
			sf.Function( &BohgeEngine::vector4_Mul_vec, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector4_Mul_num, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Arguments( sys, &Matrix44f::RTTI() );
			sf.Function( &BohgeEngine::vector4_Mul_M44, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Arguments( sys, &Quaternionf::RTTI() );
			sf.Function( &BohgeEngine::vector4_Mul_QUA, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Arguments( sys, &vector4f::RTTI() );
			sf.Function( &BohgeEngine::vector4_Div_vec, ScriptProperty::__DIV );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector4_Div_num, ScriptProperty::__DIV );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Bool::RTTI() );
			sf.Arguments( sys, &vector4f::RTTI() );
			sf.Function( &BohgeEngine::vector4_Equal, ScriptProperty::__EQU );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Function( &BohgeEngine::vector4_NormalizeSelf, "NormalizeSelf" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector4f::RTTI() );
			sf.Function( &BohgeEngine::vector4_Normalize, "Normalize" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector4_Length, "Length" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector4_LengthPow, "LengthPow" );
			sc.ClassFunction( sf );
		}

		sys.RegisterClass(sc);
	}
}