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



/////////////////////////////////////////
//			State Manage class		   //
/////////////////////////////////////////

#include "ShaderManage.h"
#include "IDriver.h"
#include "Predefine.h"
#include "Environment.h"
#include "Texture.h"
#include "EngineResourceList.h"
#include "Camera.h"
#include "ISecenNode.h"
#include "Material.h"
#include "SkyBox.h"
#include "GameSetting.h"

using namespace std;

/////////////////////
//		Shader	   //
/////////////////////
//--------------------------------------------------------------------------------------
Shader::Shader()
:m_hShaderProgram(0)
{

}
//--------------------------------------------------------------------------------------
Shader::~Shader ()
{
	IDriver::Instance().DeleteProgram( m_hShaderProgram );
}
//--------------------------------------------------------------------------------------
void Shader::UsingShader()
{
	IDriver& driver = IDriver::Instance();
	driver.CleanTextrueReference();
	driver.BindShader(m_hShaderProgram);
	this->SetParameters();
}



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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 },
	//	{ "normal",		Attributes::ATTRIBUTE_NORMAL }
	//};
	//const int nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );
	m_Attributes.PushAttribute( "normal",	Attributes::ATTRIBUTE_NORMAL );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPrePixelLight ) )
		return false;
	// Make a record of the location for each shader constant
	m_hWorldViewProjMatrixLoc	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	m_hAmbientLoc				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "fAmbient" );
	m_hLightLoc					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Light" );

	return true;
}
//--------------------------------------------------------------------------------------
void PrePixelLight::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& light = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();
	driver.SetShaderParameter(m_hWorldViewProjMatrixLoc, camera.GetViewProj());
	driver.SetShaderParameter(m_hLightDirectionLoc, -light.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );
	driver.SetShaderParameter(m_hAmbientLoc, light.GetAmbient());
	driver.SetShaderParameter(m_hLightLoc, light.GetColor());
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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 },
	//	{ "normal",		Attributes::ATTRIBUTE_NORMAL }
	//};
	//const int nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );
	m_Attributes.PushAttribute( "normal",	Attributes::ATTRIBUTE_NORMAL );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPrePixelLightTransform ) )
		return false;
	// Make a record of the location for each shader constant
	m_hWorldViewProjMatrixLoc	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
	m_hTransform				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
	m_hNormalTransform			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	m_hAmbientLoc				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "fAmbient" );
	m_hColor					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vClr" );
	m_hLightLoc					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Light" );
	
	return true;
}
//--------------------------------------------------------------------------------------
void PrePixelLightTransform::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& light = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	Matrix44f wvp = *m_pTransform * camera.GetViewProj();

	driver.SetShaderParameter(m_hTransform, *m_pTransform);
	if( NULL == m_pColor )
	{
		driver.SetShaderParameter(m_hColor, vector4f(1,1,1,1) );
	}
	else
	{
		driver.SetShaderParameter(m_hColor, *m_pColor);
	}
	driver.SetShaderParameter(m_hNormalTransform, Matrix33f( *m_pTransform ) );
	driver.SetShaderParameter(m_hWorldViewProjMatrixLoc, wvp);
	driver.SetShaderParameter(m_hLightDirectionLoc, -light.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );
	driver.SetShaderParameter(m_hAmbientLoc, light.GetAmbient());
	driver.SetShaderParameter(m_hLightLoc, light.GetColor());
	//clean up
	m_pTransform = NULL;
	m_pColor = NULL;
}
//--------------------------------------------------------------------------------------








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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "g_vVertex",	Attributes::ATTRIBUTE_POSITION },
	//	{ "g_vNormal",	Attributes::ATTRIBUTE_NORMAL }
	//};	
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

	m_Attributes.PushAttribute( "g_vVertex",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "g_vNormal",	Attributes::ATTRIBUTE_NORMAL );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rShadowVolume ) )
		return false;
	// Make a record of the location for each shader constant
	m_hLightDirectionLoc	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_vLightDirection" );
	m_hNormalTransformLoc	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_matNormal" );
	m_hModelViewProjLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_matModelViewProj" );
	m_hViewProjLoc			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_matViewProj" );

	return true;
}
//--------------------------------------------------------------------------------------
void ShadowVolume::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& light = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	Matrix44f wvp = *m_pTransform * camera.GetViewProj();
	Matrix33f normalTransform(*m_pTransform);

	driver.SetShaderParameter(m_hNormalTransformLoc, normalTransform);
	driver.SetShaderParameter(m_hModelViewProjLoc, wvp);
	driver.SetShaderParameter(m_hLightDirectionLoc, light.GetDirection());
	driver.SetShaderParameter(m_hViewProjLoc, camera.GetViewProjBias());
	//clean up
	m_pTransform = NULL;
}
//--------------------------------------------------------------------------------------





/////////////////////////
//	ShadowVolume	   //
/////////////////////////
//--------------------------------------------------------------------------------------
ShadowVolumeInstance::ShadowVolumeInstance()
:m_hLightDirectionLoc(-1),
m_hViewProjLoc(-1)
{
}
//--------------------------------------------------------------------------------------
bool ShadowVolumeInstance::Initialization()
{
	m_Attributes.PushAttribute( "g_vVertex",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "g_vNormal",	Attributes::ATTRIBUTE_NORMAL );
	m_Attributes.PushAttribute( "matColum1",	Attributes::ATTRIBUTE_MATIRX_COLUMN1 );
	m_Attributes.PushAttribute( "matColum2",	Attributes::ATTRIBUTE_MATIRX_COLUMN2 );
	m_Attributes.PushAttribute( "matColum3",	Attributes::ATTRIBUTE_MATIRX_COLUMN3 );
	m_Attributes.PushAttribute( "matColum4",	Attributes::ATTRIBUTE_MATIRX_COLUMN4 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rShadowVolumeInstance ) )
		return false;
	// Make a record of the location for each shader constant
	m_hLightDirectionLoc	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_vLightDirection" );
	m_hViewProjLoc			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_matViewProj" );

	return true;
}
//--------------------------------------------------------------------------------------
void ShadowVolumeInstance::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& light = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();
	driver.SetShaderParameter(m_hLightDirectionLoc, light.GetDirection());
	driver.SetShaderParameter(m_hViewProjLoc, camera.GetViewProjBias());
}
//--------------------------------------------------------------------------------------



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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "g_vVertex",	Attributes::ATTRIBUTE_POSITION }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

	m_Attributes.PushAttribute( "g_vVertex",	Attributes::ATTRIBUTE_POSITION );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rConstant ) )
		return false;
	// Make a record of the location for each shader constant
	m_hModelViewProjLoc	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_mModelViewProjectionMatrix" );
	m_hShadowColor	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_vColor" );
	return true;
}
//--------------------------------------------------------------------------------------
void Constant::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	ICamera&	camera = driver.GetCamera();

	Matrix44f wvp = *m_pTransform * camera.GetViewProjBias();
	driver.SetShaderParameter(m_hModelViewProjLoc, wvp);
	driver.SetShaderParameter(m_hShadowColor, vector4f(0.0f, 0.0f, 0.0f, 0.8f));
}
//--------------------------------------------------------------------------------------



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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );
	m_Attributes.PushAttribute( "color",	Attributes::ATTRIBUTE_COLOR0 );
	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rUIShader ) )
		return false;
	m_hColor	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "clr" );
	return true;
}
//--------------------------------------------------------------------------------------
void UIShader::SetParameters ()
{
	IDriver::Instance().SetShaderParameter(m_hColor, m_Color);
}
//--------------------------------------------------------------------------------------





