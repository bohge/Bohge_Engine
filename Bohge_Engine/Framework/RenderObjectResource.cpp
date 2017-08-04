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
#include "RenderObjectResource.h"
#include "IndicesBufferEntity.h"
#include "VertexBufferEntity.h"
#include "MaterialEntity.h"



namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	RenderObjectResource::RenderObjectResource(void)
		:m_eRenderMode( RenderObjectProperty::RM_TRIANGLES ),
		m_pDrawFunc(NULL),
		m_rpMaterialEntity(NULL),
		m_rpVertexBufferEntity(NULL),
		m_rpIndicesBufferEntity(NULL)
	{
	}
	//------------------------------------------------------------------------------------------------------
	RenderObjectResource::~RenderObjectResource(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResource::ReleaseResource()
	{

	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResource::SetupRenderObject( MaterialEntity* me, VertexBufferEntity* vbo, IndicesBufferEntity* ibo )
	{
		m_rpMaterialEntity = me;
		m_rpVertexBufferEntity = vbo;
		m_rpIndicesBufferEntity = ibo;
		if ( m_rpIndicesBufferEntity )
		{
			m_pDrawFunc = &RenderObjectResource::_DrawElements;
		}
		else
		{
			m_pDrawFunc = &RenderObjectResource::_DrawArrays;
		}
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResource::_DrawElements( int pass, BohgeEngine::Node* host, MaterialEntity* me, VertexBufferEntity* vbo, IndicesBufferEntity* ibo )
	{
		const Attributes& attrs = me->GetShaderAttributes( pass );
		me->ActiveMaterial( pass, *host );
		vbo->BindBuffer( attrs );
		ibo->BindBuffer();
		_DoDrawElements( m_eRenderMode, ibo->GetIndicesType(), ibo->GetDrawCount() );
		vbo->UnbindBuffer( attrs );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResource::_DrawArrays( int pass, BohgeEngine::Node* host, MaterialEntity* me, VertexBufferEntity* vbo, IndicesBufferEntity* ibo )
	{
		const Attributes& attrs = me->GetShaderAttributes( pass );
		me->ActiveMaterial( pass, *host );
		vbo->BindBuffer( attrs );
		_DoDrawArrays( m_eRenderMode, vbo->GetVertexCount() );
		vbo->UnbindBuffer( attrs );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResource::FlushVertexBuffer( const VertexStream& stream )
	{
		m_rpVertexBufferEntity->FlushVertexBuffer( stream );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResource::FlushVertexBufferImmediately( const VertexStream& stream )
	{
		m_rpVertexBufferEntity->FlushVertexBufferImmediately( stream );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResource::ChangeVertexBuffer( const VertexStream& stream )
	{
		m_rpVertexBufferEntity->ChangeVertexBuffer( stream );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResource::ChangeVertexBufferImmediately( const VertexStream& stream )
	{
		m_rpVertexBufferEntity->ChangeVertexBufferImmediately( stream );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResource::FlushIndiesBuffer( const IndicesStream& buffer )
	{
		m_rpIndicesBufferEntity->FlushIndiesBuffer( buffer );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResource::FlushIndiesBufferImmediately( const IndicesStream& buffer )
	{
		m_rpIndicesBufferEntity->FlushIndiesBufferImmediately( buffer );
	}

}