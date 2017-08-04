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
	static void vector2_Construct( ScriptValue* res )
	{
		res->SetValue( NEW vector2f(), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Construct_P2( ScriptValue* res, float* f1, float* f2 )
	{
		res->SetValue( NEW vector2f( *f1, *f2 ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Add_vec( ScriptValue* res, vector2f* v1, vector2f* v2 )
	{
		vector2f* vec2 = NEW vector2f( *v1 + *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Add_num( ScriptValue* res, vector2f* v1, float* v2 )
	{
		vector2f* vec2 = NEW vector2f( *v1 + *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Sub_vec( ScriptValue* res, vector2f* v1, vector2f* v2 )
	{
		vector2f* vec2 = NEW vector2f( *v1 - *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Sub_num( ScriptValue* res, vector2f* v1, float* v2 )
	{
		vector2f* vec2 = NEW vector2f( *v1 - *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Mul_vec( ScriptValue* res, vector2f* v1, vector2f* v2 )
	{
		vector2f* vec2 = NEW vector2f( *v1 * *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Mul_num( ScriptValue* res, vector2f* v1, float* v2 )
	{
		vector2f* vec2 = NEW vector2f( *v1 * *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Div_vec( ScriptValue* res, vector2f* v1, vector2f* v2 )
	{
		vector2f* vec2 = NEW vector2f( *v1 / *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Div_num( ScriptValue* res, vector2f* v1, float* v2 )
	{
		vector2f* vec2 = NEW vector2f( *v1 / *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Equal( bool* res, vector2f* v1, vector2f* v2 )
	{
		*res = *v1 == *v2;
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Dot( float* res, vector2f* v1, vector2f* v2 )
	{
		*res = v1->Dot( *v2 );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_NormalizeSelf( vector2f* v1 )
	{
		v1->NormalizeSelf();
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Normalize( ScriptValue* res, vector2f* v1 )
	{
		res->SetValue( NEW vector2f( v1->Normalize() ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Length( float* res, vector2f* v1 )
	{
		*res = v1->Length();
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_LengthPow( float* res, vector2f* v1 )
	{
		*res = v1->LengthPow();
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_X( float* res, vector2f* v1 )
	{
		*res = v1->m_x;
	}
	//-------------------------------------------------------------------------------------------------------
	static void vector2_Y( float* res, vector2f* v1 )
	{
		*res = v1->m_y;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_vector2( IScriptSystem& sys )
	{
		ClassRegister sc;
		sc.ClassType( &vector2f::RTTI() );

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector2_X, "x" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector2_Y, "y" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector2f::RTTI() );
			sf.Function( &BohgeEngine::vector2_Construct, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector2f::RTTI() );
			sf.Arguments( sys, &Number::RTTI(), &Number::RTTI() );
			sf.Function( &BohgeEngine::vector2_Construct_P2, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector2f::RTTI() );
			sf.Arguments( sys, &vector2f::RTTI() );
			sf.Function( &BohgeEngine::vector2_Add_vec, ScriptProperty::__ADD );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector2f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector2_Add_num, ScriptProperty::__ADD );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector2f::RTTI() );
			sf.Function( &BohgeEngine::vector2_Sub_vec, ScriptProperty::__SUB );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector2f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector2_Sub_num, ScriptProperty::__SUB );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector2f::RTTI() );
			sf.Arguments( sys, &vector2f::RTTI() );
			sf.Function( &BohgeEngine::vector2_Mul_vec, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector2f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector2_Mul_num, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector2f::RTTI() );
			sf.Arguments( sys, &vector2f::RTTI() );
			sf.Function( &BohgeEngine::vector2_Div_vec, ScriptProperty::__DIV );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector2f::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector2_Div_num, ScriptProperty::__DIV );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Bool::RTTI() );
			sf.Arguments( sys, &vector2f::RTTI() );
			sf.Function( &BohgeEngine::vector2_Equal, ScriptProperty::__EQU );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Arguments( sys, &vector2f::RTTI() );
			sf.Function( &BohgeEngine::vector2_Dot, "Dot" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Function( &BohgeEngine::vector2_NormalizeSelf, "NormalizeSelf" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &vector2f::RTTI() );
			sf.Function( &BohgeEngine::vector2_Normalize, "Normalize" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector2_Length, "Length" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::vector2_LengthPow, "LengthPow" );
			sc.ClassFunction( sf );
		}

		sys.RegisterClass(sc);
	}
}