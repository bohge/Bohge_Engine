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
#include "MaterialEntity.h"
#include "MaterialSource.h"
#include "MaterialProperty.h"
#include "MaterialResource.h"
#include "TextureEntity.h"
#include "ShaderEntity.h"
#include "Log.h"


using namespace eastl;


namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( MaterialEntity );
	//---------------------------------------------------------------------------------------------
	MaterialEntity::MaterialEntity(void)
		:m_Ambient(1,1,1),
		m_Diffuse(1,1,1),
		m_Specular(1,1,1),
		m_Emit(1,1,1),
		m_Opacity(1.0f),
		m_SpeLevel(1.0f),
		m_Shininess(1.0f),
		m_PushedTexture(0)
	{
		m_Textures.resize( MaterialProperty::ATT_TEXTURE_COUNT );
		m_UserParameters.resize( MaterialProperty::ATT_MATERIAL_USE_COUNT, NULL );
	}
	//---------------------------------------------------------------------------------------------
	MaterialEntity::~MaterialEntity(void)
	{
		FOR_EACH(TextureEntityVector, it, m_Textures)
		{
			if ( !it->m_isRef )
			{
				SAFE_DELETE( it->m_pTexture );
			}
		}
		m_Textures.clear();

		FOR_EACH(UserParameterArray, it, m_UserParameters)
		{
			SAFE_DELETE( *it );
		}
		m_UserParameters.clear();
	}
	//---------------------------------------------------------------------------------------------
	void MaterialEntity::_OnResourceCreated()
	{
		//DEBUGLOG("Material %d is loaded\n", _GetSourcePtr()->GetHashCode() );
		//const MaterialProperty::TextureSlotPropertyMap& texmap = 
		//	m_rMaterialResource->GetTextureSlotMap();
		//FOR_EACH_CONST( MaterialProperty::TextureSlotPropertyMap, slot, texmap )
		//{
		//	TextureEntity* tex = _GetTextureEntity( slot->first );
		//	tex->SetWarp( slot->second.m_Warp.m_SWarp, slot->second.m_Warp.m_TWarp );
		//	tex->SetFilter( slot->second.m_Filter.m_MagFilter, slot->second.m_Filter.m_MinFilter );
		//	tex->SetPixelFormat( slot->second.m_PixelFormat );
		//	tex->SetAnisotropic( slot->second.m_Anisotropic );
		//	tex->LoadResource();
		//}
	}
	//---------------------------------------------------------------------------------------------
	const TextureEntity* MaterialEntity::GetTextureEntity( MaterialProperty::Material_Attribute att ) const
	{
		return m_Textures[ att - MaterialProperty::ATT_TEXTURE_UNKONW ].m_pTexture;
	}
	//---------------------------------------------------------------------------------------------
	TextureEntity* MaterialEntity::GetTextureEntity( MaterialProperty::Material_Attribute att )
	{
		int index = att - MaterialProperty::ATT_TEXTURE_UNKONW;
		if( NULL == m_Textures[index].m_pTexture )
		{
			m_Textures[index].m_isRef = false;
			m_Textures[index].m_pTexture = NEW TextureEntity;
			m_Textures[index].m_pTexture->DependenceSource( _GetSourcePtr().GetUnsafePointer() );
		}
		return m_Textures[index].m_pTexture;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialEntity::_OnCreateResource()
	{
		//¼ÓÔØÎÆÀí
		FOR_EACH( TextureEntityVector, tex, m_Textures )
		{
			if ( NULL != tex->m_pTexture && !tex->m_isRef )
			{
				tex->m_pTexture->SetPriority( GetPriority() );
				tex->m_pTexture->SetJobType( GetJobType() );
				tex->m_pTexture->CreateResource();
			}
		}
	}
	//---------------------------------------------------------------------------------------------
	void MaterialEntity::SetUserParameters( MaterialProperty::Material_Attribute att, MaterialProperty::UserParameterType spt, const Object& obj )
	{
		int index = att - MaterialProperty::ATT_MATERIAL_USE_0;
		SAFE_DELETE( m_UserParameters[index] );
		MaterialProperty::UserParameter* up = NEW MaterialProperty::UserParameter;
		up->m_eUserParameterType = spt;
		up->m_isReference = false;
		up->m_pObject = obj.Duplicate();
		m_UserParameters[index] = up;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialEntity::SetUserParametersPtr( MaterialProperty::Material_Attribute att, MaterialProperty::UserParameterType spt, const Object* ptr )
	{
		int index = att - MaterialProperty::ATT_MATERIAL_USE_0;
		SAFE_DELETE( m_UserParameters[index] );
		MaterialProperty::UserParameter* up = NEW MaterialProperty::UserParameter;
		up->m_eUserParameterType = spt;
		up->m_isReference = true;
		up->m_pObject = ptr;
		m_UserParameters[index] = up;
	}
}
