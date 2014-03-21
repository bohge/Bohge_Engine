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
//		游戏中post effect的管理者	   //
/////////////////////////////////////////

#include "Pipeline.h"
#include "Device.h"
#include "IPostEffect.h"
#include "SceneManager.h"
#include "Environment.h"
#include "Bfont.h"
#include "UIManager.h"
#include "Environment.h"
#include "ActionManager.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "ShaderManager.h"
#include "RenderTarget.h"
#include "Vertex.h"
#include "RendBuffer.h"
#include "CopyCurrentScreen.h"
#include "UtilityShaders.h"
#include "Texture.h"
#include "Engine.h"
#include "RealLights.h"
#include "ShadowCaster.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	Pipeline::Pipeline(void)
		:m_isPostEffectActive(true),
		m_NormalBufferNeederCount(0),
		m_pOriginalColorDepthStencil(NULL),
		m_isDirtyAssign(true)
	{
		vector2d size = Engine::Instance().GetDevice()->GetResolutionSize();
		m_pOriginalColorDepthStencil = NEW RenderTarget(size, Device::FBO_TCOLOR_TDEPTHSTENCIL);
		m_pOriginalColorDepthStencil->GetDepthBuffer()->SetWarp( Device::TW_MIRRORED_REPEAT, Device::TW_MIRRORED_REPEAT);
		m_pNormalBuffer = NEW RenderTarget(size, Device::FBO_TCOLOR_BDEPTH);
		m_pNormalBuffer->GetColorBuffer()->SetWarp( Device::TW_MIRRORED_REPEAT, Device::TW_MIRRORED_REPEAT);
		//m_pBottomUISkyBoxColor = NEW RenderTarget(size, Device::FBO_texColor);
		m_pPreviousSceneColor[0] = NEW RenderTarget(size, Device::FBO_TCOLOR);
		m_pPreviousSceneColor[1] = NEW RenderTarget(size, Device::FBO_TCOLOR);
		m_pCopyer = NEW CopyCurrentScreen( m_pPreviousSceneColor[0], m_pPreviousSceneColor[1] );
#ifdef IOS
		m_EndSecen = NEW RenderTarget();
#endif
		VertexTexture Vertex[COUNT_OF_VERTEX];
		Vertex[0] = VertexTexture(vector4f(-1,1,0), vector2f(0,1));
		Vertex[1] = VertexTexture(vector4f(-1,-1,0), vector2f(0,0));
		Vertex[2] = VertexTexture(vector4f(1,1,0), vector2f(1,1));
		Vertex[3] = VertexTexture(vector4f(1,-1,0), vector2f(1,0));
		ushort Indies[COUNT_OF_INDIES]={0,1,2,3};
		m_pRendBuffer = NEW RendBuffer( Device::RM_TRIANGLE_STRIP);
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_STATIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, &Vertex[0] );
		m_pIndicesBuffer->FlushIndiesBuffer( COUNT_OF_INDIES, &Indies[0] );
	}
	//-------------------------------------------------------------------------------------------------------
	Pipeline::~Pipeline(void)
	{
#ifdef IOS
		SAFE_DELETE( m_EndSecen );
#endif
		SAFE_DELETE( m_pRendBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
		SAFE_DELETE( m_pOriginalColorDepthStencil );
		SAFE_DELETE( m_pNormalBuffer );
		//SAFE_DELETE( m_pBottomUISkyBoxColor );
		SAFE_DELETE( m_pPreviousSceneColor[0] );
		SAFE_DELETE( m_pPreviousSceneColor[1] );
		SAFE_DELETE( m_pCopyer );
	}
	//-------------------------------------------------------------------------------------------------------
	void Pipeline::RemovePostEffect( IPostEffect* ipe )
	{
		m_PostMap.erase( ipe->Level() );
	}
	//-------------------------------------------------------------------------------------------------------
	void Pipeline::Render(Engine& engine)
	{
		bool isAnyPostEffectActive = false;
		{//判断是否有后期
			for ( PostEffectMap::iterator it = m_PostMap.begin();//绘制特效
				it != m_PostMap.end();
				it++)
			{
				if ( it->second->isActive() )
				{
					isAnyPostEffectActive = true;
					break;
				}
			}
		}
		engine.GetDevice()->EnableDepthTest();
		engine.GetDevice()->SetCullFace( Device::CFM_BACK );
		engine.GetDevice()->DepthMask(true);
		if( engine.GetEnvironment()->hasLight() )
		{
			m_CurrentPassType = PT_DEPTH;
			engine.GetDevice()->ClearScreenColor( 1.0f, 1.0f, 1.0f, 1.0f );//重要保证深度文理的底色是纯白，既，深度数值为1，最大
			engine.GetEnvironment()->CastDepth( engine );

			if ( engine.GetEnvironment()->isCastShadow() )//实际上这个在多光源中需要循环
			{
				m_isDirtyAssign = true;
				engine.GetSceneManager()->AssignMaterial( 
					engine.GetEnvironment()->GetCurrentLight().GetCaster().isCastShadow(),
					engine.GetEnvironment()->GetCurrentLight().GetCaster().GetFirstCamera() );
			}
			else if( m_isDirtyAssign )
			{
				m_isDirtyAssign = false;
				engine.GetSceneManager()->AssignMaterial( 
					false,
					engine.GetEnvironment()->GetCurrentLight().GetCaster().GetFirstCamera() );
			}
		}

		if ( isAnyPostEffectActive )
		{
			if ( m_NormalBufferNeederCount > 0 )//渲染法线
			{
				engine.GetDevice()->DisableAlpha();
				m_CurrentPassType = PT_NORMAL;
				engine.GetDevice()->ClearScreenColor( 0.0f, 0.0f, 0.0f, 0.0f );
				engine.GetDevice()->PushRenderTarget( m_pNormalBuffer );//先将场景绘制在原始纹理上
				engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER | Device::STENCIL_BUFFER );
				engine.GetSceneManager()->RenderNormal(engine);
				engine.GetDevice()->PopRenderTarget( );
			}

			{//绘制主场景
				m_CurrentPassType = PT_LIGHTING;
				engine.GetDevice()->ClearScreenColor( 0.0f, 0.0f, 0.0f, 0.0f );//重要alpha必须是0，看volumelightpost的实现
				engine.GetDevice()->PushRenderTarget( m_pOriginalColorDepthStencil );//先将场景绘制在原始纹理上
				engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER );
				//engine.GetDevice()->ColorMask( false, false, false, false );
				//ICamera* old = engine.GetCamera();
				//StaticCamera came;
				//came.CreatePerspectiveProjection( old->GetNear()*0.95, old->GetFar()*0.95 );
				//came.LookAt( old->GetPosition(), old->GetLookAt(), old->GetUp() );
				//came.Update();
				//came.AttachCamera();
				//engine.GetSceneManage()->Render(engine);
				//engine.GetDevice()->ColorMask( true, true, true, true);
				//old->AttachCamera();
				engine.GetSceneManager()->RenderSecen(engine);
				engine.GetEnvironment()->Render( engine );//绘制halo和甲光源
				engine.GetDevice()->PopRenderTarget( );
			}
			bool swtich = false;
			bool isNeedOrig = true;
			for ( PostEffectMap::iterator it = m_PostMap.begin();//绘制特效
				it != m_PostMap.end();
				it++)
			{
				if ( it->second->isActive() )
				{
					if( isNeedOrig )//如果是第一个posteffect就把初始ogi传到secee
					{
						isNeedOrig = false;
						it->second->Process(engine, m_pOriginalColorDepthStencil,m_pOriginalColorDepthStencil, m_pPreviousSceneColor[static_cast<int>(swtich)] );
					}
					else
					{
						it->second->Process(engine, m_pOriginalColorDepthStencil,m_pPreviousSceneColor[static_cast<int>( !swtich/*注意叹号*/ )], m_pPreviousSceneColor[static_cast<int>(swtich)] );
					}
					swtich = !swtich; //交换
				}
			}
#ifdef IOS
			engine.GetDevice()->PushRenderTarget( m_EndSecen );
#endif
			engine.GetDevice()->DisableDepthTest();
			engine.GetDevice()->DisableAlpha();
			engine.GetDevice()->Clear( Device::COLOR_BUFFER );//| Device::DEPTH_BUFFER | Device::STENCIL_BUFFER );//注掉的原因看Clear函数
			QuadShader& qs = engine.GetShaderManager()->GetShader<QuadShader>(ShaderManager::QuadShader);
			engine.GetDevice()->Draw( *m_pRendBuffer, qs, m_pPreviousSceneColor[static_cast<int>( !swtich/*注意叹号*/ )]->GetColorBuffer() );
		}
		else
		{
			//绘制主场景
#ifdef IOS
			engine.GetDevice()->PushRenderTarget( m_EndSecen );
#endif
			m_CurrentPassType = PT_LIGHTING;
			engine.GetDevice()->ClearScreenColor( );//改变清屏颜色,使用客户设置的颜色
			engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER | Device::STENCIL_BUFFER );
			engine.GetSceneManager()->RenderSecen(engine);
			engine.GetEnvironment()->Render( engine );//绘制halo和甲光源
		}
		//渲染2D
		engine.GetDevice()->DisableDepthTest();
		engine.GetDevice()->EnableAlphaBlend(Device::BL_SRC_ALPHA, Device::BL_ONE_MINUS_SRC_ALPHA, Device::BL_ONE, Device::BL_ONE );
		engine.GetUIManager()->Render(engine, UIManager::UL_TOP);
		engine.GetStringManager()->Render(engine);
		m_isDepthCopyed = false;
