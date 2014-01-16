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
//		体积光的shader		 //
///////////////////////////////
#include "VolumeLightShaders.h"
#include "Device.h"
#include "Engine.h"
#include "EngineResourceList.h"
#include "Environment.h"
#include "Camera.h"
#include "Light.h"
#include "ShadowCaster.h"



namespace BohgeEngine
{
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
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	


		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rVolumeLight ) )
			return false;
		m_hTexOrginDepthHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_SenceDepth" );
		m_hTexLightDepthHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_LightDepth" );
		m_hCameraPostionHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_CameraPostion" );
		m_hViewProjInversHandle = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_matViewProjInvers" );
		m_hLightViewProjHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_matLightViewProj" );

		m_hSenceNearHandle		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_SenceNear" );
		m_hSenceFarNearHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_SenceFarNear" );
		m_hSenceFarNearQHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_SenceFarNearQ" );
		m_hLightNearHandle		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_LightNear" );
		m_hLightFarNearHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_LightFarNear" );
		m_hLightFarNearQHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_LightFarNearQ" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void VolumeLight::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		device->SetShaderParameter( m_hTexOrginDepthHandle, *m_pTexOrginial );
		device->SetShaderParameter( m_hTexLightDepthHandle, *m_pTexLight );
		device->SetShaderParameter( m_hCameraPostionHandle, engine.GetCamera()->GetPosition() );
		device->SetShaderParameter( m_hViewProjInversHandle, engine.GetCamera()->GetUnViewProj() );
		device->SetShaderParameter( m_hLightViewProjHandle, engine.GetEnvironment()->GetCurrentLight().GetCaster().GetCamera(0).GetViewProj() );

		//深度纹理线性化
		//http://www.klayge.org/2011/11/27/klayge-4-0%E4%B8%ADdeferred-rendering%E7%9A%84%E6%94%B9%E8%BF%9B%EF%BC%88%E4%B8%80%EF%BC%89%EF%BC%9A%E6%B5%81%E6%B0%B4%E7%BA%BF/
		float SenceNear = engine.GetCamera()->GetNear();
		float SenceFar = engine.GetCamera()->GetFar();
		float LightNear = engine.GetEnvironment()->GetCurrentLight().GetCaster().GetCamera(0).GetNear();
		float LightFar = engine.GetEnvironment()->GetCurrentLight().GetCaster().GetCamera(0).GetFar();
		float sq = SenceFar / (SenceFar-SenceNear);
		float sfn = SenceFar-SenceNear;
		float lq = LightFar / (LightFar-LightNear);
		float lfn = LightFar-LightNear;
		device->SetShaderParameter( m_hSenceNearHandle,		SenceNear );
		device->SetShaderParameter( m_hSenceFarNearHandle,	sfn );
		device->SetShaderParameter( m_hSenceFarNearQHandle,	sq );
		device->SetShaderParameter( m_hLightNearHandle,		LightNear );
		device->SetShaderParameter( m_hLightFarNearHandle,	lfn );
		device->SetShaderParameter( m_hLightFarNearQHandle,	lq );
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
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);	


		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );
#ifdef LOWVOLUMELIGHT
		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rVolumeLightPostLow ) )
			return false;
#else
		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rVolumeLightPost ) )
			return false;
#endif
		m_hExposureHandle				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "exposure" );
		m_hDecayHandle					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "decay" );
		m_hDensityHandle				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "density" );
		m_hWeightHandle					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "weight" );
		m_hLightPositionOnScreenHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "LightPosOnSceneCoord" );
		m_hTextureHandle				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "myTexture" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void VolumeLightPost::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		device->SetShaderParameter( m_hTextureHandle, *m_pTexOrginial );

		const Matrix44f& mvp = engine.GetCamera()->GetViewProj();
		vector4f lp( engine.GetCamera()->GetPosition() + engine.GetEnvironment()->GetCurrentLight().GetWorldDirection() * engine.GetCamera()->GetMiddle()  );
		lp *= mvp ;//与摄像机的位置没有关系，所以始终只乘投影矩阵
		lp /= lp.m_w;
		float alpha = 1.0 - Math::Clamp0to1( ( lp.m_x*lp.m_x+lp.m_y*lp.m_y - 1.0 ) / 7.0 );//这个计算公式计算的是在看到太阳前的alpha衰减，假设太阳的曝光极致是在一个屏幕空间内，所以减去2.0，修改这里需要对应修改Environment的Unpdata
		vector2f lps = vector2f(lp.m_x * 0.5 + 0.5, lp.m_y * 0.5 + 0.5);
		device->SetShaderParameter( m_hLightPositionOnScreenHandle, lps );
