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
#include "ISharedResource.h"
#include "BufferObjectProperty.h"
#include "Plugindefine.h"




namespace BohgeEngine
{
	//索引VBO
	class IndicesStream;
	class BOHGE_FRAMEWORK IndicesBufferResource : public ISharedResource
	{
	private:
		static handle							s_CurrentHandle;
	private:
		IndicesStream*							m_IndicesStream[2];
		BufferObjectProperty::IndicesType		m_eIndicesType;
		BufferObjectProperty::MemoryUseage		m_eMemoryUseage;
		uint									m_GPUResourceHandle;
		uint									m_uIndiesCount;//索引的数目
		uint									m_DrawCount;
		uint									m_FlushIndex;
		byte									m_TypeSize;
	public:
		IndicesBufferResource(void);
		virtual ~IndicesBufferResource(void);
	private:
		virtual void _DoDeleteBuffer( handle h ) = 0;//删除
		virtual void _DoBindBuffer( handle h ) = 0;//绑定
		virtual handle _DoCreateBuffer(uint datasize, uint typesize, const void* dataptr, BufferObjectProperty::MemoryUseage use) = 0;//生成
		virtual void _DoChangeData(handle h, uint offset, uint datasize, uint typesize, const void* dataptr ) = 0;//修改
	private:
		void _FlushIndiesBuffer( uint offset, uint indiesCount, uint typesize, const void* buffer );//从原来的bufferoffset开始，填充indiesCount个数据，buffer为需要填充的数据
	public:
		virtual void ReleaseResource();//释放资源
	public:
		void DoFlushIndiesBuffer();
	public:
		void FlushIndiesBuffer( const IndicesStream& buffer );
		void FlushIndiesBufferImmediately( const IndicesStream& buffer );
	protected:
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void _BindBuffer( handle h )
		{
			if ( s_CurrentHandle != h )
			{
				s_CurrentHandle = h;
				_DoBindBuffer( h );
			}
		}
	public:
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetDrawCount(uint count)//设置可绘制的个数，降低绘制开销
		{
			m_DrawCount = count;
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint DrawCount() const
		{
			return m_DrawCount;//有索引和没有索引绘制不一样
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void BindBuffer()
		{
			_BindBuffer( m_GPUResourceHandle );
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetMemoryUseage( BufferObjectProperty::MemoryUseage mu )
		{
			m_eMemoryUseage = mu;
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE BufferObjectProperty::IndicesType GetIndicesType() const
		{
			return m_eIndicesType;
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetDrawCount() const
		{
			return m_DrawCount;
		}
	};
}