/////////////////////////
//	ShapesShader	   //
/////////////////////////
//--------------------------------------------------------------------------------------
ShapesShader::ShapesShader()
:m_pTransform(NULL)
{
}
//--------------------------------------------------------------------------------------
ShapesShader::~ShapesShader()
{
}
//--------------------------------------------------------------------------------------
bool ShapesShader::Initialization()
{
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "color",		Attributes::ATTRIBUTE_COLOR0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);
	

	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "color",	Attributes::ATTRIBUTE_COLOR0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rShapesShader ) )
		return false;
	m_hModelViewProjLoc = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
	return true;
}
//--------------------------------------------------------------------------------------
void ShapesShader::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Matrix44f wvp;
	if( NULL == m_pTransform )//如果是空则使用相对坐标绘制
	{
	}
	else
	{
		ICamera&	camera = driver.GetCamera();
		wvp = *m_pTransform * camera.GetViewProj();
	}
	driver.SetShaderParameter(m_hModelViewProjLoc, wvp);
	m_pTransform = NULL;
}
//--------------------------------------------------------------------------------------




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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	


	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rLuminance ) )
		return false;
	return true;
}
//--------------------------------------------------------------------------------------
void Luminance::SetParameters ()
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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	

	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rGaussian ) )
		return false;
	m_hStepSize = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_vStepSize" );
	return true;
}
//--------------------------------------------------------------------------------------
void Gaussian::SetParameters ()
{
	IDriver::Instance().SetShaderParameter( m_hStepSize, m_vStep );
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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	

	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rCombine ) )
		return false;
	m_hTexOrginialHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "textureOriginal" );
	m_hTexSecenHandle		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "textureScene" );
	m_hWeightOrginialHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "weightOriginal" );
	m_hWeightSecenHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "weightScene" );

	return true;
}
//--------------------------------------------------------------------------------------
void Combine::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	driver.SetShaderParameter( m_hTexOrginialHandle, *m_pTexOrginial );
	driver.SetShaderParameter( m_hTexSecenHandle, *m_pTexSecen );
	driver.SetShaderParameter( m_hWeightOrginialHandle, m_fWeightOrginial );
	driver.SetShaderParameter( m_hWeightSecenHandle, m_fWeightSecen );
}
//--------------------------------------------------------------------------------------




/////////////////////////
//		VolumeLight	   //
/////////////////////////
//--------------------------------------------------------------------------------------
VolumeLight::VolumeLight()
{
}
//--------------------------------------------------------------------------------------
VolumeLight::~VolumeLight()
{
}
//--------------------------------------------------------------------------------------
bool VolumeLight::Initialization()
{
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	


	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rVolumeLight ) )
		return false;
	m_hTexOrginDepthHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_SenceDepth" );
	m_hTexLightDepthHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_LightDepth" );
	m_hCameraPostionHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_CameraPostion" );
	m_hViewProjInversHandle = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_matViewProjInvers" );
	m_hLightViewProjHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_matLightViewProj" );

	m_hSenceNearHandle		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_SenceNear" );
	m_hSenceFarNearHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_SenceFarNear" );
	m_hSenceFarNearQHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_SenceFarNearQ" );
	m_hLightNearHandle		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_LightNear" );
	m_hLightFarNearHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_LightFarNear" );
	m_hLightFarNearQHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_LightFarNearQ" );
	return true;
}
//--------------------------------------------------------------------------------------
void VolumeLight::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	driver.SetShaderParameter( m_hTexOrginDepthHandle, *m_pTexOrginial );
	driver.SetShaderParameter( m_hTexLightDepthHandle, *m_pTexLight );
	driver.SetShaderParameter( m_hCameraPostionHandle, driver.GetCamera().GetPosition() );
	driver.SetShaderParameter( m_hViewProjInversHandle, driver.GetCamera().GetUnViewProj() );
	driver.SetShaderParameter( m_hLightViewProjHandle, driver.GetEnvironment().GetCamera().GetViewProj() );

	//深度纹理线性化
	//http://www.klayge.org/2011/11/27/klayge-4-0%E4%B8%ADdeferred-rendering%E7%9A%84%E6%94%B9%E8%BF%9B%EF%BC%88%E4%B8%80%EF%BC%89%EF%BC%9A%E6%B5%81%E6%B0%B4%E7%BA%BF/
	float SenceNear = driver.GetCamera().GetNear();
	float SenceFar = driver.GetCamera().GetFar();
	float LightNear = driver.GetEnvironment().GetCamera().GetNear();
	float LightFar = driver.GetEnvironment().GetCamera().GetFar();
	float sq = SenceFar / (SenceFar-SenceNear);
	float sfn = SenceFar-SenceNear;
	float lq = LightFar / (LightFar-LightNear);
	float lfn = LightFar-LightNear;
	driver.SetShaderParameter( m_hSenceNearHandle,		SenceNear );
	driver.SetShaderParameter( m_hSenceFarNearHandle,	sfn );
	driver.SetShaderParameter( m_hSenceFarNearQHandle,	sq );
	driver.SetShaderParameter( m_hLightNearHandle,		LightNear );
	driver.SetShaderParameter( m_hLightFarNearHandle,	lfn );
	driver.SetShaderParameter( m_hLightFarNearQHandle,	lq );
}
//--------------------------------------------------------------------------------------

//#define IOS
#ifdef IOS
#define LOWVOLUMELIGHT
#endif
/////////////////////////
//	VolumeLightPost	   //
/////////////////////////
//--------------------------------------------------------------------------------------
VolumeLightPost::VolumeLightPost()
{
}
//--------------------------------------------------------------------------------------
VolumeLightPost::~VolumeLightPost()
{
}
//--------------------------------------------------------------------------------------
bool VolumeLightPost::Initialization()
{
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	


	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );
#ifdef LOWVOLUMELIGHT
	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rVolumeLightPostLow ) )
		return false;
#else
	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rVolumeLightPost ) )
		return false;
#endif
	m_hExposureHandle				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "exposure" );
	m_hDecayHandle					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "decay" );
	m_hDensityHandle				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "density" );
	m_hWeightHandle					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "weight" );
	m_hLightPositionOnScreenHandle	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "LightPosOnSceneCoord" );
	m_hTextureHandle				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "myTexture" );
	m_hLightColor					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "LightColor" );
	return true;
}
//--------------------------------------------------------------------------------------
void VolumeLightPost::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	driver.SetShaderParameter( m_hTextureHandle, *m_pTexOrginial );

	const Matrix44f& mvp = driver.GetCamera().GetViewProj();
	vector4f lp( driver.GetCamera().GetPosition() + driver.GetEnvironment().GetDirection() * driver.GetCamera().GetMiddle()  );
	lp *= mvp ;//与摄像机的位置没有关系，所以始终只乘投影矩阵
	lp /= lp.m_w;
	float alpha = 1.0 - Math::Clamp0to1( ( lp.m_x*lp.m_x+lp.m_y*lp.m_y - 1.0 ) / 7.0 );//这个计算公式计算的是在看到太阳前的alpha衰减，假设太阳的曝光极致是在一个屏幕空间内，所以减去2.0，修改这里需要对应修改Environment的Unpdata
	vector2f lps = vector2f(lp.m_x * 0.5 + 0.5, lp.m_y * 0.5 + 0.5);
	driver.SetShaderParameter( m_hLightPositionOnScreenHandle, lps );

	driver.SetShaderParameter( m_hLightColor, driver.GetEnvironment().GetColor() );
	//driver.SetShaderParameter( m_hExposureHandle, 0.000042f );//最终曝光强度
	//driver.SetShaderParameter( m_hDecayHandle, 0.97f );//暗线衰减
	//driver.SetShaderParameter( m_hDensityHandle, 1.0f );//暗线的长度
	//driver.SetShaderParameter( m_hWeightHandle, 260.0f );//黑白色差权重
#ifdef LOWVOLUMELIGHT
	driver.SetShaderParameter( m_hExposureHandle, 0.00032f );//最终曝光强度
	driver.SetShaderParameter( m_hDecayHandle, 0.87f );//暗线衰减
	driver.SetShaderParameter( m_hDensityHandle, 0.6f );//暗线的长度
	driver.SetShaderParameter( m_hWeightHandle, 520.0f * alpha );//黑白色差权重
#else
	driver.SetShaderParameter( m_hExposureHandle, 0.00032f );//最终曝光强度
	driver.SetShaderParameter( m_hDecayHandle, 0.88f );//暗线衰减
	driver.SetShaderParameter( m_hDensityHandle, 0.8f );//暗线的长度
	driver.SetShaderParameter( m_hWeightHandle, 360.0f * alpha );//黑白色差权重
