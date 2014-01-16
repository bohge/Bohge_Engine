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
//			高光的一些shader	   //
/////////////////////////////////////
#include "HDRShaders.h"
#include "Device.h"
#include "Engine.h"
#include "EngineResourceList.h"




namespace BohgeEngine
{
	/////////////////////////
	//		Luminance	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	Luminance::Luminance()
	{
	}
	//--------------------------------------------------------------------------------------
	Luminance::~Luminance()
	{
	}
	//--------------------------------------------------------------------------------------
	bool Luminance::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	


		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rLuminance ) )
			return false;
		return true;
	}
	//--------------------------------------------------------------------------------------
	void Luminance::_SetParameters ()
	{
	}
	//--------------------------------------------------------------------------------------




	/////////////////////////
	//		Gaussian	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	Gaussian::Gaussian()
	{
	}
	//--------------------------------------------------------------------------------------
	Gaussian::~Gaussian()
	{
	}
	//--------------------------------------------------------------------------------------
	bool Gaussian::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	

		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rGaussian ) )
			return false;
		m_hStepSize = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_vStepSize" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void Gaussian::_SetParameters ()
	{
		Engine::Instance().GetDevice()->SetShaderParameter( m_hStepSize, m_vStep );
	}
	//--------------------------------------------------------------------------------------



	//////////////////////////
	//		HDR's Shader	//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	HDRGenerateLuminance::HDRGenerateLuminance()
		:m_hColorTex(-1),
		m_hStepSize(-1),
		m_pColorTexture(NULL)
	{

	}
	//--------------------------------------------------------------------------------------
	HDRGenerateLuminance::~HDRGenerateLuminance()
	{

	}
	//--------------------------------------------------------------------------------------
	bool HDRGenerateLuminance::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rHDRGenerateLuminance ) )
			return false;
		m_hColorTex		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_ColorTexture" );
		m_hStepSize		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_StepSize" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void HDRGenerateLuminance::_SetParameters ()
	{
		Device* device = Engine::Instance().GetDevice();
		device->SetShaderParameter(m_hColorTex, *m_pColorTexture );
		device->SetShaderParameter(m_hStepSize, m_StepSize );
	}
	//--------------------------------------------------------------------------------------



	//--------------------------------------------------------------------------------------
	HDRAverageLuminance::HDRAverageLuminance()
		:m_hColorTex(-1),
		m_hStepSize(-1),
		m_pColorTexture(NULL)
	{

	}
	//--------------------------------------------------------------------------------------
	HDRAverageLuminance::~HDRAverageLuminance()
	{

	}
	//--------------------------------------------------------------------------------------
	bool HDRAverageLuminance::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rHDRAverageLuminance ) )
			return false;
		m_hColorTex		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_ColorTexture" );
		m_hStepSize		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_StepSize" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void HDRAverageLuminance::_SetParameters ()
	{
		Device* device = Engine::Instance().GetDevice();
		device->SetShaderParameter(m_hColorTex, *m_pColorTexture );
		device->SetShaderParameter(m_hStepSize, m_StepSize );
	}
	//--------------------------------------------------------------------------------------



	//--------------------------------------------------------------------------------------
	HDRAdaptLuminance::HDRAdaptLuminance()
		:m_hCurrentLuminance(-1),
		m_hPreviousLuminance(-1),
		m_hLambdaElapsedTime(-1),
		m_hExposureLimits(-1),
		m_pCurrentLuminance(NULL),
		m_pPreviousLuminance(NULL),
		m_ExposureLimits(0),
		m_LambdaElapsedTime(0)
	{

	}
	//--------------------------------------------------------------------------------------
	HDRAdaptLuminance::~HDRAdaptLuminance()
	{

	}
	//--------------------------------------------------------------------------------------
	bool HDRAdaptLuminance::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rHDRAdaptLuminance ) )
			return false;
		m_hCurrentLuminance		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_CurrentLuminance" );
		m_hPreviousLuminance	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_PreviousLuminance" );
		m_hLambdaElapsedTime	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "LambdaElapsedTime" );
		m_hExposureLimits		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "ExposureLimits" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void HDRAdaptLuminance::_SetParameters ()
	{
		Device* device = Engine::Instance().GetDevice();
		device->SetShaderParameter(m_hCurrentLuminance, *m_pCurrentLuminance );
		device->SetShaderParameter(m_hPreviousLuminance, *m_pPreviousLuminance );
		device->SetShaderParameter(m_hLambdaElapsedTime, m_LambdaElapsedTime );
		device->SetShaderParameter(m_hExposureLimits, m_ExposureLimits );
	}
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	HDRBrightPass::HDRBrightPass()
		:m_hCurrentLuminance(-1),
		m_hScratchTexture(-1),
		m_hMiddleGray(-1),
		m_hBrightPassWhite(-1),
		m_hBrightPassThreshold(-1),
		m_hBrightPassOffset(-1),
		m_pCurrentLuminance(NULL),
		m_pScratchTexture(NULL),
		m_MiddleGray(0),
		m_BrightPassWhite(0),
		m_BrightPassThreshold(0),
		m_BrightPassOffset(0)
	{

	}
	//--------------------------------------------------------------------------------------
	HDRBrightPass::~HDRBrightPass()
	{

	}
	//--------------------------------------------------------------------------------------
	bool HDRBrightPass::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rHDRBrightPass ) )
			return false;
		m_hCurrentLuminance		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_CurrentLuminance" );
		m_hScratchTexture		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_ScratchTexture" );

		m_hMiddleGray			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "MiddleGray" );
		m_hBrightPassWhite		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BrightPassWhite" );
		m_hBrightPassThreshold	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BrightPassThreshold" );
		m_hBrightPassOffset		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BrightPassOffset" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void HDRBrightPass::_SetParameters ()
	{
		Device* device = Engine::Instance().GetDevice();
		device->SetShaderParameter(m_hCurrentLuminance, *m_pCurrentLuminance );
		device->SetShaderParameter(m_hScratchTexture, *m_pScratchTexture );

		device->SetShaderParameter(m_hMiddleGray, m_MiddleGray );
		device->SetShaderParameter(m_hBrightPassWhite, m_BrightPassWhite );
		device->SetShaderParameter(m_hBrightPassThreshold, m_BrightPassThreshold );
		device->SetShaderParameter(m_hBrightPassOffset, m_BrightPassOffset );
	}
	//--------------------------------------------------------------------------------------



	//--------------------------------------------------------------------------------------
	HDRCombine::HDRCombine()
		:m_hCurrentLuminance(-1),
		m_hSharpTexture(-1),
		m_hBloomTexture(-1),
		m_hMiddleGray(-1),
		m_hWhite(-1),
		m_pCurrentLuminance(NULL),
		m_pSharpTexture(NULL),
		m_pBloomTexture(NULL),
		m_MiddleGray(0),
		m_White(0)
	{

	}
	//--------------------------------------------------------------------------------------
	HDRCombine::~HDRCombine()
	{

	}
	//--------------------------------------------------------------------------------------
	bool HDRCombine::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rHDRCombine ) )
			return false;
		m_hCurrentLuminance		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_CurrentLuminance" );
		m_hSharpTexture			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_SharpTexture" );
		m_hBloomTexture			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_BloomTexture" );

		m_hMiddleGray			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "MiddleGray" );
		m_hWhite				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "White" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void HDRCombine::_SetParameters ()
	{
		Device* device = Engine::Instance().GetDevice();
		device->SetShaderParameter(m_hCurrentLuminance, *m_pCurrentLuminance );
		device->SetShaderParameter(m_hSharpTexture, *m_pSharpTexture );
		device->SetShaderParameter(m_hBloomTexture, *m_pBloomTexture );

		device->SetShaderParameter(m_hMiddleGray, m_MiddleGray );
		device->SetShaderParameter(m_hWhite, m_White );
	}
	//--------------------------------------------------------------------------------------

}