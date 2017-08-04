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
#include "Framework/ReferenceVertexMetadata.h"
#include "Framework/SquareIndicesMetadata.h"
#include "Framework/RenderObjectMetadata.h"
#include "Framework/IGraphicSystem.h"
#include "Framework/JsonReader.h"
#include "WidgetProperty.h"
#include "DataLoader_1.h"
#include "UIButton.h"
#include "UILayout.h"
#include "UIString.h"
#include "UIPanel.h"
#include "UILabel.h"




using namespace eastl;
using namespace BohgeEngine;
namespace BGUI
{
	RTTI_TYPE_IMPLEMENT( UILayout );
	//------------------------------------------------------------------------------
	UILayout::UILayout(void)
		:m_Depth(0)
	{
		SetStatusChangeListener( MEMBER_FUNC_PTR( &UILayout::OnStatusChange ) );
	}
	//------------------------------------------------------------------------------
	UILayout::~UILayout(void)
	{
		ReleaseComponent();
		FOR_EACH( UIWidgetRenderMap, or, m_UIWidgetRenderMap )
		{
			SAFE_DELETE( or->second->m_pVertexStream );
			SAFE_DELETE( or->second->m_pRenderComponent );
			SAFE_DELETE( or->second );
		}
	}
	//------------------------------------------------------------------------------
	void _DoLoadWidget_1( UIWidget* pphost, const Widgets_1& wid )
	{
		UIWidget* ui = NULL;
		switch( wid.m_Type )//初始化和特殊设置
		{
		case WidgetProperty::WT_PANEL: ui = NEW UIPanel; break;
		case WidgetProperty::WT_LABEL: ui = NEW UILabel; break;
		case WidgetProperty::WT_BUTTON: ui = NEW UIButton; break;
		case  WidgetProperty::WT_STRING: ui = NEW UIString; break;
		default:ASSERT(false);
		}
		const Widgets_1_Array* arr = ui->Load_1( *wid.m_pProperty );
		pphost->AttachNode( ui );//挂接子节点
		for ( int i = 0 ; i < arr->Getsize() ; ++ i )
		{
			const Widgets_1& wid = arr->At<Widgets_1>(i);
			_DoLoadWidget_1( ui, wid );
		}
	}
	void UILayout::_Load_1( BohgeEngine::JsonReader& reader )
	{
		Layout_1 data;
		reader.Read( data );
		m_Depth = data.m_Depth;
		InsertIntoManager( data.m_Name );
		SetDesignRectangle( data.m_Rectangle.m_Rect );
		SetAdaptive( static_cast<WidgetProperty::WidgetAdaptive>( data.m_Adaptive ) );
		_DoLoadWidget_1( this, data.m_Widget );
	}
	//------------------------------------------------------------------------------
	void UILayout::_DoOnCreateResource( BohgeEngine::IAsyncSource* host )
	{
		host->SetDelayDependLoadedCallBacke( MEMBER_FUNC_PTR(&UILayout::OnDelayDependsLoaded) );
		{
			MaterialMap mlMap;
			FlushMaterials( mlMap );

			for( MaterialMap::iterator ml = mlMap.begin();
				mlMap.end() != ml;
				++ ml )
			{
				UIWidgetRender* uwr = NEW UIWidgetRender;
				uwr->m_pVertexStream = NEW VertexStream;
				uwr->m_pRenderComponent = NEW RenderComponent;
				uwr->m_pRenderComponent->SetUserParametersPtr(
					MaterialProperty::ATT_MATERIAL_USE_0,
					MaterialProperty::UPT_VEC4_ARRAY,
					&uwr->m_ParameterArray );
				uwr->m_pRenderComponent->EraseRenderProperty( RenderObjectProperty::RP_SHOW );
				uwr->m_pRenderComponent->EraseRenderProperty( RenderObjectProperty::RP_CULL );
				uwr->m_pRenderComponent->EraseRenderProperty( RenderObjectProperty::RP_SHADOW );
				uwr->m_pRenderComponent->EraseRenderProperty( RenderObjectProperty::RP_LIGHT );

				uint gomCount = FlushGeometryIndex( ml->first, 0 );
				uwr->m_ParameterArray.Resize( gomCount );
				AttachComponent( uwr->m_pRenderComponent );
				FlushLayout( ml->first, uwr->m_pVertexStream );
				FlushParameter( ml->first, uwr->m_ParameterArray );
				uint depth = DepthOffset( ml->first, 0 );
				depth += m_Depth;
				uwr->m_pRenderComponent->SetBindBox( Aabbox3df( vector3f(), vector3f(depth,depth,depth) ) );
				uwr->m_pRenderComponent->PushMetadata(						
					RenderObjectMaterialMetadata( 
					PathMetadata( ml->second ) ) );
				uwr->m_pRenderComponent->DependenceSource( host );
				if ( isDelayFlush( ml->first ) )
				{
					host->HasDelayDepends();//存在需要延迟加载的资源
				}
				else
				{
					FlushBuffer( ml->first, uwr->m_pVertexStream );
					uwr->m_pRenderComponent->PushMetadata(
						RenderObjectMeshMetadate( 
						RenderObjectProperty::RM_TRIANGLES,
						ReferenceVertexMetadata( uwr->m_pVertexStream ),
						SquareIndicesMetadata( uwr->m_pVertexStream->GetVertexCount() >> 2, BufferObjectProperty::MU_DYNAMIC ) ) );
					uwr->m_pRenderComponent->CreateResource();
				}
				m_UIWidgetRenderMap.insert( make_pair( ml->first, uwr ) );
			}
		}
	}
	//------------------------------------------------------------------------------
	void UILayout::OnDelayDependsLoaded( const SmartPtr<IAsyncSource>& source )
	{
		FOR_EACH( UIWidgetRenderMap, or, m_UIWidgetRenderMap )
		{
			if ( isDelayFlush( or->first ) )
			{
				FlushBuffer( or->first, or->second->m_pVertexStream );
				or->second->m_pRenderComponent->PushMetadata(
					RenderObjectMeshMetadate( 
					RenderObjectProperty::RM_TRIANGLES,
					ReferenceVertexMetadata( or->second->m_pVertexStream ),
					SquareIndicesMetadata( or->second->m_pVertexStream->GetVertexCount() >> 2, BufferObjectProperty::MU_DYNAMIC ) ) );
				or->second->m_pRenderComponent->CreateResource();
			}
		}
	}
	//------------------------------------------------------------------------------
	void UILayout::OnResourceLoaded()
	{
		FOR_EACH( UIWidgetRenderMap, uwr, m_UIWidgetRenderMap )
		{
			bool isok = FlushTextureEntiy( uwr->first, uwr->second->m_pRenderComponent );
			uwr->second->m_pRenderComponent->SetRenderProperty( RenderObjectProperty::RP_SHOW );
			ASSERT( isok );
		}
	}
	//------------------------------------------------------------------------------
	void UILayout::OnResizeview( )
	{
		FOR_EACH( UIWidgetRenderMap, uwr, m_UIWidgetRenderMap )
		{
			uwr->second->m_pVertexStream->ReleaseBuffer();
			FlushBuffer( uwr->first, uwr->second->m_pVertexStream );
			uwr->second->m_pRenderComponent->FlushVertexBufferImmediately( *uwr->second->m_pVertexStream );
		}
	}
	//------------------------------------------------------------------------------
	void UILayout::OnStatusChange( uint id )
	{
		UIWidgetRenderMap::iterator uwr = m_UIWidgetRenderMap.find( id );
		if ( m_UIWidgetRenderMap.end() != uwr )
		{
			FlushParameter( id, uwr->second->m_ParameterArray );
		}
	}
	//------------------------------------------------------------------------------
	void UILayout::_ReflushTexture()
	{
		FOR_EACH( UIWidgetRenderMap, uwr, m_UIWidgetRenderMap )
		{
			bool isok = FlushTextureEntiy( uwr->first, uwr->second->m_pRenderComponent );
			uwr->second->m_pRenderComponent->SetRenderProperty( RenderObjectProperty::RP_SHOW );
			ASSERT( isok );
		}
	}
	//------------------------------------------------------------------------------
	void UILayout::_ReflushBuffer( uint hash )
	{
		UIWidgetRenderMap::iterator uwr = m_UIWidgetRenderMap.find( hash );
		ASSERT( m_UIWidgetRenderMap.end() != uwr );
		uwr->second->m_pVertexStream->ReleaseBuffer();
		FlushBuffer( uwr->first, uwr->second->m_pVertexStream );
		uwr->second->m_pRenderComponent->FlushVertexBuffer( *uwr->second->m_pVertexStream );
		SquareIndicesMetadata str( uwr->second->m_pVertexStream->GetVertexCount() >> 2 );
		uwr->second->m_pRenderComponent->FlushIndiesBuffer( str.GetStream() );
	}

}