#endif
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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "color",		Attributes::ATTRIBUTE_COLOR0 },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	


	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "color",	Attributes::ATTRIBUTE_COLOR0 );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rFontShader ) )
		return false;
	m_hPos = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "pos" );
	m_hScale = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "scale" );
	return true;
}
//--------------------------------------------------------------------------------------
void FontShader::SetParameters ()
{
	IDriver::Instance().SetShaderParameter( m_hPos, vector4f(m_vPos.m_x,m_vPos.m_y,0,0) );
	IDriver::Instance().SetShaderParameter( m_hScale, m_vScale );
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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPipelineCombine ) )
		return false;
	m_hTexOrginialHandle = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Background" );
	m_hTexSecenHandle = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "PostImage" );
	return true;
}
//--------------------------------------------------------------------------------------
void PipelineCombine::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	driver.SetShaderParameter( m_hTexOrginialHandle, *m_pTexOrginial );
	driver.SetShaderParameter( m_hTexSecenHandle, *m_pTexSecen );
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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rQuadShader ) )
		return false;
	return true;
}
//--------------------------------------------------------------------------------------
void QuadShader::SetParameters ()
{
}
//--------------------------------------------------------------------------------------




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
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	//m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkyBox ) )
		return false;
	m_hTransform = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "mvp" );
	return true;
}
//--------------------------------------------------------------------------------------
void SkyBoxShader::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	vector3f cp = driver.GetCamera().GetPosition();

	Matrix44f mvp = *m_pTransform;	//让他始终跟随摄像机
	mvp.a41 = cp.m_x;
	mvp.a42 = cp.m_y;
	mvp.a43 = cp.m_z;

	mvp = mvp * driver.GetCamera().GetViewProj(); 

	driver.SetShaderParameter(m_hTransform, mvp);
	//clean up
	m_pTransform = NULL;
}
//--------------------------------------------------------------------------------------


/////////////////////////
//		SobelShader	   //
/////////////////////////
//--------------------------------------------------------------------------------------
SobelShader::SobelShader()
{
}
//--------------------------------------------------------------------------------------
SobelShader::~SobelShader()
{
}
//--------------------------------------------------------------------------------------
bool SobelShader::Initialization()
{
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSobel ) )
		return false;
	m_hTextureWidthInv = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_CoarseTextureWidthInv" );
	m_hTextureHeightInv = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_CoarseTextureHeightInv" );
	return true;
}
//--------------------------------------------------------------------------------------
void SobelShader::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	driver.SetShaderParameter(m_hTextureWidthInv, m_fTextureWidthInv);
	driver.SetShaderParameter(m_hTextureHeightInv, m_fTextureHeightInv);
}
//--------------------------------------------------------------------------------------




/////////////////////////
//	BlurEdgaShader	   //
/////////////////////////
//--------------------------------------------------------------------------------------
bool BlurEdgaShader::Initialization()
{
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rBlurEdge ) )
		return false;
	m_hTextureWidthInv = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_CoarseTextureWidthInv" );
	m_hTextureHeightInv = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_CoarseTextureHeightInv" );
	m_hTextureEdga = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_Edge" );
	m_hTextureScale = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_Scale" );
	return true;
}
//--------------------------------------------------------------------------------------
void BlurEdgaShader::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	driver.SetShaderParameter(m_hTextureWidthInv, m_fTextureWidthInv);
	driver.SetShaderParameter(m_hTextureHeightInv, m_fTextureHeightInv);
	driver.SetShaderParameter( m_hTextureEdga, *m_pTexEdga );
	driver.SetShaderParameter( m_hTextureScale, *m_pTexScale );
}
//--------------------------------------------------------------------------------------



/////////////////////////
//	BlurImageShader	   //
/////////////////////////
//--------------------------------------------------------------------------------------
bool BlurImageShader::Initialization()
{
	//IDriver::StrShaderAttributes pShaderAttributes[] =
	//{ 
	//	{ "position",	Attributes::ATTRIBUTE_POSITION },
	//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
	//};
	//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);


	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rBlurImage ) )
		return false;
	m_hTextureWidthInv = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "x" );
	m_hTextureHeightInv = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "y" );
	return true;
}
//--------------------------------------------------------------------------------------
void BlurImageShader::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	driver.SetShaderParameter(m_hTextureWidthInv, m_fTextureWidthInv);
	driver.SetShaderParameter(m_hTextureHeightInv, m_fTextureHeightInv);
}
//--------------------------------------------------------------------------------------



/////////////////////////////////
//	PrePixelLightInstance	   //
/////////////////////////////////
//--------------------------------------------------------------------------------------
PrePixelLightInstance::PrePixelLightInstance()
:m_hViewProjMatrixLoc(-1),
m_hLightDirectionLoc(-1),
m_hCameraPositionLoc(-1),
m_hAmbientLoc(-1),
m_hLightLoc(-1)
{
}
//--------------------------------------------------------------------------------------
bool PrePixelLightInstance::Initialization()
{
	m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );
	m_Attributes.PushAttribute( "normal",	Attributes::ATTRIBUTE_NORMAL );
	m_Attributes.PushAttribute( "matColum1",	Attributes::ATTRIBUTE_MATIRX_COLUMN1 );
	m_Attributes.PushAttribute( "matColum2",	Attributes::ATTRIBUTE_MATIRX_COLUMN2 );
	m_Attributes.PushAttribute( "matColum3",	Attributes::ATTRIBUTE_MATIRX_COLUMN3 );
	m_Attributes.PushAttribute( "matColum4",	Attributes::ATTRIBUTE_MATIRX_COLUMN4 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rPrePixelLightInstance ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "ViewProj" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	m_hAmbientLoc				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "fAmbient" );
	m_hLightLoc					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Light" );
	return true;
}
//--------------------------------------------------------------------------------------
void PrePixelLightInstance::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& light = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();
	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj());
	driver.SetShaderParameter(m_hLightDirectionLoc, -light.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );
	driver.SetShaderParameter(m_hAmbientLoc, light.GetAmbient());
	driver.SetShaderParameter(m_hLightLoc, light.GetColor());
}
//--------------------------------------------------------------------------------------






/////////////////////////
//		ParticleShader	   //
/////////////////////////
//--------------------------------------------------------------------------------------
ParticleShader::ParticleShader()
{
}
//--------------------------------------------------------------------------------------
ParticleShader::~ParticleShader()
{
}
//--------------------------------------------------------------------------------------
bool ParticleShader::Initialization()
{

	m_Attributes.PushAttribute( "g_vPosition",			Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "g_vColor",				Attributes::ATTRIBUTE_COLOR0 );
	m_Attributes.PushAttribute( "g_vColorIncrease",			Attributes::ATTRIBUTE_USER0 );
	m_Attributes.PushAttribute( "g_vS_L_I_S",				Attributes::ATTRIBUTE_USER1 );
	m_Attributes.PushAttribute( "g_vColorPow_AlphaPow_SizePow",	Attributes::ATTRIBUTE_USER2 );
	m_Attributes.PushAttribute( "g_vVelocity",				Attributes::ATTRIBUTE_USER3 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rParticleShader ) )
		return false;
	m_hMatViewPorject = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_matModelViewProj" );
	m_hTime = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_fTime" );
	m_hCameraPos = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_vCameraPos" );
	m_hWind = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_vWind" );
	m_hAcceleration = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_fAcceleration" );
	m_Texture = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_TextureSampler" );
	return true;
}
//--------------------------------------------------------------------------------------
void ParticleShader::SetParameters ()
{
	IDriver::Instance().SetShaderParameter(m_hMatViewPorject, IDriver::Instance().GetCamera().GetViewProj());
	IDriver::Instance().SetShaderParameter(m_hCameraPos, IDriver::Instance().GetCamera().GetPosition());
	Texture* tex = (Texture*)m_pMaterial->Attribute( Material::ATT_TEX_OPACITY );
	if( NULL != tex )
	{
		IDriver::Instance().SetShaderParameter(m_Texture, *tex );
	}
	IDriver::Instance().SetShaderParameter(m_hWind, *((vector3f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 )));
	IDriver::Instance().SetShaderParameter(m_hAcceleration, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 )));
	IDriver::Instance().SetShaderParameter(m_hTime, (float)(*((int*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 ))));
}
//--------------------------------------------------------------------------------------




