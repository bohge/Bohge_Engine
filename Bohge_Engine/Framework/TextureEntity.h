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
#include "TextureMetadata.h"
#include "TextureResource.h"
#include "TextureSource.h"
#include "SmartPtr.hpp"
#include "IAsyncEntity.h"


namespace BohgeEngine
{
	class TextureResource;
	class BOHGE_FRAMEWORK TextureEntity : public IAsyncEntity< TextureResource, TextureSource >
	{
		RTTI_DRIVER_TYPE_DECLEAR( TextureEntity, IAsyncEntity );
	public:
		TextureEntity(void);
		~TextureEntity(void);
	private:
		virtual void _OnCreateResource();//准备加载资源的时候调用
		virtual void _OnResourceCreated();//资源加载完毕的回掉
	public:
		void Resize( const vector2d& rsl );//RT调用
	public:
		BOHGE_FORCEINLINE operator uint() const
		{
			return GetHashCode();
		}
		////------------------------------------------------------------------------------------------------------
		//BOHGE_FORCEINLINE void SetTextureType( TextureProperty::TextureType tt )
		//{
		//	_GetSourcePtr()->SetTextureType( tt );
		//}
		////------------------------------------------------------------------------------------------------------
		//BOHGE_FORCEINLINE void SetWarp( TextureProperty::TextureWarp s, TextureProperty::TextureWarp t )
		//{
		//	_GetSourcePtr()->SetWarp( s, t );
		//}
		////------------------------------------------------------------------------------------------------------
		//BOHGE_FORCEINLINE void SetFilter( TextureProperty::TextureFilter mag, TextureProperty::TextureFilter min )
		//{
		//	_GetSourcePtr()->SetFilter( mag, min );
		//}
		////------------------------------------------------------------------------------------------------------
		//BOHGE_FORCEINLINE void SetPixelFormat( TextureProperty::PixelFormat pf )
		//{
		//	_GetSourcePtr()->SetPixelFormat( pf );
		//}
		////------------------------------------------------------------------------------------------------------
		//BOHGE_FORCEINLINE void SetAnisotropic( uint x )
		//{
		//	_GetSourcePtr()->SetAnisotropic( x );
		//}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE TextureProperty::PixelFormat GetPixelFormat( ) const
		{
			return _GetSourcePtr()->GetPixelFormat();
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const vector2d& GetSize() const
		{
			return _GetSourcePtr()->GetSize();
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE TextureProperty::TextureType GetTextureType( ) const
		{
			return _GetSourcePtr()->GetTextureType();
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetResourceHandle() const
		{
			return _GetResourcePtr()->GetResourceHandle();
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void ActiveTexture() const
		{
			return _GetResourcePtr()->ActiveTexture();
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE handle GetHandle() const
		{
			return _GetResourcePtr()->GetHandle();
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SubstituteTextureBuffer( const vector2d& begin, const vector2d& size, const byte* const buffer )
		{
			_GetResourcePtr()->SubstituteTextureBuffer( begin, size, buffer );
		}
	};
}
