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
#include "Framework/IFunctionInvoker.h"
#include "Framework/ScriptComponent.h"
#include "Framework/RenderComponent.h"
#include "Framework/TextureEntity.h"
#include "Framework/VertexStream.h"
#include "Framework/IAsyncSource.h"
#include "Framework/TouchInfo.h"
//#include "Framework/Log.h"
#include "UIGeometry.h"
#include "UIButton.h"

using namespace eastl;
using namespace BohgeEngine;
namespace BGUI
{
	RTTI_TYPE_IMPLEMENT( UIButton );
	//------------------------------------------------------------------------------
	UIButton::UIButton(void)
		:m_pUIGeometryArray( NEW UIGeometry[BC_GEOMETRY_COUNT] ),
		m_pTexture(NEW TextureEntity),
		m_pBind( NEW Aabrectf ),
		m_pScript( NEW ScriptComponent ),
		m_pOnPressed(NULL)
	{
		m_pScript->ConnectLoadedEvent( MEMBER_FUNC_PTR( &UIButton::OnScriptLoaded) );
		AttachComponent( m_pScript );
	}
	//------------------------------------------------------------------------------
	UIButton::~UIButton(void)
	{
		ReleaseComponent();
		SAFE_DELETE( m_pBind );
		SAFE_DELETE( m_pScript );
		SAFE_DELETE( m_pTexture );
		SAFE_DELETE_ARRAY( m_pUIGeometryArray );
	}
	//------------------------------------------------------------------------------
	void UIButton::_DoFlushBuffer( BohgeEngine::VertexStream* buff )
	{
		*m_pBind = UIGeometry::CaculateBind( this );
		for ( int i = 0 ; i < BC_GEOMETRY_COUNT ; ++ i )
		{
			m_pUIGeometryArray[i].FlushBuffer( this, buff );
		}
	}
	//------------------------------------------------------------------------------
	const Widgets_1_Array* UIButton::Load_1( const BohgeEngine::IJsonDataSet& data )
	{
		const Button_1& buttonData = static_cast< const Button_1& >( data );
		InsertIntoManager( buttonData.m_Name );
		SetDesignRectangle( buttonData.m_Rectangle.m_Rect );
		SetAdaptive( static_cast<WidgetProperty::WidgetAdaptive>( buttonData.m_Adaptive ) );
		m_pScript->SetScriptPath( buttonData.m_Script );
		for ( int i = 0 ; i < BC_GEOMETRY_COUNT ; ++ i )
		{
			const JsonUV& uv = static_cast< const JsonUV& >( buttonData.m_ImageUVArray[i] );
			m_pUIGeometryArray[i].SetImageUV( uv.m_UVStartEnd );
		}
		m_pTexture->PushMetadata(
			TextureFileMetadata(
			TextureProperty::TU_STATIC,
			TextureProperty::PF_R8G8B8A8, 0, false,
			TextureProperty::TW_CLAMP_TO_EDGE, TextureProperty::TW_CLAMP_TO_EDGE,
			TextureProperty::TF_NEAREST, TextureProperty::TF_NEAREST,
			buttonData.m_ImageSource ) );
		_SetMaterialHash( Utility::HashCode( buttonData.m_ImageSource + DefaultAsset::UIMaterial ) );
		return &buttonData.m_Child;
	}
	//------------------------------------------------------------------------------
	void UIButton::_DoOnCreateResource( BohgeEngine::IAsyncSource* host )
	{
		m_pTexture->DependenceSource( host );
		m_pTexture->CreateResource();
		m_pScript->DependenceSource( host );
		m_pScript->CreateResource();
	}
	//------------------------------------------------------------------------------
	void UIButton::OnScriptLoaded( const BohgeEngine::SmartPtr<BohgeEngine::IAsyncSource>& source )
	{
		m_pOnPressed = m_pScript->SignFunciton( WidgetProperty::BUTTON_ONPRESSED, 1 );
	}
	//------------------------------------------------------------------------------
	void UIButton::ReplacePressedCallback( IFunctionInvoker* f )
	{
		if ( m_pOnPressed )
		{
			m_pScript->UnsignFunction( m_pOnPressed );
		}
		m_pOnPressed = f;
	}
	//------------------------------------------------------------------------------
	uint UIButton::_DoFlushGeometryIndex( uint index )
	{
		for ( int i = 0 ; i < BC_GEOMETRY_COUNT ; ++ i )
		{
			m_pUIGeometryArray[i].SetIndex( index++ );
		}
		return index;
	}
	//------------------------------------------------------------------------------
	void UIButton::_DoFlushParameter( BohgeEngine::vector4arrayf& arr )
	{
		switch( m_Status )
		{
		case WidgetProperty::WS_ACTIVE:
			{
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_ACTIVE].GetIndex(), vector4f( WIDGET_STATUS_ENABLE, 0, 0, 0 ) );//对应shader中的z值，2会被裁减
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_DISABLE].GetIndex(), vector4f( WIDGET_STATUS_DISABLE, 0, 0, 0 ) );
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_PRESSED].GetIndex(), vector4f( WIDGET_STATUS_DISABLE, 0, 0, 0 ) );
			}break;
		case WidgetProperty::WS_DISABLED:
			{
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_ACTIVE].GetIndex(), vector4f( WIDGET_STATUS_DISABLE, 0, 0, 0 ) );
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_DISABLE].GetIndex(), vector4f( WIDGET_STATUS_ENABLE, 0, 0, 0 ) );
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_PRESSED].GetIndex(), vector4f( WIDGET_STATUS_DISABLE, 0, 0, 0 ) );
			}break;
		case WidgetProperty::WS_PRESSED:
			{
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_ACTIVE].GetIndex(), vector4f( WIDGET_STATUS_DISABLE, 0, 0, 0 ) );
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_DISABLE].GetIndex(), vector4f( WIDGET_STATUS_DISABLE, 0, 0, 0 ) );
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_PRESSED].GetIndex(), vector4f( WIDGET_STATUS_ENABLE, 0, 0, 0 ) );
			}break;
		case WidgetProperty::WS_HIDE:
			{
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_ACTIVE].GetIndex(), vector4f( WIDGET_STATUS_DISABLE, 0, 0, 0 ) );
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_DISABLE].GetIndex(), vector4f( WIDGET_STATUS_DISABLE, 0, 0, 0 ) );
				arr.Set( m_pUIGeometryArray[BC_GEOMETRY_PRESSED].GetIndex(), vector4f( WIDGET_STATUS_DISABLE, 0, 0, 0 ) );
			}break;
		default:ASSERT(false);
		}
	}
	//------------------------------------------------------------------------------
	bool UIButton::_DoOnActionEvent( const BohgeEngine::TouchInfo& touch )
	{
		switch( m_Status )
		{
		case WidgetProperty::WS_ACTIVE:
			{
				//DEBUGLOG("Button %s actice\r\n", GetName().c_str() );
				for ( int i = 0 ; i < touch.GetPointCount() ; ++ i )
				{
					if( m_pBind->Intersect( touch.GetTouchPoint( i ) ) )
					{
						m_Status = WidgetProperty::WS_PRESSED;
						_NoticeStatusChange( _GetMaterialHash() );
						return true;
					}
				}
				return false;
			}
		case WidgetProperty::WS_DISABLED:
			{
				//DEBUGLOG("Button %s disable\r\n", GetName().c_str() );
				return false;
			}
		case WidgetProperty::WS_PRESSED:
			{
				//DEBUGLOG("Button %s pressed\r\n", GetName().c_str() );
				for ( int i = 0 ; i < touch.GetPointCount() ; ++ i )
				{
					if( m_pBind->Intersect( touch.GetTouchPoint( i ) ) )
					{
						if ( ActionPropery::TT_RELEASE == touch.GetTouchType( i ) )
						{
							m_Status = WidgetProperty::WS_ACTIVE;
							if( m_pOnPressed )m_pOnPressed->Invoke( this );
							_NoticeStatusChange( _GetMaterialHash() );
						}
						return true;
					}
				}
				m_Status = WidgetProperty::WS_ACTIVE;
				_NoticeStatusChange( _GetMaterialHash() );
				return false;
			}
		case WidgetProperty::WS_HIDE:
			{
				return false;
			}
		default:ASSERT(false);
		}
		return false;
	}
	//------------------------------------------------------------------------------
	void UIButton::_DoFlushMaterials( MaterialMap& mm )
	{
		mm.insert( make_pair( _GetMaterialHash(), DefaultAsset::UIMaterial ) );
	}
	//------------------------------------------------------------------------------
	void UIButton::_DoFlushLayout( BohgeEngine::VertexStream* buff )
	{
		buff->SetVertexType<vector4f>(
			Attributes::ATTRIBUTE_POSITION,
			BufferObjectProperty::DT_FLOAT,
			BufferObjectProperty::DT_HALF_FLOAT );
		buff->SetVertexType<vector4f>(
			Attributes::ATTRIBUTE_TEXCOORD0,
			BufferObjectProperty::DT_FLOAT,
			BufferObjectProperty::DT_HALF_FLOAT );
	}
	//------------------------------------------------------------------------------
	void UIButton::_DoFlushTextureEntiy( BohgeEngine::RenderComponent* com )
	{
		com->SetTextureEntity( MaterialProperty::ATT_TEXTURE_DIFFUSE, m_pTexture );
	}
	//------------------------------------------------------------------------------
	void UIButton::_DoFlushBind( BohgeEngine::Aabrectf& bind )
	{
		bind.Contains( *m_pBind );
	}

}