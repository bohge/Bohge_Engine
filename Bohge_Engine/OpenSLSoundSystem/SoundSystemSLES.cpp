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
#include "Framework/Framework.h"
#include "Framework/Log.h"


#include "SoundSystemSLES.h"
#include "SoundPlayerSLES.h"


#include <SLES/OpenSLES.h>

using namespace eastl;
using namespace BohgeEngine;

namespace OpenSLESSoundSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	SoundSystemSLES::SoundSystemSLES(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundSystemSLES::~SoundSystemSLES(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundSystemSLES::RegistEvent( BohgeEngine::Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &SoundSystemSLES::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &SoundSystemSLES::OnFrameworkExit ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_DO_UPDATE, MEMBER_FUNC_PTR( &SoundSystemSLES::OnUpdate ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundSystemSLES::OnFrameworkEntry( float time, BohgeEngine::Framework& framework )
	{
		//初始化系统
		SLresult result;

		// create engine
		result = slCreateEngine(&m_pEngineObject, 0, NULL, 0, NULL, NULL);
		ASSERT(SL_RESULT_SUCCESS == result);

		// realize the engine
		result = (*m_pEngineObject)->Realize(m_pEngineObject, SL_BOOLEAN_FALSE);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get the engine interface
		result = (*m_pEngineObject)->GetInterface(m_pEngineObject, SL_IID_ENGINE, &m_pEngine);
		ASSERT(SL_RESULT_SUCCESS == result);

		// create output mix
		const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
		const SLboolean req[1] = {SL_BOOLEAN_FALSE};
		result = (*m_pEngine)->CreateOutputMix(m_pEngine, &m_pMixObject, 1, ids, req);
		ASSERT(SL_RESULT_SUCCESS == result);

		// realize the output mix
		result = (*m_pMixObject)->Realize(m_pMixObject, SL_BOOLEAN_FALSE);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get the environmental reverb interface
		SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
		result = (*m_pMixObject)->GetInterface(m_pMixObject, SL_IID_ENVIRONMENTALREVERB, &m_pEnvironmental);
		if (SL_RESULT_SUCCESS == result) 
		{
			result = (*m_pEnvironmental)->SetEnvironmentalReverbProperties(m_pEnvironmental, &reverbSettings);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundSystemSLES::OnFrameworkExit( float time, BohgeEngine::Framework& framework )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundSystemSLES::OnUpdate( float time, BohgeEngine::Framework& framework )
	{
		for ( SoundPlayerManagerMap::iterator soundmap = m_SoundMapMap.begin();
			soundmap != m_SoundMapMap.end();
			soundmap ++ )
		{
			for ( SoundPlayerMapManager::SoundPlayerMap::iterator playmap = soundmap->second->m_SoundPlayerMap.begin();
				playmap != soundmap->second->m_SoundPlayerMap.end();
				playmap ++ )
			{
				playmap->second->m_pSoundPlayer->Update();
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	ISoundComponent* SoundSystemSLES::LoadSound( const eastl::string& path )
	{
		uint hash = Utility::HashCode( path );//计算哈希码(伪代码)
		SoundPlayerSLES* player = NULL;
		SoundPlayerMapManager* spvm = NULL;

		SoundPlayerManagerMap::iterator findspm = m_SoundMapMap.find( hash ); //找到该资源是否被加载过
		if ( m_SoundMapMap.end() != findspm )
		{
			spvm = findspm->second;
		}
		else
		{
			spvm = NEW SoundPlayerMapManager;
			m_SoundMapMap.insert( make_pair( hash, spvm ) );
		}

		if ( spvm->m_bCreatedPlayer < SMC_MAX_REPEAT_SOUND_PLAYER ) //如果相同资源加载不到规定最大重复的个数
		{
			int index = spvm->m_bCreatedPlayer ++;
			player = NEW SoundPlayerSLES( hash, index );
			player->CreatePlayer( m_pEngine, m_pMixObject, IIOSystem::Instance()->PathAssembly( path ) );
			SharedSoundPlayer* ssp = NEW SharedSoundPlayer();
			ssp->m_pSoundPlayer = player;
			ssp->m_ReferenceCounting = 1;
			spvm->m_SoundPlayerMap.insert( eastl::make_pair( (intptr_t)player, ssp ) );
			spvm->m_bCurrentIndex = spvm->m_SoundPlayerMap.find( (intptr_t)player );
		}
		else //达到可以加载的上限
		{
			spvm->m_bCurrentIndex->second->m_ReferenceCounting++;
			player = static_cast<SoundPlayerSLES*>( spvm->m_bCurrentIndex->second->m_pSoundPlayer );
			spvm->m_bCurrentIndex++;
			spvm->m_bCurrentIndex = spvm->m_bCurrentIndex == spvm->m_SoundPlayerMap.end() ? spvm->m_SoundPlayerMap.begin() : spvm->m_bCurrentIndex;
		}
		return player;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundSystemSLES::UnloadSound( ISoundComponent* player )
	{
		SoundPlayerSLES* alPlayer = static_cast<SoundPlayerSLES*>(player);
		if ( NULL != alPlayer )
		{
			SoundPlayerManagerMap::iterator findspm = m_SoundMapMap.find( alPlayer->GetHashCode() ); //找到该资源
			if( findspm != m_SoundMapMap.end() )
			{
				SoundPlayerMapManager::SoundPlayerMap::iterator spm = findspm->second->m_SoundPlayerMap.find( (intptr_t)player );
				if ( 0 == -- spm->second->m_ReferenceCounting )
				{
					SAFE_DELETE( spm->second->m_pSoundPlayer );
					findspm->second->m_SoundPlayerMap.erase( spm );
					if( 0 == -- findspm->second->m_bCreatedPlayer )
					{
						SAFE_DELETE( findspm->second );
						m_SoundMapMap.erase( findspm );
					}
				}
			}
		}
	}
}
