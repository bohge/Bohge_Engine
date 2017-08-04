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
#include "Framework/RenderComponent.h"
#include "Framework/TextSetEntity.h"
#include "Framework/TextureEntity.h"
#include "Framework/GlobalDefine.h"
#include "Framework/TextString.h"
#include "Framework/Framework.h"
#include "Framework/IMetadata.h"
#include "Framework/Utility.hpp"
#include "UIString.h"
#include "UIGlyph.h"





using namespace eastl;
using namespace BohgeEngine;
namespace BGUI
{
	RTTI_TYPE_IMPLEMENT(UIString);
	//------------------------------------------------------------------------------
	UIString::UIString( void )
		:m_pTextSetEntity( NEW TextSetEntity ),
		m_rpTextureEntity(NULL),
		m_eAlign( WidgetProperty::SA_LEFT_TOP ),
		m_Size(-1),
		m_StringID(-1),
		m_GeometryIndex(0)
	{
	}
	//------------------------------------------------------------------------------
	UIString::~UIString( void )
	{
		_ReleaseString( m_StringID );
		SAFE_DELETE( m_pTextSetEntity );
	}
	//------------------------------------------------------------------------------
	const Widgets_1_Array* UIString::Load_1( const BohgeEngine::IJsonDataSet& data )
	{
		const Text_1& text = static_cast<const Text_1&>( data );
		m_Size = text.m_Size;
		m_StringID = text.m_ID;
		m_Color = text.m_Color.m_Color;
		m_eAlign = static_cast<WidgetProperty::StringAlign>(text.m_Align);
		InsertIntoManager( text.m_Name );
		SetDesignRectangle( text.m_Rectangle.m_Rect );
		SetAdaptive( static_cast<WidgetProperty::WidgetAdaptive>( text.m_Adaptive ) );
		_SetMaterialHash( Utility::HashCode( text.m_Path ) + Utility::HashCode( DefaultAsset::FontMaterial ) );
		m_pTextSetEntity->PushMetadata( PathMetadata( text.m_Path ) );
		m_pTextSetEntity->ConnectLoadedEvent( MEMBER_FUNC_PTR( &UIString::OnTextSetLoaded ) );
		return &text.m_Child;
	}
	//------------------------------------------------------------------------------
	void UIString::_DoFlushMaterials( MaterialMap& mm )
	{
		mm.insert( make_pair( _GetMaterialHash(), DefaultAsset::FontMaterial ) );
	}
	//------------------------------------------------------------------------------
	void UIString::_DoFlushBuffer( BohgeEngine::VertexStream* buff )
	{

	}
	//------------------------------------------------------------------------------
	void UIString::_DoFlushParameter( BohgeEngine::vector4arrayf& arr )
	{
		switch( m_Status )
		{
		case WidgetProperty::WS_ACTIVE: arr.Set( m_GeometryIndex, vector4f( WIDGET_STATUS_ENABLE, 0, 0, 0 ) ); break;//对应shader中的z值，2会被裁减
		case WidgetProperty::WS_HIDE: arr.Set( m_GeometryIndex, vector4f( WIDGET_STATUS_DISABLE, 0, 0, 0 ) ); break;
		}
	}
	//------------------------------------------------------------------------------
	void UIString::_DoFlushLayout( BohgeEngine::VertexStream* buff )
	{
		buff->SetVertexType<vector4f>(
			Attributes::ATTRIBUTE_POSITION,
			BufferObjectProperty::DT_FLOAT,
			BufferObjectProperty::DT_HALF_FLOAT );
		buff->SetVertexType<vector4f>(
			Attributes::ATTRIBUTE_TEXCOORD0,
			BufferObjectProperty::DT_FLOAT,
			BufferObjectProperty::DT_HALF_FLOAT );
		buff->SetVertexType<vector4f>(
			Attributes::ATTRIBUTE_COLOR0,
			BufferObjectProperty::DT_FLOAT,
			BufferObjectProperty::DT_UINT_8_8_8_8 );
	}
	//------------------------------------------------------------------------------
	uint UIString::_DoFlushGeometryIndex( uint index )
	{
		m_GeometryIndex = index;
		return index + 1;
	}
	//------------------------------------------------------------------------------
	void UIString::_DoFlushTextureEntiy( BohgeEngine::RenderComponent* com )
	{
		com->SetTextureEntity( MaterialProperty::ATT_TEXTURE_DIFFUSE, m_rpTextureEntity );
	}
	//------------------------------------------------------------------------------
	void UIString::_DoOnCreateResource( BohgeEngine::IAsyncSource* host )
	{
		m_pTextSetEntity->DependenceSource( host );
		m_pTextSetEntity->CreateResource();
	}
	//------------------------------------------------------------------------------
	bool UIString::_DoIsDelayFlush()
	{
		return true;
	}
	//------------------------------------------------------------------------------
	uint UIString::_GetFontSize() const
	{
		//vector2f sc = GetWorldScale();
		//const vector2d& rsl = Framework::Instance()->GetConfig().GetResolution();
		//sc *= rsl;
		//return Math::Ceil( Math::GetMax( sc ) * m_Size );
		return m_Size;
	}
	//------------------------------------------------------------------------------
	void UIString::ReplaceString( const eastl::string& str )
	{
		ReleaseChild();
		int temp = m_pTextSetEntity->InsertString( str );
		_CreateString( temp );
		_ReleaseString( m_StringID );
		m_StringID = temp;
		_ReflushBuffer( _GetMaterialHash() );
	}
	//------------------------------------------------------------------------------
	void UIString::OnTextSetLoaded( const BohgeEngine::SmartPtr<BohgeEngine::IAsyncSource>& )
	{		
		_CreateString( m_StringID );
	}
	//------------------------------------------------------------------------------
	void UIString::_ReleaseString( int id )
	{
		uint size = _GetFontSize();
		m_pTextSetEntity->ReleaseText( m_StringID, vector2d(size,size) );
	}
	//------------------------------------------------------------------------------
	void UIString::_CreateString( int id )
	{
		TextString str;
		uint size = _GetFontSize();
		m_pTextSetEntity->GetText( id, vector2d( size, size ), str );
		m_rpTextureEntity = str.GetTextureEntity();
		switch( m_eAlign )
		{
		case WidgetProperty::SA_LEFT_TOP: _CreateStringLeftTop( str.GetGlyphList() ); break;
		case WidgetProperty::SA_CENTER: _CreateStringCenter( str.GetGlyphList() ); break;
		default:ASSERT(false);
		}
	}
	//------------------------------------------------------------------------------
	void UIString::_CreateStringLeftTop( const list< Glyph >& gl )
	{
		uint size = _GetFontSize();
		vector4d textRect = GetLocalDesignRect();
		int width = textRect.m_z;
		vector2d pos( 0, size );
		FOR_EACH_CONST( list< Glyph >, glys, gl )
		{
			if ( pos.m_x + glys->m_Size.m_x + glys->m_XYBegin.m_x > width )
			{
				pos.m_x = 0;
				pos.m_y += size;
			}
			UIGlyph* gw = NEW UIGlyph( m_Color );
			gw->SetAdaptive( GetAdaptive() );
			gw->SetResolution( GetResolution() );
			gw->SetDesignRectangle( vector4d( pos.m_x + glys->m_XYBegin.m_x, pos.m_y - glys->m_XYBegin.m_y, glys->m_Size ) );
			gw->SetGeometryIndex( m_GeometryIndex );
			gw->SetImageUV( glys->m_UVBeginEnd );
			gw->SetMaterialHashCode( _GetMaterialHash() );
			pos.m_x += glys->m_Size.m_x + glys->m_XYBegin.m_x;
			AttachNode( gw );
		}
	}
	//------------------------------------------------------------------------------
	void UIString::_CreateStringCenter( const list< Glyph >& gl )
	{
		uint size = _GetFontSize();
		vector2d pos;
		int beginX(0);
		vector4d textRect = GetLocalDesignRect();
		int width = textRect.m_z;
		{
			int stringWidth(0);
			int row(1);
			FOR_EACH_CONST( list< Glyph >, glys, gl )
			{
				stringWidth += glys->m_Size.m_x + glys->m_XYBegin.m_x;
				if ( stringWidth + glys->m_Size.m_x + glys->m_XYBegin.m_x > width )
				{
					stringWidth = 0;
					++row;
				}
			}
			int halfHight = (size * row)>>1;
			pos.m_y = ( textRect.m_w >> 1  )- halfHight + size;
			if ( 1== row )
			{
				beginX = ( width - stringWidth ) >> 1;
				pos.m_x = beginX;
			}
		}
		FOR_EACH_CONST( list< Glyph >, glys, gl )
		{
			if ( pos.m_x + glys->m_Size.m_x + glys->m_XYBegin.m_x > width )
			{
				pos.m_x = beginX;
				pos.m_y += size;
			}
			UIGlyph* gw = NEW UIGlyph( m_Color );
			gw->SetAdaptive( GetAdaptive() );
			gw->SetResolution( GetResolution() );
			gw->SetDesignRectangle( vector4d( pos.m_x + glys->m_XYBegin.m_x, pos.m_y - glys->m_XYBegin.m_y, glys->m_Size ) );
			gw->SetGeometryIndex( m_GeometryIndex );
			gw->SetImageUV( glys->m_UVBeginEnd );
			gw->SetMaterialHashCode( _GetMaterialHash() );
			pos.m_x += glys->m_Size.m_x + glys->m_XYBegin.m_x;
			AttachNode( gw );
		}
	}
}