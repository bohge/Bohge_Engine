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
#include "VertexBufferResource.h"
#include "IGraphicSystem.h"
#include "ReflushStream.h"
#include "VertexStream.h"


namespace BohgeEngine
{

	handle VertexBufferResource::s_CurrentHandle = -1;

	//---------------------------------------------------------------------------------------------------------
	VertexBufferResource::VertexBufferResource(void)
		:m_eMemoryUseage((BufferObjectProperty::MemoryUseage)-1),
		m_GPUResourceHandle(-1),
		m_uVertexCount(0),
		m_uVertexSize(0),
		m_ChangeIndex(0),
		m_FlushIndex(0)
	{
		m_pVertexStream[0] = NULL;
		m_pVertexStream[1] = NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	VertexBufferResource::~VertexBufferResource(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResource::ReleaseResource()
	{
		if ( -1 != m_GPUResourceHandle )
		{
			_DoDeleteBuffer( m_GPUResourceHandle );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResource::FlushVertexBufferImmediately( const VertexStream& stream )
	{
		if ( -1 == m_GPUResourceHandle )//初始化
		{
			m_VertexLayout = stream.GetLayout();
			m_GPUResourceHandle = _DoCreateBuffer( stream.GetVertexCount(), stream.GetVertexStride() , stream.GetBufferData(), m_eMemoryUseage );
		}
		else if ( m_uVertexCount < stream.GetVertexCount() )//原来的不够了
		{
			m_GPUResourceHandle = _DoCreateBuffer( stream.GetVertexCount(), stream.GetVertexStride() , stream.GetBufferData(), m_eMemoryUseage );
		}
		else
		{
			ASSERT( BufferObjectProperty::MU_DYNAMIC == m_eMemoryUseage );//只有DYNAMIC可以调用更改
			_DoChangeData( m_GPUResourceHandle, 0, stream.GetVertexCount(), stream.GetVertexStride(), stream.GetReflushBufferData() );
		}
		m_uVertexSize = stream.GetVertexSize();
		m_uVertexCount = stream.GetVertexCount();
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResource::FlushVertexBuffer( const VertexStream& stream )
	{
		VertexStream* temp = NEW VertexStream( stream );
		if( NULL == m_pVertexStream[m_FlushIndex] )//第一次进入，推入回掉系统
		{
			IGraphicSystem::Instance()->HostedGraphicUpdata( MEMBER_FUNC_PTR(&VertexBufferResource::DoFlushVertexBuffer) );
		}
		else
		{
			SAFE_DELETE( m_pVertexStream[m_FlushIndex] );
		}
		m_pVertexStream[m_FlushIndex] = temp;
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResource::DoFlushVertexBuffer()
	{
		//这个地方会存在多线程问题
		VertexStream* temp = m_pVertexStream[m_FlushIndex];
		m_pVertexStream[m_FlushIndex] = NULL;
		m_FlushIndex = !m_FlushIndex;
		FlushVertexBufferImmediately( *temp );
		SAFE_DELETE( temp );
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResource::_PushReflushStream( const VertexStream& stream, const vector2d& interval )
	{
		ReflushStream* reflush = NEW ReflushStream;
		reflush->FlushStream( interval, stream.GetVertexSize(), stream.GetBufferData( interval.m_x ) );
		m_ReflushList[m_ChangeIndex].push_back( reflush );
		IGraphicSystem::Instance()->HostedGraphicUpdata( MEMBER_FUNC_PTR(&VertexBufferResource::DoChangeVertexBuffer) );
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResource::ChangeVertexBuffer( const VertexStream& stream )
	{
		const vector2d& interval = stream.GetReflushInterval();
		ASSERT( interval.m_x != interval.m_y );
		if ( interval.m_x < interval.m_y )
		{//正常刷新，产生一个刷新事件
			_PushReflushStream( stream, interval );
		}
		else
		{//当刷新越界，得时候产生两个刷新
			_PushReflushStream( stream, vector2d( interval.m_x, stream.GetVertexCount() ) );
			_PushReflushStream( stream, vector2d( 0, interval.m_y ) );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResource::DoChangeVertexBuffer()
	{
		ReflushList& rflist = m_ReflushList[ m_ChangeIndex ];
		m_ChangeIndex = !m_ChangeIndex;
		FOR_EACH( ReflushList, it, rflist )
		{
			const vector2d& inter = (*it)->GetReflushInterval();
			ASSERT( inter.m_x != inter.m_y );
			_DoChangeData( m_GPUResourceHandle, inter.m_x, (*it)->GetDataCount(), (*it)->GetDataSize(), (*it)->GetBufferData() );
			SAFE_DELETE(*it);
		}
		rflist.clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResource::ChangeVertexBufferImmediately( const VertexStream& stream )
	{
		const vector2d& interval = stream.GetReflushInterval();
		ASSERT( interval.m_x != interval.m_y );
		if ( interval.m_x < interval.m_y )
		{//正常刷新，产生一个刷新事件
			_DoChangeData(
				m_GPUResourceHandle,
				interval.m_x,
				interval.m_y - interval.m_x,
				stream.GetVertexSize(),
				stream.GetBufferData( interval.m_x ) );
		}
		else
		{//当刷新越界，得时候产生两个刷新
			_DoChangeData(
				m_GPUResourceHandle,
				interval.m_x,
				stream.GetVertexCount() - interval.m_x,
				stream.GetVertexSize(),
				stream.GetBufferData( interval.m_x ) );
			_DoChangeData(
				m_GPUResourceHandle,
				0,
				interval.m_y,
				stream.GetVertexSize(),
				stream.GetBufferData( 0 ) );
		}
		
	}
}