#ifdef IOS
		engine.GetDevice()->PopRenderTarget();
#endif
	}
	//-------------------------------------------------------------------------------------------------------
	void Pipeline::CopyScreenColor()
	{
		m_pCopyer->CopyScreenColor( Engine::Instance(), m_pOriginalColorDepthStencil );
	}
	//-------------------------------------------------------------------------------------------------------
	void Pipeline::CopyScreenDepth()
	{
		m_isDepthCopyed = true;
		m_pCopyer->CopyScreenDepth( Engine::Instance(), m_pOriginalColorDepthStencil );
	}
	//-------------------------------------------------------------------------------------------------------
	Texture* Pipeline::GetCopyColorTexture()
	{
		return m_pCopyer->ColorTexture();
	}
	//-------------------------------------------------------------------------------------------------------
	Texture* Pipeline::GetCopyDepthTexture()
	{
		return m_pCopyer->DepthTexture();
	}
	//-------------------------------------------------------------------------------------------------------
#ifdef IOS
	void Pipeline::SetRendTargetHandle( handle h, const vector2d& size )
	{
		m_EndSecen->SetRendTargetHandle( h, size);
	}
#endif
	//-------------------------------------------------------------------------------------------------------
	void Pipeline::OnSizeChange( const vector2d& size )
	{
		if ( size != vector2d::Zero() )
		{
			m_pOriginalColorDepthStencil->Resize( size );
			m_pNormalBuffer->Resize( size );
			//m_pBottomUISkyBoxColor->Resize( size );
			m_pPreviousSceneColor[0]->Resize( size );
			m_pPreviousSceneColor[1]->Resize( size );
			for ( PostEffectMap::iterator it = m_PostMap.begin();//绘制特效
				it != m_PostMap.end();
				it++)
			{
				it->second->OnSizeChange( size );
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void Pipeline::AddPostEffcet( IPostEffect* ipe )
	{
		m_PostMap.insert( std::make_pair( ipe->Level(), ipe ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void Pipeline::CostumPassType( const PassType& pt )
	{
		Device* device = Engine::Instance().GetDevice();
		switch ( pt )
		{
		case Pipeline::PT_DEPTH: device->ClearScreenColor( 1,1,1,1 );break;
		case Pipeline::PT_NORMAL: device->ClearScreenColor( 0,0,0,0 );break;
		}
		m_CurrentPassType = pt;
	}
}