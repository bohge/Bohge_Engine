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



//////////////////////////////////////////////////////////////
//		Shader的信息，包括uniform和attribute还有代码段		//
//////////////////////////////////////////////////////////////

#include "AssembledMaterialShader.h"
#include "Material.h"
#include "ISceneNode.h"
#include "Device.h"
#include "Engine.h"
#include "Camera.h"
#include "Light.h"
#include "Environment.h"

using namespace std;

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	AssembledMaterialShader::AssembledMaterialShader(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	AssembledMaterialShader::~AssembledMaterialShader(void)
	{
		FOR_EACH( UnifromFunctionList, it, m_UnifromFunctionList )
		{
			SAFE_DELETE( *it );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void AssembledMaterialShader::_SetParameters()
	{
		Device* device = Engine::Instance().GetDevice();
		FOR_EACH( UnifromFunctionList, it, m_UnifromFunctionList )
		{
			(*it)->m_Function( *device, (*it)->m_hUniform, GetNode() );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool AssembledMaterialShader::Initialization()
	{
		FOR_EACH( ParameterList, it, m_AttributeList )
		{
			m_Attributes.PushAttribute( it->m_ParamName, _MappingAttributes( it->m_eInputType ) );
		}
		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, m_VertexCode, m_FragmentCode ) )
			return false;
		FOR_EACH( ParameterList, it, m_UniformList )
		{
			ShaderUniform* su = NEW ShaderUniform;
			su->m_hUniform = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram, it->m_ParamName.c_str() );
			su->m_Function = _MappingUniformFunction( it->m_eInputType );
			m_UnifromFunctionList.push_back( su );
		}
		//清理
		m_UniformList.clear();
		m_AttributeList.clear();
		m_VertexCode.clear();
		m_FragmentCode.clear();
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	Attributes::ShaderAttributes AssembledMaterialShader::_MappingAttributes( const ShaderParameter::InputsType& it )
	{
		switch( it )
		{
		case ShaderParameter::IT_LOCALSPACE_POSITION:			return Attributes::ATTRIBUTE_POSITION;
		case ShaderParameter::IT_LOCALSPACE_COORDNATE:			return Attributes::ATTRIBUTE_TEXCOORD0;
		case ShaderParameter::IT_LOCALSPACE_NORMAL:				return Attributes::ATTRIBUTE_NORMAL;
		case ShaderParameter::IT_LOCALSPACE_TANGENT:			return Attributes::ATTRIBUTE_TANGENT;
		case ShaderParameter::IT_LOCALSPACE_BINORMAL:			return Attributes::ATTRIBUTE_BINORMAL;
		case ShaderParameter::IT_LOCALSPACE_BONE_INEX:			return Attributes::ATTRIBUTE_BONEINDICES;
		case ShaderParameter::IT_LOCALSPACE_BONE_WEIGHT:		return Attributes::ATTRIBUTE_BONEWEIGHTS;
		case ShaderParameter::IT_WORLDSPACE_POSITION:			return Attributes::ATTRIBUTE_POSITION;
		case ShaderParameter::IT_WORLDSPACE_NORMAL:				return Attributes::ATTRIBUTE_NORMAL;
		case ShaderParameter::IT_WORLDSPACE_TANGENT:			return Attributes::ATTRIBUTE_TANGENT;
		case ShaderParameter::IT_WORLDSPACE_BINORMAL:			return Attributes::ATTRIBUTE_BINORMAL;
		case ShaderParameter::IT_SCREENSPACE_POSITION:			return Attributes::ATTRIBUTE_POSITION;
		case ShaderParameter::IT_SCREENSPACE_COORDNATE:			return Attributes::ATTRIBUTE_TEXCOORD0;
		case ShaderParameter::IT_SCREENSPACE_NORMAL:			return Attributes::ATTRIBUTE_NORMAL;
		case ShaderParameter::IT_SCREENSPACE_TANGENT:			return Attributes::ATTRIBUTE_TANGENT;
		case ShaderParameter::IT_SCREENSPACE_BINORMAL:			return Attributes::ATTRIBUTE_BINORMAL;
		case ShaderParameter::IT_USER_ATTRIBUTE_0:				return Attributes::ATTRIBUTE_USER_0;
		case ShaderParameter::IT_USER_ATTRIBUTE_1:				return Attributes::ATTRIBUTE_USER_1;
		case ShaderParameter::IT_USER_ATTRIBUTE_2:				return Attributes::ATTRIBUTE_USER_2;
		case ShaderParameter::IT_USER_ATTRIBUTE_3:				return Attributes::ATTRIBUTE_USER_3;
		case ShaderParameter::IT_USER_ATTRIBUTE_4:				return Attributes::ATTRIBUTE_USER_4;
		case ShaderParameter::IT_USER_ATTRIBUTE_5:				return Attributes::ATTRIBUTE_USER_5;
		case ShaderParameter::IT_USER_ATTRIBUTE_6:				return Attributes::ATTRIBUTE_USER_6;
		case ShaderParameter::IT_USER_ATTRIBUTE_7:				return Attributes::ATTRIBUTE_USER_7;
		default: ASSERT(false && "cant mapping attributes!" );
		}
		return Attributes::ATTRIBUTE_POSITION;
	}


	//全部的uniform的设置函数
	void RealBoneArray( Device& device, handle hunifrom, const Material& mat )
	{
		std::vector<Quaternionf>* real = (std::vector<Quaternionf>*)(mat.Attribute( IAttributeNode::ATT_ANIMATION_REAL ));
		device.SetShaderParameter(hunifrom, *real);
	}
	void DualBoneArray( Device& device, handle hunifrom, const Material& mat )
	{
		std::vector<Quaternionf>* dual = (std::vector<Quaternionf>*)(mat.Attribute( IAttributeNode::ATT_ANIMATION_DUAL ));
		device.SetShaderParameter(hunifrom, *dual);
	}
	void WorldTransform( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *(Matrix44f*)(mat.Attribute( IAttributeNode::ATT_WORLDTRANSFORM )));
	}
	void WorldRotation( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *(Matrix33f*)(mat.Attribute( IAttributeNode::ATT_NORMALTRANSFORM )));
	}
	void TransViewProj( Device& device, handle hunifrom, const Material& mat )
	{
		Matrix44f& matrix = *(Matrix44f*)(mat.Attribute( IAttributeNode::ATT_WORLDTRANSFORM ));
		device.SetShaderParameter(hunifrom, matrix * Engine::Instance().GetCamera()->GetViewProj() );
	}
	void CameraPosition( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, Engine::Instance().GetCamera()->GetPosition() );
	}
	void CameraDirection( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, Engine::Instance().GetCamera()->GetForward() );
	}
	void CameraView( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, Engine::Instance().GetCamera()->GetView() );
	}
	void CameraProjection( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, Engine::Instance().GetCamera()->GetProject() );
	}
	void CameraViewProj( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, Engine::Instance().GetCamera()->GetViewProj() );
	}
	void CameraLinerParam( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, Engine::Instance().GetCamera()->LineDepthParam() );
	}
	void LightPosition( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, Engine::Instance().GetEnvironment()->GetCurrentLight().GetWorldPosition() );
	}
	void LightDirection( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, -Engine::Instance().GetEnvironment()->GetCurrentLight().GetWorldDirection() );//phong光是像素到光源的方向，所以这里反向
	}
	void LightColor( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, Engine::Instance().GetEnvironment()->GetCurrentLight().GetColor() );
	}
	void AmbientColor( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, Engine::Instance().GetEnvironment()->GetAmbient() );
	}
	void MaterialAmbient( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, mat.Ambient() );
	}
	void MaterialDiffuse( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, mat.Diffuse() );
	}
	void MaterialSpecular( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, mat.Specular() );
	}
	void MaterialSpecularLevel( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, mat.SpeLevel() );
	}
	void MaterialShininess( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, mat.Shininess() );
	}
	void MaterialLightAmbient( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, mat.Ambient() * Engine::Instance().GetEnvironment()->GetAmbient() );
	}
	void MaterialLightLight( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, mat.Diffuse() * Engine::Instance().GetEnvironment()->GetCurrentLight().GetColor() );
	}
	void MaterialLightSpecular( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, ( mat.Specular() * mat.SpeLevel() ) * Engine::Instance().GetEnvironment()->GetCurrentLight().GetColor() );
	}
	void TextureAmbient( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_AMBIENT)) );
	}
	void TextureDiffuse( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_DIFFUSE)) );
	}
	void TextureSpecular( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_SPECULAR)) );
	}
	void TextureSpecularLevel( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_SPLEVEL)) );
	}
	void TextureGlossiness( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_GLOSSINESS)) );
	}
	void TextureIllumination( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_ILLUMINATION)) );
	}
	void TextureOpacity( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_OPACITY)) );
	}
	void TextureFiter( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_FITER)) );
	}
	void TextureBump( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_BUMP)) );
	}
	void TextureReflection( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_REFLECTION)) );
	}
	void TextureRefraction( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_REFRACTION)) );
	}
	void TextureDisplacement( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_DISPLACEMENT)) );
	}
	void TextureTerrainWeight( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_TERRAIN_WEIGHT)) );
	}
	void TextureTerrainDetal( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_TERRAIN_DETAL)) );
	}
	void TextureTerrainBase( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_TERRAIN_BASE)) );
	}
	void TextureTerrainR( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_TERRAIN_R)) );
	}
	void TextureTerrainG( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_TERRAIN_G)) );
	}
	void TextureTerrainB( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_TERRAIN_B)) );
	}
	void TextureUserA( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_USER0)) );
	}
	void TextureUserB( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_USER1)) );
	}
	void TextureUserC( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_USER2)) );
	}
	void TextureUserD( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_USER3)) );
	}
	void TextureUserE( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_USER4)) );
	}
	void TextureUserF( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_USER5)) );
	}
	void TextureUserG( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_USER6)) );
	}
	void TextureUserH( Device& device, handle hunifrom, const Material& mat )
	{
		device.SetShaderParameter(hunifrom, *((Texture*)mat.Attribute(Material::ATT_TEX_USER7)) );
	}
	//-------------------------------------------------------------------------------------------------------
	AssembledMaterialShader::UnifromFunction AssembledMaterialShader::_MappingUniformFunction( const ShaderParameter::InputsType& it )
	{
		switch( it )//每一个参数一个函数，不想用一个runtime的switch，我认为会降低一些效率，以后可以测试，造出最好的办法
		{
		case ShaderParameter::IT_LOCALSPACE_REAL_ARRAY:			return &RealBoneArray;
		case ShaderParameter::IT_LOCALSPACE_DUAL_ARRAY:			return &DualBoneArray;
		case ShaderParameter::IT_LOCALWORLD_TRANSFORM:			return &WorldTransform;
		case ShaderParameter::IT_LOCALWORLD_ROTATION:			return &WorldRotation;
		case ShaderParameter::IT_LOCALSCREEN_TRANSVIEWPROJ:		return &TransViewProj;
		case ShaderParameter::IT_CAMERA_WORLDPOSITION:			return &CameraPosition;
		case ShaderParameter::IT_CAMERA_WORLDDIRECTION:			return &CameraDirection;
		case ShaderParameter::IT_CAMERA_VIEW:					return &CameraView;
		case ShaderParameter::IT_CAMERA_PROJECTION:				return &CameraProjection;
		case ShaderParameter::IT_CAMERA_VIEWPROJ:				return &CameraViewProj;
		case ShaderParameter::IT_CAMERA_LINERPARAM:				return &CameraLinerParam;
		case ShaderParameter::IT_LIGHT_POSITION:				return &LightPosition;
		case ShaderParameter::IT_LOCALSPACE_LIGHTDIRECTION:		return &LightDirection;
		case ShaderParameter::IT_WORLDSPACE_LIGHTDIRECTION:		return &LightDirection;
		case ShaderParameter::IT_LIGHT_COLOR:					return &LightColor;
		case ShaderParameter::IT_AMBIENT_COLOR:					return &AmbientColor;
		case ShaderParameter::IT_MATERIAL_AMBIENT:				return &MaterialAmbient;
		case ShaderParameter::IT_MATERIAL_DIFFUSE:				return &MaterialDiffuse;
		case ShaderParameter::IT_MATERIAL_SPECULAR:				return &MaterialSpecular;
		case ShaderParameter::IT_MATERIAL_SPECULAR_LEVEL:		return &MaterialSpecularLevel;
		case ShaderParameter::IT_MATERIAL_SHININESS:			return &MaterialShininess;
		case ShaderParameter::IT_LIGHT_MATERIAL_AMBIENT:		return &MaterialLightAmbient;
		case ShaderParameter::IT_LIGHT_MATERIAL_LIGHT:			return &MaterialLightLight;
		case ShaderParameter::IT_LIGHT_MATERIAL_SPECULAR:		return &MaterialLightSpecular;
		case ShaderParameter::IT_TEXTURE_AMBIENT:				return &TextureAmbient;
		case ShaderParameter::IT_TEXTURE_DIFFUSE:				return &TextureDiffuse;
		case ShaderParameter::IT_TEXTURE_SPECULAR:				return &TextureSpecular;
		case ShaderParameter::IT_TEXTURE_SPLEVEL:				return &TextureSpecularLevel;
		case ShaderParameter::IT_TEXTURE_GLOSSINESS:			return &TextureGlossiness;
		case ShaderParameter::IT_TEXTURE_ILLUMINATION:			return &TextureIllumination;
		case ShaderParameter::IT_TEXTURE_OPACITY:				return &TextureOpacity;
		case ShaderParameter::IT_TEXTURE_FITER:					return &TextureFiter;
		case ShaderParameter::IT_TEXTURE_BUMP:					return &TextureBump;
		case ShaderParameter::IT_TEXTURE_REFLECTION:			return &TextureReflection;
		case ShaderParameter::IT_TEXTURE_REFRACTION:			return &TextureRefraction;
		case ShaderParameter::IT_TEXTURE_DISPLACEMENT:			return &TextureDisplacement;
		case ShaderParameter::IT_TEXTURE_TERRAIN_WEIGHT:		return &TextureTerrainWeight;
		case ShaderParameter::IT_TEXTURE_TERRAIN_DETAL:			return &TextureTerrainDetal;
		case ShaderParameter::IT_TEXTURE_TERRAIN_BASE:			return &TextureTerrainBase;
		case ShaderParameter::IT_TEXTURE_TERRAIN_R:				return &TextureTerrainR;
		case ShaderParameter::IT_TEXTURE_TERRAIN_G:				return &TextureTerrainG;
		case ShaderParameter::IT_TEXTURE_TERRAIN_B:				return &TextureTerrainB;
		case ShaderParameter::IT_TEXTURE_USER0:					return &TextureUserA;
		case ShaderParameter::IT_TEXTURE_USER1:					return &TextureUserB;
		case ShaderParameter::IT_TEXTURE_USER2:					return &TextureUserC;
		case ShaderParameter::IT_TEXTURE_USER3:					return &TextureUserD;
		case ShaderParameter::IT_TEXTURE_USER4:					return &TextureUserE;
		case ShaderParameter::IT_TEXTURE_USER5:					return &TextureUserF;
		case ShaderParameter::IT_TEXTURE_USER6:					return &TextureUserG;
		case ShaderParameter::IT_TEXTURE_USER7:					return &TextureUserH;
		default: ASSERT(false && "cant mapping uniform!");//如果定义了新的属性必须修改这里
		}
		return NULL;
	}

}
