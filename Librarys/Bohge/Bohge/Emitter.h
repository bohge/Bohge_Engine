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
//			粒子系统粒子发射器		   //
/////////////////////////////////////////
#pragma once
#include "ISceneNode.h"
#include "aabbox.hpp"
#include "3DMath.h"
#include "LocatableObject.h"
#include "ScriptBase.h"
#include <string>



namespace BohgeEngine
{
	//粒子绘制的时候会有间断的感觉，这是应为索引绘制到第一个物体时他首先被绘制，而他的alpha区域覆盖的后方没有有颜色的物体
	//然后再绘制
	class Particle_ColorSizeForce;
	class RendBuffer;
	//class Texture2D;
	class Engine;
	class Material;
	class VertexBuffer;
	class Emitter : public ISceneNode, public LocatableObject
	{
		friend class EmitterMain;
	private:
		class Index
		{
		private:
			int			m_nMax;
			int			m_nIndex;
		public:
			Index()
				:m_nMax(0),m_nIndex(0)
			{
			}
			void SetMax(int max)
			{
				m_nMax = max;
			}
			int operator++ (int)
			{
				m_nIndex++;
				if( m_nIndex > m_nMax )
				{
					m_nIndex = 0;
				}
				return m_nIndex;
			}
			BOHGE_FORCEINLINE int Max()
			{
				return m_nMax;
			}
			BOHGE_FORCEINLINE operator int()
			{
				return m_nIndex;
			}
		};
	private:
		bool			m_isInview;//是否在视锥内

		bool			m_isForceStop;//强行立即停止
		bool			m_isEmit;//是否发射
		vector3f		m_PositionNoise;//发射器位置偏移噪点
		bool			m_isBeginChangeColor;//开始的颜色是否是变化的
		bool			m_isEndChangeColor;//结束的颜色是否是变化的
		Color			m_BeginColorA;//颜色起点
		Color			m_BeginColorB;//颜色起点
		Color			m_EndColorA;//颜色终点
		Color			m_EndColorB;//颜色终点
		vector2f		m_ColorPow;//颜色变化的曲率
		vector3f		m_Direction;//喷射方向
		vector3f		m_Acceleration;//加速度
		vector2f		m_Speed;//速度
		float			m_EndSpeed;
		vector3f		m_Angle;//在xyz上的浮动角度
		vector2d		m_LifeSpan;//生命范围
		vector2f		m_AlphaPow;//alpha变化的曲率
		vector2f		m_BeginSize;//初始大小范围相对的
		vector2f		m_EndSize;//结束大小范围
		vector2f		m_SizePow;//尺寸变化的曲率
		vector2d		m_CountSec;//每秒钟喷发的数量
		float			m_fResistance;//阻力，这个值等于末尾速度-最小初始速度/最大生命时间，必须保证这个值*最大生命时间小于等于1
		//Texture2D*		m_Tex;//纹理

		Index			m_Index;//当前的索引

		RendBuffer*		m_pRendBuffer;//绘制buffer
		VertexBuffer*	m_pVertexBuffer;
		Particle_ColorSizeForce*		m_pParticle;//粒子数组

		uint			m_uTimeSpan;//没有发射粒子的时间总长度
		uint			m_uLifedTime;//已经活过的时间
		uint			m_ActiveTime;//有效地，活动地，可是地时间
		int				m_EmitterLife;//发射器的生命

		int				m_ArraySize;

