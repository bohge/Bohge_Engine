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
#include "Predefine.h"
#include "VariableType.h"



namespace BohgeEngine
{
	class IJob
	{
	public:
		enum JobType
		{
			JT_ASYNCHRONOUS = 0,
			JT_SYNCHRONOUS,
		};
	private:
		JobType		m_eAsynJob;
		uint		m_nPriority;//”≈œ»º∂
	public:
		IJob():m_nPriority(0),m_eAsynJob(JT_ASYNCHRONOUS){}
		virtual ~IJob(void){}
	public:
		virtual void DoJob() = 0;
	public:
		BOHGE_FORCEINLINE void SetJobType( JobType jt )
		{
			m_eAsynJob = jt;
		}
		BOHGE_FORCEINLINE JobType GetJobType()
		{
			return m_eAsynJob;
		}
		BOHGE_FORCEINLINE void SetPriority( unsigned int p )
		{
			m_nPriority = p;
		}
		BOHGE_FORCEINLINE unsigned int GetPriority() const
		{
			return m_nPriority;
		}
	};
}