//////////////////////////
//	SkeletonAnimation	//
//////////////////////////
//--------------------------------------------------------------------------------------
SkeletonAnimation::SkeletonAnimation()
:m_hViewProjMatrixLoc(-1),
m_hTransform(-1),
m_hLightDirectionLoc(-1),
m_hCameraPositionLoc(-1),
m_hRealArray(-1),
m_hAmbient_AmbientLight(-1),
m_hSpecular_Light(-1),
m_hLight(-1),
m_hShininess(-1)
{
}
//--------------------------------------------------------------------------------------
bool SkeletonAnimation::Initialization()
{
	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
	m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );
	//m_Attributes.PushAttribute( "tangent",		Attributes::ATTRIBUTE_TANGENT );
	m_Attributes.PushAttribute( "boneIndex",	Attributes::ATTRIBUTE_BONE_INDEX );
	m_Attributes.PushAttribute( "boneweight",		Attributes::ATTRIBUTE_BONE_WEIGHT );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonAnimation ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
	m_hTransform				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
	m_hNormalTransform			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
	m_hLightDirectionLoc			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	m_hRealArray				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Real" );
	m_hDualArray				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Dual" );
	m_hTextureBase				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );

	m_hAmbient_AmbientLight		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
	m_hSpecular_Light			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
	m_hLight					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Light" );
	m_hShininess				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
	return true;
}
//--------------------------------------------------------------------------------------
void SkeletonAnimation::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& light = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	Matrix44f* trans = (Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_TRANSFORM ));
	if( NULL != trans )
	{
		driver.SetShaderParameter(m_hTransform, *trans );
		driver.SetShaderParameter(m_hNormalTransform, Matrix33f( *trans ) );
	}
	else
	{
		Matrix44f id;
		driver.SetShaderParameter(m_hTransform, id );
		driver.SetShaderParameter(m_hNormalTransform, Matrix33f( id ) );
	}
	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj());
	driver.SetShaderParameter(m_hLightDirectionLoc, -light.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );

	std::vector<Quaternionf>* real = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_REAL ));
	std::vector<Quaternionf>* dual = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_DUAL ));

	driver.SetShaderParameter(m_hRealArray, *real);
	driver.SetShaderParameter(m_hDualArray, *dual);

	const Texture* tex = (const Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE);
	if ( NULL != tex )
	{
		driver.SetShaderParameter( m_hTextureBase, *tex );
	}
	driver.SetShaderParameter(m_hAmbient_AmbientLight, light.GetAmbient() * m_pMaterial->Ambient() );
	driver.SetShaderParameter(m_hSpecular_Light, light.GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
	driver.SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * light.GetColor() );
	driver.SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );

}
//--------------------------------------------------------------------------------------



//////////////////////////
//	SkeletonAnimation	//
//////////////////////////
//--------------------------------------------------------------------------------------
SkeletonAnimationBump::SkeletonAnimationBump()
:m_hViewProjMatrixLoc(-1),
m_hTransform(-1),
m_hLightDirectionLoc(-1),
m_hCameraPositionLoc(-1),
//m_pTransform(NULL),
m_hRealArray(-1),
m_hDualArray(-1),
m_hAmbient_AmbientLight(-1),
m_hSpecular_Light(-1),
m_hLight(-1),
m_hShininess(-1)//,
//m_RealArray(NULL),
//m_DualArray(NULL),
//m_pTexBase(NULL),
//m_pTexBump(NULL)
{
}
//--------------------------------------------------------------------------------------
SkeletonAnimationBump::~SkeletonAnimationBump()
{
}
//--------------------------------------------------------------------------------------
bool SkeletonAnimationBump::Initialization()
{
	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
	m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );
	m_Attributes.PushAttribute( "tangent",		Attributes::ATTRIBUTE_TANGENT );
	m_Attributes.PushAttribute( "boneIndex",	Attributes::ATTRIBUTE_BONE_INDEX );
	m_Attributes.PushAttribute( "boneweight",		Attributes::ATTRIBUTE_BONE_WEIGHT );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonAnimationBump ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
	m_hTransform				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
	m_hNormalTransform			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );

	m_hRealArray				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Real" );
	m_hDualArray				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Dual" );

	m_hTextureBase				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
	m_hTextureBump				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BumpTexture" );

	m_hAmbient_AmbientLight		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
	m_hSpecular_Light			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
	m_hLight					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Light" );
	m_hShininess				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
	return true;
}
//--------------------------------------------------------------------------------------
void SkeletonAnimationBump::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& light = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	//driver.SetShaderParameter(m_hTransform, *m_pTransform);
	Matrix44f* trans = (Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_TRANSFORM ));
	driver.SetShaderParameter(m_hTransform, *trans );
	driver.SetShaderParameter(m_hNormalTransform, Matrix33f( *trans ) );
	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj());
	driver.SetShaderParameter(m_hLightDirectionLoc, -light.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );

	std::vector<Quaternionf>* real = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_REAL ));
	std::vector<Quaternionf>* dual = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_DUAL ));

	driver.SetShaderParameter(m_hRealArray, *real);
	driver.SetShaderParameter(m_hDualArray, *dual);

	driver.SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
	driver.SetShaderParameter( m_hTextureBump, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_BUMP)) );
	//clean up
	//m_pTransform = NULL;
	//m_pTexBase = NULL;
	//m_pTexBump = NULL;
	driver.SetShaderParameter(m_hAmbient_AmbientLight, light.GetAmbient() * m_pMaterial->Ambient() );
	driver.SetShaderParameter(m_hSpecular_Light, light.GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
	driver.SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * light.GetColor() );
	driver.SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
}
//--------------------------------------------------------------------------------------






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

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rModelPrePixelLight ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
	m_hTransform				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
	m_hNormalTransform			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	m_hTextureBase				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );

	m_hAmbient_AmbientLight		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
	m_hSpecular_Light			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
	m_hLight					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Light" );
	m_hShininess				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
	return true;
}
//--------------------------------------------------------------------------------------
void ModelPrePixelLight::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& light = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	Matrix44f* trans = (Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_TRANSFORM ));
	if( NULL != trans )
	{
		driver.SetShaderParameter(m_hTransform, *trans );
		driver.SetShaderParameter(m_hNormalTransform, Matrix33f( *trans ) );
	}
	else
	{
		Matrix44f id;
		driver.SetShaderParameter(m_hTransform, id );
		driver.SetShaderParameter(m_hNormalTransform, Matrix33f( id ) );
	}
	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj());
	driver.SetShaderParameter(m_hLightDirectionLoc, -light.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );

	driver.SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );

	driver.SetShaderParameter(m_hAmbient_AmbientLight, light.GetAmbient() * m_pMaterial->Ambient() );
	driver.SetShaderParameter(m_hSpecular_Light, light.GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
	driver.SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * light.GetColor() );
	driver.SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
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

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rModelBump ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
	m_hTransform				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
	m_hNormalTransform			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	m_hTextureBase				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
	m_hTextureBump				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BumpTexture" );

	m_hAmbient_AmbientLight		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
	m_hSpecular_Light			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
	m_hLight					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Light" );
	m_hShininess				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
	return true;
}
//--------------------------------------------------------------------------------------
void ModelBump::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& light = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	Matrix44f* trans = (Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_TRANSFORM ));
	if( NULL != trans )
	{
		driver.SetShaderParameter(m_hTransform, *trans );
		driver.SetShaderParameter(m_hNormalTransform, Matrix33f( *trans ) );
	}
	else
	{
		Matrix44f id;
		driver.SetShaderParameter(m_hTransform, id );
		driver.SetShaderParameter(m_hNormalTransform, Matrix33f( id ) );
	}
	driver.SetShaderParameter(m_hNormalTransform, Matrix33f( *trans ) );
	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj());
	driver.SetShaderParameter(m_hLightDirectionLoc, -light.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );
	driver.SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
	driver.SetShaderParameter( m_hTextureBump, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_BUMP)) );

	driver.SetShaderParameter(m_hAmbient_AmbientLight, light.GetAmbient() * m_pMaterial->Ambient() );
	driver.SetShaderParameter(m_hSpecular_Light, light.GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
	driver.SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * light.GetColor() );
	driver.SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
}
//--------------------------------------------------------------------------------------








