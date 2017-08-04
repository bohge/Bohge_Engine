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
#include "TextureResourceGL.h"
#include "Framework/IGraphicSystem.h"

#include "OpenGLGraphicSystem.h"
#include "ShaderResourceGL.h"

#include <GLEW/glew.h>

using namespace BohgeEngine;
namespace OpenGLGraphicSystemPlugin
{
	//------------------------------------------------------------------------------------------------------
	TextureResourceGL::TextureResourceGL(void)
		:m_hSlotIndex(0)
	{
	}
	//------------------------------------------------------------------------------------------------------
	TextureResourceGL::~TextureResourceGL(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	int TextureResourceGL::_GLTextureType( TextureProperty::TextureType type ) const
	{
		switch( type )
		{
		case TextureProperty::TT_TEXTURE1D: return GL_TEXTURE_1D;
		case TextureProperty::TT_TEXTURE2D: return GL_TEXTURE_2D;
		case TextureProperty::TT_TEXTURE3D: return GL_TEXTURE_3D;
		case TextureProperty::TT_TEXTURECUBE: return GL_TEXTURE_CUBE_MAP;
		default:ASSERT( false );
		}		
		return GL_TEXTURE_2D;
	}
	//------------------------------------------------------------------------------------------------------
	int TextureResourceGL::_GLWrap( TextureProperty::TextureWarp w )
	{
		switch( w )
		{
		case TextureProperty::TW_CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		case TextureProperty::TW_MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
		case TextureProperty::TW_REPEAT: return GL_REPEAT;
		default: ASSERT(false); break;
		}
		return 0;
	}
	//------------------------------------------------------------------------------------------------------
	int TextureResourceGL::_GLFilter(TextureProperty::TextureFilter f)
	{
		switch( f )
		{
		case TextureProperty::TF_NEAREST: return GL_NEAREST;
		case TextureProperty::TF_LINEAR: return GL_LINEAR;
		case TextureProperty::TF_NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
		case TextureProperty::TF_LINEAR_MIPMAP_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
		case TextureProperty::TF_NEAREST_MIPMAP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case TextureProperty::TF_LINEAR_MIPMAP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		default: ASSERT(false); break;
		}
		return 0;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoActiveTexture( handle h ) const
	{
		OpenGLGraphicSystem* glsys = static_cast<OpenGLGraphicSystem*>( IGraphicSystem::Instance() );
		const_cast<TextureResourceGL*>(this)->m_hSlotIndex = glsys->GetTextureReferencre();
		glsys->IncreaseTextureReferencre();
		glActiveTexture( GL_TEXTURE0 + m_hSlotIndex );
		CHECK_GRAPHIC_SYSTEM_ERROR;
		glBindTexture( _GLTextureType( _GetTextureType() ), h );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoSetWarpType( const TextureProperty::WarpType& wt )
	{
		ActiveTexture();
		int TexType = _GLTextureType( _GetTextureType() );
		glTexParameteri( TexType, GL_TEXTURE_WRAP_S, _GLWrap(wt.m_SWarp) );
		CHECK_GRAPHIC_SYSTEM_ERROR;
		glTexParameteri( TexType, GL_TEXTURE_WRAP_T,  _GLWrap(wt.m_TWarp) );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoSetFilterType( const TextureProperty::FilterType& ft )
	{
		ActiveTexture();
		int TexType = _GLTextureType( _GetTextureType() );
		glTexParameteri(TexType, GL_TEXTURE_MAG_FILTER, _GLFilter(ft.m_MagFilter) );
		CHECK_GRAPHIC_SYSTEM_ERROR;
		glTexParameteri(TexType, GL_TEXTURE_MIN_FILTER, _GLFilter(ft.m_MinFilter) ); 
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoSetAnisotropic( uint ani )
	{
		if( /*true == m_isUsingAnisotropic &&*/ 0 != ani )	//如果支持各向异性
		{
			ActiveTexture();
			int TexType = _GLTextureType( _GetTextureType() );
			/*uint to = x <= m_fMaxAnisotropic ? x : m_fMaxAnisotropic;*/
			glTexParameterf(TexType, GL_TEXTURE_MAX_ANISOTROPY_EXT, ani );
			CHECK_GRAPHIC_SYSTEM_ERROR;
		}
	}
	//------------------------------------------------------------------------------------------------------
	int TextureResourceGL::_GLPixelFormat( TextureProperty::PixelFormat f )
	{
		switch(f)
		{
		case TextureProperty::PF_A8: return GL_ALPHA;
		case TextureProperty::PF_L8: return GL_LUMINANCE;
		case TextureProperty::PF_R8G8B8: return GL_RGB;
		case TextureProperty::PF_R5G6B5: return GL_RGB;
		case TextureProperty::PF_R8G8B8A8: return GL_RGBA;
		case TextureProperty::PF_R4G4B4A4: return GL_RGBA;
		case TextureProperty::PF_DEPTH16: return GL_DEPTH_COMPONENT;
		case TextureProperty::PF_DEPTH24_STENCIL8: return GL_DEPTH_STENCIL;
		default: ASSERT(false);
		}
		return GL_RGBA;
	}
	//------------------------------------------------------------------------------------------------------
	int TextureResourceGL::_GLDataType( TextureProperty::PixelFormat f )
	{
		switch(f)
		{
		case TextureProperty::PF_A8: return GL_UNSIGNED_BYTE;
		case TextureProperty::PF_L8: return GL_UNSIGNED_BYTE;
		case TextureProperty::PF_R8G8B8: return GL_UNSIGNED_BYTE;
		case TextureProperty::PF_R5G6B5: return GL_UNSIGNED_SHORT_5_6_5;
		case TextureProperty::PF_R8G8B8A8: return GL_UNSIGNED_BYTE;
		case TextureProperty::PF_R4G4B4A4: return GL_UNSIGNED_SHORT_4_4_4_4;
		case TextureProperty::PF_DEPTH16: return GL_UNSIGNED_INT;
		case TextureProperty::PF_DEPTH24_STENCIL8: return GL_UNSIGNED_INT_24_8;
		default: ASSERT(false);
		}
		return GL_UNSIGNED_BYTE;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoResize( const vector2d& size )
	{
		switch( _GetTextureType() )
		{
		case TextureProperty::TT_TEXTURE2D:
			{
				ActiveTexture();
				GLint format = _GLPixelFormat( _GetPixelFormat() );
				glTexImage2D( GL_TEXTURE_2D, 0, format, size.m_x, size.m_y, 0, format,
					_GLDataType( _GetPixelFormat() ), NULL );
				CHECK_GRAPHIC_SYSTEM_ERROR;
				if( isMipMap() )glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
				CHECK_GRAPHIC_SYSTEM_ERROR;
			}break;
		case TextureProperty::TT_TEXTURE1D:
		case TextureProperty::TT_TEXTURE3D:
		case TextureProperty::TT_TEXTURECUBE: ASSERT(false); break;
		default:ASSERT(false&&"Unkown texture type!");
		}
	}
	//------------------------------------------------------------------------------------------------------
	handle TextureResourceGL::_DoGenerateTexture( const TextureProperty::TextrueSourceDataVector& vec )
	{
		handle hTex = -1;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures( 1, &hTex );
		switch( _GetTextureType() )
		{
		case TextureProperty::TT_TEXTURE1D: ASSERT(false); break;
		case TextureProperty::TT_TEXTURE2D:
			{
				glBindTexture( GL_TEXTURE_2D, hTex );
				GLint format = _GLPixelFormat( _GetPixelFormat() );
				glTexImage2D( GL_TEXTURE_2D, 0, format, vec[0]->m_Size.m_x, vec[0]->m_Size.m_y, 0, format,
					_GLDataType( _GetPixelFormat() ), vec[0]->m_pData );
				CHECK_GRAPHIC_SYSTEM_ERROR;
				if( isMipMap() )glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
				CHECK_GRAPHIC_SYSTEM_ERROR;
			}break;
		case TextureProperty::TT_TEXTURE3D: ASSERT(false); break;
		case TextureProperty::TT_TEXTURECUBE:
			{
				glBindTexture( GL_TEXTURE_CUBE_MAP, hTex );
				s_CurrentBind = hTex;
				GLint format = _GLPixelFormat( _GetPixelFormat() );
				GLint type = _GLDataType( _GetPixelFormat() );
				glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, vec[0]->m_Size.m_x, vec[0]->m_Size.m_y, 0, format, type, vec[0]->m_pData );
				CHECK_GRAPHIC_SYSTEM_ERROR;
				if( isMipMap() )glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
				CHECK_GRAPHIC_SYSTEM_ERROR;
				glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, vec[1]->m_Size.m_x, vec[1]->m_Size.m_y, 0, format, type, vec[1]->m_pData );
				CHECK_GRAPHIC_SYSTEM_ERROR;
				if( isMipMap() )glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
				CHECK_GRAPHIC_SYSTEM_ERROR;
				glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, vec[2]->m_Size.m_x, vec[2]->m_Size.m_y, 0, format, type, vec[2]->m_pData );
				CHECK_GRAPHIC_SYSTEM_ERROR;
				if( isMipMap() )glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
				CHECK_GRAPHIC_SYSTEM_ERROR;
				glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, vec[3]->m_Size.m_x, vec[3]->m_Size.m_y, 0, format, type, vec[3]->m_pData );
				CHECK_GRAPHIC_SYSTEM_ERROR;
				if( isMipMap() )glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
				CHECK_GRAPHIC_SYSTEM_ERROR;
				glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, vec[4]->m_Size.m_x, vec[4]->m_Size.m_y, 0, format, type, vec[4]->m_pData );
				CHECK_GRAPHIC_SYSTEM_ERROR;
				if( isMipMap() )glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
				CHECK_GRAPHIC_SYSTEM_ERROR;
				glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, vec[5]->m_Size.m_x, vec[5]->m_Size.m_y, 0, format, type, vec[5]->m_pData );
				CHECK_GRAPHIC_SYSTEM_ERROR;
				if( isMipMap() )glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
				CHECK_GRAPHIC_SYSTEM_ERROR;
			}break;
		default:ASSERT(false&&"Unkown texture type!");
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		return hTex;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoDeleteTexture( handle h )
	{
		glDeleteTextures(1, &h);
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoGenerateMipMap()
	{
		ActiveTexture();
		switch( _GetTextureType() )
		{
		case TextureProperty::TT_TEXTURE1D: ASSERT(false); break;
		case TextureProperty::TT_TEXTURE2D:
			{
				glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
				CHECK_GRAPHIC_SYSTEM_ERROR;
			}break;
		case TextureProperty::TT_TEXTURE3D: ASSERT(false); break;
		case TextureProperty::TT_TEXTURECUBE:
			{
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);//生成mipmap
				CHECK_GRAPHIC_SYSTEM_ERROR;
			}break;
		default:ASSERT(false&&"Unkown texture type!");
		}
	}
	//------------------------------------------------------------------------------------------------------
	handle TextureResourceGL::_DoGetHandle() const
	{
		return m_hSlotIndex;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoSubstituteTextureBuffer( const BohgeEngine::vector2d& begin, const BohgeEngine::vector2d& size, const byte* const buffer )
	{
		ActiveTexture();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GLenum glformat, gltype, gltext;		
		gltext = _GLTextureType( _GetTextureType() );
		glformat = _GLPixelFormat( _GetPixelFormat() );
		gltype = _GLDataType( _GetPixelFormat() );
		glTexSubImage2D( gltext, 0, begin.m_x, begin.m_y, size.m_x, size.m_y, glformat, gltype, buffer );
		CHECK_GRAPHIC_SYSTEM_ERROR;
		if ( isMipMap() )
		{
			glGenerateMipmap( gltext );
			CHECK_GRAPHIC_SYSTEM_ERROR;
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoSetTextureUseage( TextureProperty::TextureUseage tu )
	{
		//glPixelStorei此函数为全局所以此处的设置没有意义
		//ActiveTexture();
		//switch( tu )
		//{
		//case TextureProperty::TU_STATIC:
		//	{
		//		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		//		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		//	}break;
		//case TextureProperty::TU_READ:
		//	{
		//		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		//	}break;
		//case TextureProperty::TU_WRITE:
		//	{
		//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//	}break;
		//default:ASSERT(false);
		//}
	}

}