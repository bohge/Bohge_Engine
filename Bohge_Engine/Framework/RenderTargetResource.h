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
#include "RenderTargetProperty.h"
#include "ISharedResource.h"
#include "Delegate.hpp"
#include "3DMath.h"

#include <EASTL/vector.h>
#include <EASTL/map.h>

namespace BohgeEngine
{
	struct ViewResolution;
	class TextureEntity;
	class BOHGE_FRAMEWORK RenderTargetResource : public ISharedResource
	{
	private:
		static handle						s_CurrentBind;
		static vector4d						s_CurrentViewPort;
		static Color						s_CurrentColor;
	private:
		typedef eastl::vector< TextureEntity* > AttachmentVector;
	private:
		AttachmentVector					m_TextureVector;
		RenderTargetProperty::AttachmentMap	m_AttachmentMap;
		Color								m_ClearColor;
		vector4d							m_ViewPort;
		vector2d							m_Size;
		RenderTargetProperty::TargetType	m_eTargetType;
		uint								m_GPUResourceHandle;
		FunctionPtr							m_ResizeFunction;
	public:
		RenderTargetResource(void);
		virtual ~RenderTargetResource(void);
	private:
		virtual handle _DoGenerateRenderTarget( RenderTargetProperty::TargetType type, const vector2d& size ) = 0;
		virtual void _DoSetAttachment( handle h, RenderTargetProperty::TargetType type, RenderTargetProperty::RenderTargetAttachment att, TextureEntity* tex ) = 0;
		virtual void _DoPushRenderTarget( handle h, RenderTargetProperty::TargetType tar, RenderTargetProperty::TargetFace face, const RenderTargetProperty::AttachmentMap& attmap ) = 0;
		virtual void _DoDeleteRenderTarget( handle h ) = 0;
		virtual void _DoClearBuffer( RenderTargetProperty::ClearFlag flag ) = 0;
		virtual void _DoSetViewPort( const vector4d& vp ) = 0;
		virtual void _DoSetClearColor( const Color& c ) = 0;
		virtual void _DoResize( handle h, const vector4d& vp, const vector2d& rsl, const RenderTargetProperty::AttachmentMap& attmap ) = 0;
	public:
		virtual void ReleaseResource();
	public:
		void ListenResizeview();//设置rt需要监听resize
		void OnResizeview( const ViewResolution& vr );
	public:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE RenderTargetProperty::TargetType GetTargetType()
		{
			return m_eTargetType;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void ClearBuffer( RenderTargetProperty::ClearFlag flag )
		{
			if ( s_CurrentColor != m_ClearColor )
			{
				s_CurrentColor = m_ClearColor;
				_DoSetClearColor( m_ClearColor );
			}	
			_DoClearBuffer( flag );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void GenerateRenderTarget( RenderTargetProperty::TargetType tt, const vector2d& size )
		{
			m_eTargetType = tt;
			m_Size = size;
			m_GPUResourceHandle = _DoGenerateRenderTarget( tt, m_Size );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetViewPort( const vector4d& vp )
		{
			m_ViewPort = vp;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetAttachment( RenderTargetProperty::RenderTargetAttachment att, TextureEntity* tex )
		{
			//当附件为buffer类型的时候tex为NULL
			m_TextureVector[att] = tex;
			m_AttachmentMap.insert( eastl::make_pair(att, tex) );
			_DoSetAttachment( m_GPUResourceHandle, m_eTargetType, att, tex );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void Push( RenderTargetProperty::TargetFace face )
		{
			if ( s_CurrentBind != m_GPUResourceHandle )
			{
				s_CurrentBind = m_GPUResourceHandle;
				_DoPushRenderTarget( m_GPUResourceHandle, m_eTargetType, face, m_AttachmentMap );
				if ( s_CurrentViewPort != m_ViewPort )
				{
					s_CurrentViewPort = m_ViewPort;
					_DoSetViewPort( m_ViewPort );
				}
			}
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetClearColor( const Color& c )
		{
			m_ClearColor = c;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const TextureEntity* GetAttachment( RenderTargetProperty::RenderTargetAttachment att )
		{
			return m_TextureVector[att];
		}
	};
}

