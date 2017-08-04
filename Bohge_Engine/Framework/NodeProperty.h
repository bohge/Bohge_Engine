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
#include "Delegate.hpp"

namespace BohgeEngine
{
	class Object;
	class NodeMessage;
	typedef Unidelegate< const Object* > AttributeDelegate;//数据源提供者事件
	typedef AttributeDelegate::FunctionType ProviderFunction;

	typedef Delegate1< void, const NodeMessage& > MessageDelegate;//事件响应函数
	typedef MessageDelegate::FunctionType MessageFunction;


	namespace NodeProperty
	{
		enum SetAttribute
		{
			SA_POSITION = 0,
			SA_ROTATION,
			SA_SCALE,
			SA_VELOCITY,

			SA_USER_0,
			SA_USER_1,
			SA_USER_2,
			SA_USER_3,
			SA_USER_4,

			SA_COUNT,
		};
		enum GetAttribute
		{
			GA_WORLD_TRANSFORM = 0, //缩放，旋转，平移的一个矩阵
			GA_WORLD_TRANSFORM_INVERS, //缩放，旋转，平移的一个矩阵的逆矩阵
			GA_WORLD_POSITION,
			GA_WORLD_SCALE,
			GA_WORLD_ROTATION,
			GA_NORMAL_TRANSFORM,//法线的矩阵，只有旋转

			GA_WORLD_VELOCITY,//速度
			GA_WORLD_FORWARLD,//朝向
			GA_WORLD_UP,//方向

			GA_ANIMATION_REAL,
			GA_ANIMATION_DUAL,

			GA_USER_0,
			GA_USER_1,
			GA_USER_2,
			GA_USER_3,
			GA_USER_4,
			GA_USER_5,
			GA_USER_6,
			GA_USER_7,

			GA_COUNT,
		};
	}
}