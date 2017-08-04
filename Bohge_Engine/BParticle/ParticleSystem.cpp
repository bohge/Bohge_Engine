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
#include "Framework/IScriptSystem.h"
#include "Framework/Framework.h"
#include "ParticleSystem.h"
#include "ParticleEntity.h"
#include "DoRegister.hpp"



using namespace eastl;
using namespace BohgeEngine;
namespace ParticleSystemPlugin
{
	RTTI_TYPE_IMPLEMENT( ParticleSystem );
	SYSTEM_SINGLETON_IMPLEMENT( ParticleSystem );
	//---------------------------------------------------------------------------------------------------------
	ParticleSystem::ParticleSystem(void)
		:m_pRunningParticle(NULL),
		m_pUnmountList(NULL),
		m_pMountList(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	ParticleSystem::~ParticleSystem(void)
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void ParticleSystem::RegistEvent( BohgeEngine::Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_ENTRY, MEMBER_FUNC_PTR( &ParticleSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_EXIT, MEMBER_FUNC_PTR( &ParticleSystem::OnFrameworkExit ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_BEGIN_RENDER, MEMBER_FUNC_PTR( &ParticleSystem::OnBeginRender ) );
	}
	//---------------------------------------------------------------------------------------------------------
	void ParticleSystem::OnFrameworkEntry( float time, BohgeEngine::Framework& framework )
	{
		DoRegister( *IScriptSystem::Instance() );
		m_pRunningParticle = NEW ParticleEntityMap;
		m_pUnmountList = NEW ParticleEntityList;
		m_pMountList = NEW ParticleEntityList;
	}
	//---------------------------------------------------------------------------------------------------------
	void ParticleSystem::OnFrameworkExit( float time, BohgeEngine::Framework& framework )
	{
		SAFE_DELETE( m_pMountList );
		SAFE_DELETE( m_pUnmountList );
		SAFE_DELETE( m_pRunningParticle );
	}
	//---------------------------------------------------------------------------------------------------------
	void ParticleSystem::OnBeginRender( float time, BohgeEngine::Framework& framework )
	{
		_DoMountParticle();
		_DoUnmountParticle();
		FOR_EACH(ParticleEntityMap, pe, *m_pRunningParticle)
		{
			pe->second->UpdataEmitter( time );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ParticleSystem::_DoMountParticle()
	{
		FOR_EACH(ParticleEntityList, pe, *m_pMountList)
		{
			m_pRunningParticle->insert( make_pair( (*pe)->GetObjectID(), (*pe) ) );
		}
		m_pMountList->clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void ParticleSystem::_DoUnmountParticle()
	{
		FOR_EACH(ParticleEntityList, pe, *m_pUnmountList)
		{
			m_pRunningParticle->erase( (*pe)->GetObjectID() );
		}
		m_pUnmountList->clear();
	}

}