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
#include "MaterialProperty.h"
#include "SmartPtr.hpp"
#include "IMetadata.h"



namespace BohgeEngine
{
	namespace RenderObjectData
	{
		enum DataType
		{
			DT_MESH = 0,
			DT_MATERIAL,
			DT_TEXTURE,
		};
		struct RenderObjectBaseData
		{
		private:
			DataType		m_Type;
		public:
			RenderObjectBaseData( DataType t):m_Type(t){}
		public:
			BOHGE_FORCEINLINE DataType GetDataType()
			{
				return m_Type;
			}
		};

		struct MeshData : public RenderObjectBaseData
		{
			RenderObjectProperty::RenderMode	m_eRenderMode;
			SmartPtr<IMetadata>					m_spVertex;
			SmartPtr<IMetadata>					m_spIndices;
			MeshData(void):RenderObjectBaseData(DT_MESH){};
			~MeshData(void){};
		};

		struct MaterialData : public RenderObjectBaseData
		{
			SmartPtr<IMetadata>		m_spMaterial;
			vector3f				m_Ambient;
			vector3f				m_Diffuse;
			vector3f				m_Specular;
			vector3f				m_Emit;
			float					m_Opacity;
			float					m_SpeLevel;
			float					m_Shininess;
			MaterialData(void):RenderObjectBaseData(DT_MATERIAL){};
			~MaterialData(void){};
		};

		struct TextureData : public RenderObjectBaseData
		{
			MaterialProperty::Material_Attribute	m_TextureSlot;
			IMetadata*								m_pTextureMetadata;
			TextureData(void):RenderObjectBaseData(DT_TEXTURE){};
			~TextureData(void){};
		};
	}

	class BOHGE_FRAMEWORK RenderObjectMeshMetadate : public IMetadata
	{
		RTTI_DRIVER_TYPE_DECLEAR( RenderObjectMeshMetadate, IMetadata )
	private:
		RenderObjectData::MeshData		m_Metadata;
	public:
		RenderObjectMeshMetadate( RenderObjectProperty::RenderMode rm, const IMetadata& vertex);
		RenderObjectMeshMetadate( RenderObjectProperty::RenderMode rm, const IMetadata& vertex, const IMetadata& indices );
		~RenderObjectMeshMetadate(void);
	private:
		virtual void* _ReturnMetadata( );
	public:
		virtual void GetIdentifier( eastl::vector<byte>& bytes ) const;//在str中推入表示符号
		virtual void ProcessMetadata();//处理原始资源
		virtual void ReleaseMetadate();
	};

	//材质
	class BOHGE_FRAMEWORK RenderObjectMaterialMetadata : public IMetadata
	{
		RTTI_DRIVER_TYPE_DECLEAR( RenderObjectMaterialMetadata, IMetadata );
	private:
		RenderObjectData::MaterialData		m_Metadata;
	public:
		RenderObjectMaterialMetadata( const IMetadata& material );
		RenderObjectMaterialMetadata(
			const IMetadata& material,
			const vector3f& ambient,
			const vector3f& diffuse,
			const vector3f& specular,
			const vector3f& emit,
			float opacity,
			float speLevel,
			float shininess	);
		~RenderObjectMaterialMetadata(void);
	private:
		virtual void* _ReturnMetadata( );
	public:
		virtual void GetIdentifier( eastl::vector<byte>& bytes ) const;//在str中推入表示符号
		virtual void ProcessMetadata();//处理原始资源
		virtual void ReleaseMetadate();
	};


	//纹理的
	class BOHGE_FRAMEWORK RenderObjectTextureMetadata : public IMetadata
	{
		RTTI_DRIVER_TYPE_DECLEAR( RenderObjectTextureMetadata, IMetadata )
	private:
		RenderObjectData::TextureData			m_Metadata;
	public:
		RenderObjectTextureMetadata( MaterialProperty::Material_Attribute slot, const IMetadata& tex );
		~RenderObjectTextureMetadata(void);
	private:
		virtual void* _ReturnMetadata( );
	public:
		virtual void GetIdentifier( eastl::vector<byte>& bytes ) const;//在str中推入表示符号
		virtual void ProcessMetadata();//处理原始资源
		virtual void ReleaseMetadate();
	};

	//加载mesh的Metadata
	class BOHGE_FRAMEWORK RenderObjectMeshFileMetadate : public IMetadata
	{
		RTTI_DRIVER_TYPE_DECLEAR( RenderObjectMeshFileMetadate, IMetadata );
	private:
		eastl::string							m_Path;
		RenderObjectData::MeshData				m_Metadata;
		RenderObjectProperty::MeshStreamData*	m_pSourceData;
	public:
		RenderObjectMeshFileMetadate( const eastl::string& path );
		~RenderObjectMeshFileMetadate(void);
	private:
		virtual void* _ReturnMetadata( );
	public:
		virtual void GetIdentifier( eastl::vector<byte>& bytes ) const;//在str中推入表示符号
		virtual void ProcessMetadata();//处理原始资源
		virtual void ReleaseMetadate();
	};
}