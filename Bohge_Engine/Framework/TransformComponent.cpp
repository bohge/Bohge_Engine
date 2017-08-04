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
#include "TransformComponent.h"
#include "NodeMessage.h"
#include "Node.h"


namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT(TransformComponent);
	//-------------------------------------------------------------------------------------------------------
	TransformComponent::TransformComponent(void)
		:m_LocalScale(1,1,1),
		m_isPositionChange(true),
		m_isRotationChange(true),
		m_isScaleChange(true),
		m_isWorldRotationChange(true),
		m_isWorldTransfromChange(true),
		m_isWorldTransfromInversChange(true)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	TransformComponent::~TransformComponent(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::_DoOnAttachNode( )
	{
		//连接数据源提供者
		_HostNode()->RegisterAttributeProvider( NodeProperty::GA_WORLD_SCALE, MEMBER_FUNC_PTR( &TransformComponent::AttributeWorldScale ) );
		_HostNode()->RegisterAttributeProvider( NodeProperty::GA_WORLD_ROTATION, MEMBER_FUNC_PTR( &TransformComponent::AttributeWorldRotation ) );
		_HostNode()->RegisterAttributeProvider( NodeProperty::GA_WORLD_POSITION, MEMBER_FUNC_PTR( &TransformComponent::AttributeWorldPosition ) );
		_HostNode()->RegisterAttributeProvider( NodeProperty::GA_WORLD_TRANSFORM, MEMBER_FUNC_PTR( &TransformComponent::AttributeWorldTransform ) );
		_HostNode()->RegisterAttributeProvider( NodeProperty::GA_WORLD_TRANSFORM_INVERS, MEMBER_FUNC_PTR( &TransformComponent::AttributeWorldTransformInvers ) );
		_HostNode()->RegisterAttributeProvider( NodeProperty::GA_NORMAL_TRANSFORM, MEMBER_FUNC_PTR( &TransformComponent::AttributeWorldRotationMatrix ) );

		//连接变更监听
		_HostNode()->RegisterMessageListener( NodeProperty::SA_SCALE, MEMBER_FUNC_PTR( &TransformComponent::MessageLoaclScale ) );
		_HostNode()->RegisterMessageListener( NodeProperty::SA_POSITION, MEMBER_FUNC_PTR( &TransformComponent::MessageLoaclPosition ) );
		_HostNode()->RegisterMessageListener( NodeProperty::SA_ROTATION, MEMBER_FUNC_PTR( &TransformComponent::MessageLoaclRotation ) );
	}
	//-------------------------------------------------------------------------------------------------------
	const Object* TransformComponent::AttributeWorldPosition() const
	{
		return &GetWorldPosition();
	}
	//-------------------------------------------------------------------------------------------------------
	const Object* TransformComponent::AttributeWorldScale() const
	{
		return &GetWorldScale();
	}
	//-------------------------------------------------------------------------------------------------------
	const Object* TransformComponent::AttributeWorldRotation() const
	{
		return &GetWorldRotation();
	}
	//-------------------------------------------------------------------------------------------------------
	const Object* TransformComponent::AttributeWorldTransform() const
	{
		return &GetWorldTransform();
	}
	//-------------------------------------------------------------------------------------------------------
	const Object* TransformComponent::AttributeWorldRotationMatrix() const
	{
		return &GetWorldRotationMatrix();
	}
	//-------------------------------------------------------------------------------------------------------
	const Object* TransformComponent::AttributeWorldTransformInvers() const
	{
		return &GetWorldTransformInvers();
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::MessageLoaclRotation( const NodeMessage& message )
	{
		if ( message.isValidMessage( this ) )
		{
			_DoSetLocalRotation( message.GetData<Quaternionf>() );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::MessageLoaclPosition( const NodeMessage& message )
	{
		if ( message.isValidMessage( this ) )
		{
			_DoSetLocalPosition( message.GetData<vector3f>() );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::MessageLoaclScale( const NodeMessage& message )
	{
		if ( message.isValidMessage( this ) )
		{
			_DoSetLocalScale( message.GetData<vector3f>() );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::OnRootNodeChange()
	{
		m_isWorldTransfromInversChange = true;
		m_isWorldRotationChange = true;
		m_isWorldTransfromChange = true;
		m_isScaleChange = true;
		m_isRotationChange = true;
		m_isPositionChange = true;
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::_DoSetLocalRotation( const Quaternionf& r )
	{
		m_isWorldTransfromChange = true;
		m_isWorldRotationChange = true;
		m_isRotationChange = true;
		m_LocalRotation = r;
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::_DoSetLocalPosition( const vector3f& p )
	{
		m_isWorldTransfromChange = true;
		m_isWorldRotationChange = true;
		m_isPositionChange= true;
		m_LocalPosition = p;
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::_DoSetLocalScale( const vector3f& s )
	{
		m_isWorldTransfromChange = true;
		m_isWorldRotationChange = true;
		m_isScaleChange= true;
		m_LocalScale = s;
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::SetLocalRotation( const Quaternionf& r )
	{
		_DoSetLocalRotation( r );
		_HostNode()->SendMessage( NodeMessage( this, &r, NodeProperty::SA_ROTATION ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::SetLocalPosition( const vector3f& p )
	{
		_DoSetLocalPosition( p );
		_HostNode()->SendMessage( NodeMessage( this, &p, NodeProperty::SA_POSITION ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::SetLocalScale( const vector3f& s )
	{
		_DoSetLocalScale( s );
		_HostNode()->SendMessage( NodeMessage( this, &s, NodeProperty::SA_SCALE ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::SetWorldRotation( const Quaternionf& r )
	{
		Quaternionf rot = r;
		const Quaternionf* faRot = static_cast<const Quaternionf*>
			( _HostNode()->RootAttribute( NodeProperty::GA_WORLD_ROTATION ) );
		if ( NULL != faRot )
		{
			rot = *faRot;
			rot.InverseSelf();
			rot = rot * r;
		}
		SetLocalRotation( rot );
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::SetWorldPosition( const vector3f& p )
	{
		vector3f pos = p;
		const vector3f* faPos = static_cast<const vector3f*>
			( _HostNode()->RootAttribute( NodeProperty::GA_WORLD_POSITION ) );
		if ( NULL != faPos )
		{
			pos = *faPos;
			pos =  p - pos;
		}
		SetLocalPosition( pos );
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::SetWorldScale( const vector3f& s )
	{
		vector3f sca = s;
		const vector3f* faSca = static_cast<const vector3f*>
			( _HostNode()->RootAttribute( NodeProperty::GA_WORLD_SCALE ) );
		if ( NULL != faSca )
		{
			sca = *faSca;
			sca = s / sca;
		}
		SetLocalScale( sca );
	}
	//-------------------------------------------------------------------------------------------------------
	const vector3f& TransformComponent::GetWorldPosition() const
	{
		if ( m_isPositionChange )
		{
			const void* data;
			m_isPositionChange = false;
			data = _HostNode()->RootAttribute( NodeProperty::GA_WORLD_SCALE );
			vector3f fatherscale = NULL == data ? vector3f(1,1,1) : *((const vector3f*)data);
			data = _HostNode()->RootAttribute( NodeProperty::GA_WORLD_ROTATION );
			Quaternionf rot = NULL == data ? Quaternionf(0,0,0,1) : *((const Quaternionf*)data);
			data = _HostNode()->RootAttribute( NodeProperty::GA_WORLD_POSITION );
			m_WorldPosition = NULL == data ? m_LocalPosition * fatherscale : m_LocalPosition * fatherscale * rot + *((const vector3f*)data);
		}
		return m_WorldPosition;
	}
	//-------------------------------------------------------------------------------------------------------
	const vector3f& TransformComponent::GetWorldScale() const
	{
		if ( m_isScaleChange )
		{
			const void* data;
			m_isScaleChange = false;
			data = _HostNode()->RootAttribute( NodeProperty::GA_WORLD_SCALE );
			m_WorldScale = NULL == data ? m_LocalScale : m_LocalScale * *((const vector3f*)data);
		}
		return m_WorldScale;
	}
	//-------------------------------------------------------------------------------------------------------
	const Quaternionf& TransformComponent::GetWorldRotation() const
	{
		if ( m_isRotationChange )
		{
			const void* data;
			m_isRotationChange = false;
			data = _HostNode()->RootAttribute( NodeProperty::GA_WORLD_ROTATION );
			m_WorldRotation = NULL == data ? m_LocalRotation : m_LocalRotation * *((const Quaternionf*)data);
		}
		return m_WorldRotation;
	}
	//-------------------------------------------------------------------------------------------------------
	const Matrix44f& TransformComponent::GetWorldTransform() const
	{
		if ( m_isWorldTransfromChange )
		{
			m_isWorldTransfromChange = false;
			m_isWorldTransfromInversChange = true;
			m_WorldTransfrom = 
				Math::Transformation<float>(
				NULL, NULL,
				&GetWorldScale(),
				NULL,
				&GetWorldRotation(),
				&GetWorldPosition());
		}
		return m_WorldTransfrom;
	}
	//-------------------------------------------------------------------------------------------------------
	const Matrix33f& TransformComponent::GetWorldRotationMatrix() const
	{
		if ( m_isWorldRotationChange )
		{
			m_isWorldRotationChange = false;
			m_WorldRotationMatrix = GetWorldRotation().ToMatrix33();
		}
		return m_WorldRotationMatrix;
	}
	//-------------------------------------------------------------------------------------------------------
	const Matrix44f& TransformComponent::GetWorldTransformInvers() const
	{
		if ( m_isWorldTransfromInversChange )
		{
			m_isWorldTransfromInversChange = false;
			GetWorldTransform().GetInverse( m_WorldTransfromInvers );
		}
		return m_WorldTransfromInvers;
	}
	//-------------------------------------------------------------------------------------------------------
	void TransformComponent::Release()
	{
	}
}