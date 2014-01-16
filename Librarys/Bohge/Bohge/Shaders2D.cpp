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
//		2DÏà¹ØµÄshader		 //
///////////////////////////////

#include "Shaders2D.h"
#include "Device.h"
#include "Engine.h"
#include "EngineResourceList.h"
#include "Camera.h"


namespace BohgeEngine
{
	/////////////////////////
	//		UIShader	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	UIShader::UIShader()
	{
	}
	//--------------------------------------------------------------------------------------
	UIShader::~UIShader()
	{
	}
	//--------------------------------------------------------------------------------------
	bool UIShader::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "color",	Attributes::ATTRIBUTE_COLOR0 );
		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rUIShader ) )
			return false;
		m_hColor	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "clr" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void UIShader::_SetParameters ()
	{
		Engine::Instance().GetDevice()->SetShaderParameter(m_hColor, m_Color);
	}
	//--------------------------------------------------------------------------------------





	/////////////////////////
	//	ScreenShapes	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	bool ScreenShapes::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "color",	Attributes::ATTRIBUTE_COLOR0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rScreenShapes ) )
			return false;
		return true;
	}
	//--------------------------------------------------------------------------------------
	void ScreenShapes::_SetParameters ()
	{
	}
	//--------------------------------------------------------------------------------------


	/////////////////////////
	//		WorldShapes	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	bool WorldShapes::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "color",	Attributes::ATTRIBUTE_COLOR0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rWorldShapes ) )
			return false;
		m_hModelViewProjLoc = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void WorldShapes::_SetParameters ()
	{
		Engine::Instance().GetDevice()->SetShaderParameter(m_hModelViewProjLoc, (*m_Transform) * Engine::Instance().GetCamera()->GetViewProj());
	}
	//--------------------------------------------------------------------------------------




	/////////////////////////
	//		FontShader	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	FontShader::FontShader()
	{
	}
	//--------------------------------------------------------------------------------------
	FontShader::~FontShader()
	{
	}
	//--------------------------------------------------------------------------------------
	bool FontShader::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "color",		Attributes::ATTRIBUTE_COLOR0 },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	


		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "color",	Attributes::ATTRIBUTE_COLOR0 );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rFontShader ) )
			return false;
		m_hPos = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "pos" );
		m_hScale = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "scale" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void FontShader::_SetParameters ()
	{
		Engine::Instance().GetDevice()->SetShaderParameter( m_hPos, vector4f(m_vPos.m_x,m_vPos.m_y,0,0) );
		Engine::Instance().GetDevice()->SetShaderParameter( m_hScale, m_vScale );
	}
	//--------------------------------------------------------------------------------------









	/////////////////////////
	//	BillboardShader	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	bool BillboardShader::Initialization()
	{
		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "coordnate",Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "center",Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rBillboardShader ) )
			return false;
		m_hProjLoc			 = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matProj" );
		m_hRotation			 = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matRotation" );
		m_hCamerTransform	 = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "CameraTrans" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void BillboardShader::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hProjLoc, camera->GetProject() );
		device->SetShaderParameter(m_hRotation, camera->GetRotation() );
		device->SetShaderParameter(m_hCamerTransform, camera->GetTransform() );
	}
	//--------------------------------------------------------------------------------------

}