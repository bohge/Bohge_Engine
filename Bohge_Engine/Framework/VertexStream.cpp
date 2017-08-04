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
#include "Utility.hpp"



namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( VertexStream );
	//-------------------------------------------------------------------------------------------------------
	VertexStream::VertexStream(void)
		:m_nReserveVertexCount(0),
		m_nVertexCount(0),
		m_nChunkStride(0)
	{
		m_VertexPusherVector.resize( Attributes::ATTRIBUTE_COUNT, NULL );
	}
	//-------------------------------------------------------------------------------------------------------
	VertexStream::VertexStream( const VertexStream& rhs )
		:m_nReserveVertexCount(rhs.m_nReserveVertexCount),
		m_nVertexCount(rhs.m_nVertexCount),
		m_nChunkStride(rhs.m_nChunkStride),
		m_Layout( rhs.m_Layout ),
		m_FlushInterval( rhs.m_FlushInterval ),
		m_Buffer( rhs.m_Buffer )
	{
		m_VertexPusherVector.resize( Attributes::ATTRIBUTE_COUNT, NULL );
	}
	//-------------------------------------------------------------------------------------------------------
	VertexStream::~VertexStream(void)
	{
		FOR_EACH( VertexPusherVector, pusher, m_VertexPusherVector )
		{
			SAFE_DELETE( *pusher );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void VertexStream::ReserveBuffer( uint size )
	{
		m_nReserveVertexCount = size;
		m_Buffer.resize( size * m_nChunkStride );
	}
	//-------------------------------------------------------------------------------------------------------
	void VertexStream::_SetVertexType( Attributes::ShaderAttributes sa, BufferObjectProperty::DataType source, BufferObjectProperty::DataType dest, int count )
	{
		ASSERT( NULL == m_VertexPusherVector[sa] );
		ASSERT(0 == GetVertexCount());
		byte layoutCount = 0;
		m_VertexPusherVector[sa] = NEW VertexPusher;
		switch( source )
		{
		case BufferObjectProperty::DT_FLOAT:
			{
				switch(dest)
				{
				case BufferObjectProperty::DT_FLOAT: m_VertexPusherVector[sa]->CompressPtr = &VertexCompress::SimpleCopy; layoutCount = count; break;
				case BufferObjectProperty::DT_HALF_FLOAT: m_VertexPusherVector[sa]->CompressPtr = &VertexCompress::Float_HalfFloat; layoutCount = count; break;
				case BufferObjectProperty::DT_INT_8_8_8_8: m_VertexPusherVector[sa]->CompressPtr = &VertexCompress::Float_INT8888; layoutCount = 4; break;
				case BufferObjectProperty::DT_UINT_8_8_8_8: m_VertexPusherVector[sa]->CompressPtr = &VertexCompress::Float_UINT8888; layoutCount = 4; break;
				case BufferObjectProperty::DT_INT_2_10_10_10: m_VertexPusherVector[sa]->CompressPtr = &VertexCompress::Float_INT101010; layoutCount = 4; break;
				case BufferObjectProperty::DT_UINT_10_10_10_2: m_VertexPusherVector[sa]->CompressPtr = &VertexCompress::Float_UINT101010; layoutCount = 4; break;
				default: ASSERT(false && "Unkown vertex data type !\n");
				}
			}break;
		case BufferObjectProperty::DT_HALF_FLOAT:
			{
				ASSERT( source == dest );//其他格式必须要求输入输出是一致的，暂时这样要求，日后可以做数据转换
				layoutCount = count;
				m_VertexPusherVector[sa]->CompressPtr = &VertexCompress::SimpleCopy;
			}break;
		case BufferObjectProperty::DT_INT_8_8_8_8:
		case BufferObjectProperty::DT_UINT_8_8_8_8:
		case BufferObjectProperty::DT_INT_2_10_10_10:
		case BufferObjectProperty::DT_UINT_10_10_10_2:
			{
				ASSERT( source == dest );//其他格式必须要求输入输出是一致的，暂时这样要求，日后可以做数据转换
				layoutCount = 4;
				m_VertexPusherVector[sa]->CompressPtr = &VertexCompress::SimpleCopy;
			}break;
		default: ASSERT(false && "Unkown vertex data type !\n");
		}
		m_Layout.SetVertexLayout( sa, dest, layoutCount );//这个地方要对应最后的数据确定数组大小
		m_VertexPusherVector[sa]->PointerStride = m_nChunkStride;//数据其实指针位置
		m_VertexPusherVector[sa]->Count = count;
		m_VertexPusherVector[sa]->Bytesize = m_Layout.GetVertexLayout(sa).ByteSize();
		m_nChunkStride += m_Layout.GetVertexLayout(sa).Stride();//增加尺寸
	}
	//-------------------------------------------------------------------------------------------------------
	void VertexStream::ReleaseBuffer()
	{
		FOR_EACH( VertexPusherVector, pusher, m_VertexPusherVector )
		{
			if ( NULL != *pusher )
			{
				(*pusher)->ResetIndices();
			}
		}
		m_nReserveVertexCount = 0;
		m_nVertexCount = 0;
		m_FlushInterval = vector2d( 0, 0 );
		m_Buffer.clear();
	}

}
