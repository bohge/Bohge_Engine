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
//		阴影相关的shader		   //
////////////////////////////////////
#include "Device.h"
#include "Engine.h"
#include "Camera.h"
#include "Environment.h"
#include "ShadowShader.h"
#include "EngineResourceList.h"
#include "Light.h"



namespace BohgeEngine
{
	/////////////////////////
	//	ShadowVolume	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	ShadowVolume::ShadowVolume()
		:m_hLightDirectionLoc(0),
		m_hNormalTransformLoc(0),
		m_hModelViewProjLoc(0),
		m_hViewProjLoc(0),
		m_pTransform(NULL)
	{
	}
	//--------------------------------------------------------------------------------------
	ShadowVolume::~ShadowVolume()
	{
	}
	//--------------------------------------------------------------------------------------
	bool ShadowVolume::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "g_vVertex",	Attributes::ATTRIBUTE_POSITION },
		//	{ "g_vNormal",	Attributes::ATTRIBUTE_NORMAL }
		//};	
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

		m_Attributes.PushAttribute( "g_vVertex",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "g_vNormal",	Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rShadowVolume ) )
			return false;
		// Make a record of the location for each shader constant
		m_hLightDirectionLoc	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_vLightDirection" );
		m_hNormalTransformLoc	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_matNormal" );
		m_hModelViewProjLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_matModelViewProj" );
		m_hViewProjLoc			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_matViewProj" );

		return true;
	}
	//--------------------------------------------------------------------------------------
	void ShadowVolume::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		Matrix44f wvp = *m_pTransform * camera->GetViewProj();
		Matrix33f normalTransform(*m_pTransform);

		device->SetShaderParameter(m_hNormalTransformLoc, normalTransform);
		device->SetShaderParameter(m_hModelViewProjLoc, wvp);
		device->SetShaderParameter(m_hLightDirectionLoc, env->GetCurrentLight().GetWorldDirection());
		device->SetShaderParameter(m_hViewProjLoc, camera->GetViewProjBias());
		//clean up
		m_pTransform = NULL;
	}
	//--------------------------------------------------------------------------------------





	///////////////////////////
	////	ShadowVolume	   //
	///////////////////////////
	////--------------------------------------------------------------------------------------
	//ShadowVolumeInstance::ShadowVolumeInstance()
	//	:m_hLightDirectionLoc(-1),
	//	m_hViewProjLoc(-1)
	//{
	//}
	////--------------------------------------------------------------------------------------
	//bool ShadowVolumeInstance::Initialization()
	//{
	//	m_Attributes.PushAttribute( "g_vVertex",	Attributes::ATTRIBUTE_POSITION );
	//	m_Attributes.PushAttribute( "g_vNormal",	Attributes::ATTRIBUTE_NORMAL );
	//	m_Attributes.PushAttribute( "matColum1",	Attributes::ATTRIBUTE_MATIRX_COLUMN1 );
	//	m_Attributes.PushAttribute( "matColum2",	Attributes::ATTRIBUTE_MATIRX_COLUMN2 );
	//	m_Attributes.PushAttribute( "matColum3",	Attributes::ATTRIBUTE_MATIRX_COLUMN3 );
	//	m_Attributes.PushAttribute( "matColum4",	Attributes::ATTRIBUTE_MATIRX_COLUMN4 );

	//	if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rShadowVolumeInstance ) )
	//		return false;
	//	// Make a record of the location for each shader constant
	//	m_hLightDirectionLoc	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_vLightDirection" );
	//	m_hViewProjLoc			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_matViewProj" );

	//	return true;
	//}
	////--------------------------------------------------------------------------------------
	//void ShadowVolumeInstance::_SetParameters ()
	//{
	//	Engine& engine = Engine::Instance();
	//	Device* device = engine.GetDevice();
	//	Environment* env = engine.GetEnvironment();
	//	ICamera* camera = engine.GetCamera();
	//	device->SetShaderParameter(m_hLightDirectionLoc, env->GetCurrentLight().GetWorldDirection());
	//	device->SetShaderParameter(m_hViewProjLoc, camera->GetViewProjBias());
	//}
	////--------------------------------------------------------------------------------------






	/////////////////////////
	//		GaussianFour	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	GaussianDepth::GaussianDepth()
	{
	}
	//--------------------------------------------------------------------------------------
	GaussianDepth::~GaussianDepth()
	{
	}
	//--------------------------------------------------------------------------------------
	bool GaussianDepth::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rGaussianDepth ) )
			return false;
		m_hStepSize = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_vStepSize" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void GaussianDepth::_SetParameters ()
	{
		Engine::Instance().GetDevice()->SetShaderParameter( m_hStepSize, m_vStep );
	}
	//--------------------------------------------------------------------------------------




	/////////////////////////
	//	GaussianDepthLow  //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	GaussianDepthLow::GaussianDepthLow()
	{
	}
	//--------------------------------------------------------------------------------------
	GaussianDepthLow::~GaussianDepthLow()
	{
	}
	//--------------------------------------------------------------------------------------
	bool GaussianDepthLow::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rGaussianDepthLow ) )
			return false;
		m_hStepSize = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_vStepSize" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void GaussianDepthLow::_SetParameters ()
	{
		Engine::Instance().GetDevice()->SetShaderParameter( m_hStepSize, m_vStep );
	}
	//--------------------------------------------------------------------------------------

}