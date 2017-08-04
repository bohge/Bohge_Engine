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


#include "SoundSystemAL.h"
#include "SoundPlayerAL.h"
#include "DecoderManager.h"



#include <OpenAL/alc.h>
#include <OpenAL/al.h>

using namespace eastl;
using namespace BohgeEngine;

namespace OpenALSoundSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	SoundSystemAL::SoundSystemAL(void)
		:m_pDevice(NULL),
		m_pContext(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundSystemAL::~SoundSystemAL(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundSystemAL::RegistEvent( BohgeEngine::Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_ENTRY, MEMBER_FUNC_PTR( &SoundSystemAL::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_ON_EXIT, MEMBER_FUNC_PTR( &SoundSystemAL::OnFrameworkExit ) );
		framework.RegisterEventListener( FrameworkEvent::ET_LOGIC_DO_UPDATE, MEMBER_FUNC_PTR( &SoundSystemAL::OnUpdate ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundSystemAL::OnFrameworkEntry( float time, BohgeEngine::Framework& framework )
	{
		DecoderManager::Create( framework );

		m_pDevice = alcOpenDevice(NULL);
		DEBUGLOG("Make Al sound device %d\n", (int)m_pDevice);
		if( NULL == m_pDevice )
		{
			ALenum error = alGetError ();
			if (error != AL_NO_ERROR)
			{
				DEBUGLOG( "%s\n", alGetString (error));
			}
		}
		else
		{
			m_pContext = alcCreateContext(m_pDevice, NULL);
			ALenum error = alGetError ();
			if (error != AL_NO_ERROR)
			{
				DEBUGLOG( "%s\n", alGetString (error));
			}
			alcMakeContextCurrent(m_pContext);
			error = alGetError ();
			if (error != AL_NO_ERROR)
			{
				DEBUGLOG( "%s\n", alGetString (error));
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundSystemAL::OnFrameworkExit( float time, BohgeEngine::Framework& framework )
	{
		alcDestroyContext(m_pContext);
		alcCloseDevice(m_pDevice);

		DecoderManager::Destroy( framework );
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundSystemAL::OnUpdate( float time, BohgeEngine::Framework& framework )
	{
		DecoderManager::Instance()->Update();
		FOR_EACH( SoundPlayerMap, it, m_SoundPlayerMap )
		{
			it->second->Update();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	ISoundComponent* SoundSystemAL::LoadSound( const eastl::string& path )
	{
		SmartPtr<Decoder> decoder = DecoderManager::Instance()->LoadSoundDecoder( path );
		SoundPlayerAL* player = NEW SoundPlayerAL( decoder );
		player->Initialize();
		m_SoundPlayerMap.insert( make_pair( (intptr_t)player, player ) );
		return player;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundSystemAL::UnloadSound( ISoundComponent* player )
	{
		m_SoundPlayerMap.erase( (intptr_t)player );
		SAFE_DELETE( player );
	}
}