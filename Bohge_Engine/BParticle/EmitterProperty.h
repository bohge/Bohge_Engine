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


namespace ParticleSystemPlugin
{
	namespace EmitterProperty
	{
		enum EmitterAffector
		{
			EA_PARTICLE_DENSITY = 0,//喷射器每秒喷射个数
			EA_PARTICLE_LIFE,//粒子存活时间
			EA_PARTICLE_SPEED,//粒子速度
			EA_PARTICLE_ACCELERATION,//粒子加速度
			EA_PARTICLE_SIZE,//粒子尺寸
			EA_PARTICLE_SIZE_CURVATURE,//粒子尺寸曲率
			EA_PARTICLE_COLOR,//颜色变化
			EA_PARTICLE_COLOR_CURVATURE,//颜色变化曲率
			EA_PARTICLE_SPIN,//自旋
			EA_PARTICLE_POSITION_NOISE,//位置噪点
			EA_PARTICLE_AXIS_NOISE,//发射轴噪点


			EA_PARTICLE_USER_0,
			EA_PARTICLE_USER_1,
			EA_PARTICLE_USER_2,
			EA_PARTICLE_USER_3,
			EA_PARTICLE_USER_4,
			EA_PARTICLE_USER_5,
			EA_PARTICLE_USER_6,
			EA_PARTICLE_USER_7,

			EA_COUNT,
		};
	}
}