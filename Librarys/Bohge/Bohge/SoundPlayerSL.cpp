#include "SoundPlayerSL.h"
#include "SoundResource.h"
#include "Log.h"
#ifdef _OPENSL

#include <SLES/OpenSLES.h>


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	SoundPlayerSL::SoundPlayerSL( int hash, int index, Decoder* res )
		:SoundPlayer( hash, index, res ),
		m_fVolume(1.0f),
		m_isPasue(false),
		m_isPlaying(false),
		m_isLoop(false),
		m_uriPlayerObject(NULL),
		m_uriPlayerPlay(NULL),
		m_uriPlayerSeek(NULL),
		m_uriPlayerVolume(NULL),
		m_uriMuteSoloItf(NULL),
		m_uriPitchItf(NULL),
		m_uriPlayRateItf(NULL)
	{	
	}
	//-------------------------------------------------------------------------------------------------------
	SoundPlayerSL::~SoundPlayerSL(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSL::_OnInitialization()
	{
		_GetSoundResource()->ReleaseResource();
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSL::CreatePlayer( SLEngineItf engine, SLObjectItf mixer )
	{
		SLresult result;
#ifdef WIN32
		std::string path = _GetSoundResource()->GetFilePath();
		std::string temp = _GetSoundResource()->GetFilePath();
		int t = temp.find_last_of(".");
		path.clear();
		path.insert(0, temp.c_str(), t);
		path += std::string(".wav");
#endif
		path.push_back(0);
		//char* cp= NEW char[path.size() + 1];
		//memcpy( cp, path.c_str(), path.size() + 1);
		//SLDataLocator_URI loc_uri = {SL_DATALOCATOR_URI, (SLchar *)cp };
		SLDataLocator_URI loc_uri = {SL_DATALOCATOR_URI, (SLchar *)path.c_str() };
		SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
		SLDataSource audioSrc = {&loc_uri, &format_mime};

		// config audio sink
		SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, mixer};
		SLDataSink audioSnk = {&loc_outmix, NULL};

		// create audio player
		const int interface_req = 5;
		const SLInterfaceID ids[interface_req] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME, SL_IID_PITCH, SL_IID_PLAYBACKRATE};
		const SLboolean req[interface_req] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
		result = (*engine)->CreateAudioPlayer(engine, &m_uriPlayerObject, &audioSrc, &audioSnk, interface_req, ids, req);
		ASSERT(SL_RESULT_SUCCESS == result);

		// realize the player
		result = (*m_uriPlayerObject)->Realize(m_uriPlayerObject, SL_BOOLEAN_FALSE);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get the play interface
		result = (*m_uriPlayerObject)->GetInterface(m_uriPlayerObject, SL_IID_PLAY, &m_uriPlayerPlay);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get the seek interface
		result = (*m_uriPlayerObject)->GetInterface(m_uriPlayerObject, SL_IID_SEEK, &m_uriPlayerSeek);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get the volume interface
		result = (*m_uriPlayerObject)->GetInterface(m_uriPlayerObject, SL_IID_VOLUME, &m_uriPlayerVolume);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get mute solo interface
		result = (*m_uriPlayerObject)->GetInterface(m_uriPlayerObject, SL_IID_MUTESOLO, &m_uriMuteSoloItf);
		if ( SL_RESULT_SUCCESS != result )
		{
			m_uriMuteSoloItf = NULL;
			DEBUGLOG( "audio mute not supported\n", 1 );
		}

		// get pitch interface
		result = (*m_uriPlayerObject)->GetInterface(m_uriPlayerObject, SL_IID_PITCH, &m_uriPitchItf);
		if ( SL_RESULT_SUCCESS != result )
		{
			m_uriPitchItf = NULL;
			DEBUGLOG( "audio pitch not supported\n", 1 );
		}

		// get play rate interfaace
		result = (*m_uriPlayerObject)->GetInterface(m_uriPlayerObject, SL_IID_PLAYBACKRATE, &m_uriPlayRateItf);
		if ( SL_RESULT_SUCCESS != result )
		{
			m_uriPlayRateItf = NULL;
			DEBUGLOG( "audio rate not supported\n", 1 );
		}
		//SAFE_DELETE_ARRAY(cp);
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSL::_DoSetVolume( float volume )
	{
		m_fVolume = volume;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSL::_DoSetPitch( float pitch )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSL::_DoSetPaused( bool ispaused )
	{
		SLresult res;
		if ( m_isPlaying )
		{
			m_isPasue = true;
			if ( ispaused )
			{
				res = (*m_uriPlayerPlay)->SetPlayState(m_uriPlayerPlay, SL_PLAYSTATE_PAUSED );
			}
			else
			{
				res = (*m_uriPlayerPlay)->SetPlayState(m_uriPlayerPlay, SL_PLAYSTATE_PLAYING );
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSL::_DoSetLoop( bool isloop )
	{
		m_isLoop = isloop;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSL::_DoSetPlay( bool isplay )
	{
		SLresult res;
		m_isPlaying = isplay;
		m_isPasue = false;
		if ( isplay )
		{
			res = (*m_uriPlayerVolume)->SetVolumeLevel(m_uriPlayerVolume, -2000 * (1.0-m_fVolume));
			res = (*m_uriPlayerSeek)->SetLoop(m_uriPlayerSeek, m_isLoop ? SL_BOOLEAN_TRUE : SL_BOOLEAN_FALSE , 0, SL_TIME_UNKNOWN);
			res = (*m_uriPlayerSeek)->SetPosition(m_uriPlayerSeek, 0, SL_SEEKMODE_FAST);
			res = (*m_uriPlayerPlay)->SetPlayState(m_uriPlayerPlay, SL_PLAYSTATE_PLAYING );
		}
		else
		{
			res = (*m_uriPlayerPlay)->SetPlayState(m_uriPlayerPlay, SL_PLAYSTATE_STOPPED );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSL::_DoSet3D( bool is3d )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSL::_DoSetSoundPosition( const vector3f& pos, const vector3f& forward, const vector3f& up )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSL::_DoUpdate()
	{
		if ( m_isPlaying && !m_isPasue )
		{
			SLuint32 state;
			(*m_uriPlayerPlay)->GetPlayState( m_uriPlayerPlay, &state );
			if ( SL_PLAYSTATE_STOPPED == state
				|| SL_PLAYSTATE_PAUSED == state )
			{
				_DoSetPlay( false );
				_OnPlayDone();
			}
		}
	}
}
#endif