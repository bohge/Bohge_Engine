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
#include "UIWidget.h"



using namespace BohgeEngine;
namespace BGUI
{
	RTTI_TYPE_IMPLEMENT( UIWidget );
	//------------------------------------------------------------------------------
	UIWidget::UIWidget(void)
		:m_Status(WidgetProperty::WS_ACTIVE),
		m_pUITransform( NEW WidgetComponent ),
		m_MaterialHash(-1)
	{
		AttachComponent( m_pUITransform );
	}
	//------------------------------------------------------------------------------
	UIWidget::~UIWidget(void)
	{
		ReleaseComponent();
		ReleaseChild();
		SAFE_DELETE( m_pUITransform );
	}
	//------------------------------------------------------------------------------
	void UIWidget::SetDesignRectangle( const BohgeEngine::vector4d& size )
	{
		m_pUITransform->SetLocalDesignRect( size );
	}
	//------------------------------------------------------------------------------
	void UIWidget::SetActualRectangle( const BohgeEngine::vector4d& size )
	{
		m_pUITransform->SetWorldActualRect( size );
	}
	//------------------------------------------------------------------------------
	void UIWidget::SetLocalPosition( const BohgeEngine::vector2d& pos )
	{
		m_pUITransform->SetLocalPosition( pos );
	}
	//------------------------------------------------------------------------------
	void UIWidget::ReleaseChild()
	{
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			static_cast<UIWidget*>(subnode->second)->ReleaseChild();
			SAFE_DELETE( subnode->second );
		}
		_GetSubnodeMap().clear();
	}
	//------------------------------------------------------------------------------
	void UIWidget::FlushMaterials( MaterialMap& mm )
	{
		_DoFlushMaterials( mm );
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			static_cast< UIWidget* >( subnode->second )->FlushMaterials( mm );
		}
	}
	//------------------------------------------------------------------------------
	void UIWidget::FlushBuffer( uint id, BohgeEngine::VertexStream* buff )
	{
		if( _isMaterial( id ) )
		{
			_DoFlushBuffer( buff );
		}
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			static_cast< UIWidget* >( subnode->second )->FlushBuffer( id, buff );
		}
	}
	//------------------------------------------------------------------------------
	void UIWidget::FlushParameter( uint id, BohgeEngine::vector4arrayf& arr )
	{
		if( _isMaterial( id ) )
		{
			_DoFlushParameter( arr );
		}
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			static_cast< UIWidget* >( subnode->second )->FlushParameter( id, arr );
		}
	}
	//------------------------------------------------------------------------------
	const Widgets_1_Array* UIWidget::Load_1( const BohgeEngine::IJsonDataSet& data )
	{
		return NULL;
	}
	//------------------------------------------------------------------------------
	bool UIWidget::_DoOnActionEvent( const BohgeEngine::TouchInfo& touch )
	{
		return false;
	}
	//------------------------------------------------------------------------------
	void UIWidget::_NoticeStatusChange( uint id )
	{
		m_ChangeEvent.Multicast( id );
		if ( _GetRoot() )
		{
			static_cast< UIWidget* >( _GetRoot() )->_NoticeStatusChange( id );
		}
	}
	//------------------------------------------------------------------------------
	bool UIWidget::OnActionEvent( const BohgeEngine::TouchInfo& touch )
	{
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			if ( static_cast< UIWidget* >( subnode->second )->OnActionEvent( touch ) )
			{
				return true;
			}
		}
		return _DoOnActionEvent( touch );
	}
	//------------------------------------------------------------------------------
	void UIWidget::SetResolution( const BohgeEngine::vector2f& res )
	{
		m_pUITransform->SetResolution( res );
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			static_cast< UIWidget* >( subnode->second )->SetResolution( res );
		}
	}
	//------------------------------------------------------------------------------
	void UIWidget::CreateResource( IAsyncSource* host )
	{
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			static_cast< UIWidget* >( subnode->second )->CreateResource( host );
		}
		_DoOnCreateResource( host );
	}
	//------------------------------------------------------------------------------
	bool UIWidget::FlushLayout( uint id, BohgeEngine::VertexStream* buff )
	{
		if( _isMaterial( id ) )
		{
			_DoFlushLayout( buff );
			return true;
		}
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			if( static_cast< UIWidget* >( subnode->second )->FlushLayout( id, buff ) )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	uint UIWidget::_DoFlushGeometryIndex( uint index )
	{
		return index;
	}
	//------------------------------------------------------------------------------
	uint UIWidget::FlushGeometryIndex( uint id, uint index )
	{
		if( _isMaterial( id ) )
		{
			index = _DoFlushGeometryIndex( index );
		}
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			index = static_cast< UIWidget* >( subnode->second )->FlushGeometryIndex( id, index );
		}
		return index;
	}
	//------------------------------------------------------------------------------
	bool UIWidget::FlushTextureEntiy( uint id, BohgeEngine::RenderComponent* com )
	{
		if( _isMaterial( id ) )
		{
			_DoFlushTextureEntiy( com );
			return true;
		}
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			if( static_cast< UIWidget* >( subnode->second )->FlushTextureEntiy( id, com ) )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	UIWidget* UIWidget::FindWidget( const eastl::string& name )
	{
		if ( name == GetName() )
		{
			return this;
		}
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			UIWidget* wid = static_cast< UIWidget* >( subnode->second )->FindWidget( name );
			if ( NULL != wid )
			{
				return wid;
			}
		}
		return NULL;
	}
	//------------------------------------------------------------------------------
	void UIWidget::FlushBind( BohgeEngine::Aabrectf& bind )
	{
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			static_cast< UIWidget* >( subnode->second )->FlushBind( bind );
		}
		_DoFlushBind( bind );
	}
	//------------------------------------------------------------------------------
	void UIWidget::_SetMaterialHash( uint hash )
	{
		m_MaterialHash = hash;
	}
	//------------------------------------------------------------------------------
	bool UIWidget::isDelayFlush( uint id )
	{
		if( _isMaterial( id )
			&& _DoIsDelayFlush() )
		{			
			return true;
		}
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			if( static_cast< UIWidget* >( subnode->second )->isDelayFlush( id ) )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	bool UIWidget::_DoIsDelayFlush()
	{
		return false;
	}
	//------------------------------------------------------------------------------
	uint UIWidget::DepthOffset( uint id, uint depth )
	{
		uint d;
		if( _isMaterial( id ) )
		{
			return depth;
		}
		++ depth;
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			d = static_cast< UIWidget* >( subnode->second )->DepthOffset( id, depth );
			if ( -1 != d )
			{
				return d;
			}
		}
		return -1;
	}
	//------------------------------------------------------------------------------
	void UIWidget::_ReflushTexture()
	{
		if( _GetRoot() )
		{
			static_cast<UIWidget*>( _GetRoot() )->_ReflushTexture();
		}
	}
	//------------------------------------------------------------------------------
	void UIWidget::_ReflushBuffer( uint hash )
	{
		if( _GetRoot() )
		{
			static_cast<UIWidget*>( _GetRoot() )->_ReflushBuffer( hash );
		}
	}
	//------------------------------------------------------------------------------
	void UIWidget::Enable()
	{
		m_Status = WidgetProperty::WS_ACTIVE;
		_NoticeStatusChange( m_MaterialHash );
	}
	//------------------------------------------------------------------------------
	void UIWidget::Disable()
	{
		m_Status = WidgetProperty::WS_DISABLED;
		_NoticeStatusChange( m_MaterialHash );
	}
	//------------------------------------------------------------------------------
	void UIWidget::Hide()
	{
		m_Status = WidgetProperty::WS_HIDE;
		_NoticeStatusChange( m_MaterialHash );
	}
	//------------------------------------------------------------------------------
	void UIWidget::EnableWithChild()
	{
		Enable();
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			static_cast< UIWidget* >( subnode->second )->EnableWithChild( );
		}
	}
	//------------------------------------------------------------------------------
	void UIWidget::DisableWithChild()
	{
		Disable();
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			static_cast< UIWidget* >( subnode->second )->DisableWithChild( );
		}
	}
	//------------------------------------------------------------------------------
	void UIWidget::HideWithChild()
	{
		Hide();
		FOR_EACH( ComponentProperty::SubnodeMap, subnode, _GetSubnodeMap() )
		{
			static_cast< UIWidget* >( subnode->second )->HideWithChild( );
		}
	}
}