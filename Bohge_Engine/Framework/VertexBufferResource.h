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
#include "ISharedResource.h"
#include "3DMath.h"


namespace BohgeEngine
{
	class VertexStream;
	class ReflushStream;
	class BOHGE_FRAMEWORK VertexBufferResource : public ISharedResource
	{
	private:
		typedef eastl::list< ReflushStream* >	ReflushList;//记录需要的刷新动作
	private:
		static handle							s_CurrentHandle;
	private:
		ReflushList								m_ReflushList[2];
		VertexStream*							m_pVertexStream[2];
		VertexStreamLayout						m_VertexLayout;
		BufferObjectProperty::MemoryUseage		m_eMemoryUseage;
		uint									m_GPUResourceHandle;
		uint									m_uVertexCount;//索引的数目
		uint									m_uVertexSize;
		uint									m_FlushIndex;
		uint									m_ChangeIndex;
	public:
		VertexBufferResource(void);
		virtual ~VertexBufferResource(void);
	private:
		virtual void _DoDeleteBuffer( handle h ) = 0;//删除
		virtual void _DoBindBuffer( handle h ) = 0;//绑定
		virtual handle _DoCreateBuffer(uint vertexcount, uint typesize, const void* dataptr, BufferObjectProperty::MemoryUseage use) = 0;//生成
		virtual void _DoChangeData(handle h, uint offset, uint vertexcount, uint typesize, const void* dataptr ) = 0;//修改
		virtual void _DoBindLayout( const Attributes& attr ) = 0;
		virtual void _DoUnbindLayout( const Attributes& attrs ) = 0;
	public:
		virtual void ReleaseResource();//释放资源
	public:
		void _PushReflushStream( const VertexStream& stream, const vector2d& interval );
	public:
		void DoChangeVertexBuffer();
		void DoFlushVertexBuffer();
		void FlushVertexBuffer( const VertexStream& stream );
		void FlushVertexBufferImmediately( const VertexStream& stream );
		void ChangeVertexBuffer( const VertexStream& stream );
		void ChangeVertexBufferImmediately( const VertexStream& stream );		
	protected:
		BOHGE_FORCEINLINE const VertexStreamLayout& _GetVertexStreamLayout() const
		{
			return m_VertexLayout;
		}
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
		BOHGE_FORCEINLINE void BindBuffer( const Attributes& attrs )
		{
			_BindBuffer( m_GPUResourceHandle );
			_DoBindLayout( attrs );
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void UnbindBuffer( const Attributes& attrs )
		{
			_DoUnbindLayout( attrs );
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetMemoryUseage( BufferObjectProperty::MemoryUseage mu )
		{
			m_eMemoryUseage = mu;
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetVertexSize() const
		{
			return m_uVertexSize;
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetVertexCount() const
		{
			return m_uVertexSize;
		}
	};
}

