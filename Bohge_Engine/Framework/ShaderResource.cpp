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
#include "MaterialProperty.h"
#include "IPipelineSystem.h"
#include "LightComponent.h"
#include "ShaderResource.h"
#include "MaterialEntity.h"
#include "TextureEntity.h"
#include "ITimeSystem.h"
#include "ICamera.h"
#include "Node.h"



namespace BohgeEngine
{
	uint ShaderResource::s_CurrentBind = -1;
	//-------------------------------------------------------------------------------------------------------
	ShaderResource::ShaderResource(void)
		:m_GPUResourceHandle(-1)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderResource::~ShaderResource(void)
	{
		FOR_EACH( UnifromFunctionList, it, m_UnifromFunctionList )
		{
			SAFE_DELETE( *it )
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResource::ReleaseResource()
	{
		if ( -1 != m_GPUResourceHandle )
		{
			_DoDeleteProgram( m_GPUResourceHandle );
			s_CurrentBind = -1;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	Attributes::ShaderAttributes ShaderResource::_MappingAttributes( const ShaderParameter::InputsType& it )
	{
		switch( it )
		{
		case ShaderParameter::IT_LOCALSPACE_POSITION:			return Attributes::ATTRIBUTE_POSITION;
		case ShaderParameter::IT_LOCALSPACE_COORDNATE:			return Attributes::ATTRIBUTE_TEXCOORD0;
		case ShaderParameter::IT_LOCALSPACE_NORMAL:				return Attributes::ATTRIBUTE_NORMAL;
		case ShaderParameter::IT_LOCALSPACE_COLOR:				return Attributes::ATTRIBUTE_COLOR0;
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
		case ShaderParameter::IT_EMITTER_OFFSET:				return Attributes::ATTRIBUTE_EMITTER_OFFSET;
		case ShaderParameter::IT_EMITTER_LIFE:					return Attributes::ATTRIBUTE_EMITTER_LIFE;
		case ShaderParameter::IT_EMITTER_BORN:					return Attributes::ATTRIBUTE_EMITTER_BORN;
		case ShaderParameter::IT_EMITTER_SPEED:					return Attributes::ATTRIBUTE_EMITTER_SPEED;
		case ShaderParameter::IT_EMITTER_ACCELERATION:			return Attributes::ATTRIBUTE_EMITTER_ACCELERATION;
		case ShaderParameter::IT_EMITTER_SIZE:					return Attributes::ATTRIBUTE_EMITTER_SIZE;
		case ShaderParameter::IT_EMITTER_SIZE_CURVATURE:		return Attributes::ATTRIBUTE_EMITTER_SIZE_CURVATURE;
		case ShaderParameter::IT_EMITTER_COLOR:					return Attributes::ATTRIBUTE_EMITTER_COLOR;
		case ShaderParameter::IT_EMITTER_COLOR_CURVATURE:		return Attributes::ATTRIBUTE_EMITTER_COLOR_CURVATURE;
		case ShaderParameter::IT_EMITTER_SPIN:					return Attributes::ATTRIBUTE_EMITTER_SPIN;
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
	//-------------------------------------------------------------------------------------------------------
	//全部的uniform的设置函数
	void RealBoneArray( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		vector4arrayf* real = (vector4arrayf*)(node.Attribute( NodeProperty::GA_ANIMATION_REAL ));
		shader.SetParameter< vector4arrayf, vector4arrayf >(hunifrom, *real);
	}
	void DualBoneArray( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		vector4arrayf* dual = (vector4arrayf*)(node.Attribute( NodeProperty::GA_ANIMATION_DUAL ));
		shader.SetParameter< vector4arrayf, vector4arrayf >(hunifrom, *dual);
	}
	void WorldTransform( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< Matrix44f, Matrix44f >(hunifrom, *(Matrix44f*)(node.Attribute( NodeProperty::GA_WORLD_TRANSFORM )));
	}
	void WorldRotation( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< Matrix33f, Matrix33f >(hunifrom, *(Matrix33f*)(node.Attribute( NodeProperty::GA_NORMAL_TRANSFORM )));
	}
	void TransViewProj( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		Matrix44f& matrix = *(Matrix44f*)(node.Attribute( NodeProperty::GA_WORLD_TRANSFORM ));
		shader.SetParameter< Matrix44f, Matrix44f >(hunifrom, matrix * IPipelineSystem::Instance()->GetCamera()->GetViewProj() );
	}
	void CameraPosition( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, IPipelineSystem::Instance()->GetCamera()->GetPosition() );
	}
	void CameraRotation( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< Matrix33f, Matrix33f >(hunifrom, IPipelineSystem::Instance()->GetCamera()->GetRotation() );
	}
	void CameraDirection( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, IPipelineSystem::Instance()->GetCamera()->GetForward() );
	}
	void CameraView( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< Matrix44f, Matrix44f >(hunifrom, IPipelineSystem::Instance()->GetCamera()->GetView() );
	}
	void CameraProjection( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< Matrix44f, Matrix44f >(hunifrom, IPipelineSystem::Instance()->GetCamera()->GetProject() );
	}
	void CameraViewProj( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< Matrix44f, Matrix44f >(hunifrom, IPipelineSystem::Instance()->GetCamera()->GetViewProj() );
	}
	void CameraLinerParam( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, IPipelineSystem::Instance()->GetCamera()->LineDepthParam() );
	}
	void LightPosition( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, IPipelineSystem::Instance()->GetCurrentLight()->GetWorldPosition() );
	}
	void LightDirection( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, -IPipelineSystem::Instance()->GetCurrentLight()->GetWorldDirection() );//phong光是像素到光源的方向，所以这里反向
	}
	void LightColor( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, IPipelineSystem::Instance()->GetCurrentLight()->GetColor() );
	}
	void AmbientColor( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, IPipelineSystem::Instance()->GetAmbientLight()->GetColor() );
	}
	void MaterialAmbient( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, mat.GetAmbient() );
	}
	void MaterialDiffuse( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, mat.GetDiffuse() );
	}
	void MaterialSpecular( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, mat.GetSpecular() );
	}
	void MaterialSpecularLevel( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< float, float >(hunifrom, mat.GetSpeLevel() );
	}
	void MaterialShininess( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< float, float >(hunifrom, mat.GetShininess() );
	}
	void MaterialLightAmbient( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, mat.GetAmbient() * IPipelineSystem::Instance()->GetAmbientLight()->GetColor() );
	}
	void MaterialLightLight( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, mat.GetDiffuse() * IPipelineSystem::Instance()->GetCurrentLight()->GetColor() );
	}
	void MaterialLightSpecular( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector3f, vector3f >(hunifrom, ( mat.GetSpecular() * mat.GetSpeLevel() ) * IPipelineSystem::Instance()->GetCurrentLight()->GetColor() );
	}
	void TextureAmbient( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< TextureEntity, uint >(hunifrom, *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_AMBIENT) );
	}
	void TextureDiffuse( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_DIFFUSE);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureSpecular( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_SPECULAR);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureSpecularLevel( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_SPLEVEL);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureGlossiness( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_GLOSSINESS);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureIllumination( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_ILLUMINATION);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureOpacity( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_OPACITY);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureFiter( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_FITER);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureBump( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_BUMP);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureReflection( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_REFLECTION);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureRefraction( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_REFRACTION);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureDisplacement( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_DISPLACEMENT);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureTerrainWeight( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_TERRAIN_WEIGHT);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureTerrainDetal( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_TERRAIN_DETAL);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureTerrainBase( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_TERRAIN_BASE);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureTerrainR( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_TERRAIN_R);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureTerrainG( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_TERRAIN_G);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureTerrainB( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_TERRAIN_B);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureUserA( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_USER0);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureUserB( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_USER1);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureUserC( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_USER2);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureUserD( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_USER3);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureUserE( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_USER4);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureUserF( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_USER5);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureUserG( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_USER6);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void TextureUserH( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const TextureEntity& tex = *mat.GetTextureEntity(MaterialProperty::ATT_TEXTURE_USER7);
		tex.ActiveTexture();
		shader.SetParameter< TextureEntity, uint >(hunifrom, tex );
	}
	void LightAngle( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector2f, vector2f >(hunifrom, IPipelineSystem::Instance()->GetCurrentLight()->GetLightAngle() );
	}
	void LightInnerDiffInv( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector2f, vector2f >(hunifrom, IPipelineSystem::Instance()->GetCurrentLight()->GetLightInnerDiffInv() );
	}
	void LightRangeInv( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< float, float >(hunifrom, 1.0 / IPipelineSystem::Instance()->GetCurrentLight()->GetRange() );
	}
	void LightAtten( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter< vector4f, vector4f >(hunifrom, IPipelineSystem::Instance()->GetCurrentLight()->GetAttenuation() );
	}
	void MaterialUserData0( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const MaterialProperty::UserParameter& upt = mat.GetUserParameters( MaterialProperty::ATT_MATERIAL_USE_0 );
		shader.SetParameter( hunifrom, upt.m_eUserParameterType, upt.m_pObject );
	}
	void MaterialUserData1( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const MaterialProperty::UserParameter& upt = mat.GetUserParameters( MaterialProperty::ATT_MATERIAL_USE_1 );
		shader.SetParameter( hunifrom, upt.m_eUserParameterType, upt.m_pObject );
	}
	void MaterialUserData2( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const MaterialProperty::UserParameter& upt = mat.GetUserParameters( MaterialProperty::ATT_MATERIAL_USE_2 );
		shader.SetParameter( hunifrom, upt.m_eUserParameterType, upt.m_pObject );
	}
	void MaterialUserData3( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const MaterialProperty::UserParameter& upt = mat.GetUserParameters( MaterialProperty::ATT_MATERIAL_USE_3 );
		shader.SetParameter( hunifrom, upt.m_eUserParameterType, upt.m_pObject );
	}
	void MaterialUserData4( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const MaterialProperty::UserParameter& upt = mat.GetUserParameters( MaterialProperty::ATT_MATERIAL_USE_4 );
		shader.SetParameter( hunifrom, upt.m_eUserParameterType, upt.m_pObject );
	}
	void MaterialUserData5( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const MaterialProperty::UserParameter& upt = mat.GetUserParameters( MaterialProperty::ATT_MATERIAL_USE_5 );
		shader.SetParameter( hunifrom, upt.m_eUserParameterType, upt.m_pObject );
	}
	void MaterialUserData6( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const MaterialProperty::UserParameter& upt = mat.GetUserParameters( MaterialProperty::ATT_MATERIAL_USE_6 );
		shader.SetParameter( hunifrom, upt.m_eUserParameterType, upt.m_pObject );
	}
	void MaterialUserData7( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const MaterialProperty::UserParameter& upt = mat.GetUserParameters( MaterialProperty::ATT_MATERIAL_USE_7 );
		shader.SetParameter( hunifrom, upt.m_eUserParameterType, upt.m_pObject );
	}
	void MaterialUserData8( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const MaterialProperty::UserParameter& upt = mat.GetUserParameters( MaterialProperty::ATT_MATERIAL_USE_8 );
		shader.SetParameter( hunifrom, upt.m_eUserParameterType, upt.m_pObject );
	}
	void MaterialUserData9( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		const MaterialProperty::UserParameter& upt = mat.GetUserParameters( MaterialProperty::ATT_MATERIAL_USE_9 );
		shader.SetParameter( hunifrom, upt.m_eUserParameterType, upt.m_pObject );
	}
	void SystemTime( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter<float, float>( hunifrom, ITimeSystem::Instance()->GetTime() );
	}
	void SystemTimeSpan( ShaderResource& shader, handle hunifrom, const MaterialEntity& mat, const Node& node )
	{
		shader.SetParameter<float, float>( hunifrom, ITimeSystem::Instance()->GetFrameSpan() );
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderResource::UnifromFunction ShaderResource::_MappingUniformFunction( const ShaderParameter::InputsType& it )
	{
		switch( it )//每一个参数一个函数，不想用一个runtime的switch，我认为会降低一些效率，以后可以测试，造出最好的办法
		{
		case ShaderParameter::IT_SYSTEM_TIME:					return &SystemTime;
		case ShaderParameter::IT_SYSTEM_TIME_SPAN:				return &SystemTimeSpan;
		case ShaderParameter::IT_LOCALSPACE_REAL_ARRAY:			return &RealBoneArray;
		case ShaderParameter::IT_LOCALSPACE_DUAL_ARRAY:			return &DualBoneArray;
		case ShaderParameter::IT_LOCALWORLD_TRANSFORM:			return &WorldTransform;
		case ShaderParameter::IT_LOCALWORLD_ROTATION:			return &WorldRotation;
		case ShaderParameter::IT_LOCALSCREEN_TRANSVIEWPROJ:		return &TransViewProj;
		case ShaderParameter::IT_CAMERA_WORLDROTATION:			return &CameraRotation;
		case ShaderParameter::IT_CAMERA_WORLDPOSITION:			return &CameraPosition;
		case ShaderParameter::IT_CAMERA_WORLDDIRECTION:			return &CameraDirection;
		case ShaderParameter::IT_CAMERA_VIEW:					return &CameraView;
		case ShaderParameter::IT_CAMERA_PROJECTION:				return &CameraProjection;
		case ShaderParameter::IT_CAMERA_VIEWPROJ:				return &CameraViewProj;
		case ShaderParameter::IT_CAMERA_LINERPARAM:				return &CameraLinerParam;
		case ShaderParameter::IT_LIGHT_POSITION:				return &LightPosition;
		case ShaderParameter::IT_LIGHT_GIVEN_DIRECTION:			return &LightDirection;
		case ShaderParameter::IT_LIGHT_COLOR:					return &LightColor;
		case ShaderParameter::IT_AMBIENT_COLOR:					return &AmbientColor;
		case ShaderParameter::IT_LIGHT_ANGLE:					return &LightAngle;
		case ShaderParameter::IT_LIGHT_INNER_DIFF_INV:			return &LightInnerDiffInv;
		case ShaderParameter::IT_LIGHT_RANGE_INV:				return &LightRangeInv;
		case ShaderParameter::IT_LIGHT_ATTENUATION:				return &LightAtten;
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
		case ShaderParameter::IT_MATERIAL_USER_0:				return &MaterialUserData0;
		case ShaderParameter::IT_MATERIAL_USER_1:				return &MaterialUserData1;
		case ShaderParameter::IT_MATERIAL_USER_2:				return &MaterialUserData2;
		case ShaderParameter::IT_MATERIAL_USER_3:				return &MaterialUserData3;
		case ShaderParameter::IT_MATERIAL_USER_4:				return &MaterialUserData4;
		case ShaderParameter::IT_MATERIAL_USER_5:				return &MaterialUserData5;
		case ShaderParameter::IT_MATERIAL_USER_6:				return &MaterialUserData6;
		case ShaderParameter::IT_MATERIAL_USER_7:				return &MaterialUserData7;
		case ShaderParameter::IT_MATERIAL_USER_8:				return &MaterialUserData8;
		case ShaderParameter::IT_MATERIAL_USER_9:				return &MaterialUserData9;
		default: ASSERT(false && "cant mapping uniform!");//如果定义了新的属性必须修改这里
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	void ShaderResource::CreateProgram( const ShaderParameterInfomationList& attrs, const ShaderParameterInfomationList& unifs, const eastl::string& vscode, const eastl::string& pscode )
	{
		FOR_EACH_CONST( ShaderParameterInfomationList, it, attrs )
		{
			m_Attributes.PushAttribute( it->m_ParamName, _MappingAttributes( it->m_eInputType ) );
		}
		m_GPUResourceHandle = _DoCreateProgram( m_Attributes, vscode, pscode );
		_DoBindProgram( m_GPUResourceHandle );
		FOR_EACH_CONST( ShaderParameterInfomationList, it, unifs )
		{
			ShaderUniform* su = NEW ShaderUniform;
			switch( it->m_eType )
			{
			case ShaderParameter::VT_FLOAT: su->m_hUniform = GetParameterHandle<float>( it->m_ParamName, false, true ); break;
			case ShaderParameter::VT_VEC2: su->m_hUniform = GetParameterHandle<vector2f>( it->m_ParamName, false, true ); break;
			case ShaderParameter::VT_VEC3: su->m_hUniform = GetParameterHandle<vector3f>( it->m_ParamName, false, true ); break;
			case ShaderParameter::VT_VEC4: su->m_hUniform = GetParameterHandle<vector4f>( it->m_ParamName, false, true ); break;
			case ShaderParameter::VT_VEC2_ARRAY: su->m_hUniform = GetParameterHandle<vector2arrayf>( it->m_ParamName, true, false ); break;
			case ShaderParameter::VT_VEC3_ARRAY: su->m_hUniform = GetParameterHandle<vector3arrayf>( it->m_ParamName, true, false ); break;
			case ShaderParameter::VT_VEC4_ARRAY: su->m_hUniform = GetParameterHandle<vector4arrayf>( it->m_ParamName, true, false ); break;
			case ShaderParameter::VT_MAT3: su->m_hUniform = GetParameterHandle<Matrix33f>( it->m_ParamName, false, true ); break;
			case ShaderParameter::VT_MAT4: su->m_hUniform = GetParameterHandle<Matrix44f>( it->m_ParamName, false, true ); break;
			case ShaderParameter::VT_TEXTURE1D:
			case ShaderParameter::VT_TEXTURE2D:
			case ShaderParameter::VT_TEXTURE3D:
			case ShaderParameter::VT_TEXTURECUBE: su->m_hUniform = GetParameterHandle<uint>( it->m_ParamName, true, false ); break;//纹理资源对比哈希值
			}			
			su->m_Function = _MappingUniformFunction( it->m_eInputType );
			m_UnifromFunctionList.push_back( su );
		}
	}
}