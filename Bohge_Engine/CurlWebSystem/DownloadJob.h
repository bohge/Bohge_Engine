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
#include "Framework/IWebProperty.h"
#include "Framework/SmartPtr.hpp"
#include "ICurlJob.h"

#include "EASTL/vector.h"

namespace CurlWebSystemPlugin
{
	class DownloadJob : public ICurlJob
	{
	private:
		static size_t onWriteData(char *ptr, size_t size, size_t nmemb, void *userdata);
	private:
		class Buffer
		{
		private:
			eastl::vector<char>		m_Buffes;
			uint					m_nOffset;
		public:
			Buffer(void):m_nOffset(0){}
			~Buffer(){}
		public:
			BOHGE_FORCEINLINE void Reset( size_t size )
			{
				m_nOffset = 0;
				if (m_Buffes.size() != size)
				{
					m_Buffes.resize( size );
				}
			}
			BOHGE_FORCEINLINE void CopyBuffer( const char* source, uint size)
			{
				int rs = m_nOffset + size > BohgeEngine::IWebProperty::DPC_BUFFE_SIZE
					? BohgeEngine::IWebProperty::DPC_BUFFE_SIZE - m_nOffset
					: size;
				memcpy( m_Buffes.data() + m_nOffset, source, rs );
				m_nOffset += rs;
			}
			BOHGE_FORCEINLINE const char* GetBuffer() const
			{
				return m_Buffes.data();
			}
			BOHGE_FORCEINLINE uint RealSize() const
			{
				return m_nOffset;
			}
		};
	private:
		uint64			m_BeginPos;
		uint64			m_EndPos;
		Buffer			m_Buffer;
	public:
		DownloadJob(void);
		~DownloadJob(void);
	protected:
		virtual void _DoWebJob( BohgeEngine::SmartPtr<BohgeEngine::IJob>& self );
	public:
		virtual void Connect();
	public:		
		void SetJobState( uint64 begin_pos, uint64 end_pos );		
	public:
		BOHGE_FORCEINLINE const char* GetBuffer() const
		{
			return m_Buffer.GetBuffer();
		}
		BOHGE_FORCEINLINE uint64 GetRangeBegin() const
		{
			return m_BeginPos;
		}
		BOHGE_FORCEINLINE uint64 GetRangeEnd() const
		{
			return m_EndPos;
		}
		BOHGE_FORCEINLINE uint GetBufferSize() const
		{
			return m_Buffer.RealSize();
		}
	};
}

