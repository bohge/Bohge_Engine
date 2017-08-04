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
#include "Rtti.hpp"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	Rtti::Rtti( uint size, const Rtti* base, const char* name )
		:m_TypeID(Utility::HashCode(name)),
		m_rpBase(base),
		m_TypeSize(size),
		m_TypeName(name),
		m_pisTypeOrDriverTypeFunc( NULL == base ? &Rtti::_NonDrivedClass : &Rtti::_DrivedClass )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	Rtti::Rtti( const Rtti& rhs )
		:m_TypeID( rhs.m_TypeID ),
		m_rpBase( rhs.m_rpBase ),
		m_TypeSize( rhs.m_TypeSize ),
		m_TypeName( rhs.m_TypeName ),
		m_pisTypeOrDriverTypeFunc( rhs.m_pisTypeOrDriverTypeFunc )
	{

	}

	//-------------------------------------------------------------------------------------------------------
	Rtti::~Rtti( void )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	uint Rtti::GetTypeID() const//得到类信息ID
	{
		return m_TypeID;
	}
	//-------------------------------------------------------------------------------------------------------
	const char* Rtti::GetTypeName() const//返回类名
	{
		return m_TypeName;
	}
	//-------------------------------------------------------------------------------------------------------
	uint Rtti::GetTypeSize() const//返回类大小
	{
		return m_TypeSize;
	}
	//-------------------------------------------------------------------------------------------------------
	bool Rtti::isTypeOrDriverType( const BohgeEngine::Rtti& rtti ) const
	{
		return (this->*m_pisTypeOrDriverTypeFunc)( rtti );
	}
	//-------------------------------------------------------------------------------------------------------
	bool Rtti::_NonDrivedClass( const Rtti& rhs ) const
	{
		return GetTypeID() == rhs.GetTypeID();
	}
	//-------------------------------------------------------------------------------------------------------
	bool Rtti::_DrivedClass( const Rtti& rhs ) const
	{
		return _NonDrivedClass( rhs ) || m_rpBase->isTypeOrDriverType( rhs );
	}



}