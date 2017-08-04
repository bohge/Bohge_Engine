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
#include "RenderObjectProperty.h"
#include "BufferObjectProperty.h"
#include "MaterialEntity.h"
#include "ISharedResource.h"




namespace BohgeEngine
{
	class Node;
	class VertexBufferEntity;
	class IndicesBufferEntity;
	class BOHGE_FRAMEWORK RenderObjectResource : public ISharedResource
	{
	private:
		typedef void (RenderObjectResource::*DrawFunc)( int pass, BohgeEngine::Node* host, MaterialEntity* , VertexBufferEntity* , IndicesBufferEntity* );
	private:
		RenderObjectProperty::RenderMode	m_eRenderMode;
		DrawFunc							m_pDrawFunc;
		MaterialEntity*						m_rpMaterialEntity;
		VertexBufferEntity*					m_rpVertexBufferEntity;
		IndicesBufferEntity*				m_rpIndicesBufferEntity;
	public:
		RenderObjectResource(void);
		~RenderObjectResource(void);
	private:
		virtual void _DoDrawElements( RenderObjectProperty::RenderMode rm, BufferObjectProperty::IndicesType it, uint count ) = 0;//ibo != NULL
		virtual void _DoDrawArrays( RenderObjectProperty::RenderMode rm, uint count ) = 0;//ibo == NULL
	private:
		void _DrawElements( int pass, BohgeEngine::Node* host, MaterialEntity* me, VertexBufferEntity* vbo, IndicesBufferEntity* ibo );//ibo != NULL
		void _DrawArrays( int pass, BohgeEngine::Node* host, MaterialEntity* me, VertexBufferEntity* vbo, IndicesBufferEntity* ibo );//ibo == NULL
	public:
		virtual void ReleaseResource();
	public:
		void SetupRenderObject( MaterialEntity* me, VertexBufferEntity* vbo, IndicesBufferEntity* ibo );
		void FlushVertexBuffer( const VertexStream& stream );//必须在主线程调用
		void FlushVertexBufferImmediately( const VertexStream& stream );//必须在主线程调用
		void ChangeVertexBuffer( const VertexStream& stream );
		void ChangeVertexBufferImmediately( const VertexStream& stream );
		void FlushIndiesBuffer( const IndicesStream& buffer );
		void FlushIndiesBufferImmediately( const IndicesStream& buffer );
	protected:
		BOHGE_FORCEINLINE RenderObjectProperty::RenderMode _GetRenderMode()
		{
			return m_eRenderMode;
		}
	public:
		BOHGE_FORCEINLINE void SetRenderMode( RenderObjectProperty::RenderMode rm )
		{
			m_eRenderMode = rm;
		}
		BOHGE_FORCEINLINE void Draw( int pass, BohgeEngine::Node* host )//渲染
		{
			(this->*m_pDrawFunc)( pass, host, m_rpMaterialEntity, m_rpVertexBufferEntity, m_rpIndicesBufferEntity );
		}
		BOHGE_FORCEINLINE uint GetMaterialID( int pt ) const
		{
			return m_rpMaterialEntity->GetMaterialID( pt );
		}
		BOHGE_FORCEINLINE uint GetRenderQueue( int pt ) const
		{
			return m_rpMaterialEntity->GetRenderQueue( pt );
		}
		BOHGE_FORCEINLINE bool isTransparent( int pt )
		{
			return m_rpMaterialEntity->isTransparent( pt );
		}
		BOHGE_FORCEINLINE void SetTextureEntity( MaterialProperty::Material_Attribute att, const TextureEntity* te )
		{
			m_rpMaterialEntity->SetTextureEntity( att, te );
		}
	};
}