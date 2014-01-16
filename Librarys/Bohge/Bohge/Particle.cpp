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

#include "Particle.h"
#include "Device.h"


namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	Particle_Base::Particle_Base()
	{
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	VertexLayoutList Particle_Base::Layout()
	{
		VertexLayoutList list = VertexColor::Layout();
		static VertexLayout v1(Attributes::ATTRIBUTE_USER_0, vector4f::Size(), Device::DT_FLOAT, vector4f::BitSize(), false );
		static VertexLayout v2(Attributes::ATTRIBUTE_USER_1, vector3f::Size(), Device::DT_FLOAT, vector3f::BitSize(), false );
		list.push_back(&v1);
		list.push_back(&v2);
		return list;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Particle_Base::Create( const vector3f& pos, const Color& clr, const vector3f v, float start, float life, float size, float endsize )
	{
		m_Vertex = vector4f( pos );
		m_Color = clr;
		m_vVelocity = v;
		m_S_L_I_S.m_x = start;
		m_S_L_I_S.m_y = life;
		m_S_L_I_S.m_z = size;
		m_S_L_I_S.m_w = ( endsize - size );
	}



	//--------------------------------------------------------------------------------------------------------------------------------------
	Particle_ColorSizeForce::Particle_ColorSizeForce()
	{
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	VertexLayoutList Particle_ColorSizeForce::Layout()
	{
		VertexLayoutList list = Particle_Base::Layout();
		static VertexLayout v1(Attributes::ATTRIBUTE_USER_2, vector4f::Size(), Device::DT_FLOAT, vector4f::BitSize(), false );
		static VertexLayout v2(Attributes::ATTRIBUTE_USER_3, vector3f::Size(), Device::DT_FLOAT, vector3f::BitSize(), false );
		list.push_back(&v1);
		list.push_back(&v2);
		return list;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Particle_ColorSizeForce::Create(const vector3f& pos, const Color& begclr, const Color& endclr, const vector3f v, float start, float life, float size, float endsize,  float clrPow, float AlphaPow, float SizePow)
	{
		Particle_Base::Create( pos, begclr, v, start, life, size, endsize );
		m_ColorIncrease = ( endclr - begclr );
		m_ColorPow_AlphaPow_SizePow.m_x = clrPow;
		m_ColorPow_AlphaPow_SizePow.m_y = AlphaPow;
		m_ColorPow_AlphaPow_SizePow.m_z = SizePow;
	}

}