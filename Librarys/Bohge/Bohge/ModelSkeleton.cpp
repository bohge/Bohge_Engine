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
//		模型相关shader		 //
///////////////////////////////

#include "ModelSkeleton.h"
#include "Device.h"
#include "Engine.h"
#include "Environment.h"
#include "EngineResourceList.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "ShadowCaster.h"

namespace BohgeEngine
{
	//////////////////////////
	//	SkeletonModelBase	//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	SkeletonModelBase::SkeletonModelBase()
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
	bool SkeletonModelBase::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );
		//m_Attributes.PushAttribute( "tangent",		Attributes::ATTRIBUTE_TANGENT );
		m_Attributes.PushAttribute( "boneIndex",	Attributes::ATTRIBUTE_BONEINDICES );
		m_Attributes.PushAttribute( "boneweight",		Attributes::ATTRIBUTE_BONEWEIGHTS );

		if ( !_CreateShader() )
		{
			return false;
		}
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hTransform				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
		m_hNormalTransform			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
		m_hLightDirectionLoc			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );
		m_hRealArray				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Real" );
		m_hDualArray				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Dual" );
		m_hTextureBase				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );

		m_hAmbient_AmbientLight		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
		m_hSpecular_Light			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
		m_hLight					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Light" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void SkeletonModelBase::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		device->SetShaderParameter(m_hTransform, *(Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_WORLDTRANSFORM )) );
		device->SetShaderParameter(m_hNormalTransform, *(Matrix33f*)(m_pMaterial->Attribute( IAttributeNode::ATT_NORMALTRANSFORM )) );
		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );

		std::vector<Quaternionf>* real = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_REAL ));
		std::vector<Quaternionf>* dual = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_DUAL ));

		device->SetShaderParameter(m_hRealArray, *real);
		device->SetShaderParameter(m_hDualArray, *dual);

		const Texture* tex = (const Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE);
		if ( NULL != tex )
		{
			device->SetShaderParameter( m_hTextureBase, *tex );
		}
		device->SetShaderParameter(m_hAmbient_AmbientLight, env->GetAmbient() * m_pMaterial->Ambient() );
		device->SetShaderParameter(m_hSpecular_Light, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * env->GetCurrentLight().GetColor() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );

	}
	//--------------------------------------------------------------------------------------



	//////////////////////////
	//	SkeletonModelBumpBase	//
	//////////////////////////
	//--------------------------------------------------------------------------------------
	SkeletonModelBumpBase::SkeletonModelBumpBase()
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
	bool SkeletonModelBumpBase::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "texCoord",		Attributes::ATTRIBUTE_TEXCOORD0 );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );
		m_Attributes.PushAttribute( "tangent",		Attributes::ATTRIBUTE_TANGENT );
		m_Attributes.PushAttribute( "boneIndex",	Attributes::ATTRIBUTE_BONEINDICES );
		m_Attributes.PushAttribute( "boneweight",		Attributes::ATTRIBUTE_BONEWEIGHTS );

		//if( false == engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonAnimationBump ) )
		if ( !_CreateShader() )
		{
			return false;
		}
		// Make a record of the location for each shader constant
		m_hViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProj" );
		m_hTransform				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matTransform" );
		m_hNormalTransform			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
		m_hLightDirectionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vLightDir" );
		m_hCameraPositionLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vCamPos" );

		m_hRealArray				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Real" );
		m_hDualArray				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Dual" );

		m_hTextureBase				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BaseTexture" );
		m_hTextureBump				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "BumpTexture" );

		m_hAmbient_AmbientLight		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "vAmbient_Light" );
		m_hSpecular_Light			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Specular_Light" );
		m_hLight					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Light" );
		m_hShininess				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Shininess" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void SkeletonModelBumpBase::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		Environment* env = engine.GetEnvironment();
		ICamera* camera = engine.GetCamera();

		//device->SetShaderParameter(m_hTransform, *m_pTransform);
		device->SetShaderParameter(m_hTransform, *(Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_WORLDTRANSFORM )) );
		device->SetShaderParameter(m_hNormalTransform, *(Matrix33f*)(m_pMaterial->Attribute( IAttributeNode::ATT_NORMALTRANSFORM )) );
		device->SetShaderParameter(m_hViewProjMatrixLoc, camera->GetViewProj());
		device->SetShaderParameter(m_hLightDirectionLoc, -env->GetCurrentLight().GetWorldDirection());//phong光是像素到光源的方向，所以这里反向
		device->SetShaderParameter(m_hCameraPositionLoc, camera->GetPosition() );

		std::vector<Quaternionf>* real = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_REAL ));
		std::vector<Quaternionf>* dual = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_DUAL ));

		device->SetShaderParameter(m_hRealArray, *real);
		device->SetShaderParameter(m_hDualArray, *dual);

		device->SetShaderParameter( m_hTextureBase, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE)) );
		device->SetShaderParameter( m_hTextureBump, *((Texture*)m_pMaterial->Attribute(Material::ATT_TEX_BUMP)) );
		//clean up
		//m_pTransform = NULL;
		//m_pTexBase = NULL;
		//m_pTexBump = NULL;
		device->SetShaderParameter(m_hAmbient_AmbientLight, env->GetAmbient() * m_pMaterial->Ambient() );
		device->SetShaderParameter(m_hSpecular_Light, env->GetCurrentLight().GetColor() * m_pMaterial->Specular() * m_pMaterial->SpeLevel() );
		device->SetShaderParameter(m_hLight, m_pMaterial->Diffuse() * env->GetCurrentLight().GetColor() );
		device->SetShaderParameter(m_hShininess, m_pMaterial->Shininess() );
	}
	//--------------------------------------------------------------------------------------




	//////////////////////////////
	//	SkeletonModelDepthBase	//
	//////////////////////////////
	//--------------------------------------------------------------------------------------
	SkeletonModelDepthBase::SkeletonModelDepthBase()
		:m_hWorldViewProjMatrixLoc(-1),
		m_hLightPositionLoc(-1),
		m_hCameraLineDepthParam(-1),
		m_hRealArray(-1),
		m_hDualArray(-1),
		m_hWorldTransform(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool SkeletonModelDepthBase::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "boneIndex",	Attributes::ATTRIBUTE_BONEINDICES );
		m_Attributes.PushAttribute( "boneweight",		Attributes::ATTRIBUTE_BONEWEIGHTS );

		if ( !_CreateShader() )
		{
			return false;
		}
		// Make a record of the location for each shader constant
		m_hWorldViewProjMatrixLoc		= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldViewProj" );
		m_hWorldTransform				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matWorldTransform" );
		m_hLightPositionLoc				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "LightPosition" );
		m_hCameraLineDepthParam			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "CameraLineDepthParam" );
		m_hRealArray					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Real" );
		m_hDualArray					= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Dual" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void SkeletonModelDepthBase::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();

		Matrix44f& mat = *(Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_WORLDTRANSFORM ));

		device->SetShaderParameter(m_hWorldTransform, mat );
		device->SetShaderParameter(m_hWorldViewProjMatrixLoc, mat * camera->GetViewProj() );
		device->SetShaderParameter(m_hLightPositionLoc, engine.GetEnvironment()->GetCurrentLight().GetWorldPosition() );
		device->SetShaderParameter(m_hCameraLineDepthParam, camera->LineDepthParam() );	
		std::vector<Quaternionf>* real = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_REAL ));
		std::vector<Quaternionf>* dual = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_DUAL ));

		device->SetShaderParameter(m_hRealArray, *real);
		device->SetShaderParameter(m_hDualArray, *dual);
	}
	//--------------------------------------------------------------------------------------




	//////////////////////////////
	//	SkeletonModelNormalBase	//
	//////////////////////////////
	//--------------------------------------------------------------------------------------
	SkeletonModelNormalBase::SkeletonModelNormalBase()
		:m_hWorldViewProjMatrixLoc(-1),
		m_hRealArray(-1),
		m_hDualArray(-1)
	{
	}
	//--------------------------------------------------------------------------------------
	bool SkeletonModelNormalBase::Initialization()
	{
		m_Attributes.PushAttribute( "position",		Attributes::ATTRIBUTE_POSITION );
		m_Attributes.PushAttribute( "normal",		Attributes::ATTRIBUTE_NORMAL );
		m_Attributes.PushAttribute( "boneIndex",	Attributes::ATTRIBUTE_BONEINDICES );
		m_Attributes.PushAttribute( "boneweight",	Attributes::ATTRIBUTE_BONEWEIGHTS );

		if ( !_CreateShader() )
		{
			return false;
		}
		// Make a record of the location for each shader constant
		m_hWorldViewProjMatrixLoc	= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "matViewProjTransform" );
		m_hNormalTransform			= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "NormalTransform" );
		m_hRealArray				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Real" );
		m_hDualArray				= Engine::Instance().GetDevice()->GetShaderParameterHandle( m_hShaderProgram,  "Dual" );
		return true;
	}
	//--------------------------------------------------------------------------------------
	void SkeletonModelNormalBase::_SetParameters ()
	{
		Engine& engine = Engine::Instance();
		Device* device = engine.GetDevice();
		ICamera* camera = engine.GetCamera();


		device->SetShaderParameter(m_hWorldViewProjMatrixLoc, *(Matrix44f*)(m_pMaterial->Attribute( IAttributeNode::ATT_WORLDTRANSFORM )) * camera->GetViewProj() );
		device->SetShaderParameter(m_hNormalTransform, *(Matrix33f*)(m_pMaterial->Attribute( IAttributeNode::ATT_NORMALTRANSFORM )) );
		std::vector<Quaternionf>* real = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_REAL ));
		std::vector<Quaternionf>* dual = (std::vector<Quaternionf>*)(m_pMaterial->Attribute( IAttributeNode::ATT_ANIMATION_DUAL ));
		device->SetShaderParameter(m_hRealArray, *real);
		device->SetShaderParameter(m_hDualArray, *dual);
	}
	//--------------------------------------------------------------------------------------




	//具体的实体模型的shader

	bool SkeletonModel1B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModel1B );
	}

	bool SkeletonModel2B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModel2B );
	}

	bool SkeletonModel3B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModel3B );
	}

	bool SkeletonModel4B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModel4B );
	}





	bool SkeletonModelBump1B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelBump1B );
	}

	bool SkeletonModelBump2B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelBump2B );
	}

	bool SkeletonModelBump3B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelBump3B );
	}

	bool SkeletonModelBump4B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelBump4B );
	}







	bool SkeletonModelDepth1B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelDepth1B );
	}

	bool SkeletonModelDepth2B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelDepth2B );
	}

	bool SkeletonModelDepth3B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelDepth3B );
	}

	bool SkeletonModelDepth4B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelDepth4B );
	}












	bool SkeletonModelNormal1B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelNormal1B );
	}

	bool SkeletonModelNormal2B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelNormal2B );
	}

	bool SkeletonModelNormal3B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelNormal3B );
	}

	bool SkeletonModelNormal4B::_CreateShader()
	{
		return Engine::Instance().GetDevice()->CreateShaderProgram(m_hShaderProgram, m_Attributes, ShaderResource::rSkeletonModelNormal4B );
	}


}