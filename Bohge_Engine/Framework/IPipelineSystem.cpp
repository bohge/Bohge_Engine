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
#include "IRenderQueueSystem.h"
#include "IEnvironmentSystem.h"
#include "IPipelineSystem.h"
#include "LightComponent.h"
#include "IThreadSystem.h"
#include "IThreadEvent.h"
#include "PostEffect.h"
#include "Framework.h"
#include "ICamera.h"



using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( PipelineProperty );
	RTTI_TYPE_IMPLEMENT( IPipelineSystem );
	SYSTEM_SINGLETON_IMPLEMENT( IPipelineSystem );
	//---------------------------------------------------------------------------------------------------------
	IPipelineSystem::IPipelineSystem( void )
		:m_rpCamera(NULL),
		m_rpAttchedCamera(NULL),
		m_rpLight(NULL),
		m_pTripleEnvironmentBuffer(NULL),
		m_pTripleBuffer(NULL),
		m_pGpuWaitCpuEvent(NULL),
		m_pAmbientLight(NULL),
		m_pMountList(NULL),
		m_pUnmountList(NULL),
		m_isGPUWorking(false)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	IPipelineSystem::~IPipelineSystem( void )
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void IPipelineSystem::RegistEvent( Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &IPipelineSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &IPipelineSystem::OnFrameworkExit ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_END_UPDATE, MEMBER_FUNC_PTR( &IPipelineSystem::OnEndUpdata ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_BEGIN_RENDER, MEMBER_FUNC_PTR( &IPipelineSystem::OnBeginRender ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_DO_RENDER, MEMBER_FUNC_PTR( &IPipelineSystem::OnRender ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void IPipelineSystem::OnFrameworkEntry( float time, BohgeEngine::Framework& framework )
	{
		m_pUnmountList = NEW PostEffectList;
		m_pMountList = NEW PostEffectList;
		m_pPostEffectMap = NEW PipelineProperty::PostEffectMap();
		m_pAmbientLight = NEW LightComponent( PipelineProperty::LT_AMBIENT );
		m_pTripleBuffer = NEW TripleRenderBuffer();
		m_pTripleEnvironmentBuffer = NEW TripleEnvironmentBuffer();
		m_pGpuWaitCpuEvent = 
			static_cast< IThreadEvent* >
			( IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_EVENT ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void IPipelineSystem::OnFrameworkExit( float time, BohgeEngine::Framework& framework )
	{
		IThreadSystem::Instance()->RecycleBin( m_pGpuWaitCpuEvent );
		SAFE_DELETE( m_pUnmountList );
		SAFE_DELETE( m_pMountList );
		SAFE_DELETE( m_pAmbientLight );
		SAFE_DELETE( m_pTripleBuffer );
		SAFE_DELETE( m_pTripleEnvironmentBuffer );
		SAFE_DELETE( m_pPostEffectMap );
	}
	//---------------------------------------------------------------------------------------------------------
	void IPipelineSystem::_DoAttachPostEffect( Framework& framework )
	{
		FOR_EACH(PostEffectList, pe, *m_pMountList)
		{
			m_pPostEffectMap->insert( make_pair( (*pe)->GetQueue(), (*pe) ) );
		}
		m_pMountList->clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void IPipelineSystem::_DoDetachPostEffect()
	{
		FOR_EACH(PostEffectList, pe, *m_pUnmountList)
		{
			FOR_EACH( PipelineProperty::PostEffectMap, post, *m_pPostEffectMap )
			{
				if ( post->second == *pe )
				{
					m_pPostEffectMap->erase( post );
					break;
				}
			}
		}
		m_pUnmountList->clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void IPipelineSystem::OnEndUpdata( float time, Framework& framework )
	{
		if ( NULL != m_rpAttchedCamera )
		{
			m_rpAttchedCamera->Update();
			IEnvironmentSystem::Instance()->UpdateSystem();
			IRenderQueueSystem::Instance()->UpdateSystem();
			m_pTripleBuffer->CopyCameraState( *m_rpAttchedCamera );
			m_pTripleEnvironmentBuffer->SetCamera( m_pTripleBuffer->GetCamera() );
			_DoUpdate();
			m_pTripleBuffer->SwapFlushingBuffer();
			m_pTripleEnvironmentBuffer->SwapFlushingBuffer();
			//if ( !m_isGPUWorking )
			//{
			//	m_pGpuWaitCpuEvent->Signal();
			//}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void IPipelineSystem::OnBeginRender( float time, Framework& framework )
	{
		_DoAttachPostEffect( framework );
		_DoDetachPostEffect();
	}
	//---------------------------------------------------------------------------------------------------------
	void IPipelineSystem::OnRender( float time, Framework& framework )
	{
		//执行渲染
		//if ( !m_pTripleBuffer->isNewBuffer() )//可以注掉，防止没有渲染物体的时候卡死主线程
		//{
		//	m_isGPUWorking = false;
		//	m_pGpuWaitCpuEvent->Wait();
		//}
		//m_isGPUWorking = true;
		m_pTripleBuffer->SwapActivedBuffer();
		m_pTripleEnvironmentBuffer->SwapActivedBuffer();
		m_rpCamera = m_pTripleBuffer->GetCamera();
		_DoRender();
	}
	//---------------------------------------------------------------------------------------------------------
	void IPipelineSystem::AttachPostEffect( PostEffect* pe )
	{
		m_pMountList->push_back( pe );
	}
	//---------------------------------------------------------------------------------------------------------
	void IPipelineSystem::DetachPostEffect( PostEffect* pe )
	{
		m_pUnmountList->push_back( pe );
	}
}