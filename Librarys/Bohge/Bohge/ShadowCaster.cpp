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



#include "ShadowCaster.h"
#include "RendBuffer.h"
#include "RenderTarget.h"
#include "ShadowShader.h"
#include "ShaderManager.h"
#include "Device.h"
#include "Light.h"
#include "Camera.h"
#include "SceneManager.h"
#include "Engine.h"


using namespace std;

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	ShadowCaster::ShadowCaster( CasterType ct )
		:m_eCasterType( ct ),
		m_pGaussHelper(NULL),
		m_pRendBuffer( NULL ),
		m_isCasterShadow( false )
	{
		VertexTexture Vertex[4];
		Vertex[0] = VertexTexture(vector4f(-1,1,0), vector2f(0,1));
		Vertex[1] = VertexTexture(vector4f(-1,-1,0), vector2f(0,0));
		Vertex[2] = VertexTexture(vector4f(1,1,0), vector2f(1,1));
		Vertex[3] = VertexTexture(vector4f(1,-1,0), vector2f(1,0));
		ushort Indies[4]={0,1,2,3};
		m_pRendBuffer = NEW RendBuffer( Device::RM_TRIANGLE_STRIP);
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_STATIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pVertexBuffer->FlushVertexBuffer( 4, &Vertex[0] );
		m_pIndicesBuffer->FlushIndiesBuffer( 4, &Indies[0] );
	}
	//-------------------------------------------------------------------------------------------------------
	ShadowCaster::~ShadowCaster(void)
	{
		SAFE_DELETE( m_pRendBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
		SAFE_DELETE( m_pGaussHelper );
		for ( int i = 0 ; i < m_CameraItemArray.size() ; i ++ )
		{
			SAFE_DELETE( m_CameraItemArray[i].m_pCamera );
		}
		for ( int i = 0 ; i < m_TargetPtrArray.size() ; i ++ )
		{
			SAFE_DELETE( m_TargetPtrArray[i] );
		}

	}
	//-------------------------------------------------------------------------------------------------------
	void ShadowCaster::Initialization( const LightNode& light )
	{
		m_pGaussHelper = NEW RenderTarget( vector2d( SC_SHADOWMAPSIZE, SC_SHADOWMAPSIZE ), Device::FBO_TCOLOR );
		for ( int i = 0 ; i < m_eCasterType ; i ++ )
		{
			RenderTarget* rt = NEW RenderTarget( vector2d( SC_SHADOWMAPSIZE, SC_SHADOWMAPSIZE ), Device::FBO_TCOLOR_BDEPTH );
			m_TargetPtrArray.push_back( rt );
			m_TextureArray.push_back( rt->GetColorBuffer() );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	RenderTarget* ShadowCaster::_GetRendTarget( int index )
	{
		return m_TargetPtrArray[index];
	}
	//-------------------------------------------------------------------------------------------------------
	void ShadowCaster::CastDepth( Engine& engine )
	{
		engine.GetDevice()->DisableAlpha();//切记，拷贝深度的时候一定要把alpha混合关掉，不然会将深度数据与清屏数据混合
		for ( int i = 0 ; i < m_eCasterType ; i ++ )
		{
			if ( !m_CameraItemArray[i].m_CastShadowNode.empty() )
			{
				m_CameraItemArray[i].m_isDirty = true;
				engine.AttachLightCamera( m_CameraItemArray[i].m_pCamera );
				engine.GetDevice()->PushRenderTarget( _GetRendTarget(i) );
				engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER );

				for ( NodeMultimap::iterator it = m_CameraItemArray[i].m_CastShadowNode.begin();
					it != m_CameraItemArray[i].m_CastShadowNode.end();
					it++ )
				{
					it->second->Render( engine);
				}

				engine.GetDevice()->PopRenderTarget( );
				engine.DetachLightCamera();
				if ( CT_SHADOW_CUBE != m_eCasterType )//暂时没有cube的模糊
				{//模糊
					float Range = 1.2; //控制模糊度
					engine.GetDevice()->PushRenderTarget(m_pGaussHelper);
					engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER );
					GaussianDepthLow& gaussLow = engine.GetShaderManager()->GetShader<GaussianDepthLow>(ShaderManager::GaussianDepthLow);
					gaussLow.SetParamStep( vector2f( Range / m_pGaussHelper->GetSize().m_x, 0.0f ) );
					engine.GetDevice()->Draw( *m_pRendBuffer, gaussLow, _GetRendTarget(i)->GetColorBuffer() );
					engine.GetDevice()->PopRenderTarget();

					//y方向模糊
					engine.GetDevice()->PushRenderTarget( _GetRendTarget(i) );
					engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER );
					gaussLow.SetParamStep( vector2f( 0.0f, Range / _GetRendTarget(i)->GetSize().m_y ) );
					engine.GetDevice()->Draw( *m_pRendBuffer, gaussLow, m_pGaussHelper->GetColorBuffer() );
					engine.GetDevice()->PopRenderTarget();
				}
			}
			else if ( m_CameraItemArray[i].m_isDirty )
			{
				m_CameraItemArray[i].m_isDirty = false;
				engine.GetDevice()->PushRenderTarget( _GetRendTarget(i) );
				engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER );
				engine.GetDevice()->PopRenderTarget( );
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShadowCaster::Update( const LightNode& light, ICamera& maincamera, bool islightchange )
	{
		m_isCasterShadow = false;
		for ( int i = 0 ; i < m_eCasterType ; i ++ )
		{
			m_CameraItemArray[i].m_CastShadowNode.clear();
			SceneManager::SceneNodeList& nodelist = Engine::Instance().GetSceneManager()->GetSceneList();
			for ( SceneManager::SceneNodeList::iterator it = nodelist.begin();
				it != nodelist.end();
				it ++ )
			{
				if( (*it)->isShow() ) //计算需要绘制的物体
				{
					if( (*it)->isStatus( ISceneNode::RS_NOCULL ) )
					{
						if( (*it)->isStatus( ISceneNode::RS_SHADOW ) )
						{
							float dir = ( (*it)->WorldBindBox().GetCenter() - m_CameraItemArray[i].m_pCamera->GetPosition() ).LengthPow();
							m_CameraItemArray[i].m_CastShadowNode.insert( make_pair( dir, *it ) );
						}
					}
					else
					{
						if( (*it)->isStatus( ISceneNode::RS_SHADOW ) ) //计算是否投影阴影
						{
							if( (*it)->isInview( m_CameraItemArray[i].m_pCamera->GetFrustum() ) )
							{
								float dir = ( (*it)->WorldBindBox().GetCenter() - m_CameraItemArray[i].m_pCamera->GetPosition() ).LengthPow();
								m_CameraItemArray[i].m_CastShadowNode.insert( make_pair( dir, *it )  );
							}
						}
					}
				}
			}
			m_isCasterShadow |= !m_CameraItemArray[i].m_CastShadowNode.empty();
		}
	}




	//方向光的caster
	//-------------------------------------------------------------------------------------------------------
	DirectLightCaster::DirectLightCaster( )
		:ShadowCaster( CT_SHADOW_BASE )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	DirectLightCaster::~DirectLightCaster()
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void DirectLightCaster::Initialization( const LightNode& light )
	{
		m_CameraItemArray.push_back( CameraItem( NEW StaticCamera() ) );
		m_CameraItemArray[0].m_pCamera->CreateOrthographiProjection( SC_SHADOWRANGE, SC_SHADOWRANGE, 0.1, 1500 );
		ShadowCaster::Initialization( light );
		m_TargetPtrArray[0]->SetViewPort( 1,1, SC_SHADOWMAPSIZE-2, SC_SHADOWMAPSIZE-2 );//把黑边去掉
		m_pGaussHelper->SetViewPort( 1,1, SC_SHADOWMAPSIZE-2, SC_SHADOWMAPSIZE-2 );//把黑边去掉
	}
	//-------------------------------------------------------------------------------------------------------
	void DirectLightCaster::Update( const LightNode& light, ICamera& maincamera, bool islightchange )
	{
		//m_LightCamera->CreatePerspectiveProjection( maincamera.GetNear(), maincamera.GetFar() );
		vector3f lookat = maincamera.GetPosition();
		lookat += maincamera.GetForward() * ( SC_SHADOWMOVE );
		vector3f pos = lookat + ( -light.GetWorldDirection() * 500 );

		m_CameraItemArray[0].m_pCamera->LookAt( pos, lookat, pos.Cross( lookat ) );
		m_CameraItemArray[0].m_pCamera->Update();

		ShadowCaster::Update( light, maincamera, islightchange );
	}



	//点光的caster
	//-------------------------------------------------------------------------------------------------------
	PointLightCaster::PointLightCaster( )
		:ShadowCaster( CT_SHADOW_CUBE ),
		m_RenderTargetCube(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	PointLightCaster::~PointLightCaster()
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void PointLightCaster::Initialization( const LightNode& light )
	{
		ASSERT( CT_SHADOW_CUBE == 6 );
		for ( int i = 0 ; i < CT_SHADOW_CUBE ; i ++ )
		{
			m_CameraItemArray.push_back( CameraItem( NEW StaticCamera() ) );
		}
		OnLightChange(light);
		//m_pGaussHelper = NEW RenderTarget( vector2d( SC_SHADOWMAPSIZE, SC_SHADOWMAPSIZE ), Device::FBO_TCOLOR );//暂时没有软阴影
		m_RenderTargetCube = NEW RenderTargetCube( vector2d( SC_SHADOWMAPSIZE, SC_SHADOWMAPSIZE ) );
		m_TargetPtrArray.push_back( m_RenderTargetCube );
		m_TextureArray.push_back( m_RenderTargetCube->GetColorBuffer() );
	}
	//-------------------------------------------------------------------------------------------------------
	void PointLightCaster::OnLightChange( const LightNode& light )
	{
		m_CameraItemArray[0].m_pCamera->LookAt( light.GetWorldPosition(), light.GetWorldPosition()+vector3f(1,0,0), vector3f(0,-1,0) );//右
		m_CameraItemArray[1].m_pCamera->LookAt( light.GetWorldPosition(), light.GetWorldPosition()+vector3f(-1,0,0), vector3f(0,-1,0) );//左
		m_CameraItemArray[2].m_pCamera->LookAt( light.GetWorldPosition(), light.GetWorldPosition()+vector3f(0,1,0), vector3f(0,0,1) );//上
		m_CameraItemArray[3].m_pCamera->LookAt( light.GetWorldPosition(), light.GetWorldPosition()+vector3f(0,-1,0), vector3f(0,0,-1) );//下
		m_CameraItemArray[4].m_pCamera->LookAt( light.GetWorldPosition(), light.GetWorldPosition()+vector3f(0,0,1), vector3f(0,-1,0) );//后
		m_CameraItemArray[5].m_pCamera->LookAt( light.GetWorldPosition(), light.GetWorldPosition()+vector3f(0,0,-1), vector3f(0,-1,0) );//前
		//float far = light.GetRange() < 1000 ? 1000 : light.GetRange();
		for ( int i = 0 ; i < CT_SHADOW_CUBE ; i ++ )
		{
			m_CameraItemArray[i].m_pCamera->CreatePerspectiveProjection( PI/2, 1, 0.1, light.GetRange() * 1.5 );
			m_CameraItemArray[i].m_pCamera->Update();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	RenderTarget* PointLightCaster::_GetRendTarget( int index )
	{
		m_RenderTargetCube->SetCurrentFace( static_cast<RenderTargetCube::Face>(RenderTargetCube::CF_POSITIVE_X + index) );
		return m_RenderTargetCube;
	}




	//spot光的caster
	//-------------------------------------------------------------------------------------------------------
	SpotLightCaster::SpotLightCaster( )
		:ShadowCaster( CT_SHADOW_BASE )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SpotLightCaster::~SpotLightCaster()
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void SpotLightCaster::Initialization( const LightNode& light )
	{
		m_CameraItemArray.push_back( CameraItem( NEW StaticCamera() ) );
		OnLightChange(light);
		ShadowCaster::Initialization( light );
		m_TargetPtrArray[0]->SetViewPort( 1,1, SC_SHADOWMAPSIZE-2, SC_SHADOWMAPSIZE-2 );//把黑边去掉
		m_pGaussHelper->SetViewPort( 1,1, SC_SHADOWMAPSIZE-2, SC_SHADOWMAPSIZE-2 );//把黑边去掉
	}
	//-------------------------------------------------------------------------------------------------------
	void SpotLightCaster::OnLightChange( const LightNode& light )
	{
		//float far = light.GetRange() < 1000 ? 1000 : light.GetRange();
		m_CameraItemArray[0].m_pCamera->CreatePerspectiveProjection( light.GetLightAngle().m_y*1.4, 1, 0.1, light.GetRange() * 1.5 );
		m_CameraItemArray[0].m_pCamera->LookAt( light.GetWorldPosition(), light.GetWorldPosition()+light.GetWorldDirection(), light.GetWorldUp() );//下
		m_CameraItemArray[0].m_pCamera->Update();
	}
}
