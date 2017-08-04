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
#include "IComponent.h"
#include "Node.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	static void Node_GetName( eastl::string* res, Node* ui )
	{
		*res = ui->GetName();
	}
	//-------------------------------------------------------------------------------------------------------
	static void Node_AttachNode( Node* node, Node* son )
	{
		node->AttachNode( son );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Node_DetachNode( Node* node, Node* son )
	{
		node->DetachNode( son );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Node_AttachComponent( Node* node, IComponent* com )
	{
		node->AttachComponent( com );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Node_FindComponent( ScriptValue* res, Node* node, char* name )
	{
		IComponent* com = node->FindComponent( name );
		res->SetValue( com, &ScriptValue::NullCleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Node( IScriptSystem& sys )
	{
		ClassRegister sc;
		sc.ClassType( &Node::RTTI() );
		{
			BohgeEngine::FunctionRegister sf;
			sf.Result( sys, &BohgeEngine::String::RTTI() );
			sf.Function( &BohgeEngine::Node_GetName, "GetName" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &Node::RTTI() );
			sf.Function( &BohgeEngine::Node_AttachNode, "AttachNode" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &Node::RTTI() );
			sf.Function( &BohgeEngine::Node_DetachNode, "DetachNode" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Arguments( sys, &IComponent::RTTI() );
			sf.Function( &BohgeEngine::Node_AttachComponent, "AttachComponent" );
			sc.ClassFunction( sf );
		}
		{
			FunctionRegister sf;
			sf.Result( sys, &IComponent::RTTI() );
			sf.Arguments( sys, &String::RTTI() );
			sf.Function( &BohgeEngine::Node_FindComponent, "FindComponent" );
			sc.ClassFunction( sf );
		}
		sys.RegisterClass( sc );
	}
}