//////////////////////////
//		TerrainCDLOD	//
//////////////////////////
//--------------------------------------------------------------------------------------
TerrainCDLOD::TerrainCDLOD()
:m_hViewProjMatrixLoc(-1),
m_hLightDirectionLoc(-1),
m_hCameraPositionLoc(-1),
m_hAmbient_AmbientLight(-1),
m_hSpecular_Light(-1),
m_hLight(-1),
m_hShininess(-1),
m_hTextureBase(-1),
m_hDetalTexture(-1),
m_hMorphXY_SteperScaleInvers_DoubleChunkSize(-1),
m_hSizeScale_SizeScaleRepeat(-1),
m_hDetailIndex(-1),
m_hOffsetScale(-1),
m_hDetalDistanceInv(-1),
m_hTextureWeight(-1),
m_hRTexture(-1),
m_hGTexture(-1),
m_hBTexture(-1),
m_hTextureDiffuse(-1),
m_hShadowMap(-1),
m_ShadowWorldViewProjBaisLoc(-1)
{
}
//--------------------------------------------------------------------------------------
bool TerrainCDLOD::Initialization()
{
	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "diffHeight",	Attributes::ATTRIBUTE_TEXCOORD0 );
	m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rTerrainCDLOD ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	m_hTextureBase				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
	m_hTextureDiffuse			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "DiffuseTexture" );
	m_hDetalTexture				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "DetalTexture" );
	m_hTextureWeight			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "WeightTexture" );
	m_hRTexture					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "RTexture" );
	m_hGTexture					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "GTexture" );
	m_hBTexture					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BTexture" );
	m_hAmbient_AmbientLight		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
	m_hSpecular_Light			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
	m_hLight					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Light" );
	m_hShininess				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
	m_hDetailIndex				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "dIndex" );
	m_hOffsetScale				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vOffsetScale" );
	m_hDetalDistanceInv			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "DetailDistanceInve_Tile" );
	m_hSizeScale_SizeScaleRepeat= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vSizeScaleInv_SizeScaleRepeatInv" );
	m_hMorphXY_SteperScaleInvers_DoubleChunkSize	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vM_S_C" );
	m_hShadowMap				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "ShadowMap" );


	m_ShadowWorldViewProjBaisLoc= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matShadowWorldViewProjBais" );
	return true;
}
//--------------------------------------------------------------------------------------
void TerrainCDLOD::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& light = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj());
	driver.SetShaderParameter(m_hLightDirectionLoc, -light.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );

	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
	{
		driver.SetShaderParameter( m_hTextureDiffuse, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_WEIGHT) )
	{
		driver.SetShaderParameter( m_hTextureWeight, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_WEIGHT)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_DETAL) )
	{
		driver.SetShaderParameter( m_hDetalTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_DETAL)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_BASE) )
	{
		driver.SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_BASE)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_R) )
	{
		driver.SetShaderParameter( m_hRTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_R)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_G) )
	{
		driver.SetShaderParameter( m_hGTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_G)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_B) )
	{
		driver.SetShaderParameter( m_hBTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_B)) );
	}

	driver.SetShaderParameter(m_hAmbient_AmbientLight, light.GetAmbient() * m_pMaterial->Ambient() );
	driver.SetShaderParameter(m_hSpecular_Light, light.GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
	driver.SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * light.GetColor() );
	driver.SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
	driver.SetShaderParameter(m_hSizeScale_SizeScaleRepeat, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 ) ) );
	driver.SetShaderParameter(m_hMorphXY_SteperScaleInvers_DoubleChunkSize, *((vector4f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 ) ) );
	driver.SetShaderParameter(m_hDetailIndex, *((int*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 ) ) );
	driver.SetShaderParameter(m_hOffsetScale, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_3 ) ) );
	driver.SetShaderParameter(m_hDetalDistanceInv, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_4 ) ) );

	//阴影相关
	driver.SetShaderParameter( m_hShadowMap, light.GetDepthTexture() );
	Matrix44f Bais( 0.5, 0.0, 0.0, 0.0,
					0.0, 0.5, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.5, 0.5, 0.0, 1.0 );
	Matrix44f PBais = light.GetCamera().GetViewProj() * Bais;
	driver.SetShaderParameter(m_ShadowWorldViewProjBaisLoc, PBais );
}
//--------------------------------------------------------------------------------------








//////////////////////////
//	TerrainCDLODNoShadow//
//////////////////////////
//--------------------------------------------------------------------------------------
TerrainCDLODNoShadow::TerrainCDLODNoShadow()
:m_hViewProjMatrixLoc(-1),
m_hLightDirectionLoc(-1),
m_hCameraPositionLoc(-1),
m_hAmbient_AmbientLight(-1),
m_hSpecular_Light(-1),
m_hLight(-1),
m_hShininess(-1),
m_hTextureBase(-1),
m_hDetalTexture(-1),
m_hMorphXY_SteperScaleInvers_DoubleChunkSize(-1),
m_hSizeScale_SizeScaleRepeat(-1),
m_hDetailIndex(-1),
m_hOffsetScale(-1),
m_hDetalDistanceInv(-1),
m_hTextureWeight(-1),
m_hRTexture(-1),
m_hGTexture(-1),
m_hBTexture(-1),
m_hTextureDiffuse(-1)
{
}
//--------------------------------------------------------------------------------------
bool TerrainCDLODNoShadow::Initialization()
{
	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "diffHeight",	Attributes::ATTRIBUTE_TEXCOORD0 );
	m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rTerrainCDLODNoShadow ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	m_hTextureBase				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
	m_hTextureDiffuse			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "DiffuseTexture" );
	m_hDetalTexture				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "DetalTexture" );
	m_hTextureWeight			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "WeightTexture" );
	m_hRTexture					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "RTexture" );
	m_hGTexture					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "GTexture" );
	m_hBTexture					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BTexture" );
	m_hAmbient_AmbientLight		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
	m_hSpecular_Light			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
	m_hLight					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Light" );
	m_hShininess				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
	m_hDetailIndex				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "dIndex" );
	m_hOffsetScale				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vOffsetScale" );
	m_hDetalDistanceInv			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "DetailDistanceInve_Tile" );
	m_hSizeScale_SizeScaleRepeat= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vSizeScaleInv_SizeScaleRepeatInv" );
	m_hMorphXY_SteperScaleInvers_DoubleChunkSize	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vM_S_C" );
	return true;
}
//--------------------------------------------------------------------------------------
void TerrainCDLODNoShadow::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& light = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj());
	driver.SetShaderParameter(m_hLightDirectionLoc, -light.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );

	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
	{
		driver.SetShaderParameter( m_hTextureDiffuse, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_WEIGHT) )
	{
		driver.SetShaderParameter( m_hTextureWeight, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_WEIGHT)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_DETAL) )
	{
		driver.SetShaderParameter( m_hDetalTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_DETAL)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_BASE) )
	{
		driver.SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_BASE)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_R) )
	{
		driver.SetShaderParameter( m_hRTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_R)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_G) )
	{
		driver.SetShaderParameter( m_hGTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_G)) );
	}
	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_B) )
	{
		driver.SetShaderParameter( m_hBTexture, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_TERRAIN_B)) );
	}

	driver.SetShaderParameter(m_hAmbient_AmbientLight, light.GetAmbient() * m_pMaterial->Ambient() );
	driver.SetShaderParameter(m_hSpecular_Light, light.GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
	driver.SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * light.GetColor() );
	driver.SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
	driver.SetShaderParameter(m_hSizeScale_SizeScaleRepeat, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 ) ) );
	driver.SetShaderParameter(m_hMorphXY_SteperScaleInvers_DoubleChunkSize, *((vector4f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 ) ) );
	driver.SetShaderParameter(m_hDetailIndex, *((int*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 ) ) );
	driver.SetShaderParameter(m_hOffsetScale, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_3 ) ) );
	driver.SetShaderParameter(m_hDetalDistanceInv, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_4 ) ) );
}
//--------------------------------------------------------------------------------------








