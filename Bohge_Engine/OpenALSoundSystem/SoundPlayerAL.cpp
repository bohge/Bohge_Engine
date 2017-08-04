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


#include "SoundPlayerAL.h"
#include "SoundResource.h"
#include "Framework/Log.h"


#include <OpenAL/alc.h>
#include <OpenAL/al.h>


#ifdef _DEBUG
#define  CHECKERROR ChechError()
#else
#define  CHECKERROR
#endif
void ChechError()
{
	ALenum error;
	error = alGetError ();
	if (error != AL_NO_ERROR)
	{
		DEBUGLOG( "%s\n", alGetString (error));
	}
}

using namespace BohgeEngine;

namespace OpenALSoundSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	SoundPlayerAL::SoundPlayerAL( SmartPtr<Decoder>& res )
		:m_pResource( NEW SoundResource( res ) ),
		m_isPlaying(false),
		m_isLooping(false),
		m_SourceHandle(0)
	{
		alGenBuffers( SC_DATA_QUEUE_SIZE, m_DataQueueHandles );
		CHECKERROR;
		for ( int i = 0 ; i < SC_DATA_QUEUE_SIZE ; i ++ )
		{
			m_DataList.push_back( m_DataQueueHandles[i] ); 
		}
		alGenSources( 1, &m_SourceHandle );
		CHECKERROR;
		//alSourcei ( m_SourceHandle, AL_BUFFER, m_PlayerHandle );
		//CHECKERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	SoundPlayerAL::~SoundPlayerAL(void)
	{
		alSourceStop( m_SourceHandle );
		alDeleteSources( 1, &m_SourceHandle );
		CHECKERROR;
		alDeleteBuffers( SC_DATA_QUEUE_SIZE, m_DataQueueHandles );//清理buffer
		CHECKERROR;
		SAFE_DELETE( m_pResource );
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::Initialize()
	{
		_GetSoundResource()->RequestDecode();//要求解压数据
	}
	//-------------------------------------------------------------------------------------------------------
	int SoundPlayerAL::_GetALFormat()
	{
		Decoder::Format fm = _GetSoundResource()->GetFormat();
		ALenum al_fm;
		switch(fm)
		{
		case Decoder::DF_MONO_8: al_fm = AL_FORMAT_MONO8;break;
		case Decoder::DF_STEREO_8: al_fm = AL_FORMAT_STEREO8;break;
		case Decoder::DF_MONO_16: al_fm = AL_FORMAT_MONO16;break;
		case Decoder::DF_STEREO_16: al_fm = AL_FORMAT_STEREO16;break;
		default: ASSERT(false);
		}
		return al_fm;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::_DoUpdate()
	{
		//更新
		ALint status;
		ALuint buffer;
		ALint ProcessedBuffer;
		if ( _GetSoundResource()->isChunkReady() )//有空闲数据的时候
		{
			if ( !m_DataList.empty() )//如果没有初始化完成
			{
				_GetSoundResource()->FlushBufferData();
				uint datahandle = *m_DataList.begin();
				m_DataList.pop_front();
				alBufferData(datahandle,
					_GetALFormat(),
					_GetSoundResource()->GetBufferChunk(),
					_GetSoundResource()->GetBufferSize(),
					_GetSoundResource()->GetFrequency() );
				CHECKERROR;
				alSourceQueueBuffers(m_SourceHandle, 1, &datahandle);
				CHECKERROR;
			}
			else
			{
				/* Check if OpenAL is done with any of the queued buffers */
				alGetSourcei(m_SourceHandle, AL_BUFFERS_PROCESSED, &ProcessedBuffer);
                if(ProcessedBuffer > 0)
                {
                    /* Pop the oldest queued buffer from the source, fill it
                     * with the new data, then requeue it */
                    alSourceUnqueueBuffers(m_SourceHandle, 1, &buffer);
					_GetSoundResource()->FlushBufferData();
					alBufferData(buffer,
						_GetALFormat(),
						_GetSoundResource()->GetBufferChunk(),
						_GetSoundResource()->GetBufferSize(),
						_GetSoundResource()->GetFrequency() );
					CHECKERROR;
                    alSourceQueueBuffers(m_SourceHandle, 1, &buffer);
                    CHECKERROR;
					for( int i = ProcessedBuffer - 1; i > 0; i-- )
					{
						alSourceUnqueueBuffers(m_SourceHandle, 1, &buffer);
						m_DataList.push_back( buffer );
						CHECKERROR;
					}
					alGetSourcei(m_SourceHandle, AL_SOURCE_STATE, &status);
					if( AL_STOPPED == status )
					{
						m_isPlaying = false;
					}
                }
			}
			if (!m_isPlaying)
			{
				m_isPlaying = true;
				alSourcePlay (m_SourceHandle);
				CHECKERROR;
			}
			if ( _GetSoundResource()->isDone() && !m_isLooping )
			{
				_OnPlayDone();
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::_DoSetVolume( float volume )
	{
		alSourcef (m_SourceHandle, AL_GAIN, volume );
		CHECKERROR;;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::_DoSetPitch( float pitch )
	{
		alSourcei (m_SourceHandle, AL_PITCH, pitch );
		CHECKERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::_DoSetPaused( bool ispaused )
	{
		if ( ispaused )
		{
			alSourcePause( m_SourceHandle );
		}
		else
		{
			m_isPlaying = false;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::_DoSetLoop( bool isloop )
	{
		m_isLooping = isloop;
		//alSourcei (m_SourceHandle, AL_LOOPING, isloop ? AL_TRUE : AL_FALSE );
		//CHECKERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::_DoSetPlay( bool isplay )
	{
		if ( isplay )
		{
			m_isPlaying = false;
		}
		else
		{
			alSourceStop( m_SourceHandle );
		}
		CHECKERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::_DoSet3D( bool is3d )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::_DoSetSoundPosition( const vector3f& pos )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::_DoSetSoundVelocity( const vector3f& vel )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::_DoSetSoundOrientation( const vector3f& vel )
	{

	}

}