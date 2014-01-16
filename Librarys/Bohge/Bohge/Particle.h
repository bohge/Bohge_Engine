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



/////////////////////////////////////////
//			粒子系统的粒子顶点		   //
/////////////////////////////////////////

#pragma once
#include "Vertex.h"

namespace BohgeEngine
{
	class Particle_Base : public VertexColor
	{
	private:
		vector4f		m_S_L_I_S;//粒子的开始的时间,粒子的生命周期,粒子的初始尺寸,粒子的缩放比例
		vector3f		m_vVelocity; //速度方向
	public:
		Particle_Base();
		void Create(const vector3f& pos, const Color& clr, const vector3f v, float start, float life, float size, float endsize );
	public:
		static VertexLayoutList Layout();
	};

	//--------------------------------------------------------------------------------------------------------------------------------------
	class Particle_ColorSizeForce : public Particle_Base
	{
	private:
		vector4f		m_ColorIncrease;
		vector3f		m_ColorPow_AlphaPow_SizePow;//颜色变化的曲率，透明度变化的曲率，尺寸变化的曲率
	public:
		Particle_ColorSizeForce();
		void Create(const vector3f& pos, const Color& begclr, const Color& endclr, const vector3f v, float start, float life, float size, float endsize,  float clrPow, float AlphaPow, float SizePow);
	public:
		static VertexLayoutList Layout();
	};
}