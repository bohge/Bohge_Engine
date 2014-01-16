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



/////////////////////////////
//			纹理		   //
/////////////////////////////

#pragma once

#include "VariableType.h"
#include "Device.h"


#include <string>


//注意，读取的纹理全部是共享纹理，如果你修改其中一个读取的纹理，会导致其他以读取的该纹理产生同样的变化

namespace BohgeEngine
{
	struct OutPutTextureData;
	class Texture
	{
	private:
		struct WarpType
		{
			Device::TextureWarp m_SWarp;
			Device::TextureWarp m_TWarp;
			WarpType()
				:m_SWarp(Device::TW_CLAMP_TO_EDGE),
				m_TWarp(Device::TW_CLAMP_TO_EDGE)
			{}
		};
		struct FilterType
		{
			Device::TextureFilter m_MagFilter;
			Device::TextureFilter m_MinFilter;
			FilterType()
				:m_MagFilter( Device::TF_NEAREST ),
				m_MinFilter( Device::TF_NEAREST )
			{}
		};
		friend class Device;
	protected:
		handle					m_hTextureHandle;

		Device::TextureType	m_eType;
		bool					m_isLoadFormFile;//纹理是被生成的还是读取的
		bool					m_isLoaded;
		bool					m_hasMipMap;
		uint					m_Anisotropic;
		WarpType				m_Warp;
		FilterType				m_Filter;
		vector2d				m_vSize;//尺寸
		Device::PixelFormat	m_eFormat;
		std::string				m_Name;
	public:
		Texture(Device::TextureType type);
		virtual ~Texture();
		void UnloadTexture();
		void SetWarp( Device::TextureWarp s, Device::TextureWarp t );
		void SetFilter( Device::TextureFilter mag, Device::TextureFilter min );
		void SetAnisotropic( uint x );
		void GenerateMipmap();
	public:
		BOHGE_FORCEINLINE const handle& GetTextureHandle() const
		{
			return m_hTextureHandle;
		}
		BOHGE_FORCEINLINE const handle* GetTextureHandlePointer() const
		{
			return &m_hTextureHandle;
		}
		BOHGE_FORCEINLINE bool HasMipmap() const
		{
			return m_hasMipMap;
		}
		BOHGE_FORCEINLINE const std::string& GetName() const
		{
			return m_Name;
		}
		BOHGE_FORCEINLINE const vector2d& GetSize() const
		{
			return m_vSize;
		}
		BOHGE_FORCEINLINE Device::TextureType TextureType() const
		{
			return m_eType;
		}
		BOHGE_FORCEINLINE Device::PixelFormat GetPixelFormat() const
		{
			return m_eFormat;
		}
		BOHGE_FORCEINLINE bool isLoadFormFile()//是否是读取的
		{
			return m_isLoadFormFile;
		}
		BOHGE_FORCEINLINE bool isLoaded()
		{
			return m_isLoaded;
		}
		static void SaveTexture( const OutPutTextureData& saveto );
	};




	class Texture2D : public Texture
	{
	public:
		Texture2D();
		~Texture2D();
		bool LoadTexture( Device::PixelFormat f, const std::string& strFileName );
		bool MakeTexture( Device::PixelFormat pf, byte pixelsize, const vector2d& size, const byte* buffer);
		//bool CopyFragment( const RenderTarget* fbo );//读取当前Fbo中的数据,效率低下
		bool SubTexture( const byte* buffer );//更换纹理
	};



	class TextureCube : public Texture//立方纹理输入的全部纹理的size必须相同
	{
	public:
		TextureCube();
		~TextureCube();
		//前，后，上，下，左，右
		bool LoadTexture( Device::PixelFormat pf, const std::string& f, const std::string& b, const std::string& u, const std::string& d, const std::string& l, const std::string& r );
	};
}