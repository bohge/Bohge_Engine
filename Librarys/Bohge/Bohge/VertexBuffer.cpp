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
#include "VertexBuffer.h"
#include "Engine.h"
#include "Device.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	VertexBuffer::~VertexBuffer( void )
	{
		if ( -1 != m_VertexHanle )
		{
			Engine::Instance().GetDevice()->DeleteBuffer( m_VertexHanle );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void VertexBuffer::_FlushVertexBuffer( uint offset, uint vertexCount, const void* buffer )
	{
		if ( -1 == m_VertexHanle )//初始化
		{
			ASSERT( 0 == offset );//这种重新分配的方法必须保证传入的指针必须是从头开始的，不然我们没有办法增量添加
			m_uVertexCount = vertexCount;
			Engine::Instance().GetDevice()->CreateBuffer( m_VertexHanle, Device::VERTEX_BUFFER, vertexCount * m_uVertexSize, buffer, m_eVertexUseage );
		}
		else if ( m_uVertexCount < offset + vertexCount )//原来的不够了
		{
			ASSERT( 0 == offset );//这种重新分配的方法必须保证传入的指针必须是从头开始的，不然我们没有办法增量添加
			m_uVertexCount = vertexCount;
			Engine::Instance().GetDevice()->CreateBuffer( m_VertexHanle, Device::VERTEX_BUFFER, vertexCount * m_uVertexSize, buffer, m_eVertexUseage );
		}
		else
		{
			ASSERT( Device::MU_DYNAMIC == m_eVertexUseage );//只有DYNAMIC可以调用更改
			Engine::Instance().GetDevice()->ChangeData( m_VertexHanle, Device::VERTEX_BUFFER, offset * m_uVertexSize, vertexCount * m_uVertexSize, buffer);
		}
	}
}