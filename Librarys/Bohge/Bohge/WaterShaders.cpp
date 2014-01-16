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
//		水面相关shader		 //
///////////////////////////////
#include "WaterShaders.h"
#include "Device.h"
#include "Engine.h"
#include "Environment.h"
#include "Camera.h"
#include "Material.h"
#include "EngineResourceList.h"
#include "SkyBox.h"
#include "Light.h"
#include "ShadowCaster.h"


namespace BohgeEngine
{
	//////////////////////////
	//		WaterShader		//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	WaterShader::WaterShader()
		:m_hViewProjMatrixLoc(-1),
		m_hLightDirectionLoc(-1),
		m_hCameraPositionLoc(-1),
		m_hSpecular_Light(-1),
		m_hShininess(-1),
		m_hSkyBox(-1),
		m_hTime(-1),
		//m_hFordWaterColor(-1),
		m_hDepth(-1),
		m_hNearQ_Q(-1),
		m_hCanSeeDepth(-1),
		m_hNoise(-1),
		m_hNoiseWaveSpeed(-1),
		m_hBumpScale(-1),
		m_hBumpReduic(-1),
		m_hBumpStrength(-1),
		m_hNoiseColor(-1),
		m_hNoiseStrength(-1),
		m_hReflrefr_offset(-1),
		//m_hDepthWaterColor(-1),
		m_hNoiseTile(-1),
		m_hColorChangeDepthInv(-1),
		m_hWaterColorTex(-1),
		m_hShadowMap(-1),
		m_hBump(-1),
		m_hFogColor(-1),
		m_hFogParam(-1),
		m_ShadowWorldViewProjBaisLoc(-1),
		m_hLightPositionLoc(-1),
		m_hCameraLineDepthParam(-1)
		//,m_hNormal(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool WaterShader::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rWaterGrid );
	}
	//--------------------------------------------------------------------------------------
	bool WaterShader::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		//m_Attributes.PushAttribute( "texture",		Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if ( !_CreateShader() )
		{
			return false;
		}

		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hTime						= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Time" );
		m_hSkyBox					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "SkyBox" );
		//m_hNormal					= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NormalMap" );
		m_hDepth					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "WorldDepth" );
		m_hNearQ_Q					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NeraQ_Q" );
		m_hScene					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "SceneTex" );
		m_hWaterColorTex			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "WaterColorTex" );
		//m_hFordWaterColor			= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vFordWaterColor" );
		//m_hDepthWaterColor			= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vDepthWaterColor" );
		m_hSpecular_Light			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		m_hCanSeeDepth				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "CanSeeDepth" );
		m_hNoise					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "WaterNoise" );
		m_hNoiseColor				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NoiseColor" );
		m_hNoiseTile				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Tile" );
		m_hNoiseStrength			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NoiseStrength" );
		m_hReflrefr_offset			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "reflrefr_offset" );
		m_hColorChangeDepthInv		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "ColorChangeDepthInv" );
		m_hShadowMap				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "ShadowMap" );
		m_ShadowWorldViewProjBaisLoc= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matShadowWorldViewProjBais" );
		m_hLightPositionLoc			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "LightPosition" );
		m_hCameraLineDepthParam		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "CameraLineDepthParam" );
		m_hFogColor					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
		m_hFogParam					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
		m_hNoiseWaveSpeed			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "WaveSpeed" );
		m_hBump						= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Bumpmap" );
		m_hBumpScale				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_BumpScale" );
		m_hBumpReduic				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_BumpReduic" );
		m_hBumpStrength				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_BumpStrength" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void WaterShader::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hTime, static_cast<float>(engine.Time())/1000.0f );
		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );

		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_OPACITY) )
		{
			device->SetShaderParameter( m_hNoise, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_OPACITY)) );
		}
		//else
		//{
		//	device->SetShaderParameter( m_hNoise, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );//Edit的时候防止崩溃的
		//}


		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
		{
			device->SetShaderParameter( m_hWaterColorTex, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		}
		if( NULL != engine.GetSceneManage()->GetSkyBox().Texture() )
		{
			device->SetShaderParameter( m_hSkyBox, *(Texture*)engine.GetSceneManage()->GetSkyBox().Texture() );
		}
		if ( NULL != m_pMaterial->Attribute(Material::ATT_TEX_BUMP) )
		{
			device->SetShaderParameter( m_hBump, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_BUMP)) );
		}
		device->SetShaderParameter( m_hDepth, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );
		device->SetShaderParameter( m_hScene, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_REFRACTION)) );
		device->SetShaderParameter( m_hNoiseColor, m_pMaterial->Diffuse() * ( env->GetCurrentLight().GetColor() + env->GetAmbient() ) );

		//device->SetShaderParameter(m_hFordWaterColor, m_pMaterial->Diffuse() * ( env->GetColor() + env->GetAmbient() ) );//海水的颜色还是和光的颜色有关的
		//device->SetShaderParameter(m_hDepthWaterColor, m_pMaterial->Ambient() * ( env->GetColor() + env->GetAmbient() ) );//海水两个颜色，浅滩一个，深得地方一个

		device->SetShaderParameter(m_hSpecular_Light, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
		device->SetShaderParameter(m_hNearQ_Q, engine.GetCamera()->GetNearQ_Q() );
		device->SetShaderParameter(m_hReflrefr_offset, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 )) );
		device->SetShaderParameter(m_hCanSeeDepth, 1.0f/ *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 )) );//100是能看的深度
		device->SetShaderParameter(m_hNoiseTile, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 )) );
		device->SetShaderParameter(m_hColorChangeDepthInv, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_3 )) );
		device->SetShaderParameter(m_hNoiseStrength, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_4 )) );
		device->SetShaderParameter(m_hNoiseWaveSpeed, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_5 )) );


		device->SetShaderParameter(m_hBumpScale, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_6 )) );
		//float farinv = *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_7 ));
		//farinv = 1.0 / ( farinv * camera->GetFar() );
		//device->SetShaderParameter(m_hBumpFarInv, farinv );
		device->SetShaderParameter(m_hBumpStrength, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_8 )) );
		device->SetShaderParameter(m_hFogColor, env->GetFogColor() );
		device->SetShaderParameter(m_hFogParam, env->GetFogParam() );

		//阴影相关
		device->SetShaderParameter( m_hShadowMap, env->GetCurrentLight().GetCaster().GetDepthTexture(0) );
		Matrix44f Bais( 0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.5, 0.5, 0.0, 1.0 );
		Matrix44f mvpBais = env->GetCurrentLight().GetCaster().GetCamera(0).GetViewProj() * Bais;
		device->SetShaderParameter(m_ShadowWorldViewProjBaisLoc, mvpBais );
		device->SetShaderParameter(m_hLightPositionLoc, engine.GetEnvironment()->GetCurrentLight().GetWorldPosition() );
		device->SetShaderParameter(m_hCameraLineDepthParam, env->GetCurrentLight().GetCaster().GetCamera(0).LineDepthParam() );
	}
	//--------------------------------------------------------------------------------------





	//////////////////////////
	//	WaterShaderNoShadow	//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	WaterShaderNoShadow::WaterShaderNoShadow()
		:m_hViewProjMatrixLoc(-1),
		m_hLightDirectionLoc(-1),
		m_hCameraPositionLoc(-1),
		m_hSpecular_Light(-1),
		m_hShininess(-1),
		m_hSkyBox(-1),
		m_hTime(-1),
		//m_hFordWaterColor(-1),
		m_hDepth(-1),
		m_hNearQ_Q(-1),
		m_hCanSeeDepth(-1),
		m_hNoise(-1),
		m_hNoiseWaveSpeed(-1),
		m_hNoiseColor(-1),
		m_hBumpScale(-1),
		m_hBumpReduic(-1),
		m_hBumpStrength(-1),
		m_hNoiseStrength(-1),
		m_hBump(-1),
		m_hReflrefr_offset(-1),
		//m_hDepthWaterColor(-1),
		m_hNoiseTile(-1),
		m_hColorChangeDepthInv(-1),
		m_hWaterColorTex(-1),
		m_hFogColor(-1),
		m_hFogParam(-1)
		//,m_hNormal(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool WaterShaderNoShadow::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		//m_Attributes.PushAttribute( "texture",		Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if ( !_CreateShader() )
		{
			return false;
		}


		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hTime						= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Time" );
		m_hSkyBox					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "SkyBox" );
		//m_hNormal					= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NormalMap" );
		m_hDepth					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "WorldDepth" );
		m_hNearQ_Q					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NeraQ_Q" );
		m_hScene					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "SceneTex" );
		m_hWaterColorTex			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "WaterColorTex" );
		//m_hFordWaterColor			= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vFordWaterColor" );
		//m_hDepthWaterColor			= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vDepthWaterColor" );
		m_hSpecular_Light			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		m_hCanSeeDepth				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "CanSeeDepth" );
		m_hNoise					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "WaterNoise" );
		m_hNoiseColor				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NoiseColor" );
		m_hNoiseTile				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Tile" );
		m_hNoiseStrength			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NoiseStrength" );
		m_hReflrefr_offset			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "reflrefr_offset" );
		m_hColorChangeDepthInv		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "ColorChangeDepthInv" );
		m_hFogColor					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
		m_hFogParam					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
		m_hNoiseWaveSpeed			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "WaveSpeed" );
		m_hBump						= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Bumpmap" );
		m_hBumpScale				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_BumpScale" );
		m_hBumpReduic				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_BumpReduic" );
		m_hBumpStrength				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_BumpStrength" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void WaterShaderNoShadow::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hTime, static_cast<float>(engine.Time())/1000.0f );
		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );

		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_OPACITY) )
		{
			device->SetShaderParameter( m_hNoise, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_OPACITY)) );
		}
		//else
		//{
		//	device->SetShaderParameter( m_hNoise, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );//Edit的时候防止崩溃的
		//}


		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
		{
			device->SetShaderParameter( m_hWaterColorTex, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		}
		if( NULL != engine.GetSceneManage()->GetSkyBox().Texture() )
		{
			device->SetShaderParameter( m_hSkyBox, *(Texture*)engine.GetSceneManage()->GetSkyBox().Texture() );
		}
		if ( NULL != m_pMaterial->Attribute(Material::ATT_TEX_BUMP) )
		{
			device->SetShaderParameter( m_hBump, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_BUMP)) );
		}
		device->SetShaderParameter( m_hDepth, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );
		device->SetShaderParameter( m_hScene, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_REFRACTION)) );
		device->SetShaderParameter( m_hNoiseColor, m_pMaterial->Diffuse() * ( env->GetCurrentLight().GetColor() + env->GetAmbient() ) );

		//device->SetShaderParameter(m_hFordWaterColor, m_pMaterial->Diffuse() * ( env->GetColor() + env->GetAmbient() ) );//海水的颜色还是和光的颜色有关的
		//device->SetShaderParameter(m_hDepthWaterColor, m_pMaterial->Ambient() * ( env->GetColor() + env->GetAmbient() ) );//海水两个颜色，浅滩一个，深得地方一个

		device->SetShaderParameter(m_hSpecular_Light, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
		device->SetShaderParameter(m_hNearQ_Q, engine.GetCamera()->GetNearQ_Q() );
		device->SetShaderParameter(m_hReflrefr_offset, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 )) );
		device->SetShaderParameter(m_hCanSeeDepth, 1.0f/ *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 )) );//100是能看的深度
		device->SetShaderParameter(m_hNoiseTile, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 )) );
		device->SetShaderParameter(m_hColorChangeDepthInv, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_3 )) );
		device->SetShaderParameter(m_hNoiseStrength, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_4 )) );
		device->SetShaderParameter(m_hNoiseWaveSpeed, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_5 )) );

		//bump
		device->SetShaderParameter(m_hBumpScale, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_6 )) );
		//float farinv = *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_7 ));
		//farinv = 1.0 / ( farinv * camera->GetFar() );
		//device->SetShaderParameter(m_hBumpFarInv, farinv );
		device->SetShaderParameter(m_hBumpStrength, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_8 )) );

		device->SetShaderParameter(m_hFogColor, env->GetFogColor() );
		device->SetShaderParameter(m_hFogParam, env->GetFogParam() );
	}
	//--------------------------------------------------------------------------------------
	bool WaterShaderNoShadow::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rWaterGridNoShadow );
	}
	//--------------------------------------------------------------------------------------





	////////////////////////////
	////HighmapGenerateShader //
	////////////////////////////
	////--------------------------------------------------------------------------------------
	//HighmapGenerateShader::HighmapGenerateShader()
	//:m_hScale(-1),
	//m_hNoise1(-1),
	//m_hNoise2(-1)
	//{
	//}
	////--------------------------------------------------------------------------------------
	//bool HighmapGenerateShader::Initialization()
	//{
	//	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
	//	m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
	//
	//	if( false == engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rHighmapGenerateShader ) )
	//		return false;
	//	// Make a record of the location for each shader constant
	//	m_hScale		= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "fScale" );
	//	m_hNoise1		= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Noise1" );
	//	m_hNoise2		= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Noise2" );
	//
	//	return true;
	//}
	////--------------------------------------------------------------------------------------
	//void HighmapGenerateShader::SetParameters ()
	//{
	//	engine& engine = engine::Instance();
	//	device->SetShaderParameter(m_hScale, *((float*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_0)) );
	//	device->SetShaderParameter( m_hNoise1, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );
	//	device->SetShaderParameter( m_hNoise2, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER1)) );
	//}
	////--------------------------------------------------------------------------------------
	//
	//
	////////////////////////////
	////NormalmapGenerateShader//
	////////////////////////////
	////--------------------------------------------------------------------------------------
	//NormalmapGenerateShader::NormalmapGenerateShader()
	//:m_hStep(-1),
	//m_hcorner00(-1),
	//m_hcorner01(-1),
	//m_hcorner10(-1),
	//m_hcorner11(-1),
	//m_hAmplitude(-1),
	//m_hHeightmap(-1)
	//{
	//}
	////--------------------------------------------------------------------------------------
	//bool NormalmapGenerateShader::Initialization()
	//{
	//	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
	//	m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
	//
	//	if( false == engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rHNormalmapGenerateShader ) )
	//		return false;
	//	// Make a record of the location for each shader constant
	//	m_hStep			= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "fStep" );
	//	m_hcorner00		= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "corner00" );
	//	m_hcorner01		= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "corner01" );
	//	m_hcorner10		= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "corner10" );
	//	m_hcorner11		= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "corner11" );
	//	m_hAmplitude	= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "amplitude" );
	//	m_hHeightmap	= engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "hsampler" );
	//
	//	return true;
	//}
	////--------------------------------------------------------------------------------------
	//void NormalmapGenerateShader::SetParameters ()
	//{
	//	engine& engine = engine::Instance();
	//	device->SetShaderParameter(m_hStep, *((float*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_1)) );
	//	device->SetShaderParameter(m_hAmplitude, *((float*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_2)) );
	//	device->SetShaderParameter(m_hcorner00, *((vector4f*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_3)) );
	//	device->SetShaderParameter(m_hcorner01, *((vector4f*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_4)) );
	//	device->SetShaderParameter(m_hcorner10, *((vector4f*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_5)) );
	//	device->SetShaderParameter(m_hcorner11, *((vector4f*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_6)) );
	//	device->SetShaderParameter( m_hHeightmap, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );
	//}
	////--------------------------------------------------------------------------------------



	//////////////////////////
	//	WaterShaderNoShadow	//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	WaterShaderNormal::WaterShaderNormal()
		:m_hViewProjMatrixLoc(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool WaterShaderNormal::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rWaterNormal ) )
			return false;
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void WaterShaderNormal::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();
		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
	}
	//--------------------------------------------------------------------------------------



	//--------------------------------------------------------------------------------------
	bool WaterShaderMedium::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rWaterGridMedium );
	}

	//--------------------------------------------------------------------------------------
	bool WaterShaderNoShadowMedium::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rWaterGridMediumNoShadow );
	}


}