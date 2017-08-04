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
#include "LightComponent.h"
#include "PointLight.h"



namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( PointLight );
	//---------------------------------------------------------------------------------------------------------
	PointLight::PointLight( const LightComponent* lc )
		:ILightEntity( lc )
	{
	}
	//---------------------------------------------------------------------------------------------------------
	PointLight::~PointLight(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IntersectedType::Volume PointLight::Intersect( const Frustumf& frustum )
	{
		return frustum.IntersectType( m_BindBox );
	}
	//---------------------------------------------------------------------------------------------------------
	void PointLight::RecaculateLight( )
	{
		const float& r = _GetHost()->GetRange();
		m_BindBox = Aabbox3df( _GetHost()->GetWorldPosition()-vector3f(r,r,r), _GetHost()->GetWorldPosition()+vector3f(r,r,r) );
	}
	//---------------------------------------------------------------------------------------------------------
	bool PointLight::isEffectOnRender( const Aabbox3df& bind )
	{
		return m_BindBox.Intersect( bind );
	}
	//---------------------------------------------------------------------------------------------------------
	float PointLight::CaculateEffective( const Aabbox3df& bind )
	{
		float distance = (_GetHost()->GetWorldPosition() - bind.GetCenter() ).Length();
		const vector4f& vAttenuation = _GetHost()->GetAttenuation();
		float fAttenuation = (1.0 - distance ) / ( vAttenuation.m_x + distance * vAttenuation.m_y + distance * distance * vAttenuation.m_z );
		const vector3f& clr = _GetHost()->GetColor();
		return clr.m_x * fAttenuation + clr.m_y * fAttenuation + clr.m_z * fAttenuation;
	}

}