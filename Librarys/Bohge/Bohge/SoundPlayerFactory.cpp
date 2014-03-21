#include "SoundPlayerFactory.h"
#include "SoundPlayerAL.h"
#include "SoundPlayerSL.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	SoundPlayer* SoundPlayerFactorySL::CreatePlayer( int hash, int index, SoundResource* res )
	{
		return NEW SoundPlayerSL( hash, index, res );
	}
	//-------------------------------------------------------------------------------------------------------
	SoundPlayer* SoundPlayerFactoryAL::CreatePlayer( int hash, int index, SoundResource* res )
	{
		return NEW SoundPlayerAL( hash, index, res );
	}
}