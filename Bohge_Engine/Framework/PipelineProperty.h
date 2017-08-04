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
#include "Object.hpp"



#include <EASTL/vector.h>
#include <EASTL/list.h>
#include <EASTL/map.h>

namespace BohgeEngine
{
	class PostEffect;
	class LightComponent;
	class RenderComponent;
	class PipelineProperty : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR( PipelineProperty );
	private:
		PipelineProperty(void){}
		~PipelineProperty(void){}
	public:
		typedef eastl::vector< RenderComponent* >	VisableRender;
		typedef eastl::vector< LightComponent* >	VisableLight;

		enum PiplineType
		{
			PLT_FORWARD_SINGLE_LIGHT = 0,//前向型单个有效光源
			PLT_FORWARD_MULTI_LIGHT,//前向型多个有效光源
			PLT_DEFFERRED_LIGHT,//延迟渲染
		};
		enum PassType//为延迟渲染做准备
		{
			PT_LIGHTING_AMBIENT = 0,//环境光照
			PT_LIGHTING_DIRECTION,//平行光
			PT_LIGHTING_POINT,//点光
			PT_LIGHTING_SPOT,//聚光灯
			PT_UNIVERSAL_DEPTH,//线性深度绘制
			PT_UNIVERSAL_NORMAL,//渲染法线到RT
			PT_UNIVERSAL_DECAL,//贴花（光照和纹理混合）
			PT_UNIVERSAL_MULTI_RENDER_TARGET,//多渲染目标渲染，提供延迟渲染使用

			PT_UNIVERSAL_POST_EFFECT,//后处理

			PT_PASS_TYPE_COUNT,
		};
		enum ActivePassType
		{
			APT_LIGHTING_ALL					= 1 << 0L,//打开全部光照
			APT_UNIVERSAL_DEPTH					= 1 << 1L,//打开独立渲染深度
			APT_UNIVERSAL_NORMAL				= 1 << 2L,//打开独立渲染法线
			APT_UNIVERSAL_DECAL					= 1 << 3L,//打开贴花过程
			APT_UNIVERSAL_MULTI_RENDER_TARGET	= 1 << 4L,//打开mrt渲染
			APT_UNIVERSAL_POST_EFFECT			= 1 << 5L,//打开后处理
		};

		enum LightType
		{
			LT_AMBIENT = 0,//这里的数值要和PassType对应
			LT_DIRECTIONAL,
			LT_POINT,
			LT_SPOT,
		};

		typedef eastl::vector< PassType >				ActivePassTypeArray;//有效地pass类型
		typedef eastl::multimap< unsigned int, PostEffect* >	PostEffectMap;
	};
}