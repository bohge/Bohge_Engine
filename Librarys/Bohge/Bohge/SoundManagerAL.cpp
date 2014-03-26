#include "SoundManagerAL.h"
#include "SoundPlayerAL.h"
#include "Log.h"

#ifdef _OPENAL
#include <OpenAL/alc.h>
#include <OpenAL/al.h>

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	SoundManagerAL::SoundManagerAL(void)
		:m_pDevice(NULL),
		m_pContext(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundManagerAL::~SoundManagerAL(void)
	{
	}
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
}
#endif // __OPENAL