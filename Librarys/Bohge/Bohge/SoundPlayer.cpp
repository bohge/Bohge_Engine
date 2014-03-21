#include "SoundPlayer.h"

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	SoundPlayer::SoundPlayer( uint hash, int index, SoundResource* res )
		:m_pResource( res ),
		m_nHashCode( hash ),
		m_nIndex( index ),
		m_fVolume(1.0),
		m_fPitch(1.0),
		m_nFlag(0)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundPlayer::~SoundPlayer(void)
	{
	}
}