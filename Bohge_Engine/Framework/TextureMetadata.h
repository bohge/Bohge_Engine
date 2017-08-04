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
#pragma once
#include "TextureProperty.h"
#include "IMetadata.h"




namespace BohgeEngine
{
	class BOHGE_FRAMEWORK TextureFileMetadata : public IMetadata
	{
		RTTI_DRIVER_TYPE_DECLEAR( TextureFileMetadata, IMetadata );
	private:
		eastl::string							m_Path;
		TextureProperty::TextureUseage			m_eUsage;
		TextureProperty::PixelFormat			m_ePixelFormat;
		TextureProperty::TextureWarp			m_eSWarp;
		TextureProperty::TextureWarp			m_eTWarp;
		TextureProperty::TextureFilter			m_eMagFilter;
		TextureProperty::TextureFilter			m_eMinFilter;
		uint									m_uAnisotropic;
		bool									m_isMipMap;
		TextureProperty::TextrueSourceData*		m_pTextureData;
	public:
		TextureFileMetadata( 
			TextureProperty::TextureUseage mu, TextureProperty::PixelFormat pf,
			uint x, bool mip,
			TextureProperty::TextureWarp s, TextureProperty::TextureWarp t,
			TextureProperty::TextureFilter mag, TextureProperty::TextureFilter min,
			const eastl::string& path );
		virtual ~TextureFileMetadata();
	private:
		virtual void* _ReturnMetadata( );
	public:
		virtual void GetIdentifier( eastl::vector<byte>& bytes ) const;//在str中推入表示符号
		virtual void ProcessMetadata();//处理原始资源
		virtual void ReleaseMetadate();
	};




	//本地数据生成纹理
	class BOHGE_FRAMEWORK TextureBufferMetadata : public IMetadata
	{
		RTTI_DRIVER_TYPE_DECLEAR( TextureBufferMetadata, IMetadata );
	private:
		byte*									m_pData;
		TextureProperty::TextrueSourceData*		m_pTextureData;
		TextureProperty::TextureType			m_eType;
		TextureProperty::TextureUseage			m_Usage;
		TextureProperty::PixelFormat			m_ePixelFormat;
		TextureProperty::TextureWarp			m_eSWarp;
		TextureProperty::TextureWarp			m_eTWarp;
		TextureProperty::TextureFilter			m_eMagFilter;
		TextureProperty::TextureFilter			m_eMinFilter;
		uint									m_uAnisotropic;
		bool									m_isMipMap;
		byte									m_Channel;
		vector2d								m_Size;
	public:
		//TextureBufferMetadata(
		//	const vector2d& size, TextureProperty::TextureType type,
		//	TextureProperty::PixelFormat pf, uint anisotropic,
		//	TextureProperty::TextureWarp s, TextureProperty::TextureWarp t,
		//	TextureProperty::TextureFilter mag, TextureProperty::TextureFilter min,
		//	byte pixelsize, byte* buf );//数据可以为空，这样会生成一张白色纹理
		TextureBufferMetadata(
			const vector2d& size, TextureProperty::TextureType type,
			TextureProperty::TextureUseage mu, TextureProperty::PixelFormat pf,
			uint anisotropic, bool mip,
			TextureProperty::TextureWarp s, TextureProperty::TextureWarp t,
			TextureProperty::TextureFilter mag, TextureProperty::TextureFilter min,
			byte* buf );//数据可以为空，这样会生成一张白色纹理
		virtual ~TextureBufferMetadata();
	private:
		virtual void* _ReturnMetadata( );
	public:
		virtual void GetIdentifier( eastl::vector<byte>& bytes ) const;//在str中推入表示符号
		virtual void ProcessMetadata();//处理原始资源
		virtual void ReleaseMetadate();
	};
}