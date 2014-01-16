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
//		灯光	   //
/////////////////////

#include "Environment.h"
#include "3DMath.h"
#include "Camera.h"
#include "Engine.h"
#include "Device.h"
#include "VolumeLightPS.h"
#include "Pipeline.h"
#include "ShaderManage.h"
#include "Texture.h"
#include "GameSetting.h"
#include "Light.h"
#include "RealLights.h"
#include "Billboard.h"
#include "RendBuffer.h"
#include "EnvironmentShaders.h"
#include "Texture.h"
#include "FilePath.h"


namespace BohgeEngine
{
	//高效果
	//#define SHADOWMAPSIZE 1024//深度图的尺寸
	//#define SHADOWRANGE 400//深度的有效范围
	//#define SHADOWMOVE 180

	//低效果
#define SHADOWMAPSIZE 512//深度图的尺寸
#define SHADOWRANGE 200//深度的有效范围
#define SHADOWMOVE 90

	//-------------------------------------------------------------------
	Environment::Environment( const vector3f& pos, const vector3f& dir, const vector3f& Ambient, const vector3f& color, const vector3f& vlcolor, const vector3f& fc, const vector3f& fp )
		:m_AmbientLight(Ambient),
		m_CurrentLightIndex(0),
		m_VolumeLightColor(vlcolor),
		m_isEnableVL(false),
		m_VolumeLight(NULL),
		m_MainLight(NULL),
		m_isCastShadow( false ),
		m_FogColor( fc ),
		m_FogOgiParam( fp )
	{
		m_VolumeLight = NEW VolumeLightPS( GameSetting::Instance().isVolumeLightHeigh() );
		Engine::Instance().GetPipeline()->AddPostEffcet( m_VolumeLight );//加入阳光的特效
		m_isEnableVL = GameSetting::Instance().isVolumeLight();
		m_pRendBuffer = NEW RendBuffer( LightNode::LightHalo::GetRenderMode() );
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_DYNAMIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pSharedBuffer = NEW SharedBuffer(
			LightNode::LightHalo::GetElementCount(),
			LightNode::LightHalo::GetCountOfVertex(),
			LightNode::LightHalo::GetCountOfIndes(), 
			MAX_LIGHTHALO_SIZE );
		m_pVertexBuffer->FlushVertexBuffer( m_pSharedBuffer->CountOfAllVertex(), 
			static_cast< LightNode::LightHalo::VertexType* >( m_pSharedBuffer->VertexPtr( ) ) );
		m_pIndicesBuffer->FlushIndiesBuffer( m_pSharedBuffer->CountOfAllIndies(), m_pSharedBuffer->IndiesPtr() );
		m_pTexture = NEW Texture2D();
		m_pTexture->LoadTexture( Device::PF_A8, FILEPATH.TextureFolder() + "lighthalo.png" );
		m_LightsArray.reserve( 100 );
	}
	//-------------------------------------------------------------------
	Environment::~Environment()
	{
		SAFE_DELETE( m_VolumeLight );
		SAFE_DELETE( m_pTexture );
		SAFE_DELETE( m_pRendBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
		SAFE_DELETE( m_pSharedBuffer );
		SAFE_DELETE( m_MainLight );
	}
	//-------------------------------------------------------------------
	void Environment::Update( ICamera& maincamera, uint milisecond )
	{//根据主摄像机更新本摄像机
		int ActiveBuffer = 0;
		m_pSharedBuffer->CleanUp();
		m_isCastShadow = false;
		for ( LightsArray::iterator it = m_LightsArray.begin();
			it != m_LightsArray.end();
			it ++ )
		{
			ActiveBuffer++;
			m_isCastShadow |= (*it)->isCastShadow();
			(*it)->GetHalo( *m_pSharedBuffer );
		}
		m_pVertexBuffer->FlushVertexBuffer( 0, ActiveBuffer * LightNode::LightHalo::GetCountOfVertex(),
			static_cast< LightNode::LightHalo::VertexType* >(m_pSharedBuffer->VertexPtr()) );
		m_pIndicesBuffer->FlushIndiesBuffer( 0, ActiveBuffer * LightNode::LightHalo::GetCountOfIndes(), m_pSharedBuffer->IndiesPtr() );
		m_pRendBuffer->SetDrawCount( ActiveBuffer * LightNode::LightHalo::GetCountOfIndes() );

		//计算是否需要绘制阳光特效
		if ( m_isEnableVL && m_MainLight )
		{
			const Matrix44f& mvp = Engine::Instance().GetCamera()->GetViewProj();
			vector4f lp( Engine::Instance().GetCamera()->GetPosition() - m_MainLight->GetWorldDirection() * Engine::Instance().GetCamera()->GetMiddle()  );
			lp *= mvp ;//与摄像机的位置没有关系，所以始终只乘投影矩阵
			lp /= lp.m_w;
			if( lp.m_z < 1.0f )
			{
				float dis = lp.m_x*lp.m_x+lp.m_y*lp.m_y;
				//float visDis = 2.0;
				//visDis = visDis * visDis + visDis * visDis;
				//if ( dis < visDis )
				if( dis < 8.0 ) //8.0是按照上面计算出来的，这个的vl的可见是在两个屏幕空间内
				{
					m_VolumeLight->SetActive( true );
				}
				else
				{
					m_VolumeLight->SetActive( false );
				}
			}
			else
			{
				m_VolumeLight->SetActive( false );
			}
		}
		else
		{
			m_VolumeLight->SetActive( false );
		}
	}
	//-------------------------------------------------------------------
	void Environment::CastDepth( Engine& engine )
	{
		for ( LightsArray::iterator it = m_LightsArray.begin();
			it != m_LightsArray.end();
			it ++ )
		{
			if ( (*it)->isActive() )
			{
				(*it)->CastDepth( engine );
			}
		}
	}
	//-------------------------------------------------------------------
	void Environment::RecaculateFogParam()
	{
		m_FogParam.m_x = 1.0 / Engine::Instance().GetCamera()->GetFar() * ( 1.0 - m_FogOgiParam.m_x );
		m_FogParam.m_y = m_FogOgiParam.m_y;
		m_FogParam.m_z = 1.0 - m_FogOgiParam.m_z;
	}
	//-------------------------------------------------------------------
	void Environment::SetEnvironment( const vector3f& pos, const vector3f& dir, const vector3f& Ambient, const vector3f& color, const vector3f& vlcolor, const vector3f& fc, const vector3f& fp )
	{
		if ( NULL == m_MainLight )
		{
			m_MainLight = NEW RealDirectionLight();
			_RemoveLightNode( m_MainLight );
			m_LightsArray.insert( m_LightsArray.begin(), m_MainLight );//保证第一个灯饰住光源
		}
		m_MainLight->SetRange( Engine::Instance ().GetCamera()->GetFar() );
		m_MainLight->SetLocalPosition( pos );
		m_MainLight->SetColor( color );
		m_MainLight->SetLocalDirection( dir );
		m_MainLight->SetAttribute( RealLightNode::LA_SHADOW );
		m_AmbientLight = Ambient;
		m_FogColor = fc;
		m_FogOgiParam = vector3f( 0.1, 1.0, 0.1);//fp;
		m_VolumeLightColor = vlcolor;
		RecaculateFogParam();
	}
	//-------------------------------------------------------------------
	void Environment::Render( Engine& engine )
	{
		if ( !engine.GetPipeline()->isDepthCopyed() )
		{
			engine.GetPipeline()->CopyScreenDepth();
		}
		engine.GetDevice()->DepthMask( false );
		engine.GetDevice()->DisableDepthTest();
		engine.GetDevice()->EnableAlphaBlend( Device::BL_DST_COLOR, Device::BL_ONE, Device::BL_ZERO, Device::BL_ONE );
		for ( LightsArray::iterator it = m_LightsArray.begin();
			it != m_LightsArray.end();
			it ++ )
		{
			(*it)->Render( engine );//渲染光晕
		}
		engine.GetDevice()->EnableAlphaBlend( Device::BL_SRC_ALPHA, Device::BL_ONE, Device::BL_ZERO, Device::BL_ONE );
		HaloShader& hs = engine.GetShaderManage()->GetShader<HaloShader>(ShaderManage::HaloShader);
		hs.SetLightTexture( m_pTexture );
		hs.SetDepthTexture( engine.GetPipeline()->GetCopyDepthTexture() );
		hs.SetColor( vector3f( 0.1, 0.3, 1.0 ) );
		engine.GetDevice()->Draw( *m_pRendBuffer, hs, NULL );
	}


}