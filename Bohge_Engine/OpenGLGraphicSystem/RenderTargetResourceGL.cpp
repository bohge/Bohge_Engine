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
#include "RenderTargetResourceGL.h"
#include "Framework/TextureEntity.h"
#include "Framework/MaterialResource.h"
#include "Framework/IGraphicSystem.h"

#include <GLEW/glew.h>


using namespace BohgeEngine;

namespace OpenGLGraphicSystemPlugin
{
	//------------------------------------------------------------------------------------------------------
	RenderTargetResourceGL::RenderTargetResourceGL(void)
		:m_DepthBuffer(-1)
	{
	}
	//------------------------------------------------------------------------------------------------------
	RenderTargetResourceGL::~RenderTargetResourceGL(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	handle RenderTargetResourceGL::_DoGenerateRenderTarget( RenderTargetProperty::TargetType type, const vector2d& size )
	{
		m_Size = size;
		//生成绑定纹理
		handle rt = 0;
		if ( RenderTargetProperty::RT_RENDER_TARGET_MAIN != type )
		{
			glGenFramebuffers(1, &rt);
			CHECK_GRAPHIC_SYSTEM_ERROR;
		}
		return rt;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetResourceGL::_DoSetAttachment( handle h, RenderTargetProperty::TargetType type, RenderTargetProperty::RenderTargetAttachment att, TextureEntity* tex )
	{
		if ( RenderTargetProperty::RT_RENDER_TARGET_MAIN != type )
		{
			glBindFramebuffer(GL_FRAMEBUFFER, h);
			CHECK_GRAPHIC_SYSTEM_ERROR;
			switch( att )
			{
			case RenderTargetProperty::TA_COLOR_0:
			case RenderTargetProperty::TA_COLOR_1:
			case RenderTargetProperty::TA_COLOR_2:
			case RenderTargetProperty::TA_COLOR_3:
			case RenderTargetProperty::TA_COLOR_4:
			case RenderTargetProperty::TA_COLOR_5:
			case RenderTargetProperty::TA_COLOR_6:
			case RenderTargetProperty::TA_COLOR_7:
			case RenderTargetProperty::TA_COLOR_8:
			case RenderTargetProperty::TA_COLOR_9:
				{
					switch( type )
					{
					case RenderTargetProperty::RT_RENDER_TARGET_2D:
						{
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + att, GL_TEXTURE_2D, tex->GetResourceHandle(), 0);
							CHECK_GRAPHIC_SYSTEM_ERROR;
						}break;
					case RenderTargetProperty::RT_RENDER_TARGET_CUBE:
						{
							for ( int i = 0; i < 6; i ++ )
							{
								glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + att, GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, tex->GetResourceHandle(), 0 );
								CHECK_GRAPHIC_SYSTEM_ERROR;
							}
						}break;
					default:ASSERT(false);
					}
				}break;
			case RenderTargetProperty::TA_DEPTH:
				{
					if ( NULL != tex )
					{
						glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->GetResourceHandle() );
						CHECK_GRAPHIC_SYSTEM_ERROR;
					}
					else
					{
						ASSERT( -1 == m_DepthBuffer );
						glGenRenderbuffers( 1, &m_DepthBuffer );
						glBindRenderbuffer( GL_RENDERBUFFER, m_DepthBuffer );
						CHECK_GRAPHIC_SYSTEM_ERROR;
						glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, static_cast<GLsizei>(m_Size.m_x), static_cast<GLsizei>(m_Size.m_y) );
						CHECK_GRAPHIC_SYSTEM_ERROR;
						glBindRenderbuffer( GL_RENDERBUFFER, 0 );
						CHECK_GRAPHIC_SYSTEM_ERROR;
						glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer );
						CHECK_GRAPHIC_SYSTEM_ERROR;
					}
				}break;
			case RenderTargetProperty::TA_DEPTH_STENCIL:
				{
					if ( NULL != tex )
					{
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->GetResourceHandle(), 0);
						CHECK_GRAPHIC_SYSTEM_ERROR;
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex->GetResourceHandle(), 0);
						CHECK_GRAPHIC_SYSTEM_ERROR;
					}
					else
					{
						ASSERT( -1 == m_DepthBuffer );
						glGenRenderbuffers( 1, &m_DepthBuffer );
						glBindRenderbuffer( GL_RENDERBUFFER, m_DepthBuffer );
						glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_Size.m_x), static_cast<GLsizei>(m_Size.m_y) );
						CHECK_GRAPHIC_SYSTEM_ERROR;
						glBindRenderbuffer( GL_RENDERBUFFER, 0 );
						CHECK_GRAPHIC_SYSTEM_ERROR;
						glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer );
						CHECK_GRAPHIC_SYSTEM_ERROR;
						glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer );
						CHECK_GRAPHIC_SYSTEM_ERROR;
					}
				}break;
			default:ASSERT(false);
			}
			glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		}
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetResourceGL::_DoPushRenderTarget( handle h, RenderTargetProperty::TargetType tar, RenderTargetProperty::TargetFace face, const RenderTargetProperty::AttachmentMap& attmap )
	{
		glBindFramebuffer( GL_FRAMEBUFFER, h );
		CHECK_GRAPHIC_SYSTEM_ERROR;
		//绑定color
		if( RenderTargetProperty::RT_RENDER_TARGET_MAIN != tar )
		{
			FOR_EACH_CONST( RenderTargetProperty::AttachmentMap, it, attmap )
			{
				switch( it->first )
				{
				case RenderTargetProperty::TA_COLOR_0:
				case RenderTargetProperty::TA_COLOR_1:
				case RenderTargetProperty::TA_COLOR_2:
				case RenderTargetProperty::TA_COLOR_3:
				case RenderTargetProperty::TA_COLOR_4:
				case RenderTargetProperty::TA_COLOR_5:
				case RenderTargetProperty::TA_COLOR_6:
				case RenderTargetProperty::TA_COLOR_7:
				case RenderTargetProperty::TA_COLOR_8:
				case RenderTargetProperty::TA_COLOR_9:
					{
						ASSERT( NULL != it->second );
						GLenum target =
							RenderTargetProperty::RT_RENDER_TARGET_2D == tar
							? GL_TEXTURE_2D
							: GL_TEXTURE_CUBE_MAP_POSITIVE_X+face;
						glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + it->first, target, it->second->GetResourceHandle(), 0 );
						CHECK_GRAPHIC_SYSTEM_ERROR;
					}break;
				case RenderTargetProperty::TA_DEPTH:
					{
						if ( NULL == it->second )
						{
							glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer );
							CHECK_GRAPHIC_SYSTEM_ERROR;
						}
						else
						{
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, it->second->GetResourceHandle(), 0);
							CHECK_GRAPHIC_SYSTEM_ERROR;
						}
					}break;
				case RenderTargetProperty::TA_DEPTH_STENCIL:
					{
						if ( NULL == it->second )
						{
							glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer );
							CHECK_GRAPHIC_SYSTEM_ERROR;
							glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer );
							CHECK_GRAPHIC_SYSTEM_ERROR;
						}
						else
						{
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, it->second->GetResourceHandle(), 0);
							CHECK_GRAPHIC_SYSTEM_ERROR;
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, it->second->GetResourceHandle(), 0);
							CHECK_GRAPHIC_SYSTEM_ERROR;
						}
					}break;
				default:ASSERT(false);
				}
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetResourceGL::_DoDeleteRenderTarget( handle h )
	{
		glDeleteFramebuffers( 1, &h );
		if ( -1 != m_DepthBuffer )
		{
			glDeleteRenderbuffers( 1, &m_DepthBuffer );
		}
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetResourceGL::_DoClearBuffer( RenderTargetProperty::ClearFlag flag )
	{
		bool fixed = false;
		if ( !MaterialResource::IsDepthMask() && 0 != (flag&RenderTargetProperty::CF_DEPTH) )//当depthmask关闭的时候，清除深度会有问题
		{
			fixed = true;
			glDepthMask( GL_TRUE );
			CHECK_GRAPHIC_SYSTEM_ERROR;
		}
		GLbitfield glmask = 0;
		if( 0 != (flag&RenderTargetProperty::CF_COLOR))
		{
			glmask |= GL_COLOR_BUFFER_BIT;
		}
		if( 0 != (flag&RenderTargetProperty::CF_DEPTH))
		{
			glmask |= GL_DEPTH_BUFFER_BIT;
		}
		if( 0 != (flag&RenderTargetProperty::CF_STENCIL))
		{
			glmask |= GL_STENCIL_BUFFER_BIT;
		}
		if( 0 != glmask )
		{
			glClear( glmask );
			CHECK_GRAPHIC_SYSTEM_ERROR;
		}
		if ( fixed )
		{
			glDepthMask( GL_FALSE );
			CHECK_GRAPHIC_SYSTEM_ERROR;
		}
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetResourceGL::_DoSetViewPort( const vector4d& vp )
	{
		glViewport(vp.m_x,vp.m_y,vp.m_z,vp.m_w);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetResourceGL::_DoSetClearColor( const Color& c )
	{
		glClearColor( c.m_r, c.m_g, c.m_b, c.m_a );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetResourceGL::_DoResize( handle h, const vector4d& vp, const vector2d& rsl, const RenderTargetProperty::AttachmentMap& attmap )
	{
		if ( RenderTargetProperty::RT_RENDER_TARGET_MAIN != GetTargetType() )
		{
			FOR_EACH_CONST( RenderTargetProperty::AttachmentMap, it, attmap )
			{
				if ( NULL == it->second )
				{
					glBindFramebuffer(GL_FRAMEBUFFER, h);
					CHECK_GRAPHIC_SYSTEM_ERROR;
					switch( it->first )
					{
					case RenderTargetProperty::TA_COLOR_0:
					case RenderTargetProperty::TA_COLOR_1:
					case RenderTargetProperty::TA_COLOR_2:
					case RenderTargetProperty::TA_COLOR_3:
					case RenderTargetProperty::TA_COLOR_4:
					case RenderTargetProperty::TA_COLOR_5:
					case RenderTargetProperty::TA_COLOR_6:
					case RenderTargetProperty::TA_COLOR_7:
					case RenderTargetProperty::TA_COLOR_8:
					case RenderTargetProperty::TA_COLOR_9: ASSERT(false); break;
					case RenderTargetProperty::TA_DEPTH:
						{
							ASSERT( -1 != m_DepthBuffer );
							glBindRenderbuffer( GL_RENDERBUFFER, m_DepthBuffer );
							CHECK_GRAPHIC_SYSTEM_ERROR;
							glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, static_cast<GLsizei>(m_Size.m_x), static_cast<GLsizei>(m_Size.m_y) );
							CHECK_GRAPHIC_SYSTEM_ERROR;
							glBindRenderbuffer( GL_RENDERBUFFER, 0 );
							CHECK_GRAPHIC_SYSTEM_ERROR;
						}break;
					case RenderTargetProperty::TA_DEPTH_STENCIL:
						{
							ASSERT( -1 != m_DepthBuffer );
							glBindRenderbuffer( GL_RENDERBUFFER, m_DepthBuffer );
							glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_Size.m_x), static_cast<GLsizei>(m_Size.m_y) );
							CHECK_GRAPHIC_SYSTEM_ERROR;
							glBindRenderbuffer( GL_RENDERBUFFER, 0 );
							CHECK_GRAPHIC_SYSTEM_ERROR;
						}break;
					default:ASSERT(false);
					}
				}
			}
		}
	}

}