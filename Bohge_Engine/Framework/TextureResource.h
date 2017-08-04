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
#include "ISharedResource.h"
#include "TextureSource.h"

namespace BohgeEngine
{
	class SubTextureBuffer;
	class BOHGE_FRAMEWORK TextureResource : public ISharedResource
	{
	private:
		typedef eastl::list< SubTextureBuffer* >	SubTextureList;//记录需要的刷新动作
	protected:
		static handle						s_CurrentBind;
	private:
		vector2d							m_vSize;//尺寸
		SubTextureList						m_BufferList[2];
		uint								m_SwitchIndex;
		uint								m_Anisotropic;
		TextureProperty::TextureUseage		m_MemoryUseage;
		TextureProperty::WarpType			m_Warp;
		TextureProperty::FilterType			m_Filter;
		TextureProperty::PixelFormat		m_eFormat;
		TextureProperty::TextureType		m_eTextureType;
		uint								m_GPUResourceHandle;
		bool								m_isMipMap;
	public:
		TextureResource(void);
		virtual ~TextureResource(void);
	private:
		virtual void _DoActiveTexture( handle h ) const = 0;
		virtual void _DoSetWarpType( const TextureProperty::WarpType& wt ) = 0;//设置纹理warp
		virtual void _DoSetFilterType( const TextureProperty::FilterType& ft ) = 0;
		virtual void _DoSetAnisotropic( uint ani ) = 0;
		virtual handle _DoGenerateTexture( const TextureProperty::TextrueSourceDataVector& vec ) = 0;
		virtual void _DoDeleteTexture( handle h ) = 0;
		virtual void _DoGenerateMipMap( ) = 0;
		virtual handle _DoGetHandle() const = 0;
		virtual void _DoResize( const vector2d& size ) = 0;
		virtual void _DoSubstituteTextureBuffer( const vector2d& begin, const vector2d& size, const byte* const buffer ) = 0;
		virtual void _DoSetTextureUseage( TextureProperty::TextureUseage tu ) = 0;
	public:
		virtual void ReleaseResource();
	public:
		void OnGraphicUpdata();
		void SubstituteTextureBuffer( const vector2d& begin, const vector2d& size, const byte* const buffer );
	protected:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE TextureProperty::TextureType _GetTextureType() const
		{
			return m_eTextureType;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE TextureProperty::PixelFormat _GetPixelFormat()
		{
			return m_eFormat;
		}
	public:
		BOHGE_FORCEINLINE void Resize( const vector2d& size )
		{
			m_vSize = size;
			_DoResize( size );
		}
		BOHGE_FORCEINLINE handle GetHandle() const
		{
			return _DoGetHandle();
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void ActiveTexture() const
		{
			if ( s_CurrentBind != m_GPUResourceHandle )
			{
				s_CurrentBind = m_GPUResourceHandle;
				_DoActiveTexture( m_GPUResourceHandle );
			}
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void GenerateTexture( const TextureProperty::TextrueSourceDataVector& vec, TextureProperty::TextureType tt, TextureProperty::PixelFormat pf )
		{
			m_eTextureType = tt;
			m_eFormat = pf;
			m_vSize = vec[0]->m_Size;
			m_GPUResourceHandle = _DoGenerateTexture( vec );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetTextureUseage( TextureProperty::TextureUseage tu )
		{
			if ( m_MemoryUseage != tu )
			{
				m_MemoryUseage = tu;
				_DoSetTextureUseage( m_MemoryUseage );
			}
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetWarpType( const TextureProperty::WarpType& wt )
		{
			if ( m_Warp.m_SWarp != wt.m_SWarp || m_Warp.m_TWarp != wt.m_TWarp )
			{
				m_Warp = wt;
				_DoSetWarpType( wt );
			}
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetFilterType( const TextureProperty::FilterType& ft )
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
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetResourceHandle() const
		{
			return m_GPUResourceHandle;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void GenerateMipMap( )
		{
			m_isMipMap = true;
			_DoGenerateMipMap( );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool isMipMap()
		{
			return m_isMipMap;
		}
	};

}






//
////////////////////////////////////////////////////////////////////////////////////////
////
////						The Bohge Engine License (BEL)
////
////	Copyright (c) 2011-2014 Peng Zhao
////
////	Permission is hereby granted, free of charge, to any person obtaining a copy
////	of this software and associated documentation files (the "Software"), to deal
////	in the Software without restriction, including without limitation the rights
////	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
////	copies of the Software, and to permit persons to whom the Software is
////	furnished to do so, subject to the following conditions:
////
////	The above copyright notice and this permission notice shall be included in 
////	all copies or substantial portions of the Software. And the logo of 
////	Bohge Engine shall be displayed full screen for more than 3 seconds 
////	when the software is started. Copyright holders are allowed to develop 
////	game edit based on Bohge Engine, The edit must be released under the MIT 
////	open source license if it is going to be published. In no event shall 
////	copyright holders be prohibited from using any code of Bohge Engine 
////	to develop any other analogous game engines.
////
////	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
////	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
////	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
////	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
////	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
////	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
////
////////////////////////////////////////////////////////////////////////////////////////
//#pragma once
//#include "3DMath.h"
//#include "ISharedResource.h"
//#include "TextureSource.h"
//
//namespace BohgeEngine
//{
//	class IThreadMutex;
//	class SubTextureBuffer;
//	class BOHGE_FRAMEWORK TextureResource : public ISharedResource
//	{
//	private:
//		typedef eastl::list< SubTextureBuffer* >	SubTextureList;//记录需要的刷新动作
//	protected:
//		static handle						s_CurrentBind;
//	private:
//		vector2d							m_vSize;//尺寸
//		SubTextureList						m_BufferList;
//		uint								m_Anisotropic;
//		TextureProperty::TextureUseage		m_MemoryUseage;
//		TextureProperty::WarpType			m_Warp;
//		TextureProperty::FilterType			m_Filter;
//		TextureProperty::PixelFormat		m_eFormat;
//		TextureProperty::TextureType		m_eTextureType;
//		uint								m_GPUResourceHandle;
//		bool								m_isMipMap;
//		IThreadMutex*						m_pFlushMutex;
//	public:
//		TextureResource(void);
//		virtual ~TextureResource(void);
//	private:
//		virtual void _DoActiveTexture( handle h ) const = 0;
//		virtual void _DoSetWarpType( const TextureProperty::WarpType& wt ) = 0;//设置纹理warp
//		virtual void _DoSetFilterType( const TextureProperty::FilterType& ft ) = 0;
//		virtual void _DoSetAnisotropic( uint ani ) = 0;
//		virtual handle _DoGenerateTexture( const TextureProperty::TextrueSourceDataVector& vec ) = 0;
//		virtual void _DoDeleteTexture( handle h ) = 0;
//		virtual void _DoGenerateMipMap( ) = 0;
//		virtual handle _DoGetHandle() const = 0;
//		virtual void _DoResize( const vector2d& size ) = 0;
//		virtual void _DoSubstituteTextureBuffer( const vector2d& begin, const vector2d& size, const byte* const buffer ) = 0;
//		virtual void _DoSetTextureUseage( TextureProperty::TextureUseage tu ) = 0;
//	public:
//		virtual void ReleaseResource();
//	public:
//		void OnGraphicUpdata();
//		void SubstituteTextureBuffer( const vector2d& begin, const vector2d& size, const byte* const buffer );
//	protected:
//		//------------------------------------------------------------------------------------------------------
//		BOHGE_FORCEINLINE TextureProperty::TextureType _GetTextureType() const
//		{
//			return m_eTextureType;
//		}
//		//------------------------------------------------------------------------------------------------------
//		BOHGE_FORCEINLINE TextureProperty::PixelFormat _GetPixelFormat()
//		{
//			return m_eFormat;
//		}
//	public:
//		BOHGE_FORCEINLINE void Resize( const vector2d& size )
//		{
//			m_vSize = size;
//			_DoResize( size );
//		}
//		BOHGE_FORCEINLINE handle GetHandle() const
//		{
//			return _DoGetHandle();
//		}
//		//------------------------------------------------------------------------------------------------------
//		BOHGE_FORCEINLINE void ActiveTexture() const
//		{
//			if ( s_CurrentBind != m_GPUResourceHandle )
//			{
//				s_CurrentBind = m_GPUResourceHandle;
//				_DoActiveTexture( m_GPUResourceHandle );
//			}
//		}
//		//------------------------------------------------------------------------------------------------------
//		BOHGE_FORCEINLINE void GenerateTexture( const TextureProperty::TextrueSourceDataVector& vec, TextureProperty::TextureType tt, TextureProperty::PixelFormat pf )
//		{
//			m_eTextureType = tt;
//			m_eFormat = pf;
//			m_vSize = vec[0]->m_Size;
//			m_GPUResourceHandle = _DoGenerateTexture( vec );
//		}
//		//------------------------------------------------------------------------------------------------------
//		BOHGE_FORCEINLINE void SetTextureUseage( TextureProperty::TextureUseage tu )
//		{
//			if ( m_MemoryUseage != tu )
//			{
//				m_MemoryUseage = tu;
//				_DoSetTextureUseage( m_MemoryUseage );
//			}
//		}
//		//------------------------------------------------------------------------------------------------------
//		BOHGE_FORCEINLINE void SetWarpType( const TextureProperty::WarpType& wt )
//		{
//			if ( m_Warp.m_SWarp != wt.m_SWarp || m_Warp.m_TWarp != wt.m_TWarp )
//			{
//				m_Warp = wt;
//				_DoSetWarpType( wt );
//			}
//		}
//		//------------------------------------------------------------------------------------------------------
//		BOHGE_FORCEINLINE void SetFilterType( const TextureProperty::FilterType& ft )
//		{
//			if ( m_Filter.m_MagFilter != ft.m_MagFilter || m_Filter.m_MinFilter != ft.m_MinFilter )
//			{
//				m_Filter = ft;
//				_DoSetFilterType( ft );
//			}
//		}
//		//------------------------------------------------------------------------------------------------------
//		BOHGE_FORCEINLINE void SetAnisotropic( uint ani )
//		{
//			if ( m_Anisotropic != ani )
//			{
//				m_Anisotropic = ani;
//				_DoSetAnisotropic( ani );
//			}
//		}
//		//------------------------------------------------------------------------------------------------------
//		BOHGE_FORCEINLINE uint GetResourceHandle() const
//		{
//			return m_GPUResourceHandle;
//		}
//		//------------------------------------------------------------------------------------------------------
//		BOHGE_FORCEINLINE void GenerateMipMap( )
//		{
//			m_isMipMap = true;
//			_DoGenerateMipMap( );
//		}
//		//------------------------------------------------------------------------------------------------------
//		BOHGE_FORCEINLINE bool isMipMap()
//		{
//			return m_isMipMap;
//		}
//	};
//
//}
