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
#include "IThreadEntity.h"
#include "SmartPtr.hpp"


namespace BohgeEngine
{
	class IJob;
	class BOHGE_FRAMEWORK IJobDispatcher : public IThreadEntity
	{//该类型必须设计为线程可复用
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IJobDispatcher, IThreadEntity )
	public:
		enum SortType
		{
			ST_UNSORT = 0,
			ST_LESS,
			ST_GREATER,
		};
	private:
		SortType			m_SortType;
		byte				m_ThreadCount;
	public:
		IJobDispatcher(void):IThreadEntity( ThreadProperty::ET_JOB_DISPATCHER ),m_ThreadCount(1),m_SortType(ST_UNSORT){}
		virtual ~IJobDispatcher(void){}
	public:
		virtual void Pasue() = 0;
		virtual void Continue() = 0;
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void PushJob( SmartPtr<IJob> job ) = 0;
	public:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetThreadCount( byte c )
		{
			m_ThreadCount = c;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE byte GetThreadCount()
		{
			return m_ThreadCount;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetSortType( SortType st )
		{
			m_SortType = st;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE SortType GetSortType()
		{
			return m_SortType;
		}
	};
}