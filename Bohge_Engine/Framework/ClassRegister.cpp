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
#include "ClassRegister.h"
#include "3DMath.h"


using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( ClassRegister );
	//-------------------------------------------------------------------------------------------------------
	ClassRegister::ClassRegister(void)
		:m_prRtti(NULL),
		m_prBase(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ClassRegister::ClassRegister( const ClassRegister& sc )
	{
		_DoCopy( sc );
	}
	//-------------------------------------------------------------------------------------------------------
	ClassRegister::~ClassRegister(void)
	{
		FOR_EACH( ClassConstantArray, constant, m_ConstantArray)
		{
			SAFE_DELETE( constant->second );
		}
		FOR_EACH( ClassFuncitonArray, funcMap, m_FuncArray)
		{
			FOR_EACH( OverlappedMap, funcs, funcMap->second )
			{
				FOR_EACH( FunctionArray, func, funcs->second )
				{
					SAFE_DELETE( *func );
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	const ClassRegister& ClassRegister::operator=( const ClassRegister& sc )
	{
		_DoCopy( sc );
		return *this;
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::_DoCopy( const ClassRegister& sc )
	{
		m_prRtti = sc.m_prRtti;
		m_prBase = sc.m_prBase;
		FOR_EACH_CONST( ClassFuncitonArray, funcMap, sc.m_FuncArray )
		{
			FOR_EACH_CONST( OverlappedMap, funcs, funcMap->second )
			{
				FOR_EACH_CONST( FunctionArray, func, funcs->second )
				{
					ClassFunction( **func );
				}				
			}
		}
		FOR_EACH_CONST( ClassConstantArray, cc, sc.m_ConstantArray )
		{
			m_ConstantArray.push_back( make_pair( cc->first, cc->second->Duplicate() ) );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassFunction( const FunctionRegister& func )
	{
		FunctionRegister* pFunc = NEW FunctionRegister(func);
		ClassFuncitonArray::iterator funcOvMp = m_FuncArray.end();
		FOR_EACH( ClassFuncitonArray, it, m_FuncArray )
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
			FunctionArray funcArray;
			funcArray.push_back( pFunc );
			om.insert( make_pair( pFunc->GetArgumentCount(), funcArray ) );
			m_FuncArray.push_back( make_pair( pFunc->GetName(), om ) );
		}
		else//找到函数序列了
		{
			ASSERT( 0 != pFunc->GetArgumentCount() );//重载函数必须有参数
			ASSERT( pFunc->GetFunctionType() == (*funcOvMp->second.begin()->second.begin())->GetFunctionType() );
			OverlappedMap::iterator funcArray = funcOvMp->second.find( pFunc->GetArgumentCount() );//找到对应参数数量的数组
			if ( funcOvMp->second.end() == funcArray )//如果没有对应参数的数组
			{
				FunctionArray funcArray;
				funcArray.push_back( pFunc );
				funcOvMp->second.insert( make_pair( pFunc->GetArgumentCount(), funcArray ) );
			}
			else
			{
				funcArray->second.push_back( pFunc );
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassType( const Rtti* rtti )
	{
		m_prRtti = rtti;
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassType( const Rtti* rtti, const Rtti* base )
	{
		ClassType(rtti);
		m_prBase = base;
	}
	//-------------------------------------------------------------------------------------------------------
	uint ClassRegister::GetClassTypeID() const
	{
		return m_prRtti->GetTypeID();
	}
	//-------------------------------------------------------------------------------------------------------
	const char* ClassRegister::GetClassTypeName() const
	{
		return m_prRtti->GetTypeName();
	}
	//-------------------------------------------------------------------------------------------------------
	uint ClassRegister::GetClassTypeSize() const
	{
		return m_prRtti->GetTypeSize();
	}
	//-------------------------------------------------------------------------------------------------------
	const char* ClassRegister::GetBaseClassTypeName() const
	{
		return m_prBase->GetTypeName();
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassConstant( int v, const string& name )
	{
		m_ConstantArray.push_back( make_pair( name, NEW Number(v) ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassConstant( float v, const string& name )
	{
		m_ConstantArray.push_back( make_pair( name, NEW Number(v) ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassConstant( double v, const string& name )
	{
		m_ConstantArray.push_back( make_pair( name, NEW Number(v) ) );
	}
}