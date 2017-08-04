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
#include "UIGeometry.h"
#include "UIGlyph.h"



using namespace eastl;
using namespace BohgeEngine;
namespace BGUI
{
	RTTI_TYPE_IMPLEMENT(UIGlyph);
	//------------------------------------------------------------------------------
	UIGlyph::UIGlyph( const BohgeEngine::Color& c )
		:m_pUIGeometry(NEW UIGeometry),
		m_Color(c)
	{
	}
	//------------------------------------------------------------------------------
	UIGlyph::~UIGlyph( void )
	{
		SAFE_DELETE( m_pUIGeometry );
	}
	//------------------------------------------------------------------------------
	void UIGlyph::_DoFlushLayout( BohgeEngine::VertexStream* buff )
	{
		//由于树形结构，应该会首先调用UIString中的该函数
		ASSERT(false);
	}
	//------------------------------------------------------------------------------
	void UIGlyph::_DoFlushTextureEntiy( BohgeEngine::RenderComponent* com )
	{
		//由于树形结构，应该会首先调用UIString中的该函数
		ASSERT(false);
	}
	//------------------------------------------------------------------------------
	bool UIGlyph::_DoIsDelayFlush()
	{
		//由于树形结构，应该会首先调用UIString中的该函数
		ASSERT(false);
		return true;
	}
	//------------------------------------------------------------------------------
	void UIGlyph::_DoFlushBuffer( BohgeEngine::VertexStream* buff )
	{
		m_pUIGeometry->FlushBuffer( this, buff, m_Color );
	}
	//------------------------------------------------------------------------------
	void UIGlyph::SetGeometryIndex( uint index )
	{
		m_pUIGeometry->SetIndex( index );
	}
	//------------------------------------------------------------------------------
	void UIGlyph::SetImageUV( const BohgeEngine::vector4f& uv )
	{
		m_pUIGeometry->SetImageUV( uv );
	}

}