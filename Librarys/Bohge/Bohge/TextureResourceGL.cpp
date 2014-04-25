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
#include "IDevice.h"

#include <GLEW/glew.h>


namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	TextureResourceGL::TextureResourceGL(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	TextureResourceGL::~TextureResourceGL(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	int TextureResourceGL::_GLTextureType( TextureStatus::TextureType type )
	{
		switch( type )
		{
		case TextureStatus::TT_TEXTURE1D: return GL_TEXTURE_1D;
		case TextureStatus::TT_TEXTURE2D: return GL_TEXTURE_2D;
		case TextureStatus::TT_TEXTURE3D: return GL_TEXTURE_3D;
		case TextureStatus::TT_TEXTURECUBE: return GL_TEXTURE_CUBE_MAP;
		default:ASSERT( false );
		}		
		return GL_TEXTURE_2D;
	}
	//------------------------------------------------------------------------------------------------------
	int TextureResourceGL::_GLWrap( TextureStatus::TextureWarp w )
	{
		switch( w )
		{
		case TextureStatus::TW_CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		case TextureStatus::TW_MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
		case TextureStatus::TW_REPEAT: return GL_REPEAT;
		default: ASSERT(false); break;
		}
		return 0;
	}
	//------------------------------------------------------------------------------------------------------
	int TextureResourceGL::_GLFilter(TextureStatus::TextureFilter f)
	{
		switch( f )
		{
		case TextureStatus::TF_NEAREST: return GL_NEAREST;
		case TextureStatus::TF_LINEAR: return GL_LINEAR;
		case TextureStatus::TF_NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
		case TextureStatus::TF_LINEAR_MIPMAP_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
		case TextureStatus::TF_NEAREST_MIPMAP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case TextureStatus::TF_LINEAR_MIPMAP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		default: ASSERT(false); break;
		}
		return 0;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoActiveTexture( handle h )
	{
		glActiveTexture( GL_TEXTURE0 /*+ m_uTextrueReferenceCount*/ );
		glBindTexture( _GLTextureType( _GetTextureType() ), h );
		CHECK_ERROR_RENDER;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoSetWarpType( const TextureStatus::WarpType& wt )
	{
		ActiveTexture();
		int TexType = _GLTextureType( _GetTextureType() );
		glTexParameteri( TexType, GL_TEXTURE_WRAP_S, _GLWrap(wt.m_SWarp) );
		CHECK_ERROR_RENDER;
		glTexParameteri( TexType, GL_TEXTURE_WRAP_T,  _GLWrap(wt.m_TWarp) );
		CHECK_ERROR_RENDER;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoSetFilterType( const TextureStatus::FilterType& ft )
	{
		ActiveTexture();
		int TexType = _GLTextureType( _GetTextureType() );
		glTexParameteri(TexType, GL_TEXTURE_MAG_FILTER, _GLFilter(ft.m_MagFilter) );
		CHECK_ERROR_RENDER;
		glTexParameteri(TexType, GL_TEXTURE_MIN_FILTER, _GLFilter(ft.m_MinFilter) ); 
		CHECK_ERROR_RENDER;
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
			CHECK_ERROR_RENDER;
		}
	}
	//------------------------------------------------------------------------------------------------------
	int TextureResourceGL::_GLPixelFormat( TextureStatus::PixelFormat f )
	{
		switch(f)
		{
		case TextureStatus::PF_A8: return GL_ALPHA;
		case TextureStatus::PF_L8: return GL_LUMINANCE;
		case TextureStatus::PF_R8G8B8: return GL_RGB;
		case TextureStatus::PF_R5G6B5: return GL_RGB;
		case TextureStatus::PF_R8G8B8A8: return GL_RGBA;
		case TextureStatus::PF_R4G4B4A4: return GL_RGBA;
		case TextureStatus::PF_DEPTH16: return GL_DEPTH_COMPONENT;
		case TextureStatus::PF_DEPTH24_STENCIL8: return GL_DEPTH_COMPONENT;
		default: ASSERT(false);
		}
		return GL_RGBA;
	}
	//------------------------------------------------------------------------------------------------------
	int TextureResourceGL::_GLDataType( TextureStatus::PixelFormat f )
	{
		switch(f)
		{
		case TextureStatus::PF_A8: return GL_UNSIGNED_BYTE;
		case TextureStatus::PF_L8: return GL_UNSIGNED_BYTE;
		case TextureStatus::PF_R8G8B8: return GL_UNSIGNED_BYTE;
		case TextureStatus::PF_R5G6B5: return GL_UNSIGNED_SHORT_5_6_5;
		case TextureStatus::PF_R8G8B8A8: return GL_UNSIGNED_BYTE;
		case TextureStatus::PF_R4G4B4A4: return GL_UNSIGNED_SHORT_4_4_4_4;
		case TextureStatus::PF_DEPTH16: return GL_UNSIGNED_INT;
		case TextureStatus::PF_DEPTH24_STENCIL8: return GL_UNSIGNED_INT;
		default: ASSERT(false);
		}
		return GL_UNSIGNED_BYTE;
	}
	//------------------------------------------------------------------------------------------------------
	handle TextureResourceGL::_DoGenerateTexture( const TextureStatus::TextrueSourceDataVector& vec )
	{
		handle hTex = -1;
		glGenTextures( 1, &hTex );
		switch( _GetTextureType() )
		{
		case TextureStatus::TT_TEXTURE1D: ASSERT(false); break;
		case TextureStatus::TT_TEXTURE2D:
			{
				glBindTexture( GL_TEXTURE_2D, hTex );
				s_CurrentBind = hTex;
				GLint format = _GLPixelFormat( _GetPixelFormat() );
				glTexImage2D( GL_TEXTURE_2D, 0, format, vec[0]->m_Size.m_x, vec[0]->m_Size.m_y, 0, format,
					_GLDataType( _GetPixelFormat() ), vec[0]->m_pData );
				CHECK_ERROR_RENDER;
				glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap

			}break;
		case TextureStatus::TT_TEXTURE3D: ASSERT(false); break;
		case TextureStatus::TT_TEXTURECUBE:
			{
				glBindTexture( GL_TEXTURE_CUBE_MAP, hTex );
				s_CurrentBind = hTex;
				GLint format = _GLPixelFormat( _GetPixelFormat() );
				GLint type = _GLDataType( _GetPixelFormat() );
				glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, vec[0]->m_Size.m_x, vec[0]->m_Size.m_y, 0, format, type, vec[0]->m_pData );
				CHECK_ERROR_RENDER;
				glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, vec[1]->m_Size.m_x, vec[1]->m_Size.m_y, 0, format, type, vec[1]->m_pData );
				CHECK_ERROR_RENDER;
				glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, vec[2]->m_Size.m_x, vec[2]->m_Size.m_y, 0, format, type, vec[2]->m_pData );
				CHECK_ERROR_RENDER;
				glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, vec[3]->m_Size.m_x, vec[3]->m_Size.m_y, 0, format, type, vec[3]->m_pData );
				CHECK_ERROR_RENDER;
				glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, vec[4]->m_Size.m_x, vec[4]->m_Size.m_y, 0, format, type, vec[4]->m_pData );
				CHECK_ERROR_RENDER;
				glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, vec[5]->m_Size.m_x, vec[5]->m_Size.m_y, 0, format, type, vec[5]->m_pData );
				CHECK_ERROR_RENDER;
			}break;
		default:ASSERT(false&&"Unkown texture type!");
		}
		return hTex;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResourceGL::_DoDeleteTexture( handle &h )
	{
		glDeleteTextures(1, &h);
		h = -1;
	}
}