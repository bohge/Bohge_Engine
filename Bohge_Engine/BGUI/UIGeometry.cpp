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
#include "Framework/PipelineProperty.h"
#include "Framework/VertexStream.h"
#include "UIGeometry.h"
#include "UIWidget.h"


using namespace BohgeEngine;
namespace BGUI
{
	RTTI_TYPE_IMPLEMENT( UIGeometry );
	//------------------------------------------------------------------------------
	UIGeometry::UIGeometry(void)
		:m_Index(0)
	{
	}
	//------------------------------------------------------------------------------
	UIGeometry::~UIGeometry(void)
	{
	}
	//------------------------------------------------------------------------------
	void UIGeometry::SetImageUV( const BohgeEngine::vector4f& uv )
	{
		m_UVRect = uv;
	}
	//------------------------------------------------------------------------------
	void UIGeometry::FlushBuffer( const UIWidget* host, BohgeEngine::VertexStream* buff )
	{
		const vector4f& rect = host->GetScreenActualRect();
		buff->PushVertexData( Attributes::ATTRIBUTE_POSITION, vector4f( rect.m_x, rect.m_y,        0,        0 ) );
		buff->PushVertexData( Attributes::ATTRIBUTE_POSITION, vector4f( rect.m_x, rect.m_y,        0, rect.m_w ) );
		buff->PushVertexData( Attributes::ATTRIBUTE_POSITION, vector4f(	rect.m_x, rect.m_y, rect.m_z,        0 ) );
		buff->PushVertexData( Attributes::ATTRIBUTE_POSITION, vector4f(	rect.m_x, rect.m_y, rect.m_z, rect.m_w ) );

		buff->PushVertexData( Attributes::ATTRIBUTE_TEXCOORD0, vector4f( m_UVRect.m_x, m_UVRect.m_y, m_Index, 0.0 ) );
		buff->PushVertexData( Attributes::ATTRIBUTE_TEXCOORD0, vector4f( m_UVRect.m_x, m_UVRect.m_w, m_Index, 0.0 ) );
		buff->PushVertexData( Attributes::ATTRIBUTE_TEXCOORD0, vector4f( m_UVRect.m_z, m_UVRect.m_y, m_Index, 0.0 ) );
		buff->PushVertexData( Attributes::ATTRIBUTE_TEXCOORD0, vector4f( m_UVRect.m_z, m_UVRect.m_w, m_Index, 0.0 ) );
	}
	//------------------------------------------------------------------------------
	void UIGeometry::FlushBuffer( const UIWidget* host, BohgeEngine::VertexStream* buff, const BohgeEngine::Color& clr )
	{
		FlushBuffer( host, buff );
		buff->PushVertexData( Attributes::ATTRIBUTE_COLOR0, clr );
		buff->PushVertexData( Attributes::ATTRIBUTE_COLOR0, clr );
		buff->PushVertexData( Attributes::ATTRIBUTE_COLOR0, clr );
		buff->PushVertexData( Attributes::ATTRIBUTE_COLOR0, clr );
	}
	//------------------------------------------------------------------------------
	Aabrectf UIGeometry::CaculateBind( const UIWidget* host )
	{
		const vector4f& rect = host->GetScreenActualRect();
		vector2f pos( rect.m_x, rect.m_y );
		vector2f size = vector2f( rect.m_z, rect.m_w );
		pos += size * 0.5f;
		pos.m_x = pos.m_x * 2.0 - 1.0;
		pos.m_y = 1.0 - pos.m_y * 2.0;
		size *= 2.0f;
		return Math::Aabrect_From_CenterSize( pos, size );
	}

}