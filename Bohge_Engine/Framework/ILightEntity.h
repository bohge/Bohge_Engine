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
#include "3DMath.h"




namespace BohgeEngine
{
	class LightComponent;
	class ILightEntity : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR( ILightEntity );
	private:
		const LightComponent* const		m_pHost;
	public:
		ILightEntity( const LightComponent*	lc ):m_pHost(lc){}
		virtual ~ILightEntity(void){}
	protected:
		BOHGE_FORCEINLINE const LightComponent* _GetHost() const
		{
			return m_pHost;
		}
	public:
		virtual IntersectedType::Volume Intersect( const Frustumf& frustum ) = 0;//判断是否可见
		virtual void RecaculateLight( ) = 0;
		virtual float CaculateEffective( const Aabbox3df& bind ) = 0;//计算衰减
		virtual bool isEffectOnRender( const Aabbox3df& bind ) = 0;//是否照射在可渲染物体上
	};
}