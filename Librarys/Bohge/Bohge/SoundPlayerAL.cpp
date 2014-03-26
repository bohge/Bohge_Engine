#include "SoundPlayerAL.h"
#include "SoundResource.h"
#include "Log.h"


#ifdef _OPENAL
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


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	SoundPlayerAL::SoundPlayerAL( int hash, int index, Decoder* res )
		:SoundPlayer( hash, index, res ),
		m_isPlaying(false),
		m_isLooping(false),
		m_SourceHandle(0)
	{
		alGenBuffers( SC_DATA_QUEUE_SIZE, m_DataQueueHandles );
		CHECKERROR;
		for ( int i = 0 ; i < SC_DATA_QUEUE_SIZE ; i ++ )
		{
			m_DataQueue.push( m_DataQueueHandles[i] ); 
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
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundPlayerAL::_OnInitialization()
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
			if ( !m_DataQueue.empty() )//如果没有初始化完成
			{
				_GetSoundResource()->FlushBufferData();
				uint datahandle = m_DataQueue.front();
				m_DataQueue.pop();
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
						m_DataQueue.push( buffer );
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
	void SoundPlayerAL::_DoSetSoundPosition( const vector3f& pos, const vector3f& forward, const vector3f& up )
	{

	}
}
#endif