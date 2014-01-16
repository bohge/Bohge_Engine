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



///////////////////////////////
//		地形相关shader		 //
///////////////////////////////
#include "TerrainShaders.h"
#include "Device.h"
#include "Engine.h"
#include "EngineResourceList.h"
#include "Camera.h"
#include "Material.h"
#include "Environment.h"
#include "Light.h"
#include "ShadowCaster.h"



namespace BohgeEngine
{

	//////////////////////////
	//		TerrainCDLOD	//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	TerrainCDLOD::TerrainCDLOD()
		:m_hViewProjMatrixLoc(-1),
		m_hLightDirectionLoc(-1),
		m_hCameraPositionLoc(-1),
		m_hAmbient_AmbientLight(-1),
		m_hSpecular_Light(-1),
		m_hLight(-1),
		m_hShininess(-1),
		m_hTextureBase(-1),
		m_hDetalTexture(-1),
		m_hMorphXY_SteperScaleInvers_DoubleChunkSize(-1),
		m_hSizeScale_SizeScaleRepeat(-1),
		m_hDetailIndex(-1),
		m_hOffsetScale(-1),
		m_hDetalDistanceInv(-1),
		m_hTextureWeight(-1),
		m_hRTexture(-1),
		m_hGTexture(-1),
		m_hBTexture(-1),
		m_hTextureDiffuse(-1),
		m_hShadowMap(-1),
		m_ShadowWorldViewProjBaisLoc(-1),
		m_hFogColor(-1),
		m_hFogParam(-1),
		m_hLightPositionLoc(-1),
		m_hCameraLineDepthParam(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool TerrainCDLOD::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "diffHeight",	Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rTerrainCDLOD ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hTextureDiffuse			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		//m_hTextureBase				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		//m_hTextureDiffuse			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "DiffuseTexture" );
		//m_hDetalTexture				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "DetalTexture" );
		m_hTextureWeight			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "WeightTexture" );
		//m_hRTexture					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "RTexture" );
		//m_hGTexture					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "GTexture" );
		//m_hBTexture					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BTexture" );
		m_hAmbient_AmbientLight		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
		m_hSpecular_Light			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
		m_hLight					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Light" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		m_hDetailIndex				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "dIndex" );
		m_hOffsetScale				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vOffsetScale" );
		m_hDetalDistanceInv			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "DetailDistanceInve_Tile" );
		m_hSizeScale_SizeScaleRepeat= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vSizeScaleInv_SizeScaleRepeatInv" );
		m_hMorphXY_SteperScaleInvers_DoubleChunkSize	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vM_S_C" );
		m_hShadowMap				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "ShadowMap" );
		m_hFogColor					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
		m_hFogParam					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
		m_ShadowWorldViewProjBaisLoc= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matShadowWorldViewProjBais" );
		m_hLightPositionLoc				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "LightPosition" );
		m_hCameraLineDepthParam			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "CameraLineDepthParam" );

		return true;
	}
	//--------------------------------------------------------------------------------------
	void TerrainCDLOD::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );

		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
		{
			device->SetShaderParameter( m_hTextureDiffuse, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		}
		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_WEIGHT) )
		{
			device->SetShaderParameter( m_hTextureWeight, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_WEIGHT)) );
		}
		//if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_DETAL) )
		//{
		//	device->SetShaderParameter( m_hDetalTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_DETAL)) );
		//}
		//if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_BASE) )
		//{
		//	device->SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_BASE)) );
		//}
		//if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_R) )
		//{
		//	device->SetShaderParameter( m_hRTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_R)) );
		//}
		//if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_G) )
		//{
		//	device->SetShaderParameter( m_hGTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_G)) );
		//}
		//if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_B) )
		//{
		//	device->SetShaderParameter( m_hBTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_B)) );
		//}

		device->SetShaderParameter(m_hAmbient_AmbientLight, env->GetAmbient() * m_pMaterial->Ambient() );
		device->SetShaderParameter(m_hSpecular_Light, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * env->GetCurrentLight().GetColor() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
		device->SetShaderParameter(m_hSizeScale_SizeScaleRepeat, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 ) ) );
		device->SetShaderParameter(m_hMorphXY_SteperScaleInvers_DoubleChunkSize, *((vector4f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 ) ) );
		device->SetShaderParameter(m_hDetailIndex, *((int*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 ) ) );
		device->SetShaderParameter(m_hOffsetScale, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_3 ) ) );
		device->SetShaderParameter(m_hDetalDistanceInv, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_4 ) ) );
		device->SetShaderParameter(m_hFogColor, env->GetFogColor() );
		device->SetShaderParameter(m_hFogParam, env->GetFogParam() );

		//阴影相关
		device->SetShaderParameter( m_hShadowMap, env->GetCurrentLight().GetCaster().GetDepthTexture( 0 ) );
		Matrix44f Bais( 0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.5, 0.5, 0.0, 1.0 );
		Matrix44f PBais = env->GetCurrentLight().GetCaster().GetCamera(0).GetViewProj() * Bais;
		device->SetShaderParameter(m_ShadowWorldViewProjBaisLoc, PBais );
		device->SetShaderParameter(m_hLightPositionLoc, engine.GetEnvironment()->GetCurrentLight().GetWorldPosition() );
		device->SetShaderParameter(m_hCameraLineDepthParam, env->GetCurrentLight().GetCaster().GetCamera(0).LineDepthParam() );
	}
	//--------------------------------------------------------------------------------------








	//////////////////////////
	//	TerrainCDLODNoShadow//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	TerrainCDLODNoShadow::TerrainCDLODNoShadow()
		:m_hViewProjMatrixLoc(-1),
		m_hLightDirectionLoc(-1),
		m_hCameraPositionLoc(-1),
		m_hAmbient_AmbientLight(-1),
		m_hSpecular_Light(-1),
		m_hLight(-1),
		m_hShininess(-1),
		m_hTextureBase(-1),
		m_hDetalTexture(-1),
		m_hMorphXY_SteperScaleInvers_DoubleChunkSize(-1),
		m_hSizeScale_SizeScaleRepeat(-1),
		m_hDetailIndex(-1),
		m_hOffsetScale(-1),
		m_hDetalDistanceInv(-1),
		m_hTextureWeight(-1),
		m_hRTexture(-1),
		m_hGTexture(-1),
		m_hBTexture(-1),
		m_hTextureDiffuse(-1),
		m_hFogColor(-1),
		m_hFogParam(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool TerrainCDLODNoShadow::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "diffHeight",	Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rTerrainCDLODNoShadow ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hTextureDiffuse			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		//m_hTextureBase				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		//m_hTextureDiffuse			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "DiffuseTexture" );
		//m_hDetalTexture				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "DetalTexture" );
		m_hTextureWeight			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "WeightTexture" );
		//m_hRTexture					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "RTexture" );
		//m_hGTexture					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "GTexture" );
		//m_hBTexture					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BTexture" );
		m_hAmbient_AmbientLight		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
		m_hSpecular_Light			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
		m_hLight					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Light" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		m_hDetailIndex				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "dIndex" );
		m_hOffsetScale				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vOffsetScale" );
		m_hDetalDistanceInv			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "DetailDistanceInve_Tile" );
		m_hSizeScale_SizeScaleRepeat= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vSizeScaleInv_SizeScaleRepeatInv" );
		m_hMorphXY_SteperScaleInvers_DoubleChunkSize	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vM_S_C" );
		m_hFogColor					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
		m_hFogParam					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void TerrainCDLODNoShadow::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );

		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
		{
			device->SetShaderParameter( m_hTextureDiffuse, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		}
		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_WEIGHT) )
		{
			device->SetShaderParameter( m_hTextureWeight, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_WEIGHT)) );
		}
		//if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_DETAL) )
		//{
		//	device->SetShaderParameter( m_hDetalTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_DETAL)) );
		//}
		//if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_BASE) )
		//{
		//	device->SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_BASE)) );
		//}
		//if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_R) )
		//{
		//	device->SetShaderParameter( m_hRTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_R)) );
		//}
		//if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_G) )
		//{
		//	device->SetShaderParameter( m_hGTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_G)) );
		//}
		//if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_B) )
		//{
		//	device->SetShaderParameter( m_hBTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_B)) );
		//}

		device->SetShaderParameter(m_hAmbient_AmbientLight, env->GetAmbient() * m_pMaterial->Ambient() );
		device->SetShaderParameter(m_hSpecular_Light, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * env->GetCurrentLight().GetColor() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
		device->SetShaderParameter(m_hSizeScale_SizeScaleRepeat, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 ) ) );
		device->SetShaderParameter(m_hMorphXY_SteperScaleInvers_DoubleChunkSize, *((vector4f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 ) ) );
		device->SetShaderParameter(m_hDetailIndex, *((int*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 ) ) );
		device->SetShaderParameter(m_hOffsetScale, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_3 ) ) );
		device->SetShaderParameter(m_hDetalDistanceInv, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_4 ) ) );
		device->SetShaderParameter(m_hFogColor, env->GetFogColor() );
		device->SetShaderParameter(m_hFogParam, env->GetFogParam() );
	}
	//--------------------------------------------------------------------------------------








	//////////////////////////
	//	TerrainCDLODLow		//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	TerrainCDLODLow::TerrainCDLODLow()
		:m_hViewProjMatrixLoc(-1),
		m_hTextureDiffuse(-1),
		m_hMorphXY_SteperScaleInvers_DoubleChunkSize(-1),
		m_hSizeScale_SizeScaleRepeat(-1),
		m_hDetailIndex(-1),
		m_hOffsetScale(-1),
		m_hCameraPositionLoc(-1),
		m_hFogColor(-1),
		m_hFogParam(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool TerrainCDLODLow::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "diffHeight",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rTerrainCDLODLow ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hTextureDiffuse			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		m_hDetailIndex				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "dIndex" );
		m_hOffsetScale				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vOffsetScale" );
		m_hSizeScale_SizeScaleRepeat= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vSizeScaleInv_SizeScaleRepeatInv" );
		m_hMorphXY_SteperScaleInvers_DoubleChunkSize	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vM_S_C" );
		m_hFogColor					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
		m_hFogParam					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void TerrainCDLODLow::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );

		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
		{
			device->SetShaderParameter( m_hTextureDiffuse, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		}
		device->SetShaderParameter(m_hSizeScale_SizeScaleRepeat, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 ) ) );
		device->SetShaderParameter(m_hMorphXY_SteperScaleInvers_DoubleChunkSize, *((vector4f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 ) ) );
		device->SetShaderParameter(m_hDetailIndex, *((int*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 ) ) );
		device->SetShaderParameter(m_hOffsetScale, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_3 ) ) );

		device->SetShaderParameter(m_hFogColor, env->GetFogColor() );
		device->SetShaderParameter(m_hFogParam, env->GetFogParam() );
	}
	//--------------------------------------------------------------------------------------



	//////////////////////////
	//	TerrainCDLODNormal	//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	TerrainCDLODNormal::TerrainCDLODNormal()
		:m_hViewProjMatrixLoc(-1),
		m_hCameraPositionLoc(-1),
		m_hMorphXY_SteperScaleInvers_DoubleChunkSize(-1),
		m_hDetailIndex(-1),
		m_hOffsetScale(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool TerrainCDLODNormal::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "diffHeight",	Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rTerrainCDLODNormal ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hDetailIndex				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "dIndex" );
		m_hOffsetScale				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vOffsetScale" );
		m_hMorphXY_SteperScaleInvers_DoubleChunkSize	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vM_S_C" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void TerrainCDLODNormal::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );
		device->SetShaderParameter(m_hMorphXY_SteperScaleInvers_DoubleChunkSize, *((vector4f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 ) ) );
		device->SetShaderParameter(m_hDetailIndex, *((int*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 ) ) );
		device->SetShaderParameter(m_hOffsetScale, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_3 ) ) );
	}
	//--------------------------------------------------------------------------------------

}