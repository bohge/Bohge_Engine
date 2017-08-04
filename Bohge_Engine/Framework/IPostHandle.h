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
#include "IBaseHandle.h"



namespace BohgeEngine
{
	class BOHGE_FRAMEWORK IPostHandle : public IBaseHandle
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IPostHandle, IBaseHandle )
	private:
		eastl::string	m_PostContent;
		eastl::string	m_Result;
	public:
		IPostHandle( IJobDispatcher* dispatcher ):IBaseHandle(IWebProperty::JT_POST, dispatcher){}
		virtual ~IPostHandle(void){}
	private:
		virtual void _OnStart() = 0;
		virtual void _OnPause() = 0;
	public:
		virtual void SaveState() = 0;
		virtual void RestroeState( const eastl::string& path ) = 0;
		virtual eastl::string GetJobStateSavePath() = 0;
	protected:
		BOHGE_FORCEINLINE void _SetResult( const eastl::string& res )
		{
			m_Result = res;
		}
		BOHGE_FORCEINLINE const eastl::string& _GetPostContent() const
		{
			return m_PostContent;
		}
	public:
		BOHGE_FORCEINLINE void SetPostContent( const eastl::string& name, const eastl::string& value )
		{
			if( !m_PostContent.empty() )
			{
				m_PostContent += "&";
			}
			m_PostContent+= name + "=" + value;
		}
		BOHGE_FORCEINLINE const eastl::string& GetResult() const
		{
			return m_Result;
		}
	};
}
