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
//		环境相关的shader	 //
///////////////////////////////
#include "EnvironmentShaders.h"
#include "EngineResourceList.h"
#include "Engine.h"
#include "Camera.h"
#include "Texture.h"

namespace BohgeEngine
{
	/////////////////////////
	//		SkyBoxShader   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	SkyBoxShader::SkyBoxShader()
	{
	}
	//--------------------------------------------------------------------------------------
	SkyBoxShader::~SkyBoxShader()
	{
	}
	//--------------------------------------------------------------------------------------
	bool SkyBoxShader::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		//m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkyBox ) )
			return false;
		m_hTransform = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "mvp" );
		m_hAlpha = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Alpha" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void SkyBoxShader::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		vector3f cp = engine.GetCamera()->GetPosition();

		Matrix44f mvp = *m_pTransform;	//让他始终跟随摄像机
		mvp.a41 = cp.m_x;
		mvp.a42 = cp.m_y;
		mvp.a43 = cp.m_z;

		mvp = mvp * engine.GetCamera()->GetViewProj(); 

		device->SetShaderParameter(m_hTransform, mvp);
		device->SetShaderParameter(m_hAlpha, m_Alpha);
		//clean up
		m_pTransform = NULL;
	}
	//--------------------------------------------------------------------------------------




	//--------------------------------------------------------------------------------------
	void HaloShader::_SetParameters()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hProjLoc, camera->GetProject() );
		device->SetShaderParameter(m_hViewProjLoc, camera->GetViewProj() );
		device->SetShaderParameter(m_hRotation, camera->GetRotation() );
		device->SetShaderParameter(m_hCamerTransform, camera->GetTransform() );
		device->SetShaderParameter(m_hLightTexture, *m_pLightTexture );
		device->SetShaderParameter(m_hDepthTexture, *m_pDepthTexture );
		device->SetShaderParameter(m_hColor, m_Color );
	}
	//--------------------------------------------------------------------------------------
	bool HaloShader::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "coordnate",Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "center",Attributes::ATTRIBUTE_NORMAL );
		m_Attributes.PushAttribute( "color",Attributes::ATTRIBUTE_TANGENT );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rHaloShader ) )
			return false;
		m_hProjLoc			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matProj" );
		m_hViewProjLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hRotation			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matRotation" );
		m_hCamerTransform	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "CameraTrans" );
		m_hLightTexture		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "LightTexture" );
		m_hDepthTexture		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "DepthTexture" );
		m_hColor			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Color" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	HaloShader::HaloShader()
		:m_pLightTexture(NULL),
		m_pDepthTexture(NULL)
	{

	}





	/////////////////////////
	//	FakeLightShader	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	bool FakeLightShader::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "coordnate",Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "center",Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rFakeLightShader ) )
			return false;
		m_hColor			 = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Color" );
		m_hProjLoc			 = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matProj" );
		m_hRotation			 = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matRotation" );
		m_hCamerTransform	 = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "CameraTrans" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void FakeLightShader::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hColor, m_Color );
		device->SetShaderParameter(m_hProjLoc, camera->GetProject() );
		device->SetShaderParameter(m_hRotation, camera->GetRotation() );
		device->SetShaderParameter(m_hCamerTransform, camera->GetTransform() );
	}
	//--------------------------------------------------------------------------------------
}