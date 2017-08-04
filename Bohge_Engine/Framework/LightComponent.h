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
#include "PipelineProperty.h"
#include "ILightEntity.h"
#include "IComponent.h"
#include "3DMath.h"




namespace BohgeEngine
{
	//f = 1/(C + Ld + Qd2)
	class ILightEntity;
	class BOHGE_FRAMEWORK LightComponent : public IComponent
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( LightComponent, IComponent )
	private:
		const PipelineProperty::LightType		m_eLightType;
		vector4f		m_Attenuation;//衰减，分别代表衰减的常数，1次，2次项, 聚光灯衰减
		vector3f		m_WorldDirection;//光在世界的方向
		vector3f		m_WorldUp;//光在世界的Up
		vector3f		m_Color;//颜色
		vector2f		m_LightAngle;//spot光源的内角和外角
		vector2f		m_LightInnerDiffInv;//spot光源的内角和内外角差的逆
		float			m_MaxScale;//缩放转化
		float			m_LightRange;
		ILightEntity*	m_pLightEntity;
		bool			m_isLightChange;
	public:
		LightComponent( PipelineProperty::LightType lt );
		~LightComponent(void);
	private:
		virtual void _DoOnAttachNode( );
	public:
		virtual void OnRootNodeChange( );
		virtual void Release( );
	public:
		void MessageLoaclRotation( const NodeMessage& message );
		void MessageLoaclPosition( const NodeMessage& message );
		void MessageLoaclScale( const NodeMessage& message );
	public:
		void TryRecaculateLight();
		const vector3f& GetWorldPosition() const;
		void SetLocalDirection( const vector3f& dir );
	public:
		//x是内角度，y是外角度
		BOHGE_FORCEINLINE const vector2f& GetLightAngle() const
		{
			return m_LightAngle;
		}
		BOHGE_FORCEINLINE void SetLightAngle( const vector2f& a )
		{
			m_LightAngle = a;
			m_LightInnerDiffInv.m_x = Math::Cos( a.m_x*0.5 );
			m_LightInnerDiffInv.m_y = 1.0 / ( Math::Cos( a.m_x*0.5 ) - Math::Cos( a.m_y*0.5 ) );
			m_isLightChange = true;
		}
		BOHGE_FORCEINLINE const vector2f& GetLightInnerDiffInv() const
		{
			return m_LightInnerDiffInv;
		}
		BOHGE_FORCEINLINE void SetRange( float range )
		{
			m_LightRange = range;
			m_isLightChange = true;
		}
		BOHGE_FORCEINLINE float GetRange() const
		{
			return m_LightRange * m_MaxScale;
		}
		BOHGE_FORCEINLINE void SetAttenuation( float constant, float liner, float quadratic, float spotexp )
		{
			m_Attenuation.m_x = constant;
			m_Attenuation.m_y = liner;
			m_Attenuation.m_z = quadratic;
			m_Attenuation.m_w = spotexp;
		}
		BOHGE_FORCEINLINE void SetAttenuation( const vector4f& atten )
		{
			m_Attenuation = atten;
		}
		BOHGE_FORCEINLINE const vector4f& GetAttenuation() const
		{
			return m_Attenuation;
		}
		BOHGE_FORCEINLINE const vector3f& GetWorldDirection() const
		{
			return m_WorldDirection;
		}
		BOHGE_FORCEINLINE const vector3f& GetWorldUp() const
		{
			return m_WorldUp;
		}
		BOHGE_FORCEINLINE const vector3f& GetColor() const
		{
			return m_Color;
		}
		BOHGE_FORCEINLINE void SetColor( const vector3f& clr)
		{
			m_Color = clr;
		}
		BOHGE_FORCEINLINE PipelineProperty::LightType GetLightType() const
		{
			return m_eLightType;
		}
		BOHGE_FORCEINLINE IntersectedType::Volume Intersect( const Frustumf& frustum )//判断是否可见
		{
			return m_pLightEntity->Intersect( frustum );
		}
		BOHGE_FORCEINLINE float CaculateEffective( const Aabbox3df& bind )//计算衰减
		{
			return m_pLightEntity->CaculateEffective( bind );
		}
		BOHGE_FORCEINLINE bool isEffectOnRender( const Aabbox3df& bind )//是否照射在可渲染物体上
		{
			return m_pLightEntity->isEffectOnRender( bind );
		}
	};
}