//////////////////////////
//	TerrainCDLODLow		//
//////////////////////////
//--------------------------------------------------------------------------------------
TerrainCDLODLow::TerrainCDLODLow()
:m_hViewProjMatrixLoc(-1),
m_hTextureDiffuse(-1),
m_hMorphXY_SteperScaleInvers_DoubleChunkSize(-1),
m_hSizeScale_SizeScaleRepeat(-1),
m_hDetailIndex(-1),
m_hOffsetScale(-1),
m_hCameraPositionLoc(-1),
m_hFogColor(-1),
m_hFogParam(-1)
{
}
//--------------------------------------------------------------------------------------
bool TerrainCDLODLow::Initialization()
{
	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "diffHeight",	Attributes::ATTRIBUTE_TEXCOORD0 );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rTerrainCDLODLow ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	m_hTextureDiffuse			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
	m_hDetailIndex				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "dIndex" );
	m_hOffsetScale				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vOffsetScale" );
	m_hSizeScale_SizeScaleRepeat= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vSizeScaleInv_SizeScaleRepeatInv" );
	m_hMorphXY_SteperScaleInvers_DoubleChunkSize	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vM_S_C" );
	m_hFogColor					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
	m_hFogParam					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
	return true;
}
//--------------------------------------------------------------------------------------
void TerrainCDLODLow::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& env = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj());
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );

	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
	{
		driver.SetShaderParameter( m_hTextureDiffuse, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
	}
	driver.SetShaderParameter(m_hSizeScale_SizeScaleRepeat, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 ) ) );
	driver.SetShaderParameter(m_hMorphXY_SteperScaleInvers_DoubleChunkSize, *((vector4f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 ) ) );
	driver.SetShaderParameter(m_hDetailIndex, *((int*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 ) ) );
	driver.SetShaderParameter(m_hOffsetScale, *((vector2f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_3 ) ) );

	driver.SetShaderParameter(m_hFogColor, env.GetFogColor() );
	driver.SetShaderParameter(m_hFogParam, env.GetFogParam() );
}
//--------------------------------------------------------------------------------------







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
m_ShadowWorldViewProjBaisLoc(-1)
//,m_hNormal(-1)
{
}
//--------------------------------------------------------------------------------------
bool WaterShader::Initialization()
{
	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
	//m_Attributes.PushAttribute( "texture",		Attributes::ATTRIBUTE_TEXCOORD0 );
	m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rWaterGrid ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	m_hTime						= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Time" );
	m_hSkyBox					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "SkyBox" );
	//m_hNormal					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NormalMap" );
	m_hDepth					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "WorldDepth" );
	m_hNearQ_Q					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NeraQ_Q" );
	m_hScene					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "SceneTex" );
	m_hWaterColorTex			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "WaterColorTex" );
	//m_hFordWaterColor			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vFordWaterColor" );
	//m_hDepthWaterColor			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vDepthWaterColor" );
	m_hSpecular_Light			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
	m_hShininess				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
	m_hCanSeeDepth				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "CanSeeDepth" );
	m_hNoise					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "WaterNoise" );
	m_hNoiseColor				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NoiseColor" );
	m_hNoiseTile				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Tile" );
	m_hNoiseStrength			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NoiseStrength" );
	m_hReflrefr_offset			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "reflrefr_offset" );
	m_hColorChangeDepthInv		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "ColorChangeDepthInv" );
	m_hShadowMap				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "ShadowMap" );
	m_ShadowWorldViewProjBaisLoc= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matShadowWorldViewProjBais" );
	m_hFogColor					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
	m_hFogParam					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
	m_hNoiseWaveSpeed			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "WaveSpeed" );
	m_hBump						= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Bumpmap" );
	m_hBumpScale				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_BumpScale" );
	m_hBumpReduic				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_BumpReduic" );
	m_hBumpStrength				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_BumpStrength" );
	return true;
}
//--------------------------------------------------------------------------------------
void WaterShader::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& env = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	driver.SetShaderParameter(m_hTime, static_cast<float>(driver.Time())/1000.0f );
	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj());
	driver.SetShaderParameter(m_hLightDirectionLoc, -env.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );

	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_OPACITY) )
	{
		driver.SetShaderParameter( m_hNoise, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_OPACITY)) );
	}
	//else
	//{
	//	driver.SetShaderParameter( m_hNoise, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );//Edit的时候防止崩溃的
	//}


	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
	{
		driver.SetShaderParameter( m_hWaterColorTex, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
	}
	//if( NULL != driver.GetSecenManage().GetSkyBox().Texture() )
	//{
	//	driver.SetShaderParameter( m_hSkyBox, *driver.GetSecenManage().GetSkyBox().Texture() );
	//}
	driver.SetShaderParameter( m_hSkyBox, *driver.GetPipeline().GetReflection() );
	if ( NULL != m_pMaterial->Attribute(Material::ATT_TEX_BUMP) )
	{
		driver.SetShaderParameter( m_hBump, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_BUMP)) );
	}
	driver.SetShaderParameter( m_hDepth, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );
	driver.SetShaderParameter( m_hScene, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_REFRACTION)) );
	driver.SetShaderParameter( m_hNoiseColor, m_pMaterial->Diffuse() * ( env.GetColor() + env.GetAmbient() ) );

	//driver.SetShaderParameter(m_hFordWaterColor, m_pMaterial->Diffuse() * ( light.GetColor() + light.GetAmbient() ) );//海水的颜色还是和光的颜色有关的
	//driver.SetShaderParameter(m_hDepthWaterColor, m_pMaterial->Ambient() * ( light.GetColor() + light.GetAmbient() ) );//海水两个颜色，浅滩一个，深得地方一个

	driver.SetShaderParameter(m_hSpecular_Light, env.GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
	driver.SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
	driver.SetShaderParameter(m_hNearQ_Q, driver.GetCamera().GetNearQ_Q() );
	driver.SetShaderParameter(m_hReflrefr_offset, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 )) );
	driver.SetShaderParameter(m_hCanSeeDepth, 1.0f/ *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 )) );//100是能看的深度
	driver.SetShaderParameter(m_hNoiseTile, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 )) );
	driver.SetShaderParameter(m_hColorChangeDepthInv, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_3 )) );
	driver.SetShaderParameter(m_hNoiseStrength, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_4 )) );
	driver.SetShaderParameter(m_hNoiseWaveSpeed, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_5 )) );


	driver.SetShaderParameter(m_hBumpScale, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_6 )) );
	//float farinv = *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_7 ));
	//farinv = 1.0 / ( farinv * camera.GetFar() );
	//driver.SetShaderParameter(m_hBumpFarInv, farinv );
	driver.SetShaderParameter(m_hBumpStrength, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_8 )) );
	driver.SetShaderParameter(m_hFogColor, env.GetFogColor() );
	driver.SetShaderParameter(m_hFogParam, env.GetFogParam() );

	//阴影相关
	driver.SetShaderParameter( m_hShadowMap, env.GetDepthTexture() );
	Matrix44f Bais( 0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.5, 0.5, 0.0, 1.0 );
	Matrix44f mvpBais = env.GetCamera().GetViewProj() * Bais;
	driver.SetShaderParameter(m_ShadowWorldViewProjBaisLoc, mvpBais );
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

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rWaterGridNoShadow ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
	m_hTime						= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Time" );
	m_hSkyBox					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "SkyBox" );
	//m_hNormal					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NormalMap" );
	m_hDepth					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "WorldDepth" );
	m_hNearQ_Q					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NeraQ_Q" );
	m_hScene					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "SceneTex" );
	m_hWaterColorTex			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "WaterColorTex" );
	//m_hFordWaterColor			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vFordWaterColor" );
	//m_hDepthWaterColor			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vDepthWaterColor" );
	m_hSpecular_Light			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
	m_hShininess				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
	m_hCanSeeDepth				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "CanSeeDepth" );
	m_hNoise					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "WaterNoise" );
	m_hNoiseColor				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NoiseColor" );
	m_hNoiseTile				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Tile" );
	m_hNoiseStrength			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "NoiseStrength" );
	m_hReflrefr_offset			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "reflrefr_offset" );
	m_hColorChangeDepthInv		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "ColorChangeDepthInv" );
	m_hFogColor					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
	m_hFogParam					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
	m_hNoiseWaveSpeed			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "WaveSpeed" );
	m_hBump						= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Bumpmap" );
	m_hBumpScale				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_BumpScale" );
	m_hBumpReduic				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_BumpReduic" );
	m_hBumpStrength				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_BumpStrength" );
	return true;
}
//--------------------------------------------------------------------------------------
void WaterShaderNoShadow::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& env = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	driver.SetShaderParameter(m_hTime, static_cast<float>(driver.Time())/1000.0f );
	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj());
	driver.SetShaderParameter(m_hLightDirectionLoc, -env.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );

	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_OPACITY) )
	{
		driver.SetShaderParameter( m_hNoise, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_OPACITY)) );
	}
	//else
	//{
	//	driver.SetShaderParameter( m_hNoise, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );//Edit的时候防止崩溃的
	//}


	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
	{
		driver.SetShaderParameter( m_hWaterColorTex, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
	}
	//if( NULL != driver.GetSecenManage().GetSkyBox().Texture() )
	//{
	//	driver.SetShaderParameter( m_hSkyBox, *driver.GetSecenManage().GetSkyBox().Texture() );
	//}
	driver.SetShaderParameter( m_hSkyBox, *driver.GetPipeline().GetReflection() );
	if ( NULL != m_pMaterial->Attribute(Material::ATT_TEX_BUMP) )
	{
		driver.SetShaderParameter( m_hBump, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_BUMP)) );
	}
	driver.SetShaderParameter( m_hDepth, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );
	driver.SetShaderParameter( m_hScene, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_REFRACTION)) );
	driver.SetShaderParameter( m_hNoiseColor, m_pMaterial->Diffuse() * ( env.GetColor() + env.GetAmbient() ) );

	//driver.SetShaderParameter(m_hFordWaterColor, m_pMaterial->Diffuse() * ( light.GetColor() + light.GetAmbient() ) );//海水的颜色还是和光的颜色有关的
	//driver.SetShaderParameter(m_hDepthWaterColor, m_pMaterial->Ambient() * ( light.GetColor() + light.GetAmbient() ) );//海水两个颜色，浅滩一个，深得地方一个

	driver.SetShaderParameter(m_hSpecular_Light, env.GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
	driver.SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
	driver.SetShaderParameter(m_hNearQ_Q, driver.GetCamera().GetNearQ_Q() );
	driver.SetShaderParameter(m_hReflrefr_offset, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 )) );
	driver.SetShaderParameter(m_hCanSeeDepth, 1.0f/ *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 )) );//100是能看的深度
	driver.SetShaderParameter(m_hNoiseTile, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 )) );
	driver.SetShaderParameter(m_hColorChangeDepthInv, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_3 )) );
	driver.SetShaderParameter(m_hNoiseStrength, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_4 )) );
	driver.SetShaderParameter(m_hNoiseWaveSpeed, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_5 )) );

	//bump
	driver.SetShaderParameter(m_hBumpScale, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_6 )) );
	//float farinv = *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_7 ));
	//farinv = 1.0 / ( farinv * camera.GetFar() );
	//driver.SetShaderParameter(m_hBumpFarInv, farinv );
	driver.SetShaderParameter(m_hBumpStrength, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_8 )) );

	driver.SetShaderParameter(m_hFogColor, env.GetFogColor() );
	driver.SetShaderParameter(m_hFogParam, env.GetFogParam() );
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
//	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rHighmapGenerateShader ) )
//		return false;
//	// Make a record of the location for each shader constant
//	m_hScale		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "fScale" );
//	m_hNoise1		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Noise1" );
//	m_hNoise2		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Noise2" );
//
//	return true;
//}
////--------------------------------------------------------------------------------------
//void HighmapGenerateShader::SetParameters ()
//{
//	IDriver& driver = IDriver::Instance();
//	driver.SetShaderParameter(m_hScale, *((float*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_0)) );
//	driver.SetShaderParameter( m_hNoise1, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );
//	driver.SetShaderParameter( m_hNoise2, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER1)) );
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
//	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rHNormalmapGenerateShader ) )
//		return false;
//	// Make a record of the location for each shader constant
//	m_hStep			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "fStep" );
//	m_hcorner00		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "corner00" );
//	m_hcorner01		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "corner01" );
//	m_hcorner10		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "corner10" );
//	m_hcorner11		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "corner11" );
//	m_hAmplitude	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "amplitude" );
//	m_hHeightmap	= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "hsampler" );
//
//	return true;
//}
////--------------------------------------------------------------------------------------
//void NormalmapGenerateShader::SetParameters ()
//{
//	IDriver& driver = IDriver::Instance();
//	driver.SetShaderParameter(m_hStep, *((float*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_1)) );
//	driver.SetShaderParameter(m_hAmplitude, *((float*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_2)) );
//	driver.SetShaderParameter(m_hcorner00, *((vector4f*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_3)) );
//	driver.SetShaderParameter(m_hcorner01, *((vector4f*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_4)) );
//	driver.SetShaderParameter(m_hcorner10, *((vector4f*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_5)) );
//	driver.SetShaderParameter(m_hcorner11, *((vector4f*)m_pMaterial->Attribute(IAttributeNode::ATT_USER_6)) );
//	driver.SetShaderParameter( m_hHeightmap, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_USER0)) );
//}
////--------------------------------------------------------------------------------------






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

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rCopyDepthToRBGAShader ) )
		return false;
	return true;
}
//--------------------------------------------------------------------------------------
void CopyDepthToRBGAShader::SetParameters ()
{
}
////--------------------------------------------------------------------------------------





