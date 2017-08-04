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
#include "RenderTargetMetadata.h"
#include "Framework.h"



#include <sstream>

using namespace eastl;

namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( RenderTargetMetadata );
	//------------------------------------------------------------------------------------------------------
	RenderTargetMetadata::RenderTargetMetadata(
		RenderTargetProperty::TargetType tt,
		RenderTargetProperty::SwapTarget st,
		const Color& clr,
		const vector4d& vp,
		const vector2d& size )
	{
		m_Metadata.m_Color = clr;
		m_Metadata.m_ViewPort = vp;
		m_Metadata.m_Size = size;
		m_Metadata.m_eTargetType = tt;
		m_Metadata.m_eSwapTarget = st;
	}
	//------------------------------------------------------------------------------------------------------
	RenderTargetMetadata::~RenderTargetMetadata(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void* RenderTargetMetadata::_ReturnMetadata()
	{
		return &m_Metadata;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetMetadata::GetIdentifier( eastl::vector<byte>& bytes ) const
	{
		std::stringstream ss;
		ss.precision( 2 );
		ss<<m_Metadata.m_ViewPort.m_x
			<<m_Metadata.m_ViewPort.m_y
			<<m_Metadata.m_ViewPort.m_z
			<<m_Metadata.m_ViewPort.m_w
			<<m_Metadata.m_Size.m_x
			<<m_Metadata.m_Size.m_y
			<<m_Metadata.m_Color.m_r
			<<m_Metadata.m_Color.m_g
			<<m_Metadata.m_Color.m_b
			<<m_Metadata.m_Color.m_a;
		string str = ss.str().c_str();
		bytes.insert( bytes.end(), str.begin(), str.end() );
		bytes.push_back( m_Metadata.m_eTargetType );
		bytes.push_back( m_Metadata.m_eSwapTarget );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetMetadata::ProcessMetadata()
	{
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetMetadata::ReleaseMetadate()
	{

	}



	RTTI_TYPE_IMPLEMENT( RenderTargetDependResolutionMetadata );
	//------------------------------------------------------------------------------------------------------
	RenderTargetDependResolutionMetadata::RenderTargetDependResolutionMetadata(
		RenderTargetProperty::TargetType tt,
		RenderTargetProperty::SwapTarget st,
		const Color& clr,
		const vector2f& viewScale,
		const vector2f& resolutionScale )
		:m_ResolutionScale(resolutionScale),
		m_ViewScale(viewScale)
	{
		m_Metadata.m_Color = clr;
		m_Metadata.m_eTargetType = tt;
		m_Metadata.m_eSwapTarget = st;
	}
	//------------------------------------------------------------------------------------------------------
	RenderTargetDependResolutionMetadata::RenderTargetDependResolutionMetadata(
		RenderTargetProperty::TargetType tt,
		RenderTargetProperty::SwapTarget st,
		const Color& clr,
		const vector4d& vp,
		const vector2d& size )
	{
		m_Metadata.m_Color = clr;
		m_Metadata.m_eTargetType = tt;
		m_Metadata.m_eSwapTarget = st;

		vector4f view = Framework::Instance()->GetConfig().GetViewSize();
		vector2f rsl = Framework::Instance()->GetConfig().GetResolution();
		m_ViewScale = vector2f( vp.m_z / view.m_z, vp.m_w / view.m_w );
		m_ResolutionScale = vector2f( size.m_x / rsl.m_x, size.m_y / rsl.m_y );
	}
	//------------------------------------------------------------------------------------------------------
	RenderTargetDependResolutionMetadata::~RenderTargetDependResolutionMetadata( void )
	{

	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetDependResolutionMetadata::GetIdentifier( eastl::vector<byte>& bytes ) const
	{
		std::stringstream ss;
		ss.precision( 2 );
		ss<<m_ViewScale.m_x
			<<m_ViewScale.m_y
			<<m_ResolutionScale.m_x
			<<m_ResolutionScale.m_y
			<<m_Metadata.m_Color.m_r
			<<m_Metadata.m_Color.m_g
			<<m_Metadata.m_Color.m_b
			<<m_Metadata.m_Color.m_a;
		string str = ss.str().c_str();
		bytes.insert( bytes.end(), str.begin(), str.end() );
		bytes.push_back( m_Metadata.m_eTargetType );
		bytes.push_back( m_Metadata.m_eSwapTarget );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetDependResolutionMetadata::ProcessMetadata()
	{
		vector4f view = Framework::Instance()->GetConfig().GetViewSize();
		vector2f rsl = Framework::Instance()->GetConfig().GetResolution();
		m_Metadata.m_ViewPort = view * vector4f( m_ViewScale, m_ViewScale );
		m_Metadata.m_Size = rsl * m_ResolutionScale;
	}
	//------------------------------------------------------------------------------------------------------
	void* RenderTargetDependResolutionMetadata::_ReturnMetadata()
	{
		return &m_Metadata;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetDependResolutionMetadata::ReleaseMetadate()
	{

	}
}
