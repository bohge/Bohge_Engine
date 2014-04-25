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
//		逐像素光照的shader		   //
////////////////////////////////////
#include "PrePixelLightShaders.h"
#include "Engine.h"
#include "Device.h"
#include "Environment.h"
#include "EngineResourceList.h"
#include "Camera.h"
#include "Light.h"



namespace BohgeEngine
{
	/////////////////////////
	//	PrePixelLight	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	PrePixelLight::PrePixelLight()
		:m_hWorldViewProjMatrixLoc(0),
		m_hLightDirectionLoc(0),
		m_hCameraPositionLoc(0),
		m_hAmbientLoc(0)
	{
	}
	//--------------------------------------------------------------------------------------
	PrePixelLight::~PrePixelLight()
	{
	}
	//--------------------------------------------------------------------------------------
	bool PrePixelLight::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 },
		//	{ "normal",		Attributes::ATTRIBUTE_NORMAL }
		//};
		//const int nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",	Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPrePixelLight ) )
			return false;
		// Make a record of the location for each shader constant
		m_hWorldViewProjMatrixLoc	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hAmbientLoc				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "fAmbient" );
		m_hLightLoc					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Light" );

		return true;
	}
	//--------------------------------------------------------------------------------------
	void PrePixelLight::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();
		device->SetShaderParameter(m_hWorldViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );
		device->SetShaderParameter(m_hAmbientLoc, env->GetAmbient());
		device->SetShaderParameter(m_hLightLoc, env->GetCurrentLight().GetColor());
	}
	//--------------------------------------------------------------------------------------









	//////////////////////////
	//PrePixelLightTransform//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	PrePixelLightTransform::PrePixelLightTransform()
		:m_hWorldViewProjMatrixLoc(0),
		m_hTransform(0),
		m_hLightDirectionLoc(0),
		m_hCameraPositionLoc(0),
		m_hAmbientLoc(0),
		m_hColor(0),
		m_pColor(NULL),
		m_pTransform(NULL)
	{
	}
	//--------------------------------------------------------------------------------------
	PrePixelLightTransform::~PrePixelLightTransform()
	{
	}
	//--------------------------------------------------------------------------------------
	bool PrePixelLightTransform::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 },
		//	{ "normal",		Attributes::ATTRIBUTE_NORMAL }
		//};
		//const int nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",	Attributes::ATTRIBUTE_NORMAL );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPrePixelLightTransform ) )
			return false;
		// Make a record of the location for each shader constant
		m_hWorldViewProjMatrixLoc	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
		m_hTransform				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
		m_hNormalTransform			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hAmbientLoc				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "fAmbient" );
		m_hColor					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vClr" );
		m_hLightLoc					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Light" );

		return true;
	}
	//--------------------------------------------------------------------------------------
	void PrePixelLightTransform::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		Matrix44f wvp = *m_pTransform * camera->GetViewProj();

		device->SetShaderParameter(m_hTransform, *m_pTransform);
		if( NULL == m_pColor )
		{
			device->SetShaderParameter(m_hColor, vector4f(1,1,1,1) );
		}
		else
		{
			device->SetShaderParameter(m_hColor, *m_pColor);
		}
		device->SetShaderParameter(m_hNormalTransform, Matrix33f( *m_pTransform ) );
		device->SetShaderParameter(m_hWorldViewProjMatrixLoc, wvp);
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );
		device->SetShaderParameter(m_hAmbientLoc, env->GetAmbient());
		device->SetShaderParameter(m_hLightLoc, env->GetCurrentLight().GetColor());
		//clean up
		m_pTransform = NULL;
		m_pColor = NULL;
	}
	//--------------------------------------------------------------------------------------






	///////////////////////////////////
	////	PrePixelLightInstance	   //
	///////////////////////////////////
	////--------------------------------------------------------------------------------------
	//PrePixelLightInstance::PrePixelLightInstance()
	//	:m_hViewProjMatrixLoc(-1),
	//	m_hLightDirectionLoc(-1),
	//	m_hCameraPositionLoc(-1),
	//	m_hAmbientLoc(-1),
	//	m_hLightLoc(-1)
	//{
	//}
	////--------------------------------------------------------------------------------------
	//bool PrePixelLightInstance::Initialization()
	//{
	//	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	//	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );
	//	m_Attributes.PushAttribute( "normal",	Attributes::ATTRIBUTE_NORMAL );
	//	m_Attributes.PushAttribute( "matColum1",	Attributes::ATTRIBUTE_MATIRX_COLUMN1 );
	//	m_Attributes.PushAttribute( "matColum2",	Attributes::ATTRIBUTE_MATIRX_COLUMN2 );
	//	m_Attributes.PushAttribute( "matColum3",	Attributes::ATTRIBUTE_MATIRX_COLUMN3 );
	//	m_Attributes.PushAttribute( "matColum4",	Attributes::ATTRIBUTE_MATIRX_COLUMN4 );

	//	if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPrePixelLightInstance ) )
	//		return false;
	//	// Make a record of the location for each shader constant
	//	m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "ViewProj" );
	//	m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	//	m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	//	m_hAmbientLoc				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "fAmbient" );
	//	m_hLightLoc					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Light" );
	//	return true;
	//}
	////--------------------------------------------------------------------------------------
	//void PrePixelLightInstance::_SetParameters ()
	//{
	//	Engine& engine = Engine::Instance();
	//	Device* device = engine.GetDevice();
	//	Environment* env = engine.GetEnvironment();
	//	ICamera* camera = engine.GetCamera();
	//	device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
	//	device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
	//	device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );
	//	device->SetShaderParameter(m_hAmbientLoc, env->GetAmbient());
	//	device->SetShaderParameter(m_hLightLoc, env->GetCurrentLight().GetColor());
	//}
	////--------------------------------------------------------------------------------------

}