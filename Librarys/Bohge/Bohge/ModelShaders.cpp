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
//		模型相关shader		 //
///////////////////////////////
#include "ModelShaders.h"
#include "Device.h"
#include "Engine.h"
#include "Environment.h"
#include "EngineResourceList.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "ShadowCaster.h"





namespace BohgeEngine
{
	//////////////////////////
	//	ModelPrePixelLight	//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	ModelPrePixelLight::ModelPrePixelLight()
		:m_hViewProjMatrixLoc(-1),
		m_hTransform(-1),
		m_hLightDirectionLoc(-1),
		m_hCameraPositionLoc(-1),
		m_hAmbient_AmbientLight(-1),
		m_hSpecular_Light(-1),
		m_hLight(-1),
		m_hShininess(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool ModelPrePixelLight::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rModelPrePixelLight ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hTransform				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
		m_hNormalTransform			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hTextureBase				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );

		m_hAmbient_AmbientLight		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
		m_hSpecular_Light			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
		m_hLight					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Light" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void ModelPrePixelLight::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hTransform, *(Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_WORLDTRANSFORM )) );
		device->SetShaderParameter(m_hNormalTransform, *(Matrix33f*)(m_pMaterial->Attribute( IAttributeNode::ATT_NORMALTRANSFORM )) );
		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection() );//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );

		device->SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );

		device->SetShaderParameter(m_hAmbient_AmbientLight, env->GetAmbient() * m_pMaterial->Ambient() );
		device->SetShaderParameter(m_hSpecular_Light, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * env->GetCurrentLight().GetColor() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
	}
	//--------------------------------------------------------------------------------------



	//////////////////////////
	//		ModelBump		//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	ModelBump::ModelBump()
		:m_hViewProjMatrixLoc(-1),
		m_hTransform(-1),
		m_hLightDirectionLoc(-1),
		m_hCameraPositionLoc(-1),
		m_hAmbient_AmbientLight(-1),
		m_hSpecular_Light(-1),
		m_hLight(-1),
		m_hShininess(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool ModelBump::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );
		m_Attributes.PushAttribute( "tangent",		Attributes::ATTRIBUTE_TANGENT );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rModelBump ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hTransform				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
		m_hNormalTransform			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hTextureBase				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		m_hTextureBump				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BumpTexture" );

		m_hAmbient_AmbientLight		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
		m_hSpecular_Light			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
		m_hLight					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Light" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void ModelBump::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hTransform, *(Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_WORLDTRANSFORM )) );
		device->SetShaderParameter(m_hNormalTransform, *(Matrix33f*)(m_pMaterial->Attribute( IAttributeNode::ATT_NORMALTRANSFORM )) );
		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );
		device->SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		device->SetShaderParameter( m_hTextureBump, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_BUMP)) );

		device->SetShaderParameter(m_hAmbient_AmbientLight, env->GetAmbient() * m_pMaterial->Ambient() );
		device->SetShaderParameter(m_hSpecular_Light, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * env->GetCurrentLight().GetColor() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
	}
	//--------------------------------------------------------------------------------------







	//////////////////////////
	//		ModelDepth		//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	ModelDepth::ModelDepth()
		:m_hWorldViewProjMatrixLoc(-1),
		m_hLightPositionLoc(-1),
		m_hCameraLineDepthParam(-1),
		m_hWorldTransform(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool ModelDepth::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
#ifdef OPENGL
		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rModelDepthHigh ) )
#else
		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rModelDepth ) )
