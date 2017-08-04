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
#include "Framework/IAsynvEntityWrap.hpp"
#include "Framework/FunctionRegister.h"
#include "Framework/IScriptSystem.h"
#include "Framework/ClassRegister.h"
#include "Framework/ScriptValue.h"
#include "UIEntity.h"
#include "UILayout.h"


namespace BGUI
{
	//-------------------------------------------------------------------------------------------------------
	static void UIEntity_Construct( BohgeEngine::ScriptValue* res )
	{
		res->SetValue( NEW UIEntity, &BohgeEngine::ScriptValue::SafeDeleteCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void UIEntity_FindLayout( BohgeEngine::ScriptValue* res, UIEntity* self, char* name )
	{
		res->SetValue( self->FindLayout( name ), &BohgeEngine::ScriptValue::NullCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_UIEntity( BohgeEngine::IScriptSystem& sys )
	{
		BohgeEngine::ClassRegister sc;
		sc.ClassType( &UIEntity::RTTI() );
		{
			BohgeEngine::FunctionRegister sf;
			sf.Result( sys, &UIEntity::RTTI() );
			sf.Function( &BGUI::UIEntity_Construct, BohgeEngine::ScriptProperty::__CON );
			sc.ClassFunction( sf );
		}
		{
			BohgeEngine::FunctionRegister sf;
			sf.Result( sys, &UILayout::RTTI() );
			sf.Arguments( sys, &BohgeEngine::String::RTTI() );
			sf.Function( &BGUI::UIEntity_FindLayout, "FindLayout" );
			sc.ClassFunction( sf );
		}
		BohgeEngine::IAsyncEntity_Helper<UIEntity>( sys, sc );
		sys.RegisterClass( sc );
	}
}