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
#include "VariableType.h"
#include "Predefine.h"
#include "RttiLibrary.h"
#include "Utility.h"


#include <string>



namespace BohgeEngine
{
	class Rtti
	{
	private:
		std::string			m_TypeName;
		uint				m_TypeID;
		uint				m_TypeSize;
	public:
		Rtti( uint size, const char* name )
			:m_TypeID(Utility::HashCode(name)),
			m_TypeSize(size),
			m_TypeName(name)
		{
			RttiLibrary::Instance().Insert( this );
		}
	public:
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetTypeID() const//得到类信息ID
		{
			return m_TypeID;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const std::string& GetTypeName() const//返回类名
		{
			return m_TypeName;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetTypeSize() const//返回类大小
		{
			return m_TypeSize;
		}
	};


#define RTTI_TYPE_DECLEAR( type )\
	private:\
		static Rtti			s_Rtti;\
	public:\
		static const Rtti& RTTI(){return s_Rtti;}\
	public:\
		virtual uint GetTypeID() const\
		{\
			return s_Rtti.GetTypeID();\
		}\
		virtual bool isObject( const Rtti& rtti ) const\
		{\
			return s_Rtti.GetTypeID() == rtti.GetTypeID();\
		}\
		virtual const std::string& GetTypeName() const\
		{\
			return s_Rtti.GetTypeName();\
		}\
		virtual uint GetTypeSize() const\
		{\
			return s_Rtti.GetTypeSize();\
		}\
		virtual void CopyTo( RttiObject* dst )  const\
		{\
			new( dst ) type( *this );\
		}

#define RTTI_ABSTRACT_TYPE_DECLEAR( type )\
private:\
	static Rtti			s_Rtti;\
public:\
	static const Rtti& RTTI(){return s_Rtti;}\
public:\
	virtual uint GetTypeID() const\
	{\
	return s_Rtti.GetTypeID();\
	}\
	virtual bool isObject( const Rtti& rtti ) const\
	{\
	return s_Rtti.GetTypeID() == rtti.GetTypeID();\
	}\
	virtual const std::string& GetTypeName() const\
	{\
	return s_Rtti.GetTypeName();\
	}\
	virtual uint GetTypeSize() const\
	{\
	return s_Rtti.GetTypeSize();\
	}\
	virtual void CopyTo( RttiObject* dst )  const\
	{\
		ASSERT(false);\
	}

#define RTTI_TYPE_IMPLEMENT( type ) Rtti type::s_Rtti( sizeof(type), #type );
}
