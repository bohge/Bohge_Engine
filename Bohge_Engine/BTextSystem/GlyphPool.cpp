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
#include "GlyphLoader.h"
#include "SharedGlyph.h"
#include "GlyphPool.h"



using namespace eastl;
using namespace BohgeEngine;
namespace BTextSystemPlugin
{
	RTTI_TYPE_IMPLEMENT( GlyphPool );
	//---------------------------------------------------------------------------------------------------------
	GlyphPool::GlyphPool( FT_Face face )
		:m_pGlyphLoader( NEW GlyphLoader( face ) )
	{

	}
	//---------------------------------------------------------------------------------------------------------
	GlyphPool::~GlyphPool( void )
	{
		SAFE_DELETE( m_pGlyphLoader );
	}
	//---------------------------------------------------------------------------------------------------------
	const SharedGlyph& GlyphPool::LoadGlyph( wchar_t g, const BohgeEngine::vector2d& size )
	{
		uint id = _MakeGlyphID( g, size );
		SharedGlyph* gly = NULL;
		GlyphMap::iterator glyit = m_pMapping.find( id );
		if ( m_pMapping.end() == glyit )
		{
			gly = m_pGlyphLoader->LoadGlyph( g, size );
			m_pMapping.insert( make_pair( id, gly ) );
		}
		else
		{
			gly = glyit->second;
		}
		gly->Increment();
		return *gly;
	}
	//---------------------------------------------------------------------------------------------------------
	void GlyphPool::UnloadGlyph( wchar_t g, const BohgeEngine::vector2d& size )
	{
		uint id = _MakeGlyphID( g, size );
		GlyphMap::iterator gly = m_pMapping.find( id );
		ASSERT( m_pMapping.end() != gly );
		if( 0 == gly->second->Decrement() )
		{
			m_pGlyphLoader->UnloadGlyph( gly->second );
			m_pMapping.erase( gly );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void GlyphPool::CreateResource( BohgeEngine::IAsyncSource* source )
	{
		m_pGlyphLoader->CreateResource( source );
	}
	//---------------------------------------------------------------------------------------------------------
	const TextureEntity* GlyphPool::GetTextureEntity()
	{
		return m_pGlyphLoader->GetTextureEntity();
	}

}