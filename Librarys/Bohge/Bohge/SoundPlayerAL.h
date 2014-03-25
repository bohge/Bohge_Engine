#pragma once
#include "SoundPlayer.h"
#include <queue>


namespace BohgeEngine
{
	class SoundPlayerAL : public SoundPlayer
	{
		friend class SoundManagerAL;
	private:
		enum Constant
		{
			SC_DATA_QUEUE_SIZE = 2,
		};
	private:
		typedef std::queue<uint>	BufferDataQueue;
	private:
		BufferDataQueue			m_DataQueue;//“Ù∆µplayer
		bool					m_isPlaying;
		uint					m_DataQueueHandles[SC_DATA_QUEUE_SIZE];//“Ù∆µplayer
		uint					m_SourceHandle;//≤•∑≈µƒ“Ù‘¥æ‰±˙
	private:
		SoundPlayerAL( int hash, int index, Decoder* res );
		virtual ~SoundPlayerAL(void);
	private:
		virtual void _DoSetVolume( float volume );
		virtual void _DoSetPitch(float pitch);
		virtual void _DoSetPaused( bool ispaused );
		virtual void _DoSetLoop( bool isloop );
		virtual void _DoSetPlay( bool isplay );
		virtual void _DoSet3D( bool is3d );
		virtual void _DoSetSoundPosition( const vector3f& pos, const vector3f& forward, const vector3f& up );
		virtual void _DoUpdate();
	};
}
