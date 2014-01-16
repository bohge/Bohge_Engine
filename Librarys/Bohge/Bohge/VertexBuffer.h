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
//			顶点缓存数据		   //
/////////////////////////////////////

#pragma once
#include "Device.h"
#include "Vertex.h"



namespace BohgeEngine
{
	class VertexBuffer
	{
	private:
		Device::MemoryUseage	m_eVertexUseage;
		VertexLayoutList		m_LayoutList;
		handle					m_VertexHanle;
		uint					m_uVertexCount;//顶点的数目
		uint					m_uVertexSize;//vertex顶点的size
	public:
		explicit VertexBuffer( Device::MemoryUseage useage )
			:m_eVertexUseage( useage ),
			m_VertexHanle( -1 ),
			m_uVertexCount( 0 )
		{

		}
		~VertexBuffer(void);
	private:
		void _FlushVertexBuffer(uint offset, uint vertexCount, const void* buffer);
	public:
		template< typename T >
		BOHGE_FORCEINLINE void FlushVertexBuffer(uint offset, uint vertexCount, const T* buffer)
		{
			if ( -1 == m_VertexHanle )//初始化
			{
				m_LayoutList = T::Layout();
				m_uVertexSize = sizeof( T );
			}
#ifdef _DEBUG//暂时不支持更换顶点类型，其实可以支持，需要检测赋值，改天弄个int的index比对，效率高
			ASSERT( T::Layout() == m_LayoutList );
#endif
			_FlushVertexBuffer( offset, vertexCount, buffer );
		}
		template< typename T >
		BOHGE_FORCEINLINE void FlushVertexBuffer(uint vertexCount, const T* buffer)
		{
			FlushVertexBuffer( 0, vertexCount, buffer );
		}
		BOHGE_FORCEINLINE VertexLayoutList* GetVertexLayoutList()
		{
			return &m_LayoutList;
		}
		BOHGE_FORCEINLINE const handle& VertexHanle() const
		{
			return m_VertexHanle;
		}
		BOHGE_FORCEINLINE uint VertexSize() const
		{
			return m_uVertexSize;
		}
		BOHGE_FORCEINLINE uint VertexCount() const
		{
			return m_uVertexCount;
		}
	};
}