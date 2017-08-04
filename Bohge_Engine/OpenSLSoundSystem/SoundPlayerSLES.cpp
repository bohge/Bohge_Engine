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
#include "Framework/Log.h"

#include "SoundPlayerSLES.h"



#include <SLES/OpenSLES.h>

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	SoundPlayerSLES::SoundPlayerSLES( int hash, int index )
		:m_nHashCode(hash),
		m_nIndex(index),
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
	SoundPlayerSLES::~SoundPlayerSLES(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSLES::CreatePlayer( SLEngineItf engine, SLObjectItf mixer, const eastl::string& path )
	{
		SLresult result;
		eastl::string filepath = path;
//#ifdef WIN32
//		eastl::string temp = path;
//		int t = temp.find_last_of(".");
//		filepath.clear();
//		filepath.insert(0, temp.c_str(), t);
//		filepath += eastl::string(".wav");
//#endif
		filepath.push_back(0);
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
	void SoundPlayerSLES::_DoSetVolume( float volume )
	{
		m_fVolume = volume;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSLES::_DoSetPitch( float pitch )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSLES::_DoSetPaused( bool ispaused )
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
	void SoundPlayerSLES::_DoSetLoop( bool isloop )
	{
		m_isLoop = isloop;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSLES::_DoSetPlay( bool isplay )
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
	void SoundPlayerSLES::_DoSet3D( bool is3d )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSLES::_DoSetSoundPosition( const BohgeEngine::vector3f& pos )
	{

	}

	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSLES::_DoUpdate()
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
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSLES::_DoSetSoundVelocity( const BohgeEngine::vector3f& vel )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerSLES::_DoSetSoundOrientation( const BohgeEngine::vector3f& vel )
	{

	}

}