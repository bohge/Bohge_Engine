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



/////////////////////////////////////
//		植被相关的shader		   //
////////////////////////////////////
#include "PlantShaders.h"
#include "Device.h"
#include "Engine.h"
#include "EngineResourceList.h"
#include "Camera.h"
#include "Material.h"
#include "Environment.h"
#include "Light.h"



namespace BohgeEngine
{
	//////////////////////////
	//		TreeTrunk		//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	PlantTrunk::PlantTrunk()
		:m_hViewProjMatrixLoc(-1),
		m_hLightColor(-1),
		m_hLightDirectionLoc(-1),
		m_hCameraPositionLoc(-1),
		m_hAmbient_AmbientLight(-1),
		m_hTextureBase(-1),
		m_hSpecularLight(-1),
		m_hFogColor(-1),
		m_hFogParam(-1),
		m_hShininess(-1),
		m_hFadeAlpha(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool PlantTrunk::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPlantTrunkShader ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCameraPos" );
		m_hTextureBase				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		m_hAmbient_AmbientLight		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAmbient" );
		m_hLightColor				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightColor" );
		m_hSpecularLight			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vSpecularLight" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		m_hFogColor					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
		m_hFogParam					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
		m_hFadeAlpha				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FadeAlpha" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void PlantTrunk::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj() );
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );
		device->SetShaderParameter(m_hAmbient_AmbientLight, env->GetAmbient() * m_pMaterial->Ambient() );
		device->SetShaderParameter(m_hLightColor, m_pMaterial->Diffuse() * env->GetCurrentLight().GetColor() );
		device->SetShaderParameter(m_hSpecularLight, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
		device->SetShaderParameter(m_hFogColor, env->GetFogColor() );
		device->SetShaderParameter(m_hFogParam, env->GetFogParam() );
		device->SetShaderParameter(m_hFadeAlpha, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 )) );

		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
		{
			device->SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		}
	}
	//--------------------------------------------------------------------------------------






	//////////////////////////
	//		TreeLeaf		//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	PlantLeaf::PlantLeaf()
		:m_hViewProjMatrixLoc(-1),
		m_hLightColor(-1),
		m_hLightDirectionLoc(-1),
		m_hCameraPositionLoc(-1),
		m_hAmbient_AmbientLight(-1),
		m_hTextureBase(-1),
		m_hSpecularLight(-1),
		m_hFogColor(-1),
		m_hFogParam(-1),
		m_hShininess(-1),
		m_hFadeAlpha(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool PlantLeaf::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPlantLeafShader ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCameraPos" );
		m_hTextureBase				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		m_hAmbient_AmbientLight		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAmbient" );
		m_hLightColor				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightColor" );
		m_hSpecularLight			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vSpecularLight" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		m_hFogColor					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
		m_hFogParam					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
		m_hFadeAlpha				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FadeAlpha" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void PlantLeaf::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj() );
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );
		device->SetShaderParameter(m_hAmbient_AmbientLight, env->GetAmbient() * m_pMaterial->Ambient() );
		device->SetShaderParameter(m_hLightColor, m_pMaterial->Diffuse() * env->GetCurrentLight().GetColor() );
		device->SetShaderParameter(m_hSpecularLight, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
		device->SetShaderParameter(m_hFogColor, env->GetFogColor() );
		device->SetShaderParameter(m_hFogParam, env->GetFogParam() );
		device->SetShaderParameter(m_hFadeAlpha, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 )) );

		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
		{
			device->SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		}
	}
	//--------------------------------------------------------------------------------------









	//////////////////////////
	//	PlantTrunkNormal	//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	PlantTrunkNormal::PlantTrunkNormal()
		:m_hViewProjMatrixLoc(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool PlantTrunkNormal::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPlantTrunkNormal ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void PlantTrunkNormal::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj() );
	}
	//--------------------------------------------------------------------------------------






	//////////////////////////
	//		TreeLeaf		//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	PlantLeafNormal::PlantLeafNormal()
		:m_hViewProjMatrixLoc(-1),
		m_hTextureBase(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool PlantLeafNormal::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPlantLeafNormal ) )
			return false;
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
		m_hTextureBase				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void PlantLeafNormal::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj() );
		if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
		{
			device->SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		}
	}
	//--------------------------------------------------------------------------------------


}