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



/////////////////////////////////
//			渲染目标		   //
/////////////////////////////////

#include "RenderTarget.h"
#include "Texture.h"
#include "Engine.h"

namespace BohgeEngine
{
	////-------------------------------------------------------------------------------------------------------
	RenderTarget::RenderTarget()
		:m_eType(Device::FBO_EMPTY),
		m_hasTexColor(false),
		m_hasTexDepthStencil(false),
		m_pColorTexture(NULL),
		m_pDepthStencilTexture(NULL),
		m_hRenderTarget(-1),
		m_hDepthBuffer(-1),
		m_isGet(false)
	{

	}
	////-------------------------------------------------------------------------------------------------------
	//void RenderTarget::Clear()
	//{
	//	m_vSize = vector2d::Zero();
	//	m_eType = Device::FBO_Empty;
	//	m_hasTexColor = false;
	//	m_hasTexDepthStencil = false;
	//	m_pColorTexture = NULL
	//	m_pDepthStencilTexture = NULL;
	//	m_hRenderTarget = -1;
	//	m_hDepthBuffer = -1;
	//	m_isGet = false;
	//}
	//-------------------------------------------------------------------------------------------------------
	RenderTarget::RenderTarget(const vector2d& size, Device::TargetType type)
		:m_vViewPort( 0, 0, size.m_x, size.m_y ),
		m_vSize(size),
		m_eType(type),
		m_hasTexColor(false),
		m_hasTexDepthStencil(false),
		m_pColorTexture(NULL),
		m_pDepthStencilTexture(NULL),
		m_hRenderTarget(-1),
		m_hDepthBuffer(-1),
		m_isGet(false)
	{
		switch( m_eType )
		{
		case Device::FBO_TCOLOR:
			{
				m_hasTexColor = true;
				m_pColorTexture = NEW Texture2D();
			}break;
		case Device::FBO_TDEPTH:
			{
				m_hasTexDepthStencil = true;
				m_pDepthStencilTexture = NEW Texture2D();
			}break;
		case Device::FBO_TCOLOR_TDEPTH:
			{
				m_hasTexColor = true;
				m_pColorTexture = NEW Texture2D();
				m_hasTexDepthStencil = true;
				m_pDepthStencilTexture = NEW Texture2D();
			}break;
		case Device::FBO_TDEPTHSTENCIL:
			{
				m_hasTexDepthStencil = true;
				m_pDepthStencilTexture = NEW Texture2D();
			}break;
		case Device::FBO_TCOLOR_BDEPTH:
			{
				m_hasTexColor = true;
				m_pColorTexture = NEW Texture2D();
			}break;
		case Device::FBO_TCOLOR_BDEPTHSTENCIL:
			{
				m_hasTexColor = true;
				m_pColorTexture = NEW Texture2D();
			}break;
		case Device::FBO_TCOLOR_TDEPTHSTENCIL:
			{
				m_hasTexColor = true;
				m_pColorTexture = NEW Texture2D();
				m_hasTexDepthStencil = true;
				m_pDepthStencilTexture = NEW Texture2D();
			}break;
		case Device::FBO_CCOLOR_BDEPTH:
			{
				m_hasTexColor = true;
				m_pColorTexture = NEW TextureCube();
			}break;
		default:
			ASSERT(false);
		}
		Engine::Instance().GetDevice()->CreateRenderTarget( *this );
	}
	//-------------------------------------------------------------------------------------------------------
	RenderTarget::~RenderTarget()
	{
		if( !m_isGet )
		{
			SAFE_DELETE( m_pColorTexture );
			SAFE_DELETE( m_pDepthStencilTexture );
			Engine::Instance().GetDevice()->DestroyRenderTarget( *this );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void RenderTarget::Resize( const vector2d& size )
	{
		if( !m_isGet )
		{
			//删除旧的
			if ( NULL != m_pColorTexture )
			{
				m_pColorTexture->UnloadTexture();
			}
			if ( NULL != m_pDepthStencilTexture )
			{
				m_pDepthStencilTexture->UnloadTexture();
			}
			Engine::Instance().GetDevice()->DestroyRenderTarget( *this );
			//搞个新的
			m_vViewPort = vector4d( 0, 0, size.m_x, size.m_y);
			m_vSize = size;
			Engine::Instance().GetDevice()->CreateRenderTarget( *this );
		}
		else
		{
			ASSERT(false);
		}
	}

	////-------------------------------------------------------------------------------------------------------
	//RenderTarget& RenderTarget::operator=( const RenderTarget& rt )
	//{
	//	m_vSize = rt.m_vSize;
	//	m_eType = rt.m_eType;
	//	m_hasTexColor = rt.m_hasTexColor;
	//	m_hasTexDepthStencil = rt.m_hasTexDepthStencil;
	//	m_pColorTexture = rt.m_pColorTexture;
	//	m_pDepthStencilTexture = rt.m_pDepthStencilTexture;
	//	m_hRenderTarget = rt.m_hRenderTarget;
	//	m_hDepthBuffer = rt.m_hDepthBuffer;
	//	m_isGet = true;
	//}
	//-------------------------------------------------------------------------------------------------------
	//void RenderTarget::PushRenderTarget( )
	//{
	//	Engine::Instance().GetDevice()->PushRenderTarget( this );
	//}
	////-------------------------------------------------------------------------------------------------------
	//void RenderTarget::PopRenderTarget( )
	//{
	//	Engine::Instance().GetDevice()->PopRenderTarget();
	//}
	//-------------------------------------------------------------------------------------------------------
	//void RenderTarget::Clear(engine& engine)
	//{
	//	engine.GetDevice()->PushRenderTarget(this);
	//	switch( m_eType )
	//	{
	//	case Device::FBO_Unkonw:
	//		{
	//			engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER | Device::STENCIL_BUFFER );
	//		}break;
	//	case Device::FBO_texColor:
	//		{
	//			engine.GetDevice()->Clear( Device::COLOR_BUFFER );
	//		}break;
	//	case Device::FBO_texDepth:
	//		{
	//			engine.GetDevice()->Clear( Device::DEPTH_BUFFER );
	//		}break;
	//	case Device::FBO_texColor_texDepth:
	//		{
	//			engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER );
	//		}break;
	//	case Device::FBO_TCOLOR_BDEPTH:
	//		{
	//			engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER );
	//		}break;
	//	case Device::FBO_texDepthStencil:
	//		{
	//			engine.GetDevice()->Clear( Device::DEPTH_BUFFER | Device::STENCIL_BUFFER );
	//		}break;
	//	case Device::FBO_texColor_bufDepthStencil:
	//		{
	//			engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER | Device::STENCIL_BUFFER );
	//		}break;
	//	case Device::FBO_texColor_texDepthStencil:
	//		{
	//			engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER | Device::STENCIL_BUFFER );
	//		}break;
	//	default:
	//		ASSERT(false);
	//	}
	//	engine.GetDevice()->PopRenderTarget(this);
	//}
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------





	RenderTargetCube::RenderTargetCube( const vector2d& size )
		:RenderTarget(size, Device::FBO_CCOLOR_BDEPTH),
		m_CurrentFace( CF_POSITIVE_X )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	RenderTargetCube::~RenderTargetCube()
	{

	}

}