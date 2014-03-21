#pragma once



namespace BohgeEngine
{
	class SoundPlayer;
	class SoundResource;
	class ISoundPlayerFactory
	{
	public:
		virtual SoundPlayer* CreatePlayer( int hash, int index, SoundResource* res ) = 0;
	};




	class SoundPlayerFactorySL : public ISoundPlayerFactory
	{
	public:
		virtual SoundPlayer* CreatePlayer( int hash, int index, SoundResource* res );
	};


	class SoundPlayerFactoryAL : public ISoundPlayerFactory
	{
	public:
		virtual SoundPlayer* CreatePlayer( int hash, int index, SoundResource* res );
	};
}