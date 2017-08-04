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
#include "RenderTargetResource.h"
#include "TextureEntity.h"
#include "Framework.h"
#include "Config.h"


namespace BohgeEngine
{

	handle RenderTargetResource::s_CurrentBind = -1;
	vector4d RenderTargetResource::s_CurrentViewPort;
	Color RenderTargetResource::s_CurrentColor;

	//------------------------------------------------------------------------------------------------------
	RenderTargetResource::RenderTargetResource(void)
		:m_eTargetType( (RenderTargetProperty::TargetType)-1 ),
		m_GPUResourceHandle(-1),
		m_ResizeFunction(NULL)
	{
		m_TextureVector.resize( RenderTargetProperty::TA_COUNT, NULL );
	}
	//------------------------------------------------------------------------------------------------------
	RenderTargetResource::~RenderTargetResource(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetResource::ReleaseResource()
	{
		Framework::Instance()->RemoveResizeviewListener( m_ResizeFunction );
		if ( -1 != m_GPUResourceHandle )
		{
			_DoDeleteRenderTarget( m_GPUResourceHandle );
			s_CurrentBind = -1;
		}
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetResource::ListenResizeview()
	{
		if ( NULL == m_ResizeFunction )
		{
			m_ResizeFunction = Framework::Instance()->RegistResizeviewListener( MEMBER_FUNC_PTR( &RenderTargetResource::OnResizeview ) );
		}
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetResource::OnResizeview( const ViewResolution& vr )
	{
		vector2f rsl = m_Size;
		rsl /= vr.m_OldResolution;
		rsl *= vr.m_NewResolution;
		m_Size = rsl;

		const vector4d& ovp = m_ViewPort;
		vector2f vp( ovp.m_z, ovp.m_w );
		vp /= vector2f( vr.m_OldViewSize.m_z, vr.m_OldViewSize.m_w );
		vp *= vector2f( vr.m_NewViewSize.m_z, vr.m_NewViewSize.m_w );
		m_ViewPort = vector4d( ovp.m_x, ovp.m_y, vp );

		FOR_EACH_CONST( RenderTargetProperty::AttachmentMap, it, m_AttachmentMap )
		{
			if ( NULL != it->second )
			{
				it->second->Resize( rsl );
			}
		}
		s_CurrentBind = m_GPUResourceHandle;
		_DoResize( m_GPUResourceHandle, m_ViewPort, m_Size, m_AttachmentMap );
	}
}