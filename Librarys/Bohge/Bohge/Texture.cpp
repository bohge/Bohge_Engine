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



/////////////////////////////////////////
//			enter point of game		   //
/////////////////////////////////////////


#include "Texture.h"
#include "Predefine.h"
#include "ResourceManager.h"
#include "png.h"
#include "pnginfo.h"
#include "pngstruct.h"
#include "IOSystem.h"
#include "TextureData.h"
#include "Engine.h"
#include "IFile.h"

using namespace std;

namespace BohgeEngine
{
	//-------------------------------------------------------------------
	static void png_rw(png_structp png_ptr, png_bytep data, png_size_t length) 
	{
		IFile* file = static_cast<IFile*>(png_ptr->io_ptr);
		file->ReadFile(data, length);
		//File::Instance().ReadFile(data, length, 1, file);
	}
	static void png_flush(png_structp png_ptr) 
	{
	}
	void Texture::SaveTexture( const OutPutTextureData& saveto )
	{
		unsigned long i;
		png_structp png_ptr;
		png_infop info_ptr;
		png_colorp palette;
		png_byte *image;
		png_bytep *row_pointers;

		IFile* writefile = IOSystem::Instance().FileFactory(saveto.m_Path);
		writefile->OpenFile( IFile::AT_READ );


		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		info_ptr = png_create_info_struct(png_ptr);
		png_set_write_fn( png_ptr, writefile, png_rw, png_flush );
		//png_init_io(png_ptr, writefile.BaseFile() );

		int colortype, bitesize;
		switch( saveto.m_Pixel )
		{
		case Device::PF_R8G8B8: colortype = PNG_COLOR_TYPE_RGB; bitesize = 3; break;
		case Device::PF_R8G8B8A8: colortype = PNG_COLOR_TYPE_RGBA; bitesize = 4; break;
		}
		png_set_IHDR(png_ptr, info_ptr, saveto.m_Size.m_x, saveto.m_Size.m_y, 8, colortype,
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));
		png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
		png_write_info(png_ptr, info_ptr);
		png_set_packing(png_ptr);
		row_pointers = (png_bytep *)malloc(saveto.m_Size.m_y * sizeof(png_bytep));

