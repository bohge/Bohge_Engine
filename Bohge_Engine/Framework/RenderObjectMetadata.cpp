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
#include "IResourceSystem.h"
#include "IndicesMetadata.h"
#include "VertexMetadata.h"
#include "ByteHelper.hpp"
#include "ILoader.h"



namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT(RenderObjectMeshMetadate);
	//------------------------------------------------------------------------------------------------------
	RenderObjectMeshMetadate::RenderObjectMeshMetadate( RenderObjectProperty::RenderMode rm, const IMetadata& vertex )
	{
		m_Metadata.m_eRenderMode = rm;
		m_Metadata.m_spVertex = SmartPtr<IMetadata>( static_cast<IMetadata*>( vertex.Duplicate() ) );
	}
	//------------------------------------------------------------------------------------------------------
	RenderObjectMeshMetadate::RenderObjectMeshMetadate( RenderObjectProperty::RenderMode rm, const IMetadata& vertex, const IMetadata& indices )
	{
		m_Metadata.m_eRenderMode = rm;
		m_Metadata.m_spVertex = SmartPtr<IMetadata>( static_cast<IMetadata*>( vertex.Duplicate() ) );
		m_Metadata.m_spIndices = SmartPtr<IMetadata>( static_cast<IMetadata*>( indices.Duplicate() ) );
	}
	//------------------------------------------------------------------------------------------------------
	RenderObjectMeshMetadate::~RenderObjectMeshMetadate(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void* RenderObjectMeshMetadate::_ReturnMetadata()
	{
		return &m_Metadata;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectMeshMetadate::GetIdentifier( eastl::vector<byte>& bytes ) const
	{
		m_Metadata.m_spVertex->GetIdentifier( bytes );
		if ( NULL != m_Metadata.m_spIndices )
		{
			m_Metadata.m_spIndices->GetIdentifier( bytes );
		}
		bytes.push_back( m_Metadata.m_eRenderMode );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectMeshMetadate::ProcessMetadata()
	{
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectMeshMetadate::ReleaseMetadate()
	{
	}



	RTTI_TYPE_IMPLEMENT(RenderObjectMaterialMetadata);
	//------------------------------------------------------------------------------------------------------
	RenderObjectMaterialMetadata::RenderObjectMaterialMetadata( const IMetadata& material, const vector3f& ambient, const vector3f& diffuse, const vector3f& specular, const vector3f& emit, float opacity, float speLevel, float shininess )
	{
		m_Metadata.m_Ambient = ambient;
		m_Metadata.m_Diffuse = diffuse;
		m_Metadata.m_Specular = specular;
		m_Metadata.m_Emit = emit;
		m_Metadata.m_Opacity = opacity;
		m_Metadata.m_SpeLevel = speLevel;
		m_Metadata.m_Shininess = shininess;
		m_Metadata.m_spMaterial = SmartPtr<IMetadata>( static_cast<IMetadata*>( material.Duplicate() ) );
	}
	//------------------------------------------------------------------------------------------------------
	RenderObjectMaterialMetadata::RenderObjectMaterialMetadata( const IMetadata& material )
	{
		m_Metadata.m_Ambient = vector3f( 1.0f, 1.0f, 1.0f );
		m_Metadata.m_Diffuse = vector3f( 1.0f, 1.0f, 1.0f );
		m_Metadata.m_Specular = vector3f( 1.0f, 1.0f, 1.0f );
		m_Metadata.m_Emit = vector3f( 1.0f, 1.0f, 1.0f );
		m_Metadata.m_Opacity = 1.0f;
		m_Metadata.m_SpeLevel = 1.0f;
		m_Metadata.m_Shininess = 1.0f;
		m_Metadata.m_spMaterial = SmartPtr<IMetadata>( static_cast<IMetadata*>( material.Duplicate() ) );
	}
	//------------------------------------------------------------------------------------------------------
	RenderObjectMaterialMetadata::~RenderObjectMaterialMetadata( void )
	{

	}
	//------------------------------------------------------------------------------------------------------
	void* RenderObjectMaterialMetadata::_ReturnMetadata()
	{
		return &m_Metadata;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectMaterialMetadata::GetIdentifier( eastl::vector<byte>& bytes ) const
	{
		m_Metadata.m_spMaterial->GetIdentifier( bytes );
		Utility::Vector3T2Byte( m_Metadata.m_Ambient, bytes );
		Utility::Vector3T2Byte( m_Metadata.m_Diffuse, bytes );
		Utility::Vector3T2Byte( m_Metadata.m_Specular, bytes );
		Utility::Vector3T2Byte( m_Metadata.m_Emit, bytes );

		Utility::SingleT2Byte( m_Metadata.m_Opacity, bytes );
		Utility::SingleT2Byte( m_Metadata.m_SpeLevel, bytes );
		Utility::SingleT2Byte( m_Metadata.m_Shininess, bytes );

	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectMaterialMetadata::ProcessMetadata()
	{

	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectMaterialMetadata::ReleaseMetadate()
	{

	}





	RTTI_TYPE_IMPLEMENT(RenderObjectTextureMetadata);
	RenderObjectTextureMetadata::RenderObjectTextureMetadata( MaterialProperty::Material_Attribute slot, const IMetadata& tex )
	{
		m_Metadata.m_TextureSlot = slot;
		m_Metadata.m_pTextureMetadata = static_cast<IMetadata*>( tex.Duplicate() );
	}
	//------------------------------------------------------------------------------------------------------
	RenderObjectTextureMetadata::~RenderObjectTextureMetadata( void )
	{
	}
	//------------------------------------------------------------------------------------------------------
	void* RenderObjectTextureMetadata::_ReturnMetadata()
	{
		return &m_Metadata;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectTextureMetadata::GetIdentifier( eastl::vector<byte>& bytes ) const
	{
		m_Metadata.m_pTextureMetadata->GetIdentifier( bytes );
		bytes.push_back( m_Metadata.m_TextureSlot );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectTextureMetadata::ProcessMetadata()
	{

	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectTextureMetadata::ReleaseMetadate()
	{
		SAFE_DELETE( m_Metadata.m_pTextureMetadata );
	}




	RTTI_TYPE_IMPLEMENT( RenderObjectMeshFileMetadate );
	RenderObjectMeshFileMetadate::RenderObjectMeshFileMetadate( const eastl::string& path )
		:m_Path( path )
	{

	}
	//------------------------------------------------------------------------------------------------------
	RenderObjectMeshFileMetadate::~RenderObjectMeshFileMetadate( void )
	{

	}
	//------------------------------------------------------------------------------------------------------
	void* RenderObjectMeshFileMetadate::_ReturnMetadata()
	{
		return &m_Metadata;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectMeshFileMetadate::GetIdentifier( eastl::vector<byte>& bytes ) const
	{
		bytes.insert( bytes.end(), m_Path.begin(), m_Path.end() );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectMeshFileMetadate::ProcessMetadata()
	{
		uint hash = Utility::HashCode(m_Path);
		ILoader* loader = IResourceSystem::Instance()->LoaderFactory( IResourceProperty::SLT_MESH );
		m_pSourceData = loader->Load<RenderObjectProperty::MeshStreamData>( m_Path );
		m_Metadata.m_eRenderMode = m_pSourceData->m_eMode;
		m_Metadata.m_spIndices = SmartPtr<LoadedIndicesMetadata>( NEW LoadedIndicesMetadata( BufferObjectProperty::MU_STATIC, &m_pSourceData->m_Indices, hash ) );
		m_Metadata.m_spVertex = SmartPtr<LoadedVertexMetadata>( NEW LoadedVertexMetadata( BufferObjectProperty::MU_STATIC, &m_pSourceData->m_Vertex, hash ) );
		IResourceSystem::Instance()->RecycleBin( loader );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectMeshFileMetadate::ReleaseMetadate()
	{
		SAFE_DELETE( m_pSourceData );
	}
}