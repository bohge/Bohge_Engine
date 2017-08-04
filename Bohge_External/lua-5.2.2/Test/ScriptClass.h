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
#include "ScriptUtility.h"
#include "Predefine.h"
#include "Rtti.hpp"

#include <string>

namespace BohgeEngine
{
	class Rtti;
	class RttiObject;
	class ScriptClass
	{
	private:
		const Rtti*			m_prRtti;
		const Rtti*			m_prBase;
		FuncitonArray		m_FuncArray;
		ClassConstantArray	m_ConstantArray;
	public:
		ScriptClass(void);
		~ScriptClass(void);
	private:
		void _DoCopy( const ScriptClass& sc );
	public:
		void MemberFunction( const ScriptFunction& func );//注册成员函数，和c++一样返回值不能做为重载的区分符号
		void ClassType( const Rtti* rtti );//注册一个类
		void ClassType( const Rtti* rtti, const Rtti* base );//注册一个类,继承
		const std::string& GetTypeName() const;//返回类名
		const std::string& GetBaseTypeName() const;//返回类名
		uint GetTypeID() const;//得到类信息ID
		uint GetTypeSize() const;//返回类大小
		void SetConstant( int v, const std::string& name );
		void SetConstant( float v, const std::string& name );
		void SetConstant( double v, const std::string& name );
	public:
		//-------------------------------------------------------------------------------------------------------
		bool isDriverType() const
		{
			return NULL != m_prBase;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool isOverrideFunction( uint index )
		{
			return m_FuncArray[index].second.size() > 1;
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
		BOHGE_FORCEINLINE const std::string& GetFunctionListName( uint index )
		{
			return m_FuncArray[index].first;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE ScriptFunction* GetFunction( int index )
		{
			return m_FuncArray[index].second.begin()->second;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE ScriptFunction* GetFunction( int index, uint id )
		{
			return m_FuncArray[index].second.find(id)->second;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE CLassConstant GetClassConstant( int index )
		{
			return m_ConstantArray[index];
		}
	};
}