#ifdef WIN32
		device->SetShaderParameter( m_hExposureHandle, 0.000016f );//最终曝光强度
		device->SetShaderParameter( m_hDecayHandle, 0.98f );//暗线衰减
		device->SetShaderParameter( m_hDensityHandle, 1.2f );//暗线的长度
		device->SetShaderParameter( m_hWeightHandle, 800.0f * alpha );//黑白色差权重
#else
#ifdef LOWVOLUMELIGHT
		device->SetShaderParameter( m_hExposureHandle, 0.0003f );//最终曝光强度
		device->SetShaderParameter( m_hDecayHandle, 0.87f );//暗线衰减
		device->SetShaderParameter( m_hDensityHandle, 0.6f );//暗线的长度
		device->SetShaderParameter( m_hWeightHandle, 550.0f * alpha );//黑白色差权重
#else
		device->SetShaderParameter( m_hExposureHandle, 0.00032f );//最终曝光强度
		device->SetShaderParameter( m_hDecayHandle, 0.88f );//暗线衰减
		device->SetShaderParameter( m_hDensityHandle, 0.8f );//暗线的长度
		device->SetShaderParameter( m_hWeightHandle, 320.0f * alpha );//黑白色差权重
#endif
#endif
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
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSobel ) )
			return false;
		m_hTextureWidthInv = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_CoarseTextureWidthInv" );
		m_hTextureHeightInv = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_CoarseTextureHeightInv" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void SobelShader::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		device->SetShaderParameter(m_hTextureWidthInv, m_fTextureWidthInv);
		device->SetShaderParameter(m_hTextureHeightInv, m_fTextureHeightInv);
	}
	//--------------------------------------------------------------------------------------




	/////////////////////////
	//	BlurEdgaShader	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	bool BlurEdgaShader::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);

		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rBlurEdge ) )
			return false;
		m_hTextureWidthInv = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_CoarseTextureWidthInv" );
		m_hTextureHeightInv = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_CoarseTextureHeightInv" );
		m_hTextureEdga = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_Edge" );
		m_hTextureScale = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_Scale" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void BlurEdgaShader::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		device->SetShaderParameter(m_hTextureWidthInv, m_fTextureWidthInv);
		device->SetShaderParameter(m_hTextureHeightInv, m_fTextureHeightInv);
		device->SetShaderParameter( m_hTextureEdga, *m_pTexEdga );
		device->SetShaderParameter( m_hTextureScale, *m_pTexScale );
	}
	//--------------------------------------------------------------------------------------



	/////////////////////////
	//	BlurImageShader	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------
	bool BlurImageShader::Initialization()
	{
		//Device::StrShaderAttributes pShaderAttributes[] =
		//{ 
		//	{ "position",	Attributes::ATTRIBUTE_POSITION },
		//	{ "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 }
		//};
		//const uint nNumShaderAttributes = sizeof(pShaderAttributes)/sizeof(pShaderAttributes[0]);


		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rBlur3x3Shader ) )
			return false;
		m_hTextureWidthInv = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "x" );
		m_hTextureHeightInv = Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "y" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void BlurImageShader::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		device->SetShaderParameter(m_hTextureWidthInv, m_fTextureWidthInv);
		device->SetShaderParameter(m_hTextureHeightInv, m_fTextureHeightInv);
	}
	//--------------------------------------------------------------------------------------




	//////////////////////////
	//	VolumeLightCombine	//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	VolumeLightCombine::VolumeLightCombine()
	{
	}
	//--------------------------------------------------------------------------------------
	VolumeLightCombine::~VolumeLightCombine()
	{
	}
	//--------------------------------------------------------------------------------------
	bool VolumeLightCombine::Initialization()
	{

		m_Attributes.PushAttribute( "position",	Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",	Attributes::ATTRIBUTE_TEXCOORD0 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rVolumeLightCombine ) )
			return false;
		m_hTexOrginialHandle	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "textureOriginal" );
		m_hTexSecenHandle		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "textureScene" );
		m_hColor				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "LightColor" );

		return true;
	}
	//--------------------------------------------------------------------------------------
	void VolumeLightCombine::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		device->SetShaderParameter( m_hTexOrginialHandle, *m_pTexOrginial );
		device->SetShaderParameter( m_hTexSecenHandle, *m_pTexSecen );
		device->SetShaderParameter( m_hColor, engine.GetEnvironment()->GetVolumeLightColor() );
	}
	//--------------------------------------------------------------------------------------


}