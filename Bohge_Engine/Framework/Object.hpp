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
#include "Plugindefine.h"
#include "Predefine.h"
#include "Rtti.hpp"

#include <EASTL/string.h>

namespace BohgeEngine
{
	class BOHGE_FRAMEWORK Object
	{
		static Rtti	s_Rtti;
	public:
		Object(){}
		virtual ~Object(){}
	public:
		static const BohgeEngine::Rtti& RTTI(){return Object::s_Rtti;}
	public:
		virtual uint GetTypeID() const = 0;//得到类信息ID
		virtual bool isType( const Rtti& rtti ) const = 0;//判断是否是某个类
		virtual const char* GetTypeName() const = 0;//返回类名
		virtual uint GetTypeSize() const = 0;//返回类大小
		virtual Object* Duplicate() const = 0;//返回一份副本
		virtual bool isTypeOrDriverType( const Rtti& rtti ) const = 0;//是否是相同的类型或者继承类
		virtual const Rtti& GetRtti() const = 0;
	public:
		intptr_t GetObjectID() const;
#ifndef _TRACK_MEMORY
	public:
		void* operator new( uint size );
		void operator delete( void* ptr );
		void* operator new[]( uint size );
		void operator delete[]( void* ptr );
#endif
	};
}