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
#pragma once
#include "Framework/3DMath.h"
#include "FreeTypeDeclear.h"



#include <EASTL/map.h>
namespace BohgeEngine
{
	class IAsyncSource;
	class TextureEntity;
}
namespace BTextSystemPlugin
{
	class SharedGlyph;
	class GlyphLoader;
	class GlyphPool : public BohgeEngine::Object
	{
		RTTI_TYPE_DECLEAR( GlyphPool );
	private:
		typedef eastl::map< uint, SharedGlyph* >	GlyphMap;//字体特征与UV坐标
	private:
		GlyphMap		m_pMapping;
		GlyphLoader*	m_pGlyphLoader;
	public:
		GlyphPool( FT_Face face );
		virtual ~GlyphPool(void);
	public:
		void CreateResource( BohgeEngine::IAsyncSource* source );
		const SharedGlyph& LoadGlyph( wchar_t g, const BohgeEngine::vector2d& size );//读取一个文字，制作为图片资源，有重用状况
		void UnloadGlyph( wchar_t g, const BohgeEngine::vector2d& size );//释放一个文字，有重用状况
		const BohgeEngine::TextureEntity* GetTextureEntity();
	private:
		BOHGE_FORCEINLINE uint _MakeGlyphID( wchar_t g, const BohgeEngine::vector2d& size )
		{
			ASSERT( size.m_x < 255 && size.m_y < 255 );
			return (uint(g)) << 16 | (uint(size.m_x)) << 8 | (uint(size.m_y));
		}
	};
}