//////////////////////////
//		TreeTrunk		//
//////////////////////////
//--------------------------------------------------------------------------------------
TreeTrunk::TreeTrunk()
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
bool TreeTrunk::Initialization()
{
	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
	m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rTreeTrunkShader ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCameraPos" );
	m_hTextureBase				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
	m_hAmbient_AmbientLight		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vAmbient" );
	m_hLightColor				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightColor" );
	m_hSpecularLight			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vSpecularLight" );
	m_hShininess				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
	m_hFogColor					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
	m_hFogParam					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
	m_hFadeAlpha				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FadeAlpha" );
	return true;
}
//--------------------------------------------------------------------------------------
void TreeTrunk::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& env = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj() );
	driver.SetShaderParameter(m_hLightDirectionLoc, -env.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );
	driver.SetShaderParameter(m_hAmbient_AmbientLight, env.GetAmbient() * m_pMaterial->Ambient() );
	driver.SetShaderParameter(m_hLightColor, m_pMaterial->Diffuse() * env.GetColor() );
	driver.SetShaderParameter(m_hSpecularLight, env.GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
	driver.SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
	driver.SetShaderParameter(m_hFogColor, env.GetFogColor() );
	driver.SetShaderParameter(m_hFogParam, env.GetFogParam() );
	driver.SetShaderParameter(m_hFadeAlpha, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 )) );

	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
	{
		driver.SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
	}
}
//--------------------------------------------------------------------------------------






//////////////////////////
//		TreeLeaf		//
//////////////////////////
//--------------------------------------------------------------------------------------
TreeLeaf::TreeLeaf()
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
bool TreeLeaf::Initialization()
{
	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
	m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rTreeLeafShader ) )
		return false;
	// Make a record of the location for each shader constant
	m_hViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
	m_hLightDirectionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
	m_hCameraPositionLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vCameraPos" );
	m_hTextureBase				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
	m_hAmbient_AmbientLight		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vAmbient" );
	m_hLightColor				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vLightColor" );
	m_hSpecularLight			= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "vSpecularLight" );
	m_hShininess				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
	m_hFogColor					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FogColor" );
	m_hFogParam					= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FogParams" );
	m_hFadeAlpha				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_FadeAlpha" );
	return true;
}
//--------------------------------------------------------------------------------------
void TreeLeaf::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	Environment& env = driver.GetEnvironment();
	ICamera&	camera = driver.GetCamera();

	driver.SetShaderParameter(m_hViewProjMatrixLoc, camera.GetViewProj() );
	driver.SetShaderParameter(m_hLightDirectionLoc, -env.GetDirection());//phong光是像素到光源的方向，所以这里反向
	driver.SetShaderParameter(m_hCameraPositionLoc, camera.GetPosition() );
	driver.SetShaderParameter(m_hAmbient_AmbientLight, env.GetAmbient() * m_pMaterial->Ambient() );
	driver.SetShaderParameter(m_hLightColor, m_pMaterial->Diffuse() * env.GetColor() );
	driver.SetShaderParameter(m_hSpecularLight, env.GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
	driver.SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
	driver.SetShaderParameter(m_hFogColor, env.GetFogColor() );
	driver.SetShaderParameter(m_hFogParam, env.GetFogParam() );
	driver.SetShaderParameter(m_hFadeAlpha, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 )) );

	if( NULL != m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
	{
		driver.SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
	}
}
//--------------------------------------------------------------------------------------




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

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rGaussianDepth ) )
		return false;
	m_hStepSize = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_vStepSize" );
	return true;
}
//--------------------------------------------------------------------------------------
void GaussianDepth::SetParameters ()
{
	IDriver::Instance().SetShaderParameter( m_hStepSize, m_vStep );
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

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rGaussianDepthLow ) )
		return false;
	m_hStepSize = IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "g_vStepSize" );
	return true;
}
//--------------------------------------------------------------------------------------
void GaussianDepthLow::SetParameters ()
{
	IDriver::Instance().SetShaderParameter( m_hStepSize, m_vStep );
}
//--------------------------------------------------------------------------------------





