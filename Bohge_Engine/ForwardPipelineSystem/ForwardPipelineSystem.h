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
#include "Framework/IPipelineSystem.h"


#include <EASTL/map.h>

namespace BohgeEngine
{
	class RenderObjectEntity;
	class RenderTargetEntity;
}
namespace ForwardPipelineSystemPlugin
{
	class ForwardPipelineSystem : public BohgeEngine::IPipelineSystem
	{
	private:
		struct LightRenderComponent
		{
			BohgeEngine::RenderComponent*					m_rpRender;
			BohgeEngine::LightComponent*					m_rpLights;
		};
	private:
		typedef eastl::vector< LightRenderComponent >			LightRenderArray;//最近光源与渲染体数组
		typedef eastl::multimap< float, LightRenderComponent* >	DepthOrderMap;//摄像机到渲染体距离数组
		typedef eastl::map< uint, DepthOrderMap >				MaterialOrderMap;//相同材质数组
		typedef eastl::map< uint, MaterialOrderMap >			QueueOrderMap;//渲染顺序数组
	private:
		QueueOrderMap*						m_pRenderQueueOrderMap;
		LightRenderArray*					m_pLightRenderArray;
		BohgeEngine::RenderTargetEntity*	m_pMainRenderTarget;
		BohgeEngine::RenderTargetEntity*	m_pOriginalColorDepthStencil;
		BohgeEngine::RenderTargetEntity*	m_pPreviousSceneColor[2];
		BohgeEngine::RenderObjectEntity*	m_pRenderObject;
	public:
		ForwardPipelineSystem(void);
		virtual ~ForwardPipelineSystem(void);
	private:
		virtual void _DoUpdate( );
		virtual void _DoRender( );
	public:
		virtual void RegistEvent( BohgeEngine::Framework& framework ); //注册事件
	private:
		void _DoDrawLayer();
		void _DoDrawOverLayer();
	public:
		void OnFrameworkEntry( float time, BohgeEngine::Framework& framework );
		void OnFrameworkExit( float time, BohgeEngine::Framework& framework );
	};
}