#endif
			return false;
		// Make a record of the location for each shader constant
		m_hWorldViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
		m_hWorldTransform				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldTransform" );
		m_hLightPositionLoc				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "LightPosition" );
		m_hCameraLineDepthParam			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "CameraLineDepthParam" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void ModelDepth::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();

		Matrix44f& mat = *(Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_WORLDTRANSFORM ));
		
		device->SetShaderParameter(m_hWorldTransform, mat );
		if ( m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 ) )
		{
			device->SetShaderParameter(m_hCameraLineDepthParam, camera->LineDepthParamBais() );
			device->SetShaderParameter(m_hWorldViewProjMatrixLoc, mat * camera->GetViewProjBias() );
		}
		else
		{
			device->SetShaderParameter(m_hCameraLineDepthParam, camera->LineDepthParam() );
			device->SetShaderParameter(m_hWorldViewProjMatrixLoc, mat * camera->GetViewProj() );
		}
		device->SetShaderParameter(m_hLightPositionLoc, engine.GetEnvironment()->GetCurrentLight().GetWorldPosition() );
	}
	//--------------------------------------------------------------------------------------





	//////////////////////////
	//		ModelNormal		//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	ModelNormal::ModelNormal()
		:m_hWorldViewProjMatrixLoc(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool ModelNormal::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );
		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rModelNormal ) )
			return false;
		// Make a record of the location for each shader constant
		m_hWorldViewProjMatrixLoc= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProjTransform" );
		m_hNormalTransform			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void ModelNormal::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hWorldViewProjMatrixLoc, *(Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_WORLDTRANSFORM )) * camera->GetViewProj() );
		device->SetShaderParameter(m_hNormalTransform, *(Matrix33f*)(m_pMaterial->Attribute( IAttributeNode::ATT_NORMALTRANSFORM )) );
	}
	//--------------------------------------------------------------------------------------



	//////////////////////////////
	//	ModelPointLightPerpixel	//
	//////////////////////////////
	//--------------------------------------------------------------------------------------
	ModelPointLightPerpixel::ModelPointLightPerpixel()
		:m_hViewProjMatrixLoc(-1),
		m_hTransform(-1),
		m_hLightPositionLoc(-1),
		m_hCameraPositionLoc(-1),
		m_hAmbient_AmbientLight(-1),
		m_hSpecular_Light(-1),
		m_hLight(-1),
		m_hShininess(-1),
		m_hLightRangeInv(-1),
		m_hAttenuation(-1),
		m_hCameraLineDepthParam(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool ModelPointLightPerpixel::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rModelPointLightPerpixel ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hTransform				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
		m_hNormalTransform			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
		m_hLightPositionLoc			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightPos" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hLightRangeInv			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "fLightRangeInv" );
		m_hAttenuation				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAttenuation" );
		m_hCameraLineDepthParam		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "CameraLineDepthParam" );

		m_hTextureBase				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		m_hTextureDepth				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "TextureDepth" );
		m_hAmbient_AmbientLight		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
		m_hSpecular_Light			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
		m_hLight					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Light" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void ModelPointLightPerpixel::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hTransform, *(Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_WORLDTRANSFORM )) );
		device->SetShaderParameter(m_hNormalTransform, *(Matrix33f*)(m_pMaterial->Attribute( IAttributeNode::ATT_NORMALTRANSFORM )) );
		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hLightPositionLoc, env->GetCurrentLight().GetWorldPosition() );
		device->SetShaderParameter(m_hAttenuation, env->GetCurrentLight().GetAttenuation() );
		device->SetShaderParameter(m_hLightRangeInv, 1.0f / env->GetCurrentLight().GetRange() );
		device->SetShaderParameter(m_hCameraLineDepthParam, env->GetCurrentLight().GetCaster().GetFirstCamera().LineDepthParam() );		
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );

		device->SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		device->SetShaderParameter( m_hTextureDepth, env->GetCurrentLight().GetCaster().GetFirstDepthTexture() );

		device->SetShaderParameter(m_hAmbient_AmbientLight, env->GetAmbient() * m_pMaterial->Ambient() );
		device->SetShaderParameter(m_hSpecular_Light, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * env->GetCurrentLight().GetColor() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
	}
	//--------------------------------------------------------------------------------------




	//////////////////////////////
	//		ModelLightMaped		//
	//////////////////////////////
	//--------------------------------------------------------------------------------------
	ModelLightMaped::ModelLightMaped()
		:m_hWorldViewProj(-1),
		m_hBaseTexture(-1),
		m_hLightMapedTexture(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool ModelLightMaped::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
		//m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rModelLightMaped ) )
			return false;
		// Make a record of the location for each shader constant
		m_hWorldViewProj		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );

		m_hBaseTexture				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		m_hLightMapedTexture		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "LightmapTexture" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void ModelLightMaped::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hWorldViewProj, *(Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_WORLDTRANSFORM )) * camera->GetViewProj() );

		device->SetShaderParameter( m_hBaseTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		device->SetShaderParameter( m_hLightMapedTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_LIGHT_MAP)) );

	}
	//--------------------------------------------------------------------------------------



	//////////////////////////////
	//	ModelSpotLightPerpixel	//
	//////////////////////////////
	//--------------------------------------------------------------------------------------
	ModelSpotLightPerpixel::ModelSpotLightPerpixel()
		:m_hViewProjMatrixLoc(-1),
		m_hTransform(-1),
		m_hLightDirectionLoc(-1),
		m_hCameraPositionLoc(-1),
		m_hAmbient_AmbientLight(-1),
		m_hSpecular_Light(-1),
		m_hLight(-1),
		m_hShininess(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool ModelSpotLightPerpixel::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rModelSpotLightPerpixel ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hTransform				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
		m_hNormalTransform			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDirection" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hLightRangeInv			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "fLightRangeInv" );
		m_hInner_DiffInv			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vInner_DiffInv" );
		m_hAttenuation				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAttenuation" );
		m_hLightPosition			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightPos" );
		m_hshadowMatrixBasi			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matShadowWorldViewProjBais" );
		m_hCameraLineDepthParam		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "CameraLineDepthParam" );

		m_hTextureBase				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		m_hTextureDepth				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "TextureDepth" );
		m_hAmbient_AmbientLight		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
		m_hSpecular_Light			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
		m_hLight					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Light" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void ModelSpotLightPerpixel::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hTransform, *(Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_WORLDTRANSFORM )) );
		device->SetShaderParameter(m_hNormalTransform, *(Matrix33f*)(m_pMaterial->Attribute( IAttributeNode::ATT_NORMALTRANSFORM )) );
		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hLightDirectionLoc, env->GetCurrentLight().GetWorldDirection() );
		device->SetShaderParameter(m_hLightPosition, env->GetCurrentLight().GetWorldPosition() );
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );

		device->SetShaderParameter(m_hLightRangeInv, 1.0f / env->GetCurrentLight().GetRange() );
		device->SetShaderParameter(m_hInner_DiffInv, env->GetCurrentLight().GetLightInnerDiffInv() );
		device->SetShaderParameter(m_hAttenuation, env->GetCurrentLight().GetAttenuation() );

		Matrix44f Bais( 0.5, 0.0, 0.0, 0.0,
						0.0, 0.5, 0.0, 0.0,
						0.0, 0.0, 0.5, 0.0,
						0.5, 0.5, 0.5, 1.0 );
		Matrix44f PBais = env->GetCurrentLight().GetCaster().GetFirstCamera().GetViewProj() * Bais;
		device->SetShaderParameter(m_hshadowMatrixBasi, PBais );
		device->SetShaderParameter(m_hCameraLineDepthParam, env->GetCurrentLight().GetCaster().GetFirstCamera().LineDepthParam() );

		device->SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		device->SetShaderParameter( m_hTextureDepth, env->GetCurrentLight().GetCaster().GetFirstDepthTexture() );

		device->SetShaderParameter(m_hAmbient_AmbientLight, env->GetAmbient() * m_pMaterial->Ambient() );
		device->SetShaderParameter(m_hSpecular_Light, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * env->GetCurrentLight().GetColor() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
	}
	//--------------------------------------------------------------------------------------




}