		for (i = 0; i < saveto.m_Size.m_y; i++)
		{
			row_pointers[i] = (png_bytep)&saveto.m_Data[i * saveto.m_Size.m_y * bitesize];
		}
		png_write_image(png_ptr, row_pointers);
		png_write_end(png_ptr, info_ptr);
		png_free(png_ptr, palette);
		palette = NULL;
		png_destroy_write_struct(&png_ptr, &info_ptr);
		free(row_pointers);
		row_pointers = NULL;
		writefile->CloseFile( );
		IOSystem::Instance().FileDestroy( writefile );
	}



	//-------------------------------------------------------------------
	Texture::Texture(Device::TextureType type)
		:m_eType(type),
		m_isLoaded(false),
		m_hTextureHandle(0),
		m_hasMipMap(false),
		m_isLoadFormFile(false),
		m_Anisotropic(0)
	{

	}
	//-------------------------------------------------------------------
	Texture::~Texture()
	{
		this->UnloadTexture();
	}
	//-------------------------------------------------------------------
	void Texture::UnloadTexture()
	{
		if( false == m_isLoadFormFile )
		{
			Engine::Instance().GetDevice()->DeleteTexture( m_hTextureHandle );
		}
		else
		{
			Engine::Instance().GetResourceManager()->ReleaseTexture( m_eFormat, m_Name);
		}
		m_isLoadFormFile = false;
		m_isLoaded = false;
		m_hasMipMap = false;
		m_hTextureHandle = 0;
		m_vSize = vector2d::Zero();
		m_Name.clear();
	}
	//-------------------------------------------------------------------
	void Texture::SetWarp( Device::TextureWarp s, Device::TextureWarp t )
	{
		if ( m_Warp.m_SWarp != s && m_Warp.m_TWarp != t )
		{
			m_Warp.m_SWarp = s;
			m_Warp.m_TWarp = t;
			Engine::Instance().GetDevice()->SetTextureWarp( m_hTextureHandle, m_eType, s, t );
		}
	}	
	//-------------------------------------------------------------------
	void Texture::GenerateMipmap()
	{
		m_hasMipMap = true;
		Engine::Instance().GetDevice()->GenerateMipmap( m_hTextureHandle, m_eType );
	}
	//-------------------------------------------------------------------
	void Texture::SetFilter( Device::TextureFilter mag, Device::TextureFilter min )
	{
		if ( m_Filter.m_MagFilter != mag && m_Filter.m_MinFilter != min )
		{
			m_Filter.m_MagFilter = mag;
			m_Filter.m_MinFilter = min;
			Engine::Instance().GetDevice()->SetTextureFilter( m_hTextureHandle, m_eType, mag, min );
		}
	}
	//-------------------------------------------------------------------
	void Texture::SetAnisotropic( uint x )
	{
		if ( x != m_Anisotropic )
		{
			m_Anisotropic = Engine::Instance().GetDevice()->SetTextureAnisotropic( m_hTextureHandle, m_eType, x );
		}
	}


	//-------------------------------------------------------------------
	Texture2D::Texture2D()
		:Texture(Device::TT_TEXTURE2D)
	{
	}
	//-------------------------------------------------------------------
	Texture2D::~Texture2D()
	{
	}
	//-------------------------------------------------------------------
	bool Texture2D::LoadTexture( Device::PixelFormat f, const string& strFileName )
	{
		if( m_isLoaded && m_Name == strFileName )//确保加载相同纹理时候删除再加在的问题
		{
			return true;
		}
		else
		{
			if( true == m_isLoaded)
			{
				this->UnloadTexture();
			}
			m_isLoadFormFile = true;
			const TextureData& data = Engine::Instance().GetResourceManager()->LoadTexture2D(f,strFileName);
			m_hasMipMap = data.m_hasMipMap;
			m_hTextureHandle = data.m_hTextureHandle;
			m_vSize = data.m_vSize;
			m_eFormat = data.m_eFormat;
			m_Name = data.m_Name;
			m_isLoaded = true;
			m_Warp.m_SWarp = data.m_SWarp;
			m_Warp.m_TWarp = data.m_TWarp;
			m_Filter.m_MagFilter = data.m_MagFilter;
			m_Filter.m_MinFilter = data.m_MinFilter;
		}
		return false;
	}
	//-------------------------------------------------------------------
	bool Texture2D::MakeTexture(Device::PixelFormat pf, byte pixelsize, const vector2d& size, const byte* buffer)
	{
		if( true == m_isLoaded)
		{
			this->UnloadTexture();
		}
		m_isLoaded = true;

		TextureData td;
		td.m_hasMipMap = false;
		td.m_eFormat = pf;
		td.m_Anisotropic = 0;
		td.m_eType = Device::TT_TEXTURE2D;
		td.m_hTextureHandle = -1;
		td.m_MagFilter = Device::TF_NEAREST;
		td.m_MinFilter = Device::TF_NEAREST;
		td.m_SWarp = Device::TW_CLAMP_TO_EDGE;
		td.m_TWarp = Device::TW_CLAMP_TO_EDGE;
		td.m_PixelSize = pixelsize;
		td.m_Name = "Make";
		td.m_vSize = size;

		m_eType = Device::TT_TEXTURE2D;
		m_hasMipMap = false;
		m_Anisotropic = 0;
		m_Warp.m_SWarp = Device::TW_CLAMP_TO_EDGE;
		m_Warp.m_TWarp = Device::TW_CLAMP_TO_EDGE;
		m_Filter.m_MagFilter = Device::TF_NEAREST;
		m_Filter.m_MinFilter = Device::TF_NEAREST;
		m_vSize = size;
		m_eFormat = pf;
		m_Name = "Make";

		return Engine::Instance().GetDevice()->MakeTextureFromStream(m_hTextureHandle, td, buffer);
	}
	//-------------------------------------------------------------------
	bool Texture2D::SubTexture( const byte* buffer )
	{
		ASSERT( m_isLoaded );
		TextureData td;
		td.m_hasMipMap = m_hasMipMap;
		td.m_eFormat = m_eFormat;
		td.m_Anisotropic = m_Anisotropic;
		td.m_eType = m_eType;
		td.m_hTextureHandle = -1;
		td.m_MagFilter = m_Filter.m_MagFilter;
		td.m_MinFilter = m_Filter.m_MinFilter;
		td.m_SWarp = m_Warp.m_SWarp;
		td.m_TWarp = m_Warp.m_TWarp;
		td.m_Name = m_Name;
		td.m_vSize = m_vSize;
		return Engine::Instance().GetDevice()->SubTextureFromStream( m_hTextureHandle, td, buffer );
	}
	////-------------------------------------------------------------------
	//bool Texture2D::CopyFragment( const RenderTarget* fbo )
	//{
	//	ASSERT( m_isLoaded );
	//	engine::Instance().CopyFragment( *this, fbo );
	//	return true;
	//}






	//-------------------------------------------------------------------
	TextureCube::TextureCube()
		:Texture(Device::TT_TEXTURECUBE)
	{
	}
	//-------------------------------------------------------------------
	TextureCube::~TextureCube()
	{
	}
	//-------------------------------------------------------------------
	bool TextureCube::LoadTexture( Device::PixelFormat pf, const string& f, const string& b, const string& u, const string& d, const string& l, const string& r )
	{
		if( true == m_isLoaded)
		{
			this->UnloadTexture();
		}
		m_isLoadFormFile = true;
		const TextureData& data = Engine::Instance().GetResourceManager()->LoadTextureCube(pf, f, b, u, d, l, r);
		m_hasMipMap = data.m_hasMipMap;
		m_hTextureHandle = data.m_hTextureHandle;
		m_vSize = data.m_vSize;
		m_eFormat = data.m_eFormat;
		m_Name = data.m_Name;
		m_isLoaded = true;
		m_Warp.m_SWarp = data.m_SWarp;
		m_Warp.m_TWarp = data.m_TWarp;
		m_Filter.m_MagFilter = data.m_MagFilter;
		m_Filter.m_MinFilter = data.m_MinFilter;
		return m_isLoaded;
	}
}