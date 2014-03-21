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

#include "Emitter.h"
#include "Vertex.h"
#include "Particle.h"
#include "ShaderManager.h"
#include "Device.h"
#include "Material.h"
#include "Log.h"
#include "RendBuffer.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Engine.h"


using namespace std;

namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	Emitter::Emitter(const vector3f& pos,
		const vector3f& posNoise,
		const vector3f& dir,
		const vector3f& acce,
		const vector2f& speedPmill,
		float endspeed,
		const vector3f& angle,
		const Color& begin1,
		const Color& begin2,
		const Color& end1,
		const Color& end2,
		const vector2f& clrpow,
		const vector2d& life,
		const vector2f& alphaPow,
		const vector2f& sizebegin,
		const vector2f& sizeend,
		const vector2f& sizepow,
		const vector2d& countSec,
		const int emitterlife,
		std::string texName)
		:ISceneNode( ISceneNode::RS_TRANSPARENT ),
		LocatableObject( this ),
		m_PositionNoise(posNoise),
		m_Speed(speedPmill),
		m_Direction(dir),
		m_Acceleration(acce),
		m_Angle(angle),
		m_BeginColorA(begin1),
		m_BeginColorB(begin2),
		m_EndColorA(end1),
		m_EndColorB(end2),
		m_LifeSpan(life),
		m_BeginSize(sizebegin),
		m_EndSize(sizeend),
		m_CountSec(countSec),
		m_uTimeSpan(0),
		m_uLifedTime(0),
		m_ActiveTime(0),
		m_EmitterLife(emitterlife),
		m_isEmit(true),
		m_isForceStop(false),
		m_isInview(false),
		m_ColorPow(clrpow),
		m_AlphaPow(alphaPow),
		m_SizePow(sizepow),
		m_EndSpeed(endspeed),
		m_fResistance(((endspeed / speedPmill.m_y) - 1.0f )/static_cast<float>(life.m_y))
	{
		SetLocalPosition( pos );
		m_Direction.NormalizeSelf();
		float maxlife = Math::GetMax(life.m_x, life.m_y);
		float maxcount = Math::GetMax(countSec.m_x, countSec.m_y);
		//int maxParticle = static_cast<int>( ( ceil(TO_SEC( maxlife )) ) * maxcount );
		int maxParticle = 0;
		if ( -1 != emitterlife )
		{
			maxParticle = static_cast<int>(  ( TO_SEC( emitterlife ) ) * ( TO_SEC( maxlife ) ) * maxcount );
		}
		else
		{
			maxParticle = static_cast<int>( ( TO_SEC( maxlife ) ) * maxcount );
		}
		m_ArraySize = maxParticle;
		m_Index.SetMax(maxParticle);
		m_pParticle = NEW Particle_ColorSizeForce[maxParticle+1];//此处分配的时候一定要多分配一个，防止glEnableVertexAttribArray越界访问，造成堆栈损坏
		m_pRendBuffer = NEW RendBuffer( Device::RM_POINTS );
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pVertexBuffer->FlushVertexBuffer( m_ArraySize, &m_pParticle[0] );

		//ushort* indies = NEW ushort[maxParticle];
		//for ( int i = 0;i < maxParticle ; i ++ )
		//{
		//	indies[i] = i;
		//}
		//m_pRendBuffer->MakeIndiesBuffer(indies, maxParticle, Device::MU_STATIC);
		//SAFE_DELETE_ARRAY(indies);
		//m_Tex = NEW Texture2D();
		//m_Tex->LoadTexture( Device::PF_A8, texName);
		m_isBeginChangeColor = m_BeginColorA == m_BeginColorB ? false : true;
		m_isEndChangeColor = m_EndColorA == m_EndColorB ? false : true;

		//材质
		m_pMaterial = NEW Material;
		m_pMaterial->SetNode( this );
		m_pMaterial->SetShader( Pipeline::PT_LIGHTING, ShaderManager::ParticleShader );
		m_pMaterial->LoadTexture( Material::ATT_TEX_DIFFUSE, Device::PF_R8G8B8A8, texName );
		m_pMaterial->GetDrawState( Pipeline::PT_LIGHTING ).m_isDepthMask = false;
		m_pMaterial->GetAlpahState( Pipeline::PT_LIGHTING ).m_isAlphaEnable = true;

		//计算绑定盒子
		//由于喷射角度关系，绑定盒子很难精确计算，将采用随机测试的方法在编辑器中输出

	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	Emitter::~Emitter()
	{
		SAFE_DELETE_ARRAY(m_pParticle);
		SAFE_DELETE(m_pRendBuffer);
		SAFE_DELETE(m_pVertexBuffer);
		//SAFE_DELETE(m_Tex);
		SAFE_DELETE( m_pMaterial );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Emitter::SetTexture( const std::string& texname )
	{
		m_pMaterial->LoadTexture( Material::ATT_TEX_OPACITY, texname );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Emitter::Update(uint millisecond)
	{
		LocatableObject::Update( millisecond );
		m_isInview = false;
		if( !m_isForceStop )
		{
			if( isEmit() || m_uLifedTime <= m_EmitterLife + m_LifeSpan.m_y )//如果粒子还活着
			{
				aabbox3df worldbox = m_BindBox; //初始的绑定和不能变
				worldbox.SetCenter( GetWorldPosition() );
				if ( Engine::Instance().GetCamera()->GetFrustum().Intersect( worldbox ) )//判断是否可视
				{
					m_isInview = true;
				}
				//更新粒子
				m_uLifedTime += millisecond;
				if( m_isInview )
				{
					m_ActiveTime += millisecond;
					if( m_isEmit )
					{
						m_isEmit = m_uLifedTime < m_EmitterLife || m_EmitterLife < 0;//计算是否继续发射
						vector2d view = Engine::Instance().GetDevice()->GetResolutionSize();
						float particlesize = ( view.m_x + view.m_y ) * 0.5;
						float worldscale = Math::GetMax( GetWorldScale().m_x, Math::GetMax( GetWorldScale().m_y, GetWorldScale().m_z ) );
						m_uTimeSpan += millisecond;
						int randomNum = Math::Random(m_CountSec.m_x, m_CountSec.m_y);
						float fnum = randomNum * TO_SEC( m_uTimeSpan );
						int relaseNum =  fnum;
						if( 0 != relaseNum)
						{
							m_uTimeSpan = ( fnum - relaseNum ) / randomNum * m_uTimeSpan; //计算一下小数位置的粒子个数是对应多长时间差内的粒子个数，这个是降低误差用的
							relaseNum = Math::GetMin( relaseNum , m_Index.Max() );
							int beginIndex = m_Index;
							for (int i = 0 ; i < relaseNum ; i++)
							{
								Quaternionf rotation;
								rotation.YawPitchRoll(Math::fRandom(-m_Angle.m_y,m_Angle.m_y), Math::fRandom(-m_Angle.m_x,m_Angle.m_x) , Math::fRandom(-m_Angle.m_z,m_Angle.m_z));
								//Matrix33f rotation;
								//rotation.YawPitchRoll(fRandom(-m_Angle.m_y,m_Angle.m_y), fRandom(-m_Angle.m_x,m_Angle.m_x) , fRandom(-m_Angle.m_z,m_Angle.m_z));
								vector3f Velocity = m_Direction * rotation * GetWorldRotation();
								Velocity.NormalizeSelf();
								Velocity *= ( Math::fRandom( m_Speed.m_x , m_Speed.m_y ) );
								Color Beginclr, Endclr, clrInc;
								if( m_isBeginChangeColor )
								{
									Beginclr = Color(Math::fRandom(m_BeginColorA.m_r, m_BeginColorB.m_r),Math::fRandom(m_BeginColorA.m_g, m_BeginColorB.m_g),Math::fRandom(m_BeginColorA.m_b, m_BeginColorB.m_b),Math::fRandom(m_BeginColorA.m_a, m_BeginColorB.m_a));
								}
								else
								{
									Beginclr = m_BeginColorA;
								}
								if( m_isEndChangeColor )
								{
									Endclr = Color(Math::fRandom(m_EndColorA.m_r, m_EndColorB.m_r),Math::fRandom(m_EndColorA.m_g, m_EndColorB.m_g),Math::fRandom(m_EndColorA.m_b, m_EndColorB.m_b),Math::fRandom(m_EndColorA.m_a, m_EndColorB.m_a));
								}
								else
								{
									Endclr = m_EndColorA;
								}
								if( m_PositionNoise == vector3f::Zero() )
								{
									m_pParticle[m_Index].Create(
										GetWorldPosition(),
										Beginclr,
										Endclr, 
										Velocity * worldscale, 
										m_ActiveTime, 
										Math::Random(m_LifeSpan.m_x, m_LifeSpan.m_y), 
										Math::fRandom(m_BeginSize.m_x, m_BeginSize.m_y) * particlesize * worldscale,
										Math::fRandom(m_EndSize.m_x, m_EndSize.m_y) * particlesize * worldscale,
										Math::fRandom(m_ColorPow.m_x, m_ColorPow.m_y),
										Math::fRandom(m_AlphaPow.m_x, m_AlphaPow.m_y),
										Math::fRandom(m_SizePow.m_x, m_SizePow.m_y)
										);
									m_Index++;

								}
								else
								{
									m_pParticle[m_Index].Create(
										GetWorldPosition() + vector3f( Math::fRandom( -m_PositionNoise.m_x, m_PositionNoise.m_x ), Math::fRandom( -m_PositionNoise.m_y, m_PositionNoise.m_y ), Math::fRandom( -m_PositionNoise.m_z, m_PositionNoise.m_z ) ) * worldscale,
										Beginclr,
										Endclr, 
										Velocity * worldscale, 
										m_ActiveTime, 
										Math::Random(m_LifeSpan.m_x, m_LifeSpan.m_y), 
										Math::fRandom(m_BeginSize.m_x, m_BeginSize.m_y) * particlesize * worldscale,
										Math::fRandom(m_EndSize.m_x, m_EndSize.m_y) * particlesize * worldscale,
										Math::fRandom(m_ColorPow.m_x, m_ColorPow.m_y),
										Math::fRandom(m_AlphaPow.m_x, m_AlphaPow.m_y),
										Math::fRandom(m_SizePow.m_x, m_SizePow.m_y)
										);
									m_Index++;
								}
							}
							if( beginIndex + relaseNum > m_ArraySize  ) //头尾事件，从新flush
							{
								int toend = m_ArraySize - beginIndex;
								m_pVertexBuffer->FlushVertexBuffer( beginIndex, toend, &m_pParticle[beginIndex] );
								m_pVertexBuffer->FlushVertexBuffer( 0, relaseNum - toend, &m_pParticle[0] );
							}
							else
							{
								m_pVertexBuffer->FlushVertexBuffer( beginIndex, relaseNum, &m_pParticle[beginIndex] );
							}
						}
					}
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Emitter::Move(const vector3f& dir)
	{
		SetLocalPosition( GetLocalPosition() + dir );
		m_BindBox.SetCenter( GetLocalPosition() );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	bool Emitter::isInview( const Frustumf& cFru )
	{
		//绘制需要再绘制一个粒子最大生命周期时间，保证全部粒子自然死掉
		if( m_isEmit || m_uLifedTime <= m_EmitterLife + m_LifeSpan.m_y )//如果粒子还活着
		{
			if ( m_isInview )//在视锥内
			{
				return true;
			}
		}
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Emitter::Render( Engine& engine )
	{
		engine.GetDevice()->Draw( *m_pRendBuffer, *m_pMaterial );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	const void* Emitter::Attribute( const Node_Attribute& name ) const
	{
		switch ( name )
		{
		case IAttributeNode::ATT_USER_0: return m_Acceleration.GetVectorArray(); break;
		case IAttributeNode::ATT_USER_1: return &m_fResistance; break;
		case IAttributeNode::ATT_USER_2: return &m_ActiveTime; break;
		default: return LocatableAttribute( name );
		}
		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Emitter::CaculateBindBox()
	{
		m_BindBox.Reset();
		for ( int i = 0; i < 1000; i ++ )
		{
			Quaternionf rotation;
			rotation.YawPitchRoll(Math::fRandom(-m_Angle.m_y,m_Angle.m_y), Math::fRandom(-m_Angle.m_x,m_Angle.m_x) , Math::fRandom(-m_Angle.m_z,m_Angle.m_z));
			//rotation.YawPitchRoll(fRandom(-m_Angle.m_y,m_Angle.m_y), fRandom(-m_Angle.m_x,m_Angle.m_x) , fRandom(-m_Angle.m_z,m_Angle.m_z));
			vector3f direction = m_Direction * rotation;
			direction.NormalizeSelf();
			vector3f velocity = direction * m_Speed.m_y;
			{//就是那终结点
				vector3f speed = velocity * ( 1.0 + 0.5 * m_LifeSpan.m_y * m_fResistance ) + m_Acceleration * ( 0.5 * m_LifeSpan.m_y );
				vector3f endpos = GetLocalPosition() + speed * m_LifeSpan.m_y; 
				m_BindBox.Contains( endpos );
			}
			{//计算该方向上的极值点
				//计算加速度速度在速度方向上的投影
				if ( 0 != m_Acceleration.Length() )
				{
					vector3f acceleration = m_Acceleration;
					acceleration.NormalizeSelf();
					float cosA = acceleration.Dot( direction );
					if ( cosA < 0 )
					{
						float projV = cosA * velocity.Dot(m_Acceleration) / m_Acceleration.Length();
						float ltime = Math::Abs( projV / m_Acceleration.Length() );
						vector3f speed = velocity * ( 1.0 + 0.5 * ltime * m_fResistance ) + m_Acceleration * ( 0.5 * ltime );
						vector3f endpos = GetLocalPosition() + speed * ltime; 
						m_BindBox.Contains( endpos );
					}
				}
			}

		}
		m_BindBox.Contains( GetLocalPosition() );
		m_BindBox.SetMin( m_BindBox.GetMin() - m_PositionNoise );
		m_BindBox.SetMax( m_BindBox.GetMax() + m_PositionNoise );
	}

}