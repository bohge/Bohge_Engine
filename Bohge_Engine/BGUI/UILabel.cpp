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
#include "Framework/RenderComponent.h"
#include "Framework/TextureEntity.h"
#include "Framework/VertexStream.h"
#include "UIGeometry.h"
#include "UILabel.h"


//0-------2
//|	    / |
//|	  /   |
//| /	  |
//1-------3
using namespace eastl;
using namespace BohgeEngine;
namespace BGUI
{
	RTTI_TYPE_IMPLEMENT( UILabel );
	//------------------------------------------------------------------------------
	UILabel::UILabel(void)
		:m_pGeometry(NEW UIGeometry),
		m_pTexture(NEW TextureEntity),
		m_pSwitch(NULL)
	{
	}
	//------------------------------------------------------------------------------
	UILabel::~UILabel(void)
	{
		SAFE_DELETE( m_pTexture );
		SAFE_DELETE( m_pGeometry );
	}
	//------------------------------------------------------------------------------
	const Widgets_1_Array* UILabel::Load_1( const BohgeEngine::IJsonDataSet& data )
	{
		const Label_1& labelData = static_cast< const Label_1& >(data);
		InsertIntoManager( labelData.m_Name );
		SetDesignRectangle( labelData.m_Rectangle.m_Rect );
		SetAdaptive( static_cast<WidgetProperty::WidgetAdaptive>( labelData.m_Adaptive ) );
		m_pGeometry->SetImageUV( labelData.m_ImageUV.m_UVStartEnd );
		m_pTexture->PushMetadata(
			TextureFileMetadata(
			TextureProperty::TU_STATIC,
			TextureProperty::PF_R8G8B8A8, 0, false,
			TextureProperty::TW_CLAMP_TO_EDGE, TextureProperty::TW_CLAMP_TO_EDGE,
			TextureProperty::TF_NEAREST, TextureProperty::TF_NEAREST,
			labelData.m_ImageSource ) );
		_SetMaterialHash( Utility::HashCode( labelData.m_ImageSource + DefaultAsset::UIMaterial ) );
		return &labelData.m_Child;
	}
	//------------------------------------------------------------------------------
	void UILabel::_DoOnCreateResource( BohgeEngine::IAsyncSource* host )
	{
		m_pTexture->DependenceSource( host );
		m_pTexture->CreateResource();
	}
	//------------------------------------------------------------------------------
	uint UILabel::_DoFlushGeometryIndex( uint index )
	{
		m_pGeometry->SetIndex( index++ );
		return index;
	}
	//------------------------------------------------------------------------------
	void UILabel::_DoFlushBuffer( BohgeEngine::VertexStream* buff )
	{
		m_pGeometry->FlushBuffer( this, buff );
	}
	//------------------------------------------------------------------------------
	void UILabel::_DoFlushParameter( BohgeEngine::vector4arrayf& arr )
	{
		switch( m_Status )
		{
		case WidgetProperty::WS_ACTIVE: arr.Set( m_pGeometry->GetIndex(), vector4f( WIDGET_STATUS_ENABLE, 0, 0, 0 ) ); break;//对应shader中的z值，2会被裁减
		case WidgetProperty::WS_HIDE: arr.Set( m_pGeometry->GetIndex(), vector4f( WIDGET_STATUS_DISABLE, 0, 0, 0 ) ); break;
		}
	}
	//------------------------------------------------------------------------------
	void UILabel::_DoFlushMaterials( MaterialMap& mm )
	{
		mm.insert( make_pair( _GetMaterialHash(), DefaultAsset::UIMaterial ) );
	}
	//------------------------------------------------------------------------------
	void UILabel::_DoFlushLayout( BohgeEngine::VertexStream* buff )
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
	void UILabel::_DoFlushTextureEntiy( BohgeEngine::RenderComponent* com )
	{
		com->SetTextureEntity( MaterialProperty::ATT_TEXTURE_DIFFUSE, m_pTexture );
	}
	//------------------------------------------------------------------------------
	void UILabel::ReplaceTexture( const eastl::string& name )
	{
		m_pSwitch = NEW TextureEntity();
		m_pSwitch->PushMetadata(
			TextureFileMetadata(
			TextureProperty::TU_STATIC,
			TextureProperty::PF_R8G8B8A8, 0, false,
			TextureProperty::TW_CLAMP_TO_EDGE, TextureProperty::TW_CLAMP_TO_EDGE,
			TextureProperty::TF_NEAREST, TextureProperty::TF_NEAREST,
			name ) );
		m_pSwitch->ConnectLoadedEvent( MEMBER_FUNC_PTR( &UILabel::OnReplaceTextureLoaded ) );
		m_pSwitch->CreateResource();
	}
	//------------------------------------------------------------------------------
	void UILabel::OnReplaceTextureLoaded( const SmartPtr<IAsyncSource>& res )
	{
		//这个函数在主线程调用，所以可以安全更换材质
		TextureEntity* temp = m_pTexture;
		m_pTexture = m_pSwitch;
		m_pSwitch = temp;
		_ReflushTexture( );
		SAFE_DELETE( m_pSwitch );
	}

}