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
//		基本工具shader		 //
///////////////////////////////
#include "UtilityShaders.h"
#include "Device.h"
#include "Engine.h"
#include "EngineResourceList.h"
#include "Camera.h"



namespace BohgeEngine
{
	/////////////////////////
	//		Constant	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	Constant::Constant()
		:m_hModelViewProjLoc(0),
		m_pTransform(NULL)
	{
	}
	//--------------------------------------------------------------------------------------
	Constant::~Constant()
	{
	}
	//--------------------------------------------------------------------------------------
	bool Constant::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "g_vVertex",	Attributes::ATTRIBUTE_POSITION }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

		m_Attributes.PushAttribute( "g_vVertex",	Attributes::ATTRIBUTE_POSITION );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rConstant ) )
			return false;
		// Make a record of the location for each shader constant
		m_hModelViewProjLoc	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_mModelViewProjectionMatrix" );
		m_hShadowColor	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_vColor" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void Constant::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();

		Matrix44f wvp = *m_pTransform * camera->GetViewProj();
		device->SetShaderParameter(m_hModelViewProjLoc, wvp);
		device->SetShaderParameter(m_hShadowColor, vector4f(0.0f, 0.0f, 0.0f, 0.8f));
	}
	//--------------------------------------------------------------------------------------






	//////////////////////////
	//		Combine			//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	Combine::Combine()
	{
	}
	//--------------------------------------------------------------------------------------
	Combine::~Combine()
	{
	}
	//--------------------------------------------------------------------------------------
	bool Combine::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	

		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rCombine ) )
			return false;
		m_hTexOrginialHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "textureOriginal" );
		m_hTexSecenHandle		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "textureScene" );
		m_hWeightOrginialHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "weightOriginal" );
		m_hWeightSecenHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "weightScene" );

		return true;
	}
	//--------------------------------------------------------------------------------------
	void Combine::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		device->SetShaderParameter( m_hTexOrginialHandle, *m_pTexOrginial );
		device->SetShaderParameter( m_hTexSecenHandle, *m_pTexSecen );
		device->SetShaderParameter( m_hWeightOrginialHandle, m_fWeightOrginial );
		device->SetShaderParameter( m_hWeightSecenHandle, m_fWeightSecen );
	}
	//--------------------------------------------------------------------------------------





	/////////////////////////
	//	PipelineCombine   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	PipelineCombine::PipelineCombine()
	{
	}
	//--------------------------------------------------------------------------------------
	PipelineCombine::~PipelineCombine()
	{
	}
	//--------------------------------------------------------------------------------------
	bool PipelineCombine::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPipelineCombine ) )
			return false;
		m_hTexOrginialHandle = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Background" );
		m_hTexSecenHandle = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "PostImage" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void PipelineCombine::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		device->SetShaderParameter( m_hTexOrginialHandle, *m_pTexOrginial );
		device->SetShaderParameter( m_hTexSecenHandle, *m_pTexSecen );
	}
	//--------------------------------------------------------------------------------------



	/////////////////////////
	//		QuadShader	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	QuadShader::QuadShader()
	{
	}
	//--------------------------------------------------------------------------------------
	QuadShader::~QuadShader()
	{
	}
	//--------------------------------------------------------------------------------------
	bool QuadShader::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rQuadShader ) )
			return false;
		return true;
	}
	//--------------------------------------------------------------------------------------
	void QuadShader::_SetParameters ()
	{
	}
	//--------------------------------------------------------------------------------------



	//////////////////////////
	//CopyDepthToRBGAShader //
	//////////////////////////
	//--------------------------------------------------------------------------------------
	CopyDepthToRBGAShader::CopyDepthToRBGAShader()
	{
	}
	//--------------------------------------------------------------------------------------
	bool CopyDepthToRBGAShader::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rCopyDepthToRBGAShader ) )
			return false;
		return true;
	}
	//--------------------------------------------------------------------------------------
	void CopyDepthToRBGAShader::_SetParameters ()
	{
	}
	////--------------------------------------------------------------------------------------






	//--------------------------------------------------------------------------------------
	Blur2x2Shader::Blur2x2Shader()
		:m_hColorTex(-1),
		m_hStepSize(-1),
		m_pColorTexture(NULL)
	{

	}
	//--------------------------------------------------------------------------------------
	Blur2x2Shader::~Blur2x2Shader()
	{

	}
	//--------------------------------------------------------------------------------------
	bool Blur2x2Shader::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rBlur2x2Shader ) )
			return false;
		m_hColorTex		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_ColorTexture" );
		m_hStepSize		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_StepSize" );
		m_hGaussWeight	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_GaussWeight" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void Blur2x2Shader::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		device->SetShaderParameter(m_hColorTex, *m_pColorTexture );
		device->SetShaderParameter(m_hStepSize, m_StepSize );
		device->SetShaderParameter(m_hGaussWeight, m_GaussWeight );
	}
	//--------------------------------------------------------------------------------------

}