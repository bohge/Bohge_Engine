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
	static void Quaternion_Construct( ScriptValue* res )
	{
		res->SetValue( NEW Quaternionf(), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_Construct_P4( ScriptValue* res, float* f1, float* f2, float* f3, float* f4 )
	{
		res->SetValue( NEW Quaternionf( *f1, *f2, *f3, *f4 ), &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_Add_qua( ScriptValue* res, Quaternionf* v1, Quaternionf* v2 )
	{
		Quaternionf* vec2 = NEW Quaternionf( *v1 + *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_Div_qua( ScriptValue* res, Quaternionf* v1, Quaternionf* v2 )
	{
		Quaternionf* vec2 = NEW Quaternionf( *v1 - *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_Mul_qua( ScriptValue* res, Quaternionf* v1, Quaternionf* v2 )
	{
		Quaternionf* vec2 = NEW Quaternionf( *v1 * *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_Mul_num( ScriptValue* res, Quaternionf* v1, float* v2 )
	{
		Quaternionf* vec2 = NEW Quaternionf( *v1 * *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_Div_num( ScriptValue* res, Quaternionf* v1, float* v2 )
	{
		Quaternionf* vec2 = NEW Quaternionf( *v1 / *v2 );
		res->SetValue( vec2, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_Equal( bool* res, Quaternionf* v1, Quaternionf* v2 )
	{
		*res = v1->operator==( *v2 );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_NormalizeSelf( Quaternionf* v1 )
	{
		v1->NormalizeSelf();
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_Dot( float* res, Quaternionf* q1, Quaternionf* q2 )
	{
		*res = q1->Dot( *q2 );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_YawPitchRoll( Quaternionf* self, float* y, float* p, float* r )
	{
		self->YawPitchRoll( *y, *p, *r );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_RotateXYZ( Quaternionf* self, float* x, float* y, float* z )
	{
		self->RotateXYZ( *x, *y, *z );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_AxisToAxis( Quaternionf* self, vector3f* f, vector3f* t )
	{
		self->AxisToAxis( *f, *t );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_RotateAxis( Quaternionf* self, vector3f* a, float* r )
	{
		self->RotateAxis( *a, *r );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_Length( float* res, Quaternionf* v1 )
	{
		*res = v1->Length();
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_LengthPow( float* res, Quaternionf* v1 )
	{
		*res = v1->LengthPow();
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_GetInverse( ScriptValue* res, Quaternionf* v1 )
	{
		Quaternionf* qua = NEW Quaternionf;
		v1->GetInverse( *qua );
		res->SetValue( qua, &ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_InverseSelf( Quaternionf* self )
	{
		self->InverseSelf();
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_X( float* res, Quaternionf* v1 )
	{
		*res =  v1->m_x;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_Y( float* res, Quaternionf* v1 )
	{
		*res = v1->m_y;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_Z( float* res, Quaternionf* v1 )
	{
		*res = v1->m_z;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Quaternion_W( float* res, Quaternionf* v1 )
	{
		*res = v1->m_w;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Quaternion( IScriptSystem& sys )
	{
		ClassRegister sc;
		sc.ClassType( &Quaternionf::RTTI() );

		{
			FunctionRegister sf;
			sf.Result( sys, &Quaternionf::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Construct, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Quaternionf::RTTI() );
			sf.Arguments( sys, &Number::RTTI(), &Number::RTTI(), &Number::RTTI(), &Number::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Construct_P4, ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Quaternionf::RTTI() );
			sf.Arguments( sys, &Quaternionf::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Add_qua, ScriptProperty::__ADD );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Quaternionf::RTTI() );
			sf.Arguments( sys, &Quaternionf::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Div_qua, ScriptProperty::__DIV );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Quaternionf::RTTI() );
			sf.Arguments( sys, &Quaternionf::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Mul_qua, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Quaternionf::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Mul_num, ScriptProperty::__MUL );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Quaternionf::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Div_num, ScriptProperty::__DIV );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Bool::RTTI() );
			sf.Arguments( sys, &Quaternionf::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Equal, ScriptProperty::__EQU );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Function( &BohgeEngine::Quaternion_NormalizeSelf, "NormalizeSelf" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Arguments( sys, &Quaternionf::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Dot, "Dot" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &Number::RTTI(), &Number::RTTI(), &Number::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_YawPitchRoll, "YawPitchRoll" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &vector3f::RTTI(), &vector3f::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_AxisToAxis, "AxisToAxis" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &vector3f::RTTI(), &Number::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_RotateAxis, "RotateAxis" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Length, "Length" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_LengthPow, "LengthPow" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Quaternionf::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_GetInverse, "GetInverse" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Function( &BohgeEngine::Quaternion_InverseSelf, "InverseSelf" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_X, "x" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Y, "y" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_Z, "z" );
			sc.ClassFunction( sf );
		}

		{
			FunctionRegister sf;
			sf.Result( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::Quaternion_W, "w" );
			sc.ClassFunction( sf );
		}



		sys.RegisterClass(sc);
	}
}