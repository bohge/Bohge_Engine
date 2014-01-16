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
//		dofºÍssvoµÄshader		   //
////////////////////////////////////
#include "ConcisePEShaders.h"
#include "EngineResourceList.h"
#include "Device.h"
#include "Engine.h"
#include "Camera.h"




namespace BohgeEngine
{
	/////////////////////////
	//		DOFCombine		//
	/////////////////////////
	//--------------------------------------------------------------------------------------
	DOFCombine::DOFCombine()
		:m_hOgiTexture(-1),
		m_hDepthTexture(-1),
		m_hBloomTexture(-1),
		m_hNearQ(-1),
		m_hFocus(-1),
		m_pOgiTexture(NULL),
		m_pBloomTexture(NULL),
		m_pDepthTexture(NULL)
	{

	}
	//--------------------------------------------------------------------------------------
	DOFCombine::~DOFCombine()
	{

	}
	//--------------------------------------------------------------------------------------
	bool DOFCombine::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rDOFCombine ) )
			return false;
		m_hOgiTexture			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_SceneTexture" );
		m_hDepthTexture			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_DepthTexture" );
		m_hBloomTexture			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_BlurredTexture" );

		m_hNearQ				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_NeraQ_Q" );
		m_hFocus				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_FocalDistRange" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void DOFCombine::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();

		device->SetShaderParameter(m_hOgiTexture, *m_pOgiTexture );
		device->SetShaderParameter(m_hDepthTexture, *m_pDepthTexture );
		device->SetShaderParameter(m_hBloomTexture, *m_pBloomTexture );

		device->SetShaderParameter(m_hNearQ, engine.GetCamera()->GetNearQ_Q() );
		device->SetShaderParameter(m_hFocus, m_Focus );
	}
	//--------------------------------------------------------------------------------------





	/////////////////////////
	//		SSAOShader		//
	/////////////////////////
	//--------------------------------------------------------------------------------------
	SSAOShader::SSAOShader()
		:m_hNormalTexture(-1),
		m_hDepthTexture(-1),
		m_hJitterTexture(-1),
		m_hProjInv(-1),
		m_hStepSize(-1),
		m_hAOEdgeFinder(-1),
		m_hAOIntensity(-1),
		m_hJitterAmount(-1),
		m_pNormalTexture(NULL),
		m_pJitterTexture(NULL),
		m_pDepthTexture(NULL),
		m_AOEdgeFinder(0.0f),
		m_AOIntensity(0.0f),
		m_JitterAmount(0.0f)
	{

	}
	//--------------------------------------------------------------------------------------
	SSAOShader::~SSAOShader()
	{

	}
	//--------------------------------------------------------------------------------------
	bool SSAOShader::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSSVOshader ) )
			return false;
		m_hNormalTexture	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_NormalTexture" );
		m_hDepthTexture		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_DepthTexture" );
		m_hJitterTexture	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_JitterTexture" );

		m_hProjInv			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_ProjInv" );
		m_hStepSize			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_StepSize" );
		m_hAOEdgeFinder		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_AOEdgeFinder" );
		m_hAOIntensity		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_AOIntensity" );
		m_hJitterAmount		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_JitterAmount" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void SSAOShader::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();

		device->SetShaderParameter(m_hNormalTexture, *m_pNormalTexture );
		device->SetShaderParameter(m_hDepthTexture, *m_pDepthTexture );
		device->SetShaderParameter(m_hJitterTexture, *m_pJitterTexture );

		device->SetShaderParameter(m_hProjInv, engine.GetCamera()->GetUnProject() );
		device->SetShaderParameter(m_hStepSize, m_Step );
		device->SetShaderParameter(m_hAOEdgeFinder, m_AOEdgeFinder );
		device->SetShaderParameter(m_hAOIntensity, m_AOIntensity );
		device->SetShaderParameter(m_hJitterAmount, m_JitterAmount );
	}
	//--------------------------------------------------------------------------------------



	//--------------------------------------------------------------------------------------
	SSAOCombine::SSAOCombine()
		:m_hSceneTexture(-1),
		m_hSSVOTexture(-1),
		m_pSceneTexture(NULL),
		m_pSSVOTexture(NULL)
	{

	}
	//--------------------------------------------------------------------------------------
	SSAOCombine::~SSAOCombine()
	{

	}
	//--------------------------------------------------------------------------------------
	bool SSAOCombine::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSSVOCombine ) )
			return false;
		m_hSceneTexture		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "scen" );
		m_hSSVOTexture		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "ssvo" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void SSAOCombine::_SetParameters ()
	{
		Device* device = Engine::Instance().GetDevice();
		device->SetShaderParameter(m_hSceneTexture, *m_pSceneTexture );
		device->SetShaderParameter(m_hSSVOTexture, *m_pSSVOTexture );
	}
	//--------------------------------------------------------------------------------------







	/////////////////////////
	//		FXAAShder		//
	/////////////////////////
	//--------------------------------------------------------------------------------------
	FXAAShder::FXAAShder()
		:m_hSceneTexture(-1),
		m_hStepSize(-1),
		m_pSceneTexture(NULL)
	{

	}
	//--------------------------------------------------------------------------------------
	bool FXAAShder::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rFXAAShader ) )
			return false;
		m_hSceneTexture		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "secenTex" );
		m_hStepSize			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "StepSize" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void FXAAShder::_SetParameters ()
	{
		Device* device = Engine::Instance().GetDevice();
		device->SetShaderParameter(m_hSceneTexture, *m_pSceneTexture );
		device->SetShaderParameter(m_hStepSize, m_StepSize );
	}
	//--------------------------------------------------------------------------------------

}