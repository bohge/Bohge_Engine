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



/////////////////////
//		灯光			//
/////////////////////

#pragma once
#include "3DMath.h"
#include "ISceneNode.h"
#include "LocatableObject.h"



namespace BohgeEngine
{
	static const vector3f LightDirectionRaw(0,-1,0);
	static const vector3f LightUpRaw(0,0,-1);
	//f = 1/(C + Ld + Qd2)


	class Material;
	class RendBuffer;
	class Texture2D;
	class ShadowCaster;
	class Billboard;
	class LightNode : public LocatableObject, public ISceneNode
	{
	public:
		typedef	Billboard LightHalo;
	public:
		//以下这两个枚举不要修改，应为他对应资源序列化
		enum LightStyle
		{
			LS_FAKE = 0,
			LS_REAL,
			LS_LIGHTMAPED,
		};
		enum LightType
		{
			LT_POINT = 0,
			LT_DIRECTIONAL,
			LT_SPOT,
		};
		enum LightAttribute
		{
			LA_SHADOW				= 1UL << 0,
			LA_INDIRCTLIGHTING		= 1UL << 0,
		};
	private:
		LightStyle		m_eFakeReal;
		LightType		m_eLightType;
		int				m_LightAttrib;
		bool			m_isLightChange;
		bool			m_isActive;
		float			m_LightRange;
		vector3f		m_Color;//颜色
		vector2f		m_LightAngle;//spot光源的内角和外角
		vector2f		m_LightInnerDiffInv;//spot光源的内角和内外角差的逆
		vector4f		m_Attenuation;//衰减，分别代表衰减的常数，1次，2次项, 聚光灯衰减
		vector2f		m_HaloSize;//halo的尺寸
		float			m_MaxScale;//缩放转化
		//位置属性
		vector3f		m_WorldDirection;//光在世界的方向
		vector3f		m_WorldUp;//光在世界的Up
	protected:
		bool			m_isCastShadow;
		ShadowCaster*	m_pShadowCaster;
		LightHalo*		m_pLighthalo;
	public:
		explicit LightNode( LightType lt, LightStyle la );
		virtual ~LightNode( );
	public:
		virtual void BeginUpdate( Engine& engine ){};
		virtual void Update( uint milisecon );
		virtual void EndUpdate( Engine& engine ){};
		virtual bool isInview( const Frustumf& cFru ){return false;};
		virtual const void* Attribute( const Node_Attribute& name ) const;
		virtual void GetHalo( SharedBuffer& input );//渲染后期光晕
		virtual void SetAttribute( const LightAttribute& att );
		virtual void EraseAttribute( const LightAttribute& att );
	protected:
		virtual void _OnLightChangeRecaculater();//灯光变化的时候，位置阿，等等变化的时候被调用
	public:
		virtual bool IsCollisionWith( const aabbox3df& bindbox ) = 0;//是否会对这个bind产生影响
		virtual void CastDepth( Engine& engine ) = 0;//渲染深度图
		virtual void Render( Engine& engine ) = 0;//渲染（假灯光需要）
		virtual void BeginRender( Engine& engine ){};
		virtual void EndRender( Engine& engine ){};
		virtual void SetLocalRotation( const Quaternionf& r );
	protected:
		BOHGE_FORCEINLINE bool isLightChange()
		{
			return isChange() || isFatherChange() || m_isLightChange;
		}
	public:
		BOHGE_FORCEINLINE bool isCastShadow( )
		{
			return m_isCastShadow;
		}
		BOHGE_FORCEINLINE void SetLocalDirection( const vector3f& dir )
		{
			SetLocalRotation( Math::AxisToAxis( LightDirectionRaw, dir ) );
		}
		BOHGE_FORCEINLINE const vector3f& GetWorldDirection() const
		{
			return m_WorldDirection;
		}
		BOHGE_FORCEINLINE const vector3f& GetWorldUp() const
		{
			return m_WorldUp;
		}
		BOHGE_FORCEINLINE void SetActive( bool b )
		{
			m_isActive = b;
		}
		BOHGE_FORCEINLINE bool isActive() const
		{
			return m_isActive;
		}
		BOHGE_FORCEINLINE LightType GetLightType() const
		{
			return m_eLightType;
		}
		BOHGE_FORCEINLINE const vector3f& GetColor() const
		{
			return m_Color;
		}
		BOHGE_FORCEINLINE void SetColor( const vector3f& clr)
		{
			m_Color = clr;
		}
		BOHGE_FORCEINLINE const vector2f& GetHaloSize() const
		{
			return m_HaloSize;
		}
		BOHGE_FORCEINLINE void SetHaloSize( const vector2f& s)
		{
			m_HaloSize = s;
		}
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
			m_isLightChange = true;//告诉自己的继承类，灯光变了
		}
		BOHGE_FORCEINLINE const vector2f& GetLightInnerDiffInv() const
		{
			return m_LightInnerDiffInv;
		}
		BOHGE_FORCEINLINE void SetRange( float range )
		{
			m_LightRange = range;
			m_isLightChange = true;//告诉自己的继承类，灯光变了
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
		BOHGE_FORCEINLINE const LightStyle& GetLightStyle() const
		{
			return m_eFakeReal;
		}
		BOHGE_FORCEINLINE ShadowCaster& GetCaster()
		{
			return *m_pShadowCaster;
		}
		BOHGE_FORCEINLINE bool isAttribute( const LightAttribute& att )
		{
			return (m_LightAttrib && att) != 0;
		}
	};




	/////////////////////
	//		真光			//
	/////////////////////
	class RealLightNode : public LightNode
	{
	public:
		explicit RealLightNode( LightType lt );
		virtual ~RealLightNode();
	public:
		virtual void CastDepth( Engine& engine );
		virtual void Render( Engine& engine );
		virtual void Update( uint milisecon );
	protected:
		virtual void _OnLightChangeRecaculater();//灯光变化的时候，位置阿，等等变化的时候被调用
	private:
		virtual void _NewShadowCaster() = 0;
	public:
		virtual void SetAttribute( const LightAttribute& att );
		virtual void EraseAttribute( const LightAttribute& att );
	};



	/////////////////////////////
	//		预渲染后的光			//
	/////////////////////////////
	class LightMapedNode : public LightNode//当真实光源以light map方式选然后的halo部分
	{
	public:
		explicit LightMapedNode( LightType lt );
		virtual ~LightMapedNode();
	public:
		virtual void CastDepth( Engine& engine );
		virtual void Render( Engine& engine );
		virtual bool IsCollisionWith( const aabbox3df& bindbox )
		{
			return false;
		}
	};



	/////////////////////
	//		假光			//
	/////////////////////
	class VertexBuffer;
	class IndicesBuffer;
	class FakeLightNode : public LightNode
	{
	protected:
		Texture2D*			m_pTexture;
		RendBuffer*			m_pRendBuffer;
		VertexBuffer*		m_pVertexBuffer;
		IndicesBuffer*		m_pIndicesBuffer;
	public:
		FakeLightNode( LightType type );
		virtual ~FakeLightNode();
	public:
		virtual void CastDepth( Engine& engine );//渲染深度图
		virtual void Render( Engine& engine );//渲染（假灯光需要）
		virtual bool IsCollisionWith( const aabbox3df& bindbox )
		{
			return false;
		}
	};
}