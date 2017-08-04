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
#include "RenderTargetEntity.h"
#include "FunctionRegister.h"
#include "IScriptSystem.h"
#include "TextureEntity.h"
#include "ClassRegister.h"
#include "ScriptValue.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	static void RenderTargetEntity_PushRenderTarget( ScriptValue* res, RenderTargetEntity* rt )
	{
		RenderTargetEntity* old = rt->PushRenderTarget();
		res->SetValue( old, &ScriptValue::NullCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void RenderTargetEntity_GetAttachment(  ScriptValue* res, RenderTargetEntity* rt, float* param )
	{
		const TextureEntity* tex = rt->GetAttachment( static_cast<RenderTargetProperty::RenderTargetAttachment>( (int)*param ) );
		res->SetValue( const_cast<TextureEntity*>(tex), &ScriptValue::NullCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void RenderTargetEntity_ClearBuffer( RenderTargetEntity* rt, float* param )
	{
		rt->ClearBuffer( static_cast<RenderTargetProperty::ClearFlag>( (int)*param ) );
	}
	//-------------------------------------------------------------------------------------------------------
	static void RenderTargetEntity_ActiveTargetFace( RenderTargetEntity* rt, float* param )
	{
		rt->ActiveTargetFace( static_cast<RenderTargetProperty::TargetFace>( (int)*param ) );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_RenderTargetEntity( IScriptSystem& sys )
	{
		{
			ClassRegister sc;
			sc.ClassType( &RenderTargetEntity::RTTI() );
			{
				FunctionRegister sf;
				sf.Result( sys, &RenderTargetEntity::RTTI() );
				sf.Function( &BohgeEngine::RenderTargetEntity_PushRenderTarget, "PushRenderTarget" );
				sc.ClassFunction( sf );
			}

			{
				FunctionRegister sf;
				sf.Arguments( sys, &Number::RTTI() );
				sf.Function( &BohgeEngine::RenderTargetEntity_ClearBuffer, "ClearBuffer" );
				sc.ClassFunction( sf );
			}

			{
				FunctionRegister sf;
				sf.Arguments( sys, &Number::RTTI() );
				sf.Function( &BohgeEngine::RenderTargetEntity_ActiveTargetFace, "ActiveTargetFace" );
				sc.ClassFunction( sf );
			}

			{
				FunctionRegister sf;
				sf.Result( sys, &TextureEntity::RTTI() );
				sf.Arguments( sys, &Number::RTTI() );
				sf.Function( &BohgeEngine::RenderTargetEntity_GetAttachment, "GetAttachment" );
				sc.ClassFunction( sf );
			}
			sys.RegisterClass(sc);
		}

		{
			ClassRegister sc;
			sc.ClassType( &RenderTargetProperty::RTTI() );
			sc.ClassConstant( RenderTargetProperty::ST_SWAP_A, "ST_SWAP_A" );
			sc.ClassConstant( RenderTargetProperty::ST_SWAP_B, "ST_SWAP_B" );
			sc.ClassConstant( RenderTargetProperty::ST_SWAP_C, "ST_SWAP_C" );
			sc.ClassConstant( RenderTargetProperty::ST_SWAP_D, "ST_SWAP_D" );

			sc.ClassConstant( RenderTargetProperty::RT_RENDER_TARGET_MAIN, "RT_RENDER_TARGET_MAIN" );
			sc.ClassConstant( RenderTargetProperty::RT_RENDER_TARGET_2D, "RT_RENDER_TARGET_2D" );
			sc.ClassConstant( RenderTargetProperty::RT_RENDER_TARGET_CUBE, "RT_RENDER_TARGET_CUBE" );

			sc.ClassConstant( RenderTargetProperty::TF_POSITIVE_X, "TF_POSITIVE_X" );
			sc.ClassConstant( RenderTargetProperty::TF_NEGATIVE_X, "TF_NEGATIVE_X" );
			sc.ClassConstant( RenderTargetProperty::TF_POSITIVE_Y, "TF_POSITIVE_Y" );
			sc.ClassConstant( RenderTargetProperty::TF_NEGATIVE_Y, "TF_NEGATIVE_Y" );
			sc.ClassConstant( RenderTargetProperty::TF_POSITIVE_Z, "TF_POSITIVE_Z" );
			sc.ClassConstant( RenderTargetProperty::TF_NEGATIVE_Z, "TF_NEGATIVE_Z" );
			sc.ClassConstant( RenderTargetProperty::TF_DEFAULT, "TF_DEFAULT" );

			sc.ClassConstant( RenderTargetProperty::TA_COLOR_0, "TA_COLOR_0" );
			sc.ClassConstant( RenderTargetProperty::TA_COLOR_1, "TA_COLOR_1" );
			sc.ClassConstant( RenderTargetProperty::TA_COLOR_2, "TA_COLOR_2" );
			sc.ClassConstant( RenderTargetProperty::TA_COLOR_3, "TA_COLOR_3" );
			sc.ClassConstant( RenderTargetProperty::TA_COLOR_4, "TA_COLOR_4" );
			sc.ClassConstant( RenderTargetProperty::TA_COLOR_5, "TA_COLOR_5" );
			sc.ClassConstant( RenderTargetProperty::TA_COLOR_6, "TA_COLOR_6" );
			sc.ClassConstant( RenderTargetProperty::TA_COLOR_7, "TA_COLOR_7" );
			sc.ClassConstant( RenderTargetProperty::TA_COLOR_8, "TA_COLOR_8" );
			sc.ClassConstant( RenderTargetProperty::TA_COLOR_9, "TA_COLOR_9" );
			sc.ClassConstant( RenderTargetProperty::TA_DEPTH, "TA_DEPTH" );
			sc.ClassConstant( RenderTargetProperty::TA_DEPTH_STENCIL, "TA_DEPTH_STENCIL" );

			sc.ClassConstant( RenderTargetProperty::CF_COLOR, "CF_COLOR" );
			sc.ClassConstant( RenderTargetProperty::CF_DEPTH, "CF_DEPTH" );
			sc.ClassConstant( RenderTargetProperty::CF_STENCIL, "CF_STENCIL" );
			sc.ClassConstant( RenderTargetProperty::CF_COLOR_DEPTH, "CF_COLOR_DEPTH" );
			sc.ClassConstant( RenderTargetProperty::CF_COLOR_STENCIL, "CF_COLOR_STENCIL" );
			sc.ClassConstant( RenderTargetProperty::CF_DEPTH_STENCIL, "CF_DEPTH_STENCIL" );
			sc.ClassConstant( RenderTargetProperty::CF_COLOR_DEPTH_STENCIL, "CF_COLOR_DEPTH_STENCIL" );

			sys.RegisterClass(sc);
		}
	}
}