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
#include "Framework/MaterialEntity.h"
#include "Framework/TextString.h"
#include "Framework/IIOSystem.h"
#include "Framework/IFile.h"
#include "FontResource.h"
#include "SharedGlyph.h"
#include "GlyphPool.h"



#include "ft2build.h"
#include FT_FREETYPE_H

using namespace eastl;
using namespace BohgeEngine;
namespace BTextSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	FontResource::FontResource( void )
		:m_pFontStream(NULL),
		m_FT2Lib(NULL),
		m_FT_Face(NULL),
		m_pGlyphPool(NULL)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	FontResource::~FontResource( void )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void FontResource::ReleaseResource()
	{
		SAFE_DELETE( m_pGlyphPool );
		SAFE_DELETE( m_pFontStream );
	}
	//-------------------------------------------------------------------------------------------------------
	void FontResource::LoadFont( const eastl::string& path )
	{
		FT_Error error = FT_Init_FreeType( &m_FT2Lib);
		ASSERT( 0 == error );
		{//加载字体
			IFile* readfile = IIOSystem::Instance()->FileFactory( path );
			readfile->OpenFile( IFile::AT_READ );
			uint size = readfile->GetSize();
			m_pFontStream = NEW FT_Byte[ size ];
			readfile->ReadFile( m_pFontStream, size );
			readfile->CloseFile();
			IIOSystem::Instance()->RecycleBin( readfile );

			//加载一个字体,取默认的Face,一般为Regualer
			error = FT_New_Memory_Face( m_FT2Lib, m_pFontStream, size, 0, &m_FT_Face );
			ASSERT( 0 == error );
		}
		error = FT_Select_Charmap(m_FT_Face, FT_ENCODING_UNICODE);
		ASSERT( 0 == error );
		//error = FT_Set_Pixel_Sizes(m_FT_Face, 64, 64 );
		m_pGlyphPool = NEW GlyphPool(m_FT_Face);
	}
	//-------------------------------------------------------------------------------------------------------
	void FontResource::CreateResource( BohgeEngine::IAsyncSource* source )
	{
		m_pGlyphPool->CreateResource( source );
	}
	//-------------------------------------------------------------------------------------------------------
	void FontResource::LoadText( const BohgeEngine::vector2d& size, const eastl::wstring& str, BohgeEngine::TextString& ts )
	{
		FT_Error error = FT_Set_Pixel_Sizes(m_FT_Face, size.m_x, size.m_y );
		ASSERT( 0 == error );
		ts.SetTextureEntity( m_pGlyphPool->GetTextureEntity() );
		for ( int i = 0; i < str.size(); ++ i )
		{
			wchar_t wc = str[i];
			wc = L' ' == wc ? L'_': wc;
			const SharedGlyph& gly = m_pGlyphPool->LoadGlyph( wc, size );
			ts.PushGlyph( Glyph( gly.GetXYBegin(), gly.GetPixelSize(), gly.GetCoordinate() ) );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void FontResource::UnLoadText( const BohgeEngine::vector2d& size, const eastl::wstring& str )
	{
		for ( int i = 0; i < str.size(); ++ i )
		{
			wchar_t wc = str[i];
			wc = L' ' == wc ? L'_': wc;
			m_pGlyphPool->UnloadGlyph( wc, size );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	const TextureEntity* FontResource::GetTextureEntity()
	{
		return m_pGlyphPool->GetTextureEntity();
	}

}