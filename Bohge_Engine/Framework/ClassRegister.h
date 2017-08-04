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

#include <EASTL/string.h>
#include <EASTL/map.h>

namespace BohgeEngine
{
	class Rtti;
	class Object;
	class BOHGE_FRAMEWORK ClassRegister : public Object
	{
		RTTI_TYPE_DECLEAR( ClassRegister )
	private:
		const Rtti*			m_prRtti;
		const Rtti*			m_prBase;
		ClassFuncitonArray	m_FuncArray;
		ClassConstantArray	m_ConstantArray;
	public:
		ClassRegister(void);
		~ClassRegister(void);
	public:
		ClassRegister( const ClassRegister& sc );
		const ClassRegister& operator = ( const ClassRegister& sc );
	private:
		void _DoCopy( const ClassRegister& sc );
	public:
		void ClassFunction( const FunctionRegister& func );//注册成员函数，和c++一样返回值不能做为重载的区分符号
		void ClassType( const Rtti* rtti );//注册一个类
		void ClassType( const Rtti* rtti, const Rtti* base );//注册一个类,继承
		void ClassConstant( int v, const eastl::string& name );
		void ClassConstant( float v, const eastl::string& name );
		void ClassConstant( double v, const eastl::string& name );
		const char* GetClassTypeName() const;//返回类名
		const char* GetBaseClassTypeName() const;//返回类名
		uint GetClassTypeID() const;//得到类信息ID
		uint GetClassTypeSize() const;//返回类大小
	public:
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE FunctionRegister* GetFunction( int index, const Rtti* o1 )
		{
			OverlappedMap::iterator funcArray = m_FuncArray[index].second.find( 1 );//一个参数
			if ( 1 == funcArray->second.size() )
			{
				return *(funcArray->second.begin());
			}
			else
			{
				FOR_EACH( FunctionArray, func, funcArray->second )
				{
					if ( (*func)->isMatchArguments( o1 ) )
					{
						return *func;
					}
				}
			}
			ASSERT( false && "no match arguments!" );
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE FunctionRegister* GetFunction( int index, const Rtti* o1, const Rtti* o2 )
		{
			OverlappedMap::iterator funcArray = m_FuncArray[index].second.find( 2 );//一个参数
			if ( 1 == funcArray->second.size() )
			{
				return *(funcArray->second.begin());
			}
			else
			{
				FOR_EACH( FunctionArray, func, funcArray->second )
				{
					if ( (*func)->isMatchArguments( o1, o2 ) )
					{
						return *func;
					}
				}
			}
			ASSERT( false && "no match arguments!" );
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE FunctionRegister* GetFunction( int index, const Rtti* o1, const Rtti* o2, const Rtti* o3 )
		{
			OverlappedMap::iterator funcArray = m_FuncArray[index].second.find( 3 );//一个参数
			if ( 1 == funcArray->second.size() )
			{
				return *(funcArray->second.begin());
			}
			else
			{
				FOR_EACH( FunctionArray, func, funcArray->second )
				{
					if ( (*func)->isMatchArguments( o1, o2, o3 ) )
					{
						return *func;
					}
				}
			}
			ASSERT( false && "no match arguments!" );
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE FunctionRegister* GetFunction( int index, const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4 )
		{
			OverlappedMap::iterator funcArray = m_FuncArray[index].second.find( 4 );//一个参数
			if ( 1 == funcArray->second.size() )
			{
				return *(funcArray->second.begin());
			}
			else
			{
				FOR_EACH( FunctionArray, func, funcArray->second )
				{
					if ( (*func)->isMatchArguments( o1, o2, o3, o4 ) )
					{
						return *func;
					}
				}
			}
			ASSERT( false && "no match arguments!" );
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE FunctionRegister* GetFunction( int index, const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4, const Rtti* o5 )
		{
			OverlappedMap::iterator funcArray = m_FuncArray[index].second.find( 5 );//一个参数
			if ( 1 == funcArray->second.size() )
			{
				return *(funcArray->second.begin());
			}
			else
			{
				FOR_EACH( FunctionArray, func, funcArray->second )
				{
					if ( (*func)->isMatchArguments( o1, o2, o3, o4, o5 ) )
					{
						return *func;
					}
				}
			}
			ASSERT( false && "no match arguments!" );
			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE FunctionRegister* GetFunction( int index, const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4, const Rtti* o5, const Rtti* o6 )
		{
			OverlappedMap::iterator funcArray = m_FuncArray[index].second.find( 6 );//一个参数
			if ( 1 == funcArray->second.size() )
			{
				return *(funcArray->second.begin());
			}
			else
			{
				FOR_EACH( FunctionArray, func, funcArray->second )
				{
					if ( (*func)->isMatchArguments( o1, o2, o3, o4, o5, o6 ) )
					{
						return *func;
					}
				}
			}
			ASSERT( false && "no match arguments!" );
			return NULL;
		}
	public:
		//-------------------------------------------------------------------------------------------------------
		bool isDriverType() const
		{
			return NULL != m_prBase;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool isOverrideFunction( uint index )
		{
			int count = 0;
			FOR_EACH( OverlappedMap, funcs, m_FuncArray[index].second )
			{
				FOR_EACH( FunctionArray, func, funcs->second )
				{
					++ count;
				}
			}
			return count > 1;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetFunctionListCount()
		{
			return m_FuncArray.size();
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetClassConstantCount()
		{
			return m_ConstantArray.size();
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const eastl::string& GetFunctionListName( uint index )
		{
			return m_FuncArray[index].first;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE ScriptProperty::FunctionType GetFunctionListType( uint index )
		{
			return GetFunction(index)->GetFunctionType();
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE FunctionRegister* GetFunction( int index )
		{
			return *(m_FuncArray[index].second.begin()->second.begin());
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const OverlappedMap& GetFunctions( int index ) const
		{
			return m_FuncArray[index].second;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE CLassConstant GetClassConstant( int index )
		{
			return m_ConstantArray[index];
		}
	};
}
