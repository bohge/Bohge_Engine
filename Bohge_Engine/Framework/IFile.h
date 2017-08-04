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
#include "Object.hpp"


#include <EASTL/string.h>

namespace BohgeEngine
{
	//基本类型
	class BOHGE_FRAMEWORK IFile : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR( IFile );
	public:
		enum ActionType
		{
			AT_NONE = 0, //没有初始化的
			AT_READ,
			AT_WRITE,
		};
	public:
		IFile(void){};
		virtual ~IFile(void){};
	public:
		virtual bool OpenFile( ActionType at ) = 0;
		virtual bool isOpen() = 0;
		virtual bool CloseFile() = 0;
		virtual int Seek( int to, int whence ) = 0;
		virtual int Tell() = 0;
		virtual int ReadFile( void* data, int bitesize ) = 0;
		virtual int WriteFile( const void* data, int bitesize ) = 0;
		virtual const eastl::string& GetFilePath() = 0;
		virtual uint GetSize() = 0;
	public:
		//-------------------------------------------------------------------------------------------------------
		template<typename T> BOHGE_FORCEINLINE int ReadFile( T* ptr )
		{
			ReadFile( ptr, sizeof(T) );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> BOHGE_FORCEINLINE int WriteFile( T* ptr )
		{
			WriteFile( ptr, sizeof(T) );
		}
	};
}