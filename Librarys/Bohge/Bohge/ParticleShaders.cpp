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
//		Á£×ÓµÄshader		 //
///////////////////////////////
#include "ParticleShaders.h"
#include "Device.h"
#include "Engine.h"
#include "EngineResourceList.h"
#include "Camera.h"
#include "Material.h"



namespace BohgeEngine
{
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

		m_Attributes.PushAttribute( "g_vPosition",					Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "g_vColor",						Attributes::ATTRIBUTE_COLOR0 );
		m_Attributes.PushAttribute( "g_vS_L_I_S",					Attributes::ATTRIBUTE_USER_0 );
		m_Attributes.PushAttribute( "g_vVelocity",					Attributes::ATTRIBUTE_USER_1 );
		m_Attributes.PushAttribute( "g_vColorIncrease",				Attributes::ATTRIBUTE_USER_2 );
		m_Attributes.PushAttribute( "g_vColorPow_AlphaPow_SizePow",	Attributes::ATTRIBUTE_USER_3 );

		if( false == Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rParticleShader ) )
			return false;
		m_hMatViewPorject =		Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_matModelViewProj" );
		m_hTime =				Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_fTime" );
		m_hCameraPos =			Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_vCameraPos" );
		m_hWind =				Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_vWind" );
		m_hAcceleration =		Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_fAcceleration" );
		m_Texture =				Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "g_TextureSampler" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void ParticleShader::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		device->SetShaderParameter(m_hMatViewPorject, engine.GetCamera()->GetViewProj());
		device->SetShaderParameter(m_hCameraPos, engine.GetCamera()->GetPosition());
		Texture* tex = (Texture*)m_pMaterial->Attribute( Material::ATT_TEX_DIFFUSE );
		if( NULL != tex )
		{
			device->SetShaderParameter(m_Texture, *tex );
		}
		device->SetShaderParameter(m_hWind, *((vector3f*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_0 )));
		device->SetShaderParameter(m_hAcceleration, *((float*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_1 )));
		device->SetShaderParameter(m_hTime, (float)(*((int*)m_pMaterial->Attribute( IAttributeNode::ATT_USER_2 ))));
	}
	//--------------------------------------------------------------------------------------

}