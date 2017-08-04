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
#include "RenderTargetResource.h"
#include "RenderTargetSource.h"
#include "TextureProperty.h"
#include "IAsyncEntity.h"
#include "3DMath.h"


namespace BohgeEngine
{
	class RenderTargetSource;
	class BOHGE_FRAMEWORK RenderTargetEntity : public IAsyncEntity< RenderTargetResource, RenderTargetSource >
	{
		RTTI_DRIVER_TYPE_DECLEAR( RenderTargetEntity, IAsyncEntity );
	private:
		static RenderTargetEntity*				s_CurrentRT;
	private:
		bool									m_isListenResizeview;
		RenderTargetProperty::TargetType		m_eTargetType;
		RenderTargetProperty::TargetFace		m_eTargetFace;
	public:
		RenderTargetEntity(void);
		~RenderTargetEntity(void);
	private:
		virtual void _OnCreateResource();//准备加载资源的时候调用
		virtual void _OnResourceCreated();//资源加载完毕的回掉
	public:
		//bool OnPushedEvent( SmartPtr<IAsyncSource> ptr );
		//void SetRenderTargetType( RenderTargetProperty::TargetType tt );
		//void SetSize( int x, int y );
		//void SetSize( const vector2d& size );
		//void SetViewPort( int bx, int by, int ex, int ey );
		//void SetViewPort( const vector4d& rect );
		//void SetClearColor( float r, float g, float b, float a );
		//void SetClearColor( const Color& clr );
		//void SetTargetType( RenderTargetProperty::TargetType tt );
		//void SetSwapTarget( RenderTargetProperty::SwapTarget st );
		void Resize( const vector2d& size );
		void MakeBufferAttachment( RenderTargetProperty::RenderTargetAttachment rta );//设置buffer
		TextureEntity* MakeTextureAttachment( RenderTargetProperty::RenderTargetAttachment rta );//设置纹理
		void ListenResizeview();//设置rt需要监听resize
	public:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void ActiveTargetFace( RenderTargetProperty::TargetFace tf )
		{
			m_eTargetFace = RenderTargetProperty::RT_RENDER_TARGET_CUBE == m_eTargetType ? tf : RenderTargetProperty::TF_DEFAULT;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE RenderTargetEntity* PushRenderTarget()
		{
			RenderTargetEntity* res = s_CurrentRT;
			s_CurrentRT = this;
			_GetResourcePtr()->Push( m_eTargetFace );
			return res;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void ClearBuffer( RenderTargetProperty::ClearFlag flag )
		{
			_GetResourcePtr()->ClearBuffer( flag );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const TextureEntity* GetAttachment( RenderTargetProperty::RenderTargetAttachment att )
		{
			return _GetResourcePtr()->GetAttachment( att );
		}
	};
}

