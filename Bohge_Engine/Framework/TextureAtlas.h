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
#include "PackageData.h"
#include "3DMath.h"



namespace BohgeEngine
{
	class PackageTree;
	class IAsyncSource;
	class TextureEntity;
	class BOHGE_FRAMEWORK TextureAtlas : public Object
	{
		RTTI_TYPE_DECLEAR( TextureAtlas );
	private:
		vector2d						m_Size;
		PackageTree*					m_pAtlasTree;
		TextureEntity*					m_pTextureEntity;
		TextureProperty::PixelFormat	m_ePixelFormat;
		TextureProperty::TextureFilter	m_eMagFilter;
		TextureProperty::TextureFilter	m_eMinFilter;
	public:
		TextureAtlas(void);
		virtual ~TextureAtlas(void);
	public:
		void CreateTextureEntity( IAsyncSource* source );
		TextureAtlasData InsertTextureBuffer( const vector2d& size, const byte* buffer );
		void EraseBuffer( uint handle );
	public:
		BOHGE_FORCEINLINE void SetFilter( TextureProperty::TextureFilter mag, TextureProperty::TextureFilter min )
		{
			m_eMagFilter = mag;
			m_eMinFilter = min;
		}
		BOHGE_FORCEINLINE void SetPixelFormat( TextureProperty::PixelFormat pf )
		{
			m_ePixelFormat = pf;
		}
		BOHGE_FORCEINLINE void SetSize( const vector2d& size )
		{
			m_Size = size;
		}
		BOHGE_FORCEINLINE const TextureEntity* GetTextureEntity()
		{
			return m_pTextureEntity;
		}
	};
}

