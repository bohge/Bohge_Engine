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
#include "TripleEnvironmentBuffer.h"
#include "TripleRenderBuffer.h"
#include "PipelineProperty.h"
#include "SystemSingleton.h"
#include "IPlugin.h"
#include "3DMath.h"


#include <EASTL/list.h>

namespace BohgeEngine
{
	class ICamera;
	class IThreadEvent;
	struct ViewResolution;
	class BOHGE_FRAMEWORK IPipelineSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IPipelineSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IPipelineSystem, IPlugin );
	private:
		typedef eastl::list< PostEffect* >	PostEffectList;
	private:
		PostEffectList*						m_pMountList;
		PostEffectList*						m_pUnmountList;
		PipelineProperty::PostEffectMap*	m_pPostEffectMap;
		LightComponent*						m_pAmbientLight;
		ICamera*							m_rpAttchedCamera;
		ICamera*							m_rpCamera;
		LightComponent*						m_rpLight;
		TripleEnvironmentBuffer*			m_pTripleEnvironmentBuffer;
		TripleRenderBuffer*					m_pTripleBuffer;
		IThreadEvent*						m_pGpuWaitCpuEvent;
		bool								m_isGPUWorking;
	public:
		IPipelineSystem(void);
		virtual ~IPipelineSystem(void);
	private:
		virtual void _DoUpdate( ) = 0;
		virtual void _DoRender( ) = 0;
	private:
		void _DoAttachPostEffect( Framework& framework );
		void _DoDetachPostEffect();
	public:
		virtual void RegistEvent( Framework& framework ); //×¢²áÊÂ¼þ
		void OnFrameworkEntry( float time, BohgeEngine::Framework& framework );
		void OnFrameworkExit( float time, BohgeEngine::Framework& framework );
		void OnEndUpdata( float time, Framework& framework );
		void OnBeginRender( float time, Framework& framework );
		void OnRender( float time, Framework& framework );
	public:
		void AttachPostEffect( PostEffect* pe );
		void DetachPostEffect( PostEffect* pe );
	protected:
		BOHGE_FORCEINLINE IInserter* _GetRenderBufferInserter()
		{
			return m_pTripleBuffer;
		}
		BOHGE_FORCEINLINE IInserter* _GetEnvironmentInserter()
		{
			return m_pTripleEnvironmentBuffer;
		}
		BOHGE_FORCEINLINE const PipelineProperty::VisableRender& _GetVisableRender()
		{
			return m_pTripleBuffer->GetVisableRender();
		}
		BOHGE_FORCEINLINE const PipelineProperty::VisableLight& _GetVisableLight()
		{
			return m_pTripleEnvironmentBuffer->GetVisableLight();
		}
		BOHGE_FORCEINLINE void _SetCurrentLight( LightComponent* light )
		{
			m_rpLight = light;
		}
		BOHGE_FORCEINLINE LightComponent* _GetAmbientLight()
		{
			return m_pAmbientLight;
		}
		BOHGE_FORCEINLINE const PipelineProperty::PostEffectMap& _GetPostEffectMap()
		{
			return *m_pPostEffectMap;
		}
	public:
		BOHGE_FORCEINLINE void AttachCamera( ICamera* came )
		{
			m_rpAttchedCamera = came;
		}
		BOHGE_FORCEINLINE void DetachCamera( ICamera* came )
		{
			if ( m_rpAttchedCamera == came )
			{
				m_rpAttchedCamera = NULL;
			}
		}
		BOHGE_FORCEINLINE ICamera* GetCamera()
		{
			return m_rpCamera;
		}
		BOHGE_FORCEINLINE const LightComponent* GetCurrentLight()
		{
			return m_rpLight;
		}
		BOHGE_FORCEINLINE const LightComponent* GetAmbientLight()
		{
			return m_pAmbientLight;
		}
	};
}