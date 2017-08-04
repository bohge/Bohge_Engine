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
#include "RenderObjectMetadata.h"
#include "RenderObjectResource.h"
#include "IndicesBufferEntity.h"
#include "RenderObjectSource.h"
#include "VertexBufferEntity.h"
#include "MaterialEntity.h"
#include "IGraphicSystem.h"
#include "TextureEntity.h"


namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	RenderObjectSource::RenderObjectSource(void)
		:ISharedSource( IResourceProperty::SST_RENDEROBJECT, IResourceProperty::DT_RENDER_THREAD, true ),
		m_pMaterialEntity(NEW MaterialEntity()),
		m_pVertexBufferEntity(NEW VertexBufferEntity()),
		m_pIndicesBufferEntity(NULL),
		m_eRenderMode( RenderObjectProperty::RM_POINTS )
	{
	}
	//------------------------------------------------------------------------------------------------------
	RenderObjectSource::~RenderObjectSource(void)
	{
		SAFE_DELETE( m_pVertexBufferEntity );
		SAFE_DELETE( m_pIndicesBufferEntity );
		SAFE_DELETE( m_pMaterialEntity );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectSource::_DoLoadResource()
	{
		const ISourceMetadataList& metas = GetSourceMetadata();
		FOR_EACH_CONST( ISourceMetadataList, it, metas )
		{
			(*it)->ProcessMetadata();//处理数据
			RenderObjectData::RenderObjectBaseData* data = (*it)->GetMetadata<RenderObjectData::RenderObjectBaseData>();
			switch( data->GetDataType() )
			{
			case RenderObjectData::DT_MESH:
				{
					RenderObjectData::MeshData* metadata 
						= static_cast<RenderObjectData::MeshData*>(data);

					m_eRenderMode = metadata->m_eRenderMode;

					m_pVertexBufferEntity->PushMetadata( *metadata->m_spVertex );
					m_pVertexBufferEntity->DependenceSource( this );

					if ( NULL != metadata->m_spIndices )
					{
						m_pIndicesBufferEntity = NEW IndicesBufferEntity();
						m_pIndicesBufferEntity->PushMetadata( *metadata->m_spIndices );
						m_pIndicesBufferEntity->DependenceSource( this );
					}
				}break;
			case RenderObjectData::DT_MATERIAL:
				{
					RenderObjectData::MaterialData* metadata 
						= static_cast<RenderObjectData::MaterialData*>(data);
					m_pMaterialEntity->PushMetadata( *metadata->m_spMaterial );
					m_pMaterialEntity->SetAmbient( metadata->m_Ambient );
					m_pMaterialEntity->SetDiffuse( metadata->m_Diffuse );
					m_pMaterialEntity->SetSpecular( metadata->m_Specular );
					m_pMaterialEntity->SetEmit( metadata->m_Emit );
					m_pMaterialEntity->SetSpeLevel( metadata->m_SpeLevel );
					m_pMaterialEntity->SetShininess( metadata->m_Shininess );
					m_pMaterialEntity->SetOpacity( metadata->m_Opacity );
					m_pMaterialEntity->DependenceSource( this );
				}break;
			case RenderObjectData::DT_TEXTURE:
				{
					RenderObjectData::TextureData* meatadata
						= static_cast<RenderObjectData::TextureData*>(data);
					TextureEntity* tex = m_pMaterialEntity->GetTextureEntity( meatadata->m_TextureSlot );
					tex->PushMetadata( *meatadata->m_pTextureMetadata );//纹理在shader加载完毕后自动加载
				}break;
			default:ASSERT(false);
			}
		}
		m_pMaterialEntity->CreateResource();
		m_pVertexBufferEntity->CreateResource();
		if ( NULL != m_pIndicesBufferEntity )
		{
			m_pIndicesBufferEntity->CreateResource();
		}
	}
	//------------------------------------------------------------------------------------------------------
	ISharedResource* RenderObjectSource::MakeResource()
	{
		RenderObjectResource* ro = static_cast<RenderObjectResource*>( IGraphicSystem::Instance()->CreateResource( IResourceProperty::SRT_RENDEROBJECT ) );
		ro->SetupRenderObject( m_pMaterialEntity, m_pVertexBufferEntity, m_pIndicesBufferEntity );
		ro->SetRenderMode( m_eRenderMode );
		return ro;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectSource::_DoReleaseUnnecessaryDataAfterLoaded()
	{

	}

}