		Material*		m_pMaterial;
	public:
		Emitter(const vector3f& pos,//粒子的发射位置
			const vector3f& posNoise,//粒子的抖动噪点
			const vector3f& dir,//粒子的发生放向
			const vector3f& acce,//粒子的加速度
			const vector2f& speedPmill,//粒子的速度
			float endspeed,//粒子结束速度
			const vector3f& angle,//粒子的角度
			const Color& begin1,//粒子的颜色
			const Color& begin2,
			const Color& end1,
			const Color& end2,
			const vector2f& clrpow,//粒子颜色曲率
			const vector2d& life,//粒子生命时间
			const vector2f& alphaPow,//粒子alpha的曲率
			const vector2f& sizebegin,//粒子尺寸
			const vector2f& sizeend,
			const vector2f& sizepow,//粒子尺寸曲率
			const vector2d& countSec,//每秒喷射
			const int emitterlife,//发射器生命周期
			std::string texName );//纹理
		virtual ~Emitter();
	public:
		virtual void Update( uint milisecon );
	public:
		void Move(const vector3f& dir);
		void CaculateBindBox();//计算绑定盒子,由于计算的复杂性，这里采用抽样检测的方法，所以这个函数禁止在runtime反复调用
	public:
		virtual bool isInview( const Frustumf& cFru );//判断当前帧是否可见是否需要绘制
		virtual void Render( Engine& engine );
		virtual const void* Attribute(const Node_Attribute& name) const;
	public:
		void SetTexture( const std::string& texname );
	public:
		BOHGE_FORCEINLINE void SetPositionNoise( const vector3f& noise )
		{
			m_PositionNoise = noise;
		}
		BOHGE_FORCEINLINE void SetDirect( const vector3f& dir )
		{
			m_Direction = dir;
		}
		BOHGE_FORCEINLINE void SetAcceleration( const vector3f& acce )
		{
			m_Acceleration = acce;
		}
		BOHGE_FORCEINLINE void SetAngle( const vector3f& a )
		{
			m_Angle = a;
		}
		BOHGE_FORCEINLINE void SetBeginSpeedRange( const vector2f& range )
		{
			m_Speed = range;
			m_fResistance = ((m_EndSpeed / m_Speed.m_y) - 1.0f )/static_cast<float>(m_LifeSpan.m_y);
		}
		BOHGE_FORCEINLINE void SetEndSpeed( const float& speed )
		{
			m_EndSpeed = speed;
			m_fResistance = ((m_EndSpeed / m_Speed.m_y) - 1.0f )/static_cast<float>(m_LifeSpan.m_y);
		}
		BOHGE_FORCEINLINE void SetBeginColor1( const Color& clr )
		{
			m_BeginColorA = clr;
		}
		BOHGE_FORCEINLINE void SetBeginColor2( const Color& clr )
		{
			m_BeginColorB = clr;
		}
		BOHGE_FORCEINLINE void SetEndColor1( const Color& clr )
		{
			m_EndColorA = clr;
		}
		BOHGE_FORCEINLINE void SetEndColor2( const Color& clr )
		{
			m_EndColorB = clr;
		}
		BOHGE_FORCEINLINE void SetColorPow( const vector2f& ver )
		{
			m_ColorPow = ver;
		}
		BOHGE_FORCEINLINE void SetAlphaPow( const vector2f& ver )
		{
			m_AlphaPow = ver;
		}
		BOHGE_FORCEINLINE void SetBeginSizeRange( const vector2f& ver )
		{
			m_BeginSize = ver;
		}
		BOHGE_FORCEINLINE void SetEndSizeRange( const vector2f& ver )
		{
			m_EndSize = ver;
		}
		BOHGE_FORCEINLINE void SetSizePow( const vector2f& ver )
		{
			m_SizePow = ver;
		}
	public:
#ifdef DRAWAABBOX
		BOHGE_FORCEINLINE void RenderBind( Engine& dirver )
		{
			if( m_isEmit || m_uLifedTime <= m_EmitterLife + m_LifeSpan.m_y )//如果粒子还活着
			{
				if ( m_isInview )//在视锥内
				{
					m_BindBox.Render( dirver );
				}
			}
		}
#endif
		BOHGE_FORCEINLINE aabbox3df& WorldBindBox()
		{
			return m_BindBox;
		}
		BOHGE_FORCEINLINE void Relife()
		{
			m_isForceStop = false;
			m_isEmit = true;
			m_uLifedTime = 0;
			m_uTimeSpan = 0;
			m_ActiveTime = 0;
		}
		BOHGE_FORCEINLINE void StopEmit() //停止发射
		{
			m_uLifedTime = 0;
			m_isEmit = false;
		}
		BOHGE_FORCEINLINE void ForceStop()//强行停止，停止更新以及绘制
		{
			m_isForceStop = true;
			m_isEmit = false;
		}
		BOHGE_FORCEINLINE bool isLife()
		{
			return m_uLifedTime < m_EmitterLife + m_LifeSpan.m_y || m_EmitterLife < 0;
		}
		BOHGE_FORCEINLINE bool isForceStop()
		{
			return m_isForceStop;
		}
		BOHGE_FORCEINLINE bool isEmit()
		{
			return m_isEmit;
		}
	};
}