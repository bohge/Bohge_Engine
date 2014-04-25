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
#include "3DMath.h"
#include "IResource.h"
#include "TextureSource.h"

namespace BohgeEngine
{
	class TextureResource : public IResource
	{
	protected:
		static handle					s_CurrentBind;
	private:
		handle							m_hResHandle;
		vector2d						m_vSize;//³ß´ç
		uint							m_Anisotropic;
		TextureStatus::WarpType			m_Warp;
		TextureStatus::FilterType		m_Filter;
		TextureStatus::PixelFormat		m_eFormat;
		TextureStatus::TextureType		m_eTextureType;
	public:
		TextureResource(void);
		virtual ~TextureResource(void);
	public:
		static TextureResource* Factory();
	private:
		virtual void _DoActiveTexture( handle h ) = 0;
		virtual void _DoSetWarpType( const TextureStatus::WarpType& wt ) = 0;//ÉèÖÃÎÆÀíwarp
		virtual void _DoSetFilterType( const TextureStatus::FilterType& ft ) = 0;
		virtual void _DoSetAnisotropic( uint ani ) = 0;
		virtual handle _DoGenerateTexture( const TextureStatus::TextrueSourceDataVector& vec ) = 0;
		virtual void _DoDeleteTexture( handle &h ) = 0;
	public:
		virtual void ReleaseResource();
	protected:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE TextureStatus::TextureType _GetTextureType()
		{
			return m_eTextureType;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE TextureStatus::PixelFormat _GetPixelFormat()
		{
			return m_eFormat;
		}
	public:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void ActiveTexture()
		{
			if ( s_CurrentBind != m_hResHandle )
			{
				s_CurrentBind = m_hResHandle;
				_DoActiveTexture( m_hResHandle );
			}
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void GenerateTexture( const TextureStatus::TextrueSourceDataVector& vec, TextureStatus::TextureType tt, TextureStatus::PixelFormat pf )
		{
			m_eTextureType = tt;
			m_eFormat = pf;
			m_vSize = vec[0]->m_Size;
			m_hResHandle = _DoGenerateTexture( vec );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetWarpType( const TextureStatus::WarpType& wt )
		{
			if ( m_Warp.m_SWarp != wt.m_SWarp || m_Warp.m_TWarp != wt.m_TWarp )
			{
				m_Warp = wt;
				_DoSetWarpType( wt );
			}
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetFilterType( const TextureStatus::FilterType& ft )
		{
			if ( m_Filter.m_MagFilter != ft.m_MagFilter || m_Filter.m_MinFilter != ft.m_MinFilter )
			{
				m_Filter = ft;
				_DoSetFilterType( ft );
			}
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetAnisotropic( uint ani )
		{
			if ( m_Anisotropic != ani )
			{
				m_Anisotropic = ani;
				_DoSetAnisotropic( ani );
			}
		}
	};

}
