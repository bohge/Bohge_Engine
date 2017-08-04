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
#include "MaterialResource.h"
#include "MaterialProperty.h"
#include "MaterialSource.h"
#include "ShaderEntity.h"
#include "IAsyncEntity.h"
#include "SmartPtr.hpp"


#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace BohgeEngine
{
	class Node;
	class TextureEntity;
	class MaterialResource;
	class VertexBufferEntity;
	class BOHGE_FRAMEWORK MaterialEntity : public IAsyncEntity< MaterialResource, MaterialSource >
	{
		RTTI_DRIVER_TYPE_DECLEAR( MaterialEntity, IAsyncEntity );
	private:
		struct TextureEntityData
		{
			TextureEntity*		m_pTexture;
			bool				m_isRef;
			TextureEntityData():m_pTexture(NULL),m_isRef(true){}
		};
	private:
		typedef eastl::vector< TextureEntityData > TextureEntityVector;
		typedef eastl::vector< MaterialProperty::UserParameter* > UserParameterArray;
	private:
		UserParameterArray	m_UserParameters;
		TextureEntityVector	m_Textures;
		vector3f			m_Ambient;
		vector3f			m_Diffuse;
		vector3f			m_Specular;
		vector3f			m_Emit;
		float				m_Opacity;
		float				m_SpeLevel;
		float				m_Shininess;
		byte				m_PushedTexture;
	public:
		MaterialEntity(void);
		~MaterialEntity(void);
	private:
		virtual void _OnCreateResource();//准备加载资源的时候调用
		virtual void _OnResourceCreated();//资源加载完毕的回掉
	public:
		const TextureEntity* GetTextureEntity( MaterialProperty::Material_Attribute att ) const;
		TextureEntity* GetTextureEntity( MaterialProperty::Material_Attribute att );//纹理在shader加载完毕后自动加载
		void SetUserParameters( MaterialProperty::Material_Attribute att, MaterialProperty::UserParameterType spt, const Object& obj );
		void SetUserParametersPtr( MaterialProperty::Material_Attribute att, MaterialProperty::UserParameterType spt, const Object* ptr );
	public:
		BOHGE_FORCEINLINE const MaterialProperty::UserParameter& GetUserParameters( MaterialProperty::Material_Attribute att ) const
		{
			return *m_UserParameters[att - MaterialProperty::ATT_MATERIAL_USE_0];
		}
		BOHGE_FORCEINLINE void SetTextureEntity( MaterialProperty::Material_Attribute att, const TextureEntity* te )
		{
			TextureEntityData & tex = m_Textures[att - MaterialProperty::ATT_TEXTURE_UNKONW];
			ASSERT( NULL == tex.m_pTexture || tex.m_isRef );
			tex.m_isRef = true;
			tex.m_pTexture = const_cast<TextureEntity*>( te );
		}
		BOHGE_FORCEINLINE uint GetMaterialID( int pt ) const
		{
			return _GetResourcePtr()->GetMaterialID( pt );
		}
		BOHGE_FORCEINLINE uint GetRenderQueue( int pt ) const
		{
			return _GetResourcePtr()->GetRenderQueue( pt );
		}
		BOHGE_FORCEINLINE bool isTransparent( int pt )
		{
			return _GetResourcePtr()->isTransparent( pt );
		}
		BOHGE_FORCEINLINE void ActiveMaterial( int pt, const Node& node )
		{
			_GetResourcePtr()->BindRenderState( pt );
			ShaderEntity* shader = _GetResourcePtr()->GetShaderEntity( pt );
			shader->ActiveShader( *this, node );
		}
		BOHGE_FORCEINLINE const Attributes& GetShaderAttributes( int pt )
		{
			ShaderEntity* shader = _GetResourcePtr()->GetShaderEntity( pt );
			return shader->GetShaderAttributes();
		}
		BOHGE_FORCEINLINE const vector3f& GetAmbient() const
		{
			return m_Ambient;
		}
		BOHGE_FORCEINLINE const vector3f& GetDiffuse() const
		{
			return m_Diffuse;
		}
		BOHGE_FORCEINLINE const vector3f& GetSpecular() const
		{
			return m_Specular;
		}
		BOHGE_FORCEINLINE const vector3f& GetEmit() const
		{
			return m_Emit;
		}
		BOHGE_FORCEINLINE const float& GetOpacity() const
		{
			return m_Opacity;
		}
		BOHGE_FORCEINLINE const float& GetSpeLevel() const
		{
			return m_SpeLevel;
		}
		BOHGE_FORCEINLINE const float& GetShininess() const
		{
			return m_Shininess;
		}
		BOHGE_FORCEINLINE void SetAmbient( const vector3f& v )
		{
			m_Ambient = v;
		}
		BOHGE_FORCEINLINE void SetDiffuse( const vector3f& v )
		{
			m_Diffuse = v;
		}
		BOHGE_FORCEINLINE void SetSpecular( const vector3f& v )
		{
			m_Specular = v;
		}
		BOHGE_FORCEINLINE void SetEmit( const vector3f& v )
		{
			m_Emit = v;
		}
		BOHGE_FORCEINLINE void SetOpacity( float v )
		{
			m_Opacity = v;
		}
		BOHGE_FORCEINLINE void SetSpeLevel( float v )
		{
			m_SpeLevel = v;
		}
		BOHGE_FORCEINLINE void SetShininess( float v )
		{
			m_Shininess = v;
		}
	};
}
