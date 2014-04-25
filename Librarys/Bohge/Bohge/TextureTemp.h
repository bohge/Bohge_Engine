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
#include "SmartPtr.hpp"
#include "TextureSource.h"
#include "TextureResource.h"

namespace BohgeEngine
{
	class TextureTemp
	{
	private:
		TextureResource*			m_pResource;//GPU资源
		SmartPtr<TextureSource>		m_spSource;//CPU资源
	public:
		TextureTemp(void);
		~TextureTemp(void);
	public:
		bool ResourceCallBack( IResource* res );
		void LoadResource( );
	public:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void PushResourcePath( const std::string& path )
		{
			m_spSource->PushResourcePath( path );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetTextureType( TextureStatus::TextureType tt )
		{
			m_spSource->SetTextureType( tt );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetWarp( TextureStatus::TextureWarp s, TextureStatus::TextureWarp t )
		{
			m_spSource->SetWarp( s, t );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetFilter( TextureStatus::TextureFilter mag, TextureStatus::TextureFilter min )
		{
			m_spSource->SetFilter( mag, min );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetPixelFormat( TextureStatus::PixelFormat pf )
		{
			m_spSource->SetPixelFormat( pf );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetAnisotropic( uint x )
		{
			m_spSource->SetAnisotropic( x );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool isActive()
		{
			return NULL != m_pResource;
		}
	};
}
