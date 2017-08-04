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
#include "FunctionRegister.h"
#include "IScriptSystem.h"
#include "TextureEntity.h"
#include "ClassRegister.h"
#include "ScriptValue.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	static void RenderObjectEntity_SetTextureEntity( RenderObjectEntity* ro, float* param, TextureEntity* tex )
	{
		ro->SetTextureEntity( static_cast<MaterialProperty::Material_Attribute>( (int)*param ), tex );
	}
	//-------------------------------------------------------------------------------------------------------
	static void RenderObjectEntity_SetUserParameters( RenderObjectEntity* ro, float* att, float* spt, Object* obj )
	{
		ro->SetUserParameters( 
			static_cast<MaterialProperty::Material_Attribute>( (int)*att ),
			static_cast<MaterialProperty::UserParameterType>( (int)*spt ),
			*obj );
	}
	//-------------------------------------------------------------------------------------------------------
	static void RenderObjectEntity_Draw( RenderObjectEntity* ro, float* param )
	{
		ro->Draw( *param );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_RenderObjectEntity( IScriptSystem& sys )
	{
		{
			ClassRegister sc;
			sc.ClassType( &RenderObjectEntity::RTTI() );
			{
				FunctionRegister sf;
				sf.Arguments( sys, &Number::RTTI(), &TextureEntity::RTTI() );
				sf.Function( &BohgeEngine::RenderObjectEntity_SetTextureEntity, "SetTextureEntity" );
				sc.ClassFunction( sf );
			}
			{
				FunctionRegister sf;
				sf.Arguments( sys, &Number::RTTI(), &Number::RTTI(), &Object::RTTI() );
				sf.Function( &BohgeEngine::RenderObjectEntity_SetUserParameters, "SetUserParameters" );
				sc.ClassFunction( sf );
			}
			{
				FunctionRegister sf;
				sf.Arguments( sys, &Number::RTTI() );
				sf.Function( &BohgeEngine::RenderObjectEntity_Draw, "Draw" );
				sc.ClassFunction( sf );
			}
			sys.RegisterClass(sc);
		}
	}
}