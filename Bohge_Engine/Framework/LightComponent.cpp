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
#include "IEnvironmentSystem.h"
#include "LightComponent.h"
#include "DirectionLight.h"
#include "AmbientLight.h"
#include "GlobalDefine.h"
#include "NodeMessage.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Node.h"



namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT(LightComponent);
	//---------------------------------------------------------------------------------------------------------
	LightComponent::LightComponent( PipelineProperty::LightType lt )
		:m_eLightType(lt),
		m_MaxScale(0.0f),
		m_LightRange(0.0f),
		m_isLightChange(false),
		m_pLightEntity(NULL),
		m_Color(0.2,0.2,0.2),
		m_LightAngle(PI/4,PI/3),
		m_Attenuation(1,0,0,0)
	{
		switch ( m_eLightType )
		{
		case PipelineProperty::LT_AMBIENT: m_pLightEntity = NEW AmbientLight(this); break;
		case PipelineProperty::LT_DIRECTIONAL: m_pLightEntity = NEW DirectionLight(this); break;
		case PipelineProperty::LT_POINT: m_pLightEntity = NEW PointLight(this); break;
		case PipelineProperty::LT_SPOT: m_pLightEntity = NEW SpotLight(this); break;
		default:ASSERT(false);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	LightComponent::~LightComponent(void)
	{
		SAFE_DELETE( m_pLightEntity );
	}
	//---------------------------------------------------------------------------------------------------------
	void LightComponent::_DoOnAttachNode()
	{
		//连接变更监听
		_HostNode()->
			RegisterMessageListener( 
			NodeProperty::SA_SCALE, 
			MEMBER_FUNC_PTR( &LightComponent::MessageLoaclScale ) );
		_HostNode()->
			RegisterMessageListener( 
			NodeProperty::SA_POSITION, 
			MEMBER_FUNC_PTR( &LightComponent::MessageLoaclPosition ) );
		_HostNode()->
			RegisterMessageListener( 
			NodeProperty::SA_ROTATION, 
			MEMBER_FUNC_PTR( &LightComponent::MessageLoaclRotation ) );
		IEnvironmentSystem::Instance()->AttachLightComponent( this );
	}
	//---------------------------------------------------------------------------------------------------------
	void LightComponent::OnRootNodeChange()
	{
		m_isLightChange = true;
	}
	//---------------------------------------------------------------------------------------------------------
	void LightComponent::Release()
	{
		IEnvironmentSystem::Instance()->DetachLightComponent( this );
	}
	//---------------------------------------------------------------------------------------------------------
	void LightComponent::MessageLoaclRotation( const NodeMessage& message )
	{
		m_isLightChange = true;
	}
	//---------------------------------------------------------------------------------------------------------
	void LightComponent::MessageLoaclPosition( const NodeMessage& message )
	{
		m_isLightChange = true;
	}
	//---------------------------------------------------------------------------------------------------------
	void LightComponent::MessageLoaclScale( const NodeMessage& message )
	{
		m_isLightChange = true;
	}
	//---------------------------------------------------------------------------------------------------------
	void LightComponent::TryRecaculateLight()
	{
		if ( m_isLightChange )
		{
			m_isLightChange = false;
			const Quaternionf* rot = _HostNode()->Attribute<Quaternionf>( NodeProperty::GA_WORLD_ROTATION );
			const vector3f* ws = _HostNode()->Attribute<vector3f>( NodeProperty::GA_WORLD_SCALE );
			m_MaxScale = Math::GetMax( ws->m_x, Math::GetMax( ws->m_y, ws->m_z ) );
			m_WorldDirection = GlobalDefine::Vector3_DefualtForward * *rot;
			m_WorldDirection.NormalizeSelf();
			m_WorldUp = GlobalDefine::Vector3_DefualtUp * *rot;
			m_WorldUp.NormalizeSelf();
			m_pLightEntity->RecaculateLight( );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	const vector3f& LightComponent::GetWorldPosition() const
	{
		return *_HostNode()->Attribute<vector3f>( NodeProperty::GA_WORLD_POSITION );
	}
	//---------------------------------------------------------------------------------------------------------
	void LightComponent::SetLocalDirection( const vector3f& dir )
	{
		Quaternionf rot;
		rot.AxisToAxis( GlobalDefine::Vector3_DefualtForward, dir );
		_HostNode()->SendMessage( NodeMessage( this, &rot, NodeProperty::SA_ROTATION ) );
	}

}