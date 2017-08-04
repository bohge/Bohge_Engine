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
#include "VertexStreamLayout.h"
#include "VertexCompress.h"
#include "3DMath.h"

#include <EASTL/vector.h>

namespace BohgeEngine
{
	class BOHGE_FRAMEWORK VertexStream : public Object
	{
		RTTI_TYPE_DECLEAR( VertexStream );
	private:
		enum Constant
		{
			VC_RESERVE_BUFFER_SIZE = 256,
		};
	private:
		struct VertexPusher//数据压榨结构体
		{
			VertexCompress::VertexCompressFunction		CompressPtr;//压缩函数指针
			uint										IndicesCount;//索引计数
			byte										PointerStride;//地址偏移的byte位
			byte										Count;
			byte										Bytesize;//每一个数据所含的byte个数
			VertexPusher():CompressPtr(NULL),PointerStride(0),Count(0),Bytesize(0),IndicesCount(0){}
			void ResetIndices()
			{
				IndicesCount = 0;
			}
		};
	private:
		typedef eastl::vector<VertexPusher*> VertexPusherVector;
		typedef eastl::vector<byte> Buffer;
	private:
		VertexStreamLayout		m_Layout;//数据构成
		VertexPusherVector		m_VertexPusherVector;
		vector2d				m_FlushInterval;//需要填充更新的区间
		uint					m_nReserveVertexCount;//预分配的顶点内存个数
		uint					m_nVertexCount;//顶点个数
		byte					m_nChunkStride;//每一组数据的大小
		Buffer					m_Buffer;
	public:
		VertexStream(void);
		VertexStream( const VertexStream& rhs );
		~VertexStream(void);
	private:
		void _SetVertexType( Attributes::ShaderAttributes sa, BufferObjectProperty::DataType source, BufferObjectProperty::DataType dest, int count );//添加buffer的属性 count是每一个数据float的数量
	public:
		void ReserveBuffer( uint size );//生成buffer
		void ReleaseBuffer();
	public:
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		void SetVertexType( Attributes::ShaderAttributes sa, BufferObjectProperty::DataType source, BufferObjectProperty::DataType dest )//生成数据压缩函数，数据类型等数据，设置输入
		{
			_SetVertexType( sa, source, dest, T::Size() );
		}
		//-------------------------------------------------------------------------------------------------------
		void PushVertexData( Attributes::ShaderAttributes sa, const Volume& data )
		{
			if ( m_nVertexCount >= m_nReserveVertexCount )
			{
				ReserveBuffer( m_nReserveVertexCount + VC_RESERVE_BUFFER_SIZE );
			}
			PushVertexDataFast( sa, data );
		}
		//-------------------------------------------------------------------------------------------------------
		void PushVertexDataFast( Attributes::ShaderAttributes sa, const Volume& data )
		{
			VertexPusher* pusher = m_VertexPusherVector[sa];
			ASSERT( data.Count() == pusher->Count );
			pusher->CompressPtr( data.Data(), &m_Buffer[ pusher->IndicesCount*m_nChunkStride+pusher->PointerStride], pusher->Count, pusher->Bytesize );
			++ pusher->IndicesCount;
			//m_nVertexCount = Math::GetMax( m_nVertexCount, pusher->IndicesCount );//找到最大的数值，这个数值是当前顶点数量
			m_nVertexCount = pusher->IndicesCount;//非安全做法，可能面临数据丢失和缺失，可以在push的时候检测数据安全性
			m_FlushInterval.m_y = m_nVertexCount;
		}
		//-------------------------------------------------------------------------------------------------------
		void ChangeVertexData( Attributes::ShaderAttributes sa, uint index, const Volume& data )
		{
			VertexPusher* pusher = m_VertexPusherVector[sa];
			ASSERT( data.Count() == pusher->Count );
			pusher->CompressPtr( data.Data(), &m_Buffer[ index*m_nChunkStride+pusher->PointerStride], pusher->Count, pusher->Bytesize );
		}
	public:
		BOHGE_FORCEINLINE const void* GetBufferData() const
		{
			return &m_Buffer[0];
		}
		BOHGE_FORCEINLINE const void* GetBufferData( uint vertexIndex ) const
		{
			return &m_Buffer[m_nChunkStride * vertexIndex ];
		}
		BOHGE_FORCEINLINE const void* GetReflushBufferData() const
		{
			return &m_Buffer[ m_nChunkStride * m_FlushInterval.m_x ];
		}
		BOHGE_FORCEINLINE uint GetVertexStride() const
		{
			return m_nChunkStride;
		}
		BOHGE_FORCEINLINE uint GetVertexSize() const
		{
			return m_nChunkStride;
		}
		BOHGE_FORCEINLINE uint GetVertexCount() const
		{
			return m_nVertexCount;
		}
		BOHGE_FORCEINLINE const VertexStreamLayout& GetLayout() const
		{
			return m_Layout;
		}
		BOHGE_FORCEINLINE const Buffer& GetBufferArray() const
		{
			return m_Buffer;
		}
		BOHGE_FORCEINLINE bool hasVertex( Attributes::ShaderAttributes sa )
		{
			return NULL != m_VertexPusherVector[sa];
		}
		BOHGE_FORCEINLINE void SetReflushInterval( int begin, int end )
		{
			m_FlushInterval.m_x = begin;
			m_FlushInterval.m_y = end;
		}
		BOHGE_FORCEINLINE const vector2d& GetReflushInterval() const
		{
			return m_FlushInterval;
		}
	};
}