//////////////////////////////
//	SkeletonAnimationDepth	//
//////////////////////////////
//--------------------------------------------------------------------------------------
SkeletonAnimationDepth::SkeletonAnimationDepth()
:m_ShadowWorldViewProjBaisLoc(-1),
m_hRealArray(-1),
m_hDualArray(-1)
{
}
//--------------------------------------------------------------------------------------
bool SkeletonAnimationDepth::Initialization()
{
	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
	m_Attributes.PushAttribute( "boneIndex",	Attributes::ATTRIBUTE_BONE_INDEX );
	m_Attributes.PushAttribute( "boneweight",		Attributes::ATTRIBUTE_BONE_WEIGHT );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonAnimationDepth ) )
		return false;
	// Make a record of the location for each shader constant
	m_ShadowWorldViewProjBaisLoc= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
	m_hRealArray				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Real" );
	m_hDualArray				= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "Dual" );
	return true;
}
//--------------------------------------------------------------------------------------
void SkeletonAnimationDepth::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	ICamera&	camera = driver.GetCamera();

	Matrix44f* trans = (Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_TRANSFORM ));
	if( NULL != trans )
	{
		driver.SetShaderParameter(m_ShadowWorldViewProjBaisLoc, *trans * camera.GetViewProj() );
	}
	else
	{
		driver.SetShaderParameter(m_ShadowWorldViewProjBaisLoc, camera.GetViewProj() );
	}
	std::vector<Quaternionf>* real = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_REAL ));
	std::vector<Quaternionf>* dual = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_DUAL ));

	driver.SetShaderParameter(m_hRealArray, *real);
	driver.SetShaderParameter(m_hDualArray, *dual);
}
//--------------------------------------------------------------------------------------




//////////////////////////
//		ModelDepth		//
//////////////////////////
//--------------------------------------------------------------------------------------
ModelDepth::ModelDepth()
:m_hWorldViewProjMatrixLoc(-1)
{
}
//--------------------------------------------------------------------------------------
bool ModelDepth::Initialization()
{
	m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );

	if( false == IDriver::Instance().CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rModelDepth ) )
		return false;
	// Make a record of the location for each shader constant
	m_hWorldViewProjMatrixLoc		= IDriver::Instance().GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
	return true;
}
//--------------------------------------------------------------------------------------
void ModelDepth::SetParameters ()
{
	IDriver& driver = IDriver::Instance();
	ICamera&	camera = driver.GetCamera();

	Matrix44f* trans = (Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_TRANSFORM ));
	if( NULL != trans )
	{
		driver.SetShaderParameter(m_hWorldViewProjMatrixLoc, *trans * camera.GetViewProj() );
	}
	else
	{
		driver.SetShaderParameter(m_hWorldViewProjMatrixLoc, camera.GetViewProj() );
	}
}
//--------------------------------------------------------------------------------------






/////////////////////////
//		ShaderManage   //
/////////////////////////
ShaderManage::ShaderManage()
{
	//m_mpShaderMap.insert( make_pair( PrePixelLight,  NEW class PrePixelLight() ) );
	//m_mpShaderMap.insert( make_pair( PrePixelLightTransform,  NEW class PrePixelLightTransform() ) );
	//m_mpShaderMap.insert( make_pair( ShadowVolume,  NEW class ShadowVolume() ) );
	//m_mpShaderMap.insert( make_pair( ShadowVolumeInstance,  NEW class ShadowVolumeInstance() ) );
	//m_mpShaderMap.insert( make_pair( Constant,  NEW class Constant() ) );
	m_mpShaderMap.insert( make_pair( UIShader,  NEW class UIShader() ) );
	m_mpShaderMap.insert( make_pair( ShapesShader,  NEW class ShapesShader() ) );
	m_mpShaderMap.insert( make_pair( Luminance,  NEW class Luminance() ) );
	m_mpShaderMap.insert( make_pair( Gaussian,  NEW class Gaussian() ) );
	m_mpShaderMap.insert( make_pair( Combine,  NEW class Combine() ) );
	//m_mpShaderMap.insert( make_pair( VolumeLight,  NEW class VolumeLight() ) );
	m_mpShaderMap.insert( make_pair( VolumeLightPost,  NEW class VolumeLightPost() ) );
	m_mpShaderMap.insert( make_pair( FontShader,  NEW class FontShader() ) );
	m_mpShaderMap.insert( make_pair( ParticleShader,  NEW class ParticleShader() ) );
	m_mpShaderMap.insert( make_pair( PipelineCombine,  NEW class PipelineCombine() ) );
	m_mpShaderMap.insert( make_pair( QuadShader,  NEW class QuadShader() ) );
	m_mpShaderMap.insert( make_pair( SkyBoxShader,  NEW class SkyBoxShader() ) );
	m_mpShaderMap.insert( make_pair( SobelShader,  NEW class SobelShader() ) );
	m_mpShaderMap.insert( make_pair( BlurEdgaShader,  NEW class BlurEdgaShader() ) );
	m_mpShaderMap.insert( make_pair( BlurImageShader,  NEW class BlurImageShader() ) );
	//m_mpShaderMap.insert( make_pair( PrePixelLightInstance,  NEW class PrePixelLightInstance() ) );
	m_mpShaderMap.insert( make_pair( SkeletonAnimation,  NEW class SkeletonAnimation() ) );
	m_mpShaderMap.insert( make_pair( SkeletonAnimationBump,  NEW class SkeletonAnimationBump() ) );
	m_mpShaderMap.insert( make_pair( ModelPrePixelLight,  NEW class ModelPrePixelLight() ) );
	m_mpShaderMap.insert( make_pair( ModelBump,  NEW class ModelBump() ) );
	m_mpShaderMap.insert( make_pair( TerrainCDLOD,  NEW class TerrainCDLOD() ) );
	m_mpShaderMap.insert( make_pair( TerrainCDLODNoShadow,  NEW class TerrainCDLODNoShadow() ) );
	m_mpShaderMap.insert( make_pair( TerrainCDLODLow,  NEW class TerrainCDLODLow() ) );
	//m_mpShaderMap.insert( make_pair( TerrainLineCDLOD,  NEW class TerrainLineCDLOD() ) );
	m_mpShaderMap.insert( make_pair( WaterShader,  NEW class WaterShader() ) );
	m_mpShaderMap.insert( make_pair( WaterShaderNoShadow,  NEW class WaterShaderNoShadow() ) );
	//m_mpShaderMap.insert( make_pair( HighmapGenerateShader,  NEW class HighmapGenerateShader() ) );
	//m_mpShaderMap.insert( make_pair( NormalmapGenerateShader,  NEW class NormalmapGenerateShader() ) );
	m_mpShaderMap.insert( make_pair( CopyDepthToRBGAShader,  NEW class CopyDepthToRBGAShader() ) );
	m_mpShaderMap.insert( make_pair( TreeTrunk,  NEW class TreeTrunk() ) );
	m_mpShaderMap.insert( make_pair( TreeLeaf,  NEW class TreeLeaf() ) );
	m_mpShaderMap.insert( make_pair( GaussianDepth,  NEW class GaussianDepth() ) );
	m_mpShaderMap.insert( make_pair( SkeletonAnimationDepth,  NEW class SkeletonAnimationDepth() ) );
	m_mpShaderMap.insert( make_pair( ModelDepth,  NEW class ModelDepth() ) );
	m_mpShaderMap.insert( make_pair( GaussianDepthLow,  NEW class GaussianDepthLow() ) );

	for ( ShaderMap::iterator it = m_mpShaderMap.begin() ;
		it != m_mpShaderMap.end();
		it ++ )
	{
		it->second->Initialization();
	}
}
//-------------------------------------------------------------------
ShaderManage::~ShaderManage()
{
	for ( ShaderMap::iterator it = m_mpShaderMap.begin() ;
		it != m_mpShaderMap.end();
		it ++ )
	{
		SAFE_DELETE( it->second );
	}
}
//-------------------------------------------------------------------
void ShaderManage::InsertShader( int index, Shader* s )
{
	ShaderMap::iterator it = m_mpShaderMap.find( index );
	ASSERT( it == m_mpShaderMap.end() );
	m_mpShaderMap.insert( make_pair( index, s ) );
	s->Initialization();
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------