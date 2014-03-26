#pragma once
#include "SoundPlayer.h"

#ifdef _OPENSL


struct SLObjectItf_;
typedef const struct SLObjectItf_ * const * SLObjectItf;
struct SLEngineItf_;
typedef const struct SLEngineItf_ * const * SLEngineItf;
struct SLPlayItf_;
typedef const struct SLPlayItf_ * const * SLPlayItf;
struct SLSeekItf_;
typedef const struct SLSeekItf_ * const * SLSeekItf;
struct SLEnvironmentalReverbItf_;
typedef const struct SLEnvironmentalReverbItf_ * const * SLEnvironmentalReverbItf;
struct SLVolumeItf_;
typedef const struct SLVolumeItf_ * const * SLVolumeItf;
struct SLPlaybackRateItf_;
typedef const struct SLPlaybackRateItf_ * const * SLPlaybackRateItf;
struct SLPitchItf_;
typedef const struct SLPitchItf_ * const * SLPitchItf;
struct SLMuteSoloItf_;
typedef const struct SLMuteSoloItf_ * const * SLMuteSoloItf;


namespace BohgeEngine
{
	class SoundPlayerSL : public SoundPlayer
	{
		friend class SoundManagerSL;
	private:
		bool				m_isPasue;
		bool				m_isPlaying;
		bool				m_isLoop;
		float				m_fVolume;
		SLObjectItf			m_uriPlayerObject;
		SLPlayItf			m_uriPlayerPlay;
		SLSeekItf			m_uriPlayerSeek;
		SLVolumeItf			m_uriPlayerVolume;
		SLMuteSoloItf		m_uriMuteSoloItf;
		SLPitchItf			m_uriPitchItf;
		SLPlaybackRateItf   m_uriPlayRateItf;
	private:
		SoundPlayerSL( int hash, int index, Decoder* res );
		virtual ~SoundPlayerSL(void);
	private:
		virtual void _OnInitialization();
		virtual void _DoSetVolume( float volume );
		virtual void _DoSetPitch(float pitch);
		virtual void _DoSetPaused( bool ispaused );
		virtual void _DoSetLoop( bool isloop );
		virtual void _DoSetPlay( bool isplay );
		virtual void _DoSet3D( bool is3d );
		virtual void _DoSetSoundPosition( const vector3f& pos, const vector3f& forward, const vector3f& up );
		virtual void _DoUpdate();
	public:
		void CreatePlayer( SLEngineItf engine, SLObjectItf mixer );
	};
}

#endif