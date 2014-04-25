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
#include "ISource.h"
#include "TextureStatus.h"

#include <string>



namespace BohgeEngine
{
	class TextureSource : public ISource
	{
	private:
		TextureStatus::TextrueSourceDataVector		m_DataVector;
		TextureStatus::WarpType						m_Warp;
		TextureStatus::FilterType					m_Filter;
		TextureStatus::PixelFormat					m_eFormat;
		TextureStatus::TextureType					m_eTextureType;
		uint										m_Anisotropic;
		bool										m_isLoaded;
	public:
		TextureSource( void );
		virtual ~TextureSource( void );
	private:
		virtual uint _GetResourceHashCode() const;//返回资源的哈希值
		virtual void _DoLoadResource();//加载资源
	public:
		virtual IResource* DoMakeResource();//将资源数据制作为实体资源，制作资源需要在主线程进行
	private:
		byte* _FitFormat( int width, int height, int channel, byte* input );//按照要求格式输出
		TextureStatus::TextrueSourceData* _LoadTga(std::string filename);
		TextureStatus::TextrueSourceData* _LoadPng(std::string filename);
		TextureStatus::TextrueSourceData* _LoadTextureSource( const std::string& path );
		void _ReleaseMemory();//释放内存资源
	public:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetTextureType( TextureStatus::TextureType tt )
		{
			ASSERT(false==m_isLoaded);
			m_eTextureType = tt;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetWarp( TextureStatus::TextureWarp s, TextureStatus::TextureWarp t )
		{
			ASSERT(false==m_isLoaded);
			m_Warp.m_SWarp = s;
			m_Warp.m_TWarp = t;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetFilter( TextureStatus::TextureFilter mag, TextureStatus::TextureFilter min )
		{
			ASSERT(false==m_isLoaded);
			m_Filter.m_MagFilter = mag;
			m_Filter.m_MinFilter = min;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetPixelFormat( TextureStatus::PixelFormat pf )
		{
			ASSERT(false==m_isLoaded);
			m_eFormat = pf;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetAnisotropic( uint x )
		{
			ASSERT(false==m_isLoaded);
			m_Anisotropic = x;
		}
	};
}

