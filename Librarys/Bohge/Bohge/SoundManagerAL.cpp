#include "SoundManagerAL.h"
#include "SoundPlayerAL.h"
#include "Log.h"

#ifdef _OPENAL
#include <OpenAL/alc.h>
#include <OpenAL/al.h>
#endif

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	SoundManagerAL::SoundManagerAL(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundManagerAL::~SoundManagerAL(void)
	{
	}
#ifdef _OPENAL
	//-------------------------------------------------------------------------------------------------------
	void SoundManagerAL::_OnCreate()
	{
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
	void SoundManagerAL::_OnDestroy()
	{
		alcDestroyContext(m_pContext);
		alcCloseDevice(m_pDevice);
	}
	//-------------------------------------------------------------------------------------------------------
	SoundPlayer* SoundManagerAL::CreatePlayer( int hash, int index, Decoder* res )
	{
		return NEW SoundPlayerAL( hash, index, res );
	}
#else
	//-------------------------------------------------------------------------------------------------------
	void SoundManagerAL::_OnCreate()
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundManagerAL::_OnDestroy()
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundPlayer* SoundManagerAL::CreatePlayer( int hash, int index, Decoder* res )
	{
		return NULL;
	}
#endif // __OPENAL
}