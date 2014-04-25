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
#include "VertexStream.h"




namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	VertexStream::VertexStream(void)
		:m_nVertexCount(0),
		m_nChunkStride(0)
	{
		m_VertexPusherVector.resize( Attributes::ATTRIBUTE_COUNT );
	}
	//-------------------------------------------------------------------------------------------------------
	VertexStream::~VertexStream(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void VertexStream::CreateBuffer( uint size )
	{
		ASSERT(0 == m_nVertexCount);
		m_nVertexCount = size;
		m_Buffer.resize( size * m_nChunkStride );
	}
	//-------------------------------------------------------------------------------------------------------
	void VertexStream::_PushVertexType( Attributes::ShaderAttributes sa, BufferObjectStatus::DataType dt, int count )
	{
		ASSERT(0 == m_nVertexCount);
		m_Layout.SetVertexLayout( sa, dt, count, false );
		switch(dt)
		{
		case BufferObjectStatus::DT_FLOAT: m_VertexPusherVector[sa].CompressPtr = &VertexCompress::Float_Float; break;
		case BufferObjectStatus::DT_HALF_FLOAT: m_VertexPusherVector[sa].CompressPtr = &VertexCompress::Float_HalfFloat; break;
		case BufferObjectStatus::DT_INT_8_8_8_8: m_VertexPusherVector[sa].CompressPtr = &VertexCompress::Float_INT8888; break;
		case BufferObjectStatus::DT_UINT_8_8_8_8: m_VertexPusherVector[sa].CompressPtr = &VertexCompress::Float_UINT8888; break;
		case BufferObjectStatus::DT_INT_10_10_10_2: m_VertexPusherVector[sa].CompressPtr = &VertexCompress::Float_INT101010; break;
		case BufferObjectStatus::DT_UINT_10_10_10_2: m_VertexPusherVector[sa].CompressPtr = &VertexCompress::Float_UINT101010; break;
		default: ASSERT(false && "Unkown vertex data type !\n");
		}
		m_VertexPusherVector[sa].PointerStride = m_nChunkStride;//数据其实指针位置
		m_VertexPusherVector[sa].Count = count;
		m_nChunkStride += m_Layout.GetVertexLayout(sa).Stride();//增加尺寸
	}

}
