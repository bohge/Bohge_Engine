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
#include "TextureEntity.h"
#include "TextureAtlas.h"
#include "PackageTree.h"




namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( TextureAtlas );
	//------------------------------------------------------------------------------------------------------
	TextureAtlas::TextureAtlas(void)
		:m_pTextureEntity( NEW TextureEntity ),
		m_ePixelFormat( TextureProperty::PF_A8 ),
		m_eMagFilter( TextureProperty::TF_NEAREST ),
		m_eMinFilter( TextureProperty::TF_NEAREST ),
		m_pAtlasTree( NEW PackageTree )
	{
	}
	//------------------------------------------------------------------------------------------------------
	TextureAtlas::~TextureAtlas(void)
	{
		SAFE_DELETE( m_pAtlasTree );
		SAFE_DELETE( m_pTextureEntity );
	}
	//------------------------------------------------------------------------------------------------------
	void TextureAtlas::CreateTextureEntity( IAsyncSource* source )
	{
		m_pTextureEntity->PushMetadata(
			TextureBufferMetadata(
			m_Size,
			TextureProperty::TT_TEXTURE2D,
			TextureProperty::TU_WRITE,
			m_ePixelFormat,	0, false,
			TextureProperty::TW_CLAMP_TO_EDGE, TextureProperty::TW_CLAMP_TO_EDGE,
			m_eMagFilter, m_eMinFilter,
			NULL ) );
		m_pTextureEntity->DependenceSource( source );
		m_pTextureEntity->CreateResource();
		m_pAtlasTree->Initialize( m_Size );
	}
	//------------------------------------------------------------------------------------------------------
	TextureAtlasData TextureAtlas::InsertTextureBuffer( const vector2d& size, const byte* buffer )
	{
		TextureAtlasData data = m_pAtlasTree->InsertPackage( size );
		m_pTextureEntity->SubstituteTextureBuffer( data.m_UVBegin, size, buffer );
		return data;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureAtlas::EraseBuffer( uint handle )
	{
		m_pAtlasTree->ErasePackage( handle );
	}

}