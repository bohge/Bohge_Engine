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



/////////////////////
//		真光	   //
/////////////////////

#include "RealLights.h"
#include "ShadowCaster.h"
#include "Billboard.h"
#include "Camera.h"

namespace BohgeEngine
{
	/////////////////////
	//		平行光	   //
	/////////////////////
	//-------------------------------------------------------------------------------------------------------
	RealDirectionLight::RealDirectionLight()
		:RealLightNode( LightNode::LT_DIRECTIONAL )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void RealDirectionLight::_NewShadowCaster()
	{
		ASSERT( NULL == m_pShadowCaster );
		m_pShadowCaster = NEW DirectLightCaster();
		m_pShadowCaster->Initialization( *this );
	}
	//-------------------------------------------------------------------------------------------------------
	void RealDirectionLight::Update( uint milisecon )
	{
		RealLightNode::Update( milisecon );
		SetLocalPosition( GetCaster().GetFirstCamera().GetPosition() );
		ICamera& maincamera = *Engine::Instance().GetCamera();
		const vector3f& pos = maincamera.GetPosition(); //改变灯光光晕
		float cfar =  maincamera.GetFar() * 0.98;
		float size = cfar * GetHaloSize().m_x * 5.5;
		m_pLighthalo->SetBillBorad( pos - GetWorldDirection() * cfar, 
			vector2f( size, size * GetHaloSize().m_y * 1.5 ) );
	}
	//-------------------------------------------------------------------------------------------------------
	bool RealDirectionLight::IsCollisionWith( const aabbox3df& bindbox )
	{
		return true;
	}


	/////////////////////
	//		点光	   //
	/////////////////////
	//-------------------------------------------------------------------------------------------------------
	RealPointLight::RealPointLight()
		:RealLightNode( LightNode::LT_POINT )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void RealPointLight::_NewShadowCaster()
	{
		ASSERT( NULL == m_pShadowCaster );
		m_pShadowCaster = NEW PointLightCaster();
		m_pShadowCaster->Initialization( *this );
	}
	//-------------------------------------------------------------------------------------------------------
	void RealPointLight::Update( uint milisecon )
	{
		bool ischange = isLightChange();
		RealLightNode::Update( milisecon );
		if ( ischange )
		{
			const float& r = GetRange();
			m_BindBox = aabbox3df( GetWorldPosition()-vector3f(r,r,r), GetWorldPosition()+vector3f(r,r,r) );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool RealPointLight::IsCollisionWith( const aabbox3df& bindbox )
	{
		return m_BindBox.Intersection( bindbox );
	}




	/////////////////////
	//		聚光灯	   //
	/////////////////////
	//-------------------------------------------------------------------------------------------------------
	RealSpotLight::RealSpotLight()
		:RealLightNode( LightNode::LT_SPOT )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void RealSpotLight::_NewShadowCaster()
	{
		ASSERT( NULL == m_pShadowCaster );
		m_pShadowCaster = NEW SpotLightCaster();
		m_pShadowCaster->Initialization( *this );
	}
	//-------------------------------------------------------------------------------------------------------
	void RealSpotLight::Update( uint milisecon )
	{
		bool ischange = isLightChange();
		RealLightNode::Update( milisecon );
		if ( ischange )//计算视锥
		{
			m_LightFrustum.FromMatrix( 
				Math::ViewMatrix( GetWorldPosition(), GetWorldPosition()+GetWorldDirection(), GetWorldUp() )
				* Math::ProjectMatrix( GetLightAngle().m_x, 1.0f, 0.1f, GetRange() )
				);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool RealSpotLight::IsCollisionWith( const aabbox3df& bindbox )
	{
		return m_LightFrustum.Intersect( bindbox );
	}

}