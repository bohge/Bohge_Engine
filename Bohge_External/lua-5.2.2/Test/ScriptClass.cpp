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
#include "ScriptClass.h"
#include "Predefine.h"
#include "ScriptFunction.h"
#include "ScriptType.h"


using namespace std;

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	ScriptClass::ScriptClass(void)
		:m_prRtti(NULL),
		m_prBase(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ScriptClass::~ScriptClass(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void ScriptClass::_DoCopy( const ScriptClass& sc )
	{
		//m_prRtti = sc.m_prRtti;
		//m_prBase = sc.m_prBase;
		//FOR_EACH_CONST( ClassConstantArray, cConst, sc.m_ConstantArray )
		//{
		//	m_ConstantArray.push_back( make_pair( cConst->first, NEW  ) )
		//}
	}
	//-------------------------------------------------------------------------------------------------------
	void ScriptClass::MemberFunction( const ScriptFunction& func )
	{
		ScriptFunction* pFunc = NEW ScriptFunction(func);
		FuncitonArray::iterator funcOvMp = m_FuncArray.end();
		FOR_EACH( FuncitonArray, it, m_FuncArray )
		{
			if ( it->first == pFunc->GetName() )
			{
				funcOvMp = it;
				break;
			}
		}
		if ( m_FuncArray.end() == funcOvMp ) //第一次加入这个名称的函数
		{
			OverlappedMap om;
			om.insert( make_pair( pFunc->GetID(), pFunc ) );
			m_FuncArray.push_back( make_pair( pFunc->GetName(), om ) );
		}
		else//找到函数序列了
		{
			ASSERT( funcOvMp->second.end() == funcOvMp->second.find( pFunc->GetID() ) );
			funcOvMp->second.insert( make_pair( pFunc->GetID(), pFunc ) );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ScriptClass::ClassType( const Rtti* rtti )
	{
		m_prRtti = rtti;
	}
	//-------------------------------------------------------------------------------------------------------
	void ScriptClass::ClassType( const Rtti* rtti, const Rtti* base )
	{
		ClassType(rtti);
		m_prBase = base;
	}
	//-------------------------------------------------------------------------------------------------------
	uint ScriptClass::GetTypeID() const
	{
		return m_prRtti->GetTypeID();
	}
	//-------------------------------------------------------------------------------------------------------
	const std::string& ScriptClass::GetTypeName() const
	{
		return m_prRtti->GetTypeName();
	}
	//-------------------------------------------------------------------------------------------------------
	uint ScriptClass::GetTypeSize() const
	{
		return m_prRtti->GetTypeSize();
	}
	//-------------------------------------------------------------------------------------------------------
	const std::string& ScriptClass::GetBaseTypeName() const
	{
		return m_prBase->GetTypeName();
	}
	//-------------------------------------------------------------------------------------------------------
	void ScriptClass::SetConstant( int v, const string& name )
	{
		m_ConstantArray.push_back( make_pair( name, NEW Int(v) ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void ScriptClass::SetConstant( float v, const string& name )
	{
		m_ConstantArray.push_back( make_pair( name, NEW Float(v) ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void ScriptClass::SetConstant( double v, const string& name )
	{
		m_ConstantArray.push_back( make_pair( name, NEW Double(v) ) );
	}

}