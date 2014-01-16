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



/////////////////////////////////////
//			索引缓存数据		   //
/////////////////////////////////////
#pragma once
#include "Device.h"



namespace BohgeEngine
{
	class IndicesBuffer //索引VBO
	{
	private:
		Device::MemoryUseage		m_eIndiesUseage;
		handle						m_IndexHanle;
		uint						m_uIndiesCount;//索引的数目
		uint						m_DrawCount;
	public:
		explicit IndicesBuffer( Device::MemoryUseage useage );
		~IndicesBuffer();
		void FlushIndiesBuffer( uint offset, uint indiesCount, const ushort* buffer);//从原来的bufferoffset开始，填充indiesCount个数据，buffer为需要填充的数据
		BOHGE_FORCEINLINE void FlushIndiesBuffer(uint indiesCount, const ushort* buffer)
		{
			FlushIndiesBuffer( 0, indiesCount, buffer );
		}
		BOHGE_FORCEINLINE void SetDrawCount(uint count)//设置可绘制的个数，降低绘制开销
		{
			m_DrawCount = count;
		}
		BOHGE_FORCEINLINE uint DrawCount() const
		{
			return m_DrawCount;//有索引和没有索引绘制不一样
		}
		BOHGE_FORCEINLINE const handle& IndexHandle() const
		{
			return m_IndexHanle;
		}
	};
}