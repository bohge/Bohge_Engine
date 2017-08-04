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
#include "Framework/RenderTargetMetadata.h"
#include "Framework/RenderObjectMetadata.h"
#include "Framework/RenderTargetEntity.h"
#include "Framework/RenderObjectEntity.h"
#include "Framework/IRenderQueueSystem.h"
#include "Framework/IEnvironmentSystem.h"
#include "Framework/ShapeBufferUtility.h"
#include "Framework/TextureMetadata.h"
#include "Framework/RenderComponent.h"
#include "Framework/IMaterialSystem.h"
#include "Framework/LightComponent.h"
#include "Framework/TextureEntity.h"
#include "Framework/BinaryIndex.hpp"
#include "Framework/GlobalDefine.h"
#include "Framework/GlobalDefine.h"
#include "ForwardPipelineSystem.h"
#include "Framework/PostEffect.h"
#include "Framework/Framework.h"
#include "Framework/ICamera.h"
#include "Framework/Log.h"



using namespace eastl;
using namespace BohgeEngine;
namespace ForwardPipelineSystemPlugin
{
	//---------------------------------------------------------------------------------------------------------
	ForwardPipelineSystem::ForwardPipelineSystem( void )
		:m_pLightRenderArray(NULL),
		m_pMainRenderTarget(NULL),
		m_pRenderQueueOrderMap(NULL),
		m_pOriginalColorDepthStencil(NULL),
		m_pPreviousSceneColor(),
		m_pRenderObject(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	ForwardPipelineSystem::~ForwardPipelineSystem( void )
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void ForwardPipelineSystem::RegistEvent( BohgeEngine::Framework& framework )
	{
		IPipelineSystem::RegistEvent( framework );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &ForwardPipelineSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &ForwardPipelineSystem::OnFrameworkExit ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void ForwardPipelineSystem::OnFrameworkEntry( float time, BohgeEngine::Framework& framework )
	{
		m_pRenderQueueOrderMap = NEW QueueOrderMap();
		m_pLightRenderArray = NEW LightRenderArray();
		IMaterialSystem::Instance()->SetPipelienType(
			PipelineProperty::PLT_FORWARD_SINGLE_LIGHT,
			PipelineProperty::APT_LIGHTING_ALL
			| PipelineProperty::APT_UNIVERSAL_DEPTH
			| PipelineProperty::APT_UNIVERSAL_POST_EFFECT );
		{
			m_pMainRenderTarget = NEW RenderTargetEntity;
			m_pMainRenderTarget->ListenResizeview();
			m_pMainRenderTarget->SetJobType( IJob::JT_SYNCHRONOUS );
			m_pMainRenderTarget->PushMetadata(
				RenderTargetDependResolutionMetadata(
					RenderTargetProperty::RT_RENDER_TARGET_MAIN,
					RenderTargetProperty::ST_SWAP_A,
					GlobalDefine::Color_Gray,
					framework.GetConfig().GetViewSize(),
					framework.GetConfig().GetResolution()
				));
			m_pMainRenderTarget->CreateResource();
		}

		{
			m_pOriginalColorDepthStencil = NEW RenderTargetEntity;
			m_pOriginalColorDepthStencil->ListenResizeview();
			m_pOriginalColorDepthStencil->SetJobType( IJob::JT_SYNCHRONOUS );
			m_pOriginalColorDepthStencil->PushMetadata(
				RenderTargetDependResolutionMetadata(
				RenderTargetProperty::RT_RENDER_TARGET_2D,
				RenderTargetProperty::ST_SWAP_A,
				GlobalDefine::Color_Black,
				framework.GetConfig().GetViewSize(),
				framework.GetConfig().GetResolution()
				));
			TextureEntity* tex = m_pOriginalColorDepthStencil->MakeTextureAttachment( RenderTargetProperty::TA_COLOR_0 );
			tex->PushMetadata( 
				TextureBufferMetadata( 
						framework.GetConfig().GetResolution(),
						TextureProperty::TT_TEXTURE2D,
						TextureProperty::TU_STATIC,
						TextureProperty::PF_R8G8B8A8, 1, false,
						TextureProperty::TW_CLAMP_TO_EDGE,
						TextureProperty::TW_CLAMP_TO_EDGE,
						TextureProperty::TF_NEAREST,
						TextureProperty::TF_NEAREST,
						NULL ) );
			tex = m_pOriginalColorDepthStencil->MakeTextureAttachment( RenderTargetProperty::TA_DEPTH_STENCIL );
			tex->PushMetadata( 
				TextureBufferMetadata( 
					framework.GetConfig().GetResolution(),
					TextureProperty::TT_TEXTURE2D,
					TextureProperty::TU_STATIC,
					TextureProperty::PF_DEPTH24_STENCIL8, 1, false,
					TextureProperty::TW_CLAMP_TO_EDGE,
					TextureProperty::TW_CLAMP_TO_EDGE,
					TextureProperty::TF_NEAREST,
					TextureProperty::TF_NEAREST,
					NULL ) );
			m_pOriginalColorDepthStencil->CreateResource();
		}

		{
			m_pPreviousSceneColor[0] = NEW RenderTargetEntity;
			m_pPreviousSceneColor[0]->ListenResizeview();
			m_pPreviousSceneColor[0]->SetJobType( IJob::JT_SYNCHRONOUS );
			m_pPreviousSceneColor[0]->PushMetadata(
				RenderTargetDependResolutionMetadata(
				RenderTargetProperty::RT_RENDER_TARGET_2D,
				RenderTargetProperty::ST_SWAP_A,
				GlobalDefine::Color_Black,
				framework.GetConfig().GetViewSize(),
				framework.GetConfig().GetResolution()
				));
			TextureEntity* tex = m_pPreviousSceneColor[0]->MakeTextureAttachment( RenderTargetProperty::TA_COLOR_0 );
			tex->PushMetadata( 
				TextureBufferMetadata( 
					framework.GetConfig().GetResolution(),
					TextureProperty::TT_TEXTURE2D,
					TextureProperty::TU_STATIC,
					TextureProperty::PF_R8G8B8A8, 1, false,
					TextureProperty::TW_CLAMP_TO_EDGE,
					TextureProperty::TW_CLAMP_TO_EDGE,
					TextureProperty::TF_NEAREST,
					TextureProperty::TF_NEAREST,
					NULL ) );
			m_pPreviousSceneColor[0]->CreateResource();
		}

		{
			m_pPreviousSceneColor[1] = NEW RenderTargetEntity;
			m_pPreviousSceneColor[1]->ListenResizeview();
			m_pPreviousSceneColor[1]->SetJobType( IJob::JT_SYNCHRONOUS );
			m_pPreviousSceneColor[1]->PushMetadata(
				RenderTargetDependResolutionMetadata(
					RenderTargetProperty::RT_RENDER_TARGET_2D,
					RenderTargetProperty::ST_SWAP_B,
					GlobalDefine::Color_Black,
					framework.GetConfig().GetViewSize(),
					framework.GetConfig().GetResolution()
				));
			TextureEntity* tex = m_pPreviousSceneColor[1]->MakeTextureAttachment( RenderTargetProperty::TA_COLOR_0 );
			tex->PushMetadata( 
				TextureBufferMetadata( 
				framework.GetConfig().GetResolution(),
				TextureProperty::TT_TEXTURE2D,
				TextureProperty::TU_STATIC,
				TextureProperty::PF_R8G8B8A8, 1, false,
				TextureProperty::TW_CLAMP_TO_EDGE,
				TextureProperty::TW_CLAMP_TO_EDGE,
				TextureProperty::TF_NEAREST,
				TextureProperty::TF_NEAREST,
				NULL ) );
			m_pPreviousSceneColor[1]->CreateResource();
		}
		{
			m_pRenderObject = NEW RenderObjectEntity();
			m_pRenderObject->SetJobType( IJob::JT_SYNCHRONOUS );
			m_pRenderObject->PushMetadata(
				RenderObjectMeshMetadate( 
					RenderObjectProperty::RM_TRIANGLES,
					QuadVertexMetadata( BufferObjectProperty::MU_STATIC ),
					QuadIndicesMetadata( BufferObjectProperty::MU_STATIC ) )
				);
			m_pRenderObject->PushMetadata( RenderObjectMaterialMetadata( PathMetadata( DefaultAsset::ImageBlitMaterial ) ) );
			m_pRenderObject->CreateResource();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ForwardPipelineSystem::OnFrameworkExit( float time, BohgeEngine::Framework& framework )
	{
		SAFE_DELETE( m_pRenderObject );
		SAFE_DELETE( m_pRenderQueueOrderMap );
		SAFE_DELETE( m_pMainRenderTarget );
		SAFE_DELETE( m_pLightRenderArray );
		SAFE_DELETE( m_pOriginalColorDepthStencil );
		SAFE_DELETE( m_pPreviousSceneColor[0] );
		SAFE_DELETE( m_pPreviousSceneColor[1] );
	}
	//---------------------------------------------------------------------------------------------------------
	void ForwardPipelineSystem::_DoUpdate( )
	{
		IRenderQueueSystem::Instance()->Culling( _GetRenderBufferInserter() );//裁剪可见物体
		IEnvironmentSystem::Instance()->Culling( _GetEnvironmentInserter() );//裁剪可见灯光
	}
	//---------------------------------------------------------------------------------------------------------
	void ForwardPipelineSystem::_DoDrawLayer()
	{
		FOR_EACH( QueueOrderMap, queueMap, *m_pRenderQueueOrderMap )
		{
			if ( queueMap->first < MaterialProperty::MRQ_POSTEFFECT )
			{
				FOR_EACH( MaterialOrderMap, depthMap, queueMap->second )
				{
					FOR_EACH( DepthOrderMap, render, depthMap->second )
					{
						_SetCurrentLight( render->second->m_rpLights );
						render->second->m_rpRender->Draw( render->second->m_rpLights->GetLightType() );
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ForwardPipelineSystem::_DoDrawOverLayer()
	{
		FOR_EACH( QueueOrderMap, queueMap, *m_pRenderQueueOrderMap )
		{
			if ( queueMap->first >= MaterialProperty::MRQ_OVERLAY )
			{
				FOR_EACH( MaterialOrderMap, depthMap, queueMap->second )
				{
					FOR_EACH( DepthOrderMap, render, depthMap->second )
					{
						_SetCurrentLight( render->second->m_rpLights );
						render->second->m_rpRender->Draw( render->second->m_rpLights->GetLightType() );
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ForwardPipelineSystem::_DoRender()
	{	
		const PipelineProperty::VisableLight& lights = _GetVisableLight();
		const PipelineProperty::VisableRender& renders = _GetVisableRender();

		//计算环境光照强度
		{
			vector3f ambient;
			FOR_EACH_CONST( PipelineProperty::VisableLight, light, lights )
			{
				if ( PipelineProperty::LT_AMBIENT == (*light)->GetLightType() )
				{
					ambient += (*light)->GetColor();
				}
			}
			_GetAmbientLight()->SetColor( ambient );
		}

		//分配数组
		const uint rendersSize = renders.size();
		{
			if ( m_pLightRenderArray->size() < rendersSize )
			{
				m_pLightRenderArray->resize( rendersSize );
			}
		}
		//DEBUGLOG( "visable light %d, render %d\n", lights.size(), renders.size() );

		//计算每一个渲染物体的
		{
			int index = 0;
			FOR_EACH_CONST( PipelineProperty::VisableRender, render, renders )
			{
				float minEff = std::numeric_limits< float >::lowest();
				(*m_pLightRenderArray)[index].m_rpRender = *render;
				(*m_pLightRenderArray)[index].m_rpLights = _GetAmbientLight();
				FOR_EACH_CONST( PipelineProperty::VisableLight, light, lights )
				{
					if ( PipelineProperty::LT_AMBIENT != (*light)->GetLightType() )
					{
						if ( (*light)->isEffectOnRender( (*render)->GetBindBox() ) )
						{
							float effect = (*light)->CaculateEffective( (*render)->GetBindBox() );
							if ( effect > minEff )
							{
								minEff = effect;
								(*m_pLightRenderArray)[index].m_rpLights = *light;
							}
						}
					}
				}
				++ index;
			}
		}

		////单一光源光照渲染
		//{
		//	FOR_EACH( LightRenderArray, render, *m_pLightRenderArray )
		//	{
		//		_SetCurrentLight( render->m_rpLights );
		//		render->m_rpRender->Draw( render->m_rpLights->GetLightType() );
		//	}
		//}

		//排序
		{
			m_pRenderQueueOrderMap->clear();
			for ( int i = 0 ;
				i < rendersSize;
				++ i )
			{
				LightRenderComponent* render = &m_pLightRenderArray->at( i );
				uint que = render->m_rpRender->GetRenderQueue( render->m_rpLights->GetLightType() );
				QueueOrderMap::iterator materialMap = m_pRenderQueueOrderMap->find( que );
				if ( m_pRenderQueueOrderMap->end() == materialMap )
				{
					materialMap = m_pRenderQueueOrderMap->insert( make_pair( que, MaterialOrderMap() ) ).first;
				}
				uint hash = -1;
				if ( !render->m_rpRender->isTransparent( render->m_rpLights->GetLightType() ) )//透明物体不能材质排序
				{
					hash = render->m_rpRender->GetMaterialID( render->m_rpLights->GetLightType() );
				}
				MaterialOrderMap::iterator OrderMap = materialMap->second.find( hash );
				if ( materialMap->second.end() == OrderMap )
				{
					OrderMap = materialMap->second.insert( make_pair( hash, DepthOrderMap() ) ).first;
				}
				float dis;
				if ( que >= MaterialProperty::MRQ_OVERLAY )//多层渲染的通过Depth来区别先后
				{
					dis = render->m_rpRender->GetBindBox().GetCenter().LengthPow();
				}
				else
				{
					dis = ( GetCamera()->GetPosition() - render->m_rpRender->GetBindBox().GetCenter() ).LengthPow();
				}
				OrderMap->second.insert( make_pair( dis, &*render ) );
			}
		}

		//渲染
		{
			const PipelineProperty::PostEffectMap& postMap = _GetPostEffectMap();
			if ( 0 == postMap.size() )
			{//没有后处理
				m_pMainRenderTarget->PushRenderTarget();
				m_pMainRenderTarget->ClearBuffer( RenderTargetProperty::CF_COLOR_DEPTH_STENCIL );
				_DoDrawLayer();
				_DoDrawOverLayer();
			}
			else
			{
				m_pOriginalColorDepthStencil->PushRenderTarget();
				m_pOriginalColorDepthStencil->ClearBuffer( RenderTargetProperty::CF_COLOR_DEPTH_STENCIL );
				_DoDrawLayer();

				uint swtich(0);
				bool isNeedOrig = true;
				FOR_EACH_CONST( PipelineProperty::PostEffectMap, post, postMap )
				{
					if( isNeedOrig )//如果是第一个posteffect就把初始ogi传到secee
					{
						isNeedOrig = false;
						post->second->ProcessPostEffect( m_pOriginalColorDepthStencil,m_pOriginalColorDepthStencil, m_pPreviousSceneColor[swtich] );
					}
					else
					{
						post->second->ProcessPostEffect( m_pOriginalColorDepthStencil,m_pPreviousSceneColor[!swtich/*注意叹号*/], m_pPreviousSceneColor[swtich] );
					}
					swtich = !swtich; //交换
				}
				m_pMainRenderTarget->PushRenderTarget();
				m_pMainRenderTarget->ClearBuffer( RenderTargetProperty::CF_COLOR_DEPTH_STENCIL );
				m_pRenderObject->SetTextureEntity(
					MaterialProperty::ATT_TEXTURE_DIFFUSE,
					m_pPreviousSceneColor[!swtich/*注意叹号*/]->GetAttachment( RenderTargetProperty::TA_COLOR_0 )
					);
				m_pRenderObject->Draw( PipelineProperty::PT_UNIVERSAL_POST_EFFECT );
				_DoDrawOverLayer();
			}
		}
	}
}