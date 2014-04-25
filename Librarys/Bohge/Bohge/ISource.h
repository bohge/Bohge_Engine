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
#include "IJob.h"
#include "Event.hpp"

#include <vector>
#include <string>

namespace BohgeEngine
{
	class IResource;
	class ResourceManager;
	class ISource : public IJob //资源的原始数据
	{
	public:
		enum SourceType
		{
			ST_TEXTURE = 0,
		};
	private:
		std::vector< std::string >						m_ResourcePaths;
		ResourceManager*								m_refResourceManager;
		SourceType										m_eResType;
		uint											m_nHashCode;
		Event<bool,IResource*>::FunctionType*			m_FuncPtr;
	public:
		ISource( SourceType st );
		virtual ~ISource();
	private:
		virtual uint _GetResourceHashCode() const = 0;//返回资源的哈希值
		virtual void _DoLoadResource() = 0;//加载资源
	public:
		virtual IResource* DoMakeResource() = 0;//将资源数据制作为实体资源，制作资源需要在主线程进行
	public:
		virtual void DoJob( SmartPtr<IJob>& self );
	public:
		BOHGE_FORCEINLINE void SetListener( Event<bool,IResource*>::FunctionType* const ptr )
		{
			ASSERT( NULL == m_FuncPtr );
			m_FuncPtr = ptr;
		}
		BOHGE_FORCEINLINE Event<bool,IResource*>::FunctionType* GetListener() const
		{
			return m_FuncPtr;
		}
		BOHGE_FORCEINLINE void PushResourcePath( const std::string& path )
		{
			m_ResourcePaths.push_back( path );
		}
		BOHGE_FORCEINLINE const std::vector< std::string >& GetResourcePaths() const
		{
			return m_ResourcePaths;
		}
		BOHGE_FORCEINLINE uint GetHashCode()
		{
			if ( 0 == m_nHashCode )
			{
				m_nHashCode = _GetResourceHashCode();
			}
			return m_nHashCode;
		}
	};
}




