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
#include "ISharedSource.h"
#include "RenderTargetProperty.h"
#include "TextureProperty.h"
#include "3DMath.h"


#include <EASTL/map.h>

namespace BohgeEngine
{
	class TextureEntity;
	class RenderTargetSource : public ISharedSource
	{
	private:
		typedef eastl::map< RenderTargetProperty::RenderTargetAttachment, TextureEntity* > AttachmentMap;
	private:
		AttachmentMap							m_TextureBufferMap;
		Color									m_Color;
		vector4d								m_ViewPort;
		vector2d								m_Size;
		RenderTargetProperty::TargetType		m_eTargetType;
		RenderTargetProperty::SwapTarget		m_eSwapTarget;
	public:
		RenderTargetSource(void);
		~RenderTargetSource(void);
	private:
		virtual void _DoLoadResource();//加载资源
		virtual uint _GetResourceHashCode() const;//返回资源的哈希值
		virtual void _DoReleaseUnnecessaryDataAfterLoaded( );
	public:
		virtual ISharedResource* MakeResource();//将资源数据制作为实体资源，制作资源需要在主线程进行

	public:
		void MakeBufferAttachment( RenderTargetProperty::RenderTargetAttachment rta );//设置buffer
		TextureEntity* MakeTextureAttachment( RenderTargetProperty::RenderTargetAttachment rta );//设置纹理
	public:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetSwapTarget( RenderTargetProperty::SwapTarget st )
		{
			m_eSwapTarget = st;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetSize( const vector2d& size )
		{
			m_Size = size;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const vector2d& GetSize()
		{
			return m_Size;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetViewPort( const vector4d& rect )
		{
			m_ViewPort = rect;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetClearColor( const Color& c )
		{
			m_Color = c;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const vector4d& GetViewPort()
		{
			return m_ViewPort;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetTargetType( RenderTargetProperty::TargetType tt )
		{
			m_eTargetType = tt;
		}
	};
}

