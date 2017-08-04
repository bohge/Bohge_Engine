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
#include "Framework/TextureAtlas.h"
#include "SharedGlyph.h"
#include "GlyphLoader.h"
#include "Framework/Log.h"

#include "ft2build.h"
#include FT_FREETYPE_H 
#include FT_GLYPH_H




using namespace BohgeEngine;
namespace BTextSystemPlugin
{
	RTTI_TYPE_IMPLEMENT(GlyphLoader);
	//---------------------------------------------------------------------------------------------------------
	GlyphLoader::GlyphLoader(  FT_Face face  )
		:m_rpFT_Face( face ),
		m_pTextureAtlas(NEW TextureAtlas)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	GlyphLoader::~GlyphLoader( void )
	{
		SAFE_DELETE( m_pTextureAtlas );
	}
	//---------------------------------------------------------------------------------------------------------
	SharedGlyph* GlyphLoader::LoadGlyph( wchar_t g, const BohgeEngine::vector2d& size )
	{
		FT_Load_Glyph(m_rpFT_Face,  FT_Get_Char_Index( m_rpFT_Face, g ), FT_LOAD_DEFAULT);//FT_LOAD_FORCE_AUTOHINT| FT_LOAD_TARGET_NORMAL); 

		//得到字模
		FT_Glyph glyph;
		FT_Get_Glyph( m_rpFT_Face->glyph, &glyph );
		//转化成位图
		FT_Render_Glyph( m_rpFT_Face->glyph,   FT_RENDER_MODE_LCD );

		FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		//取道位图数据
		FT_Bitmap& bitmap=bitmap_glyph->bitmap;

		//重新计算数据
		float scale = static_cast<float>(bitmap.rows) / static_cast<float>(m_rpFT_Face->glyph->metrics.height);//计算文本高宽缩放到bitmap高宽的缩放比
		vector2d xy( Math::Ceil( m_rpFT_Face->glyph->metrics.horiBearingX * scale ),//y偏移
			Math::Ceil( m_rpFT_Face->glyph->metrics.horiBearingY * scale ) );//x偏移

		vector2d pixelsize = vector2d(bitmap.width, bitmap.rows);

		TextureAtlasData data = m_pTextureAtlas->InsertTextureBuffer( pixelsize, bitmap.buffer );

		FT_Done_Glyph(glyph);
		return NEW SharedGlyph(
			data.m_Index,
			xy,
			pixelsize,
			vector4f(
			static_cast<float>(data.m_UVBegin.m_x)/GLC_TEXTURE_SIZE,
			static_cast<float>(data.m_UVBegin.m_y)/GLC_TEXTURE_SIZE,
			static_cast<float>(data.m_UVBegin.m_x+data.m_Size.m_x)/GLC_TEXTURE_SIZE,
			static_cast<float>(data.m_UVBegin.m_y+data.m_Size.m_y)/GLC_TEXTURE_SIZE
			));
	}
	//---------------------------------------------------------------------------------------------------------
	void GlyphLoader::UnloadGlyph( SharedGlyph* gly )
	{
		m_pTextureAtlas->EraseBuffer( gly->GetHandle() );
		SAFE_DELETE( gly );
	}
	//---------------------------------------------------------------------------------------------------------
	void GlyphLoader::CreateResource( IAsyncSource* source )
	{
		m_pTextureAtlas->SetPixelFormat( TextureProperty::PF_A8 );
		m_pTextureAtlas->SetFilter( TextureProperty::TF_NEAREST, TextureProperty::TF_NEAREST );
		m_pTextureAtlas->SetSize( vector2d( GLC_TEXTURE_SIZE, GLC_TEXTURE_SIZE ) );
		m_pTextureAtlas->CreateTextureEntity( source );
	}
	//---------------------------------------------------------------------------------------------------------
	const BohgeEngine::TextureEntity* GlyphLoader::GetTextureEntity()
	{
		return m_pTextureAtlas->GetTextureEntity();
	}

}