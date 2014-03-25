#include "SoundPlayer.h"
#include "SoundResource.h"

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	SoundPlayer::SoundPlayer( uint hash, int index, Decoder* res )
		:m_pResource( NULL ),
		m_nHashCode( hash ),
		m_nIndex( index ),
		m_fVolume(1.0),
		m_fPitch(1.0),
		m_nFlag(0)
	{
		m_pResource = NEW SoundResource( res );
	}
	//-------------------------------------------------------------------------------------------------------
	SoundPlayer::~SoundPlayer(void)
	{
		SAFE_DELETE( m_pResource );
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayer::Initialization()
	{
		m_pResource->DecoderNextChunk();
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayer::Update()
	{
		if ( _GetFlag(SSF_PLAYING) && !_GetFlag(SSF_PAUSED) )
		{
			_DoUpdate();
		}
	}

}