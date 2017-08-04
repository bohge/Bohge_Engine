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
#include "BufferObjectProperty.h"
#include "3DMath.h"



#include <EASTL/vector.h>

namespace BohgeEngine
{
	class VertexStream;
	class IndicesStream;
	class BOHGE_FRAMEWORK ReflushStream : public Object
	{
		RTTI_TYPE_DECLEAR( ReflushStream );
	private:
		typedef eastl::vector<byte> Buffer;
	private:
		Buffer					m_Buffer;
		vector2d				m_FlushInterval;//需要填充更新的区间
		uint					m_nDataCount;//数据个数
		byte					m_nEachDataSize;//每一组数据的大小
	public:
		ReflushStream(void);
		virtual ~ReflushStream(void);
	public:
		void FlushStream( const vector2d& interval, byte size, const void* data  );
	public:
		BOHGE_FORCEINLINE const void* GetBufferData() const
		{
			return &m_Buffer[0];
		}
		BOHGE_FORCEINLINE uint GetDataCount() const
		{
			return m_nDataCount;
		}
		BOHGE_FORCEINLINE uint GetDataSize() const
		{
			return m_nEachDataSize;
		}
		BOHGE_FORCEINLINE const vector2d& GetReflushInterval() const
		{
			return m_FlushInterval;
		}
	};
}
