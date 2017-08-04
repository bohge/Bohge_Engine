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
#include "Plugindefine.h"
#include "VariableType.h"
#include "Utility.hpp"
#include "Predefine.h"



#include <EASTL/string.h>



namespace BohgeEngine
{
	class BOHGE_FRAMEWORK Rtti
	{
	private:
		typedef bool (Rtti::*isTypeOrDriverTypeFunc)( const Rtti& ) const;
	private:
		isTypeOrDriverTypeFunc	m_pisTypeOrDriverTypeFunc;
		const Rtti*				m_rpBase;
		const char*				m_TypeName;
		uint					m_TypeID;
		uint					m_TypeSize;
	public:
		Rtti( uint size, const Rtti* base, const char* name );
		Rtti( const Rtti& rhs );
		virtual ~Rtti(void);
	private:
		bool _NonDrivedClass( const Rtti& rhs ) const;
		bool _DrivedClass( const Rtti& rhs ) const;
	public:
		uint GetTypeID() const;//得到类信息ID
		const char* GetTypeName() const;//返回类名
		uint GetTypeSize() const;//返回类大小
		bool isTypeOrDriverType( const BohgeEngine::Rtti& rtti ) const;
	};


#define RTTI_TYPE_DECLEAR( type )\
	private:\
		static BohgeEngine::Rtti			s_Rtti;\
	public:\
		static BohgeEngine::Rtti _MakeRtti(){return BohgeEngine::Rtti( sizeof(type), NULL, #type );}\
		static const BohgeEngine::Rtti& RTTI(){return type::s_Rtti;}\
	public:\
		virtual uint GetTypeID() const\
		{\
			return type::s_Rtti.GetTypeID();\
		}\
		virtual bool isType( const BohgeEngine::Rtti& rtti ) const\
		{\
			return type::s_Rtti.GetTypeID() == rtti.GetTypeID();\
		}\
		virtual const char* GetTypeName() const\
		{\
			return type::s_Rtti.GetTypeName();\
		}\
		virtual uint GetTypeSize() const\
		{\
			return type::s_Rtti.GetTypeSize();\
		}\
		virtual BohgeEngine::Object* Duplicate() const\
		{\
			type* res = NEW type(*this);\
			return res;\
		}\
		virtual bool isTypeOrDriverType( const BohgeEngine::Rtti& rtti ) const\
		{\
			return type::s_Rtti.isTypeOrDriverType( rtti );\
		}\
		virtual const BohgeEngine::Rtti& GetRtti() const\
		{\
			return s_Rtti;\
		}


#define RTTI_ABSTRACT_TYPE_DECLEAR( type )\
	private:\
		static BohgeEngine::Rtti			s_Rtti;\
	public:\
		static BohgeEngine::Rtti _MakeRtti(){return BohgeEngine::Rtti( sizeof(type), NULL, #type );}\
		static const BohgeEngine::Rtti& RTTI(){return type::s_Rtti;}\
	public:\
		virtual uint GetTypeID() const\
		{\
			return type::s_Rtti.GetTypeID();\
		}\
		virtual bool isType( const BohgeEngine::Rtti& rtti ) const\
		{\
			return type::s_Rtti.GetTypeID() == rtti.GetTypeID();\
		}\
		virtual const const char* GetTypeName() const\
		{\
			return type::s_Rtti.GetTypeName();\
		}\
		virtual uint GetTypeSize() const\
		{\
			return type::s_Rtti.GetTypeSize();\
		}\
		virtual BohgeEngine::Object* Duplicate() const\
		{\
			ASSERT(false&&"Abstract class can not be duplicate");\
			return NULL;\
		}\
		virtual bool isTypeOrDriverType( const BohgeEngine::Rtti& rtti ) const\
		{\
			return type::s_Rtti.isTypeOrDriverType( rtti );\
		}\
		virtual const BohgeEngine::Rtti& GetRtti() const\
		{\
			return s_Rtti;\
		}



#define RTTI_DRIVER_TYPE_DECLEAR( type, base )\
	private:\
		static BohgeEngine::Rtti			s_Rtti;\
	public:\
		static BohgeEngine::Rtti _MakeRtti(){return BohgeEngine::Rtti( sizeof(type), &base::RTTI(), #type );}\
		static const BohgeEngine::Rtti& RTTI(){return type::s_Rtti;}\
	public:\
		virtual uint GetTypeID() const\
		{\
			return type::s_Rtti.GetTypeID();\
		}\
		virtual bool isType( const BohgeEngine::Rtti& rtti ) const\
		{\
			return type::s_Rtti.GetTypeID() == rtti.GetTypeID();\
		}\
		virtual const char* GetTypeName() const\
		{\
			return type::s_Rtti.GetTypeName();\
		}\
		virtual uint GetTypeSize() const\
		{\
			return type::s_Rtti.GetTypeSize();\
		}\
		virtual BohgeEngine::Object* Duplicate() const\
		{\
			type* res = NEW type(*this);\
			return res;\
		}\
		virtual bool isTypeOrDriverType( const BohgeEngine::Rtti& rtti ) const\
		{\
			return type::s_Rtti.isTypeOrDriverType( rtti );\
		}\
		virtual const BohgeEngine::Rtti& GetRtti() const\
		{\
			return s_Rtti;\
		}


#define RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( type, base )\
	private:\
		static BohgeEngine::Rtti			s_Rtti;\
	public:\
		static BohgeEngine::Rtti _MakeRtti(){return BohgeEngine::Rtti( sizeof(type), &base::RTTI(), #type );}\
		static const BohgeEngine::Rtti& RTTI(){return type::s_Rtti;}\
	public:\
		virtual uint GetTypeID() const\
		{\
			return type::s_Rtti.GetTypeID();\
		}\
		virtual bool isType( const BohgeEngine::Rtti& rtti ) const\
		{\
			return type::s_Rtti.GetTypeID() == rtti.GetTypeID();\
		}\
		virtual const const char* GetTypeName() const\
		{\
			return type::s_Rtti.GetTypeName();\
		}\
		virtual uint GetTypeSize() const\
		{\
			return type::s_Rtti.GetTypeSize();\
		}\
		virtual BohgeEngine::Object* Duplicate() const\
		{\
			ASSERT(false&&"Abstract class can not be duplicate");\
			return NULL;\
		}\
		virtual bool isTypeOrDriverType( const BohgeEngine::Rtti& rtti ) const\
		{\
			return type::s_Rtti.isTypeOrDriverType( rtti );\
		}\
		virtual const BohgeEngine::Rtti& GetRtti() const\
		{\
			return s_Rtti;\
		}

#define RTTI_TYPE_IMPLEMENT( type ) BohgeEngine::Rtti type::s_Rtti( type::_MakeRtti() );
}
