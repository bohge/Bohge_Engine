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



/////////////////////////////////
//			渲染目标		   //
/////////////////////////////////
#pragma once
#include "VariableType.h"
#include "Device.h"


namespace BohgeEngine
{
	class Texture2D;
	class RenderTarget
	{
	private:
		friend class Device;
	protected:
		bool					m_isGet;//是否是读取的当前的绑定rt
		Device::TargetType		m_eType;
		bool					m_hasTexColor;//是否有色彩纹理
		bool					m_hasTexDepthStencil;//是否有DepthStencil纹理
		handle					m_hRenderTarget;//渲染目标句柄
		handle					m_hDepthBuffer;//深度buffer句柄
		Texture*				m_pColorTexture;//纹理句柄
		Texture*				m_pDepthStencilTexture;//纹理句柄
		vector4d				m_vViewPort;//视口尺寸
		vector2d				m_vSize;//纹理大小
	private:
		//void Clear();//engine 调用，清楚当前记录的FBO
		RenderTarget( const RenderTarget& ){}
		RenderTarget& operator = ( const RenderTarget& rt ){}
	public:
		RenderTarget(); // for ios
		RenderTarget(const vector2d& size, Device::TargetType type);
		virtual ~RenderTarget();
		void Resize( const vector2d& size );
		//void PushRenderTarget( );
		//void PopRenderTarget( );
		//void Clear(engine& engine);
		BOHGE_FORCEINLINE Texture* GetColorBuffer() const
		{
			ASSERT(m_hasTexColor);
			return m_pColorTexture;
		}
		BOHGE_FORCEINLINE Texture* GetDepthBuffer() const
		{
			ASSERT(m_hasTexDepthStencil);
			return m_pDepthStencilTexture;
		}
		BOHGE_FORCEINLINE void SetViewPort( const vector4d& vp )
		{
			m_vViewPort = vp;
		}
		BOHGE_FORCEINLINE void SetViewPort( const int& x, const int& y, const int& w, const int& h )
		{
			m_vViewPort = vector4d( x, y , w, h );
		}
		BOHGE_FORCEINLINE const vector4d& GetViewPort() const
		{
			return m_vViewPort;
		}
		BOHGE_FORCEINLINE const vector2d& GetSize() const
		{
			return m_vSize;
		}
		BOHGE_FORCEINLINE const Device::TargetType& GetType() const
		{
			return m_eType;
		}
#ifdef IOS
		BOHGE_FORCEINLINE void SetRendTargetHandle( handle h, const vector2d& size ) //只为苹果准备的
		{
			m_isGet = true;
			m_hRenderTarget = h;
			m_vViewPort = vector4d(0,0,size.m_x,size.m_y);
		}
#endif
	};


	//立方体渲染对象
	class RenderTargetCube : public RenderTarget
	{
	public:
		enum Face
		{
			CF_POSITIVE_X = 0, //x 正方向
			CF_NEGATIVE_X,
			CF_POSITIVE_Y,
			CF_NEGATIVE_Y,
			CF_POSITIVE_Z,
			CF_NEGATIVE_Z,
		};
	private:
		Face		m_CurrentFace;//当前渲染的面
	public:
		RenderTargetCube( const vector2d& size );
		~RenderTargetCube();
	public:
		BOHGE_FORCEINLINE void SetCurrentFace( const Face& f )
		{
			m_CurrentFace = f;
		}
		BOHGE_FORCEINLINE const Face& GetCurrentFace( )
		{
			return m_CurrentFace;
		}
	};

}