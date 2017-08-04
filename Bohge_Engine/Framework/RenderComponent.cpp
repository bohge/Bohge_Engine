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
#include "IRenderQueueSystem.h"
#include "RenderComponent.h"


#include "Node.h"



namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( RenderComponent );
	//---------------------------------------------------------------------------------------------------------
	RenderComponent::RenderComponent(void)
		:m_eRenderProperty( RenderObjectProperty::RP_CULL | RenderObjectProperty::RP_SHADOW | RenderObjectProperty::RP_SHOW | RenderObjectProperty::RP_LIGHT ),
		m_pRenderObject( NEW RenderObjectEntity ),
		m_pUserData(NULL),
		m_isDirtyBindBox(false),
		m_BinaryIndex(0),
		m_isVisible(false)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	RenderComponent::~RenderComponent(void)
	{
		SAFE_DELETE( m_pRenderObject );
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::_DoOnAttachNode()
	{
		if ( m_pRenderObject->isReady() )
		{
			IRenderQueueSystem::Instance()->AttachRenderComponent( this );
		}
		else
		{
			m_pRenderObject->ConnectLoadedEvent( MEMBER_FUNC_PTR( &RenderComponent::OnEntityLoaded ) );
		}
		_HostNode()->RegisterMessageListener( NodeProperty::SA_POSITION, MEMBER_FUNC_PTR( &RenderComponent::MessagePosition ) );
		_HostNode()->RegisterMessageListener( NodeProperty::SA_SCALE, MEMBER_FUNC_PTR( &RenderComponent::MessageScale ) );
		m_isDirtyBindBox = true;
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::OnEntityLoaded( const SmartPtr<IAsyncSource>& sou )
	{
		if ( m_isDirtyBindBox )
		{
			m_isDirtyBindBox = false;
			_DoCaculateBindbox();
		}
		IRenderQueueSystem::Instance()->AttachRenderComponent( this );
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::MessageScale( const NodeMessage& message )
	{
		m_isDirtyBindBox = true;
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::MessagePosition( const NodeMessage& message )
	{
		m_isDirtyBindBox = true;
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::_DoCaculateBindbox()
	{
		if ( isRenderProperty( RenderObjectProperty::RP_CULL ) )
		{
			const vector3f* sca = _HostNode()->Attribute<vector3f>( NodeProperty::GA_WORLD_SCALE );
			vector3f min = m_OgiBindBox.GetMin();
			vector3f max = m_OgiBindBox.GetMax();
			m_TransBindBox[!m_BinaryIndex].SetMin( min * *sca );
			m_TransBindBox[!m_BinaryIndex].SetMax( max * *sca );
			const vector3f* pos = _HostNode()->Attribute<vector3f>( NodeProperty::GA_WORLD_POSITION );
			m_TransBindBox[!m_BinaryIndex].SetCenter( *pos );
			m_BinaryIndex = !m_BinaryIndex;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::TryRecaculateBindBox()
	{
		if ( m_isDirtyBindBox )
		{
			m_isDirtyBindBox = false;
			_DoCaculateBindbox();
			IRenderQueueSystem::Instance()->RenderComponentBindChange( this );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::OnRootNodeChange()
	{
		if ( isRenderProperty( RenderObjectProperty::RP_CULL ) )
		{
			m_isDirtyBindBox = true;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::Release()
	{
		IRenderQueueSystem::Instance()->DetachRenderComponent( this );
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::SetJobType( IJob::JobType jt )
	{
		m_pRenderObject->SetJobType( jt );
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::PushMetadata( const IMetadata& meta )
	{
		m_pRenderObject->PushMetadata( meta );
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::SetTextureEntity( MaterialProperty::Material_Attribute att, const TextureEntity* te )
	{
		m_pRenderObject->SetTextureEntity( att, te );
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::SetUserParameters( MaterialProperty::Material_Attribute att, MaterialProperty::UserParameterType spt, const Object& obj )
	{
		m_pRenderObject->SetUserParameters( att, spt, obj );
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::SetUserParametersPtr( MaterialProperty::Material_Attribute att, MaterialProperty::UserParameterType spt, const Object* ptr )
	{
		m_pRenderObject->SetUserParametersPtr( att, spt, ptr );
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::DependenceSource( IAsyncSource* source )
	{
		m_pRenderObject->DependenceSource( source );
	}
	//---------------------------------------------------------------------------------------------------------
	void RenderComponent::ConnectLoadedEvent( Delegate1<void, const SmartPtr<IAsyncSource>& >::FunctionType* ptr )
	{
		m_pRenderObject->ConnectLoadedEvent( ptr );
	}

}
