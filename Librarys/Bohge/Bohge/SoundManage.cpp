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



/////////////////////////////////////
//			声音播放与管理		   //
/////////////////////////////////////


#include "FilePath.h"
#include "SoundManage.h"
#include "Log.h"
#include "3DMath.h"
#include "UsualFile.h"

#if defined( OPENAL )
#include <vorbis/vorbisfile.h> 
#include <OpenAL/alc.h>
#include <OpenAL/al.h>
#include <vector>
#elif  defined( OPENSL )
#include <SLES/OpenSLES.h>
#endif

namespace BohgeEngine
{
#if defined( OPENAL )
	//-----------------------------------------------------------------------------------
	SoundManage::SoundManage(void)
		:m_isAnyMusicPlaying(false),
		m_MusicVolume(0.6),
		m_SoundVolume(0.4),
		m_hMusicKey(-1)
	{
		m_Device = alcOpenDevice(NULL);
		DEBUGLOG("Make Al sound device %d\n", (int)m_Device);
		if( NULL == m_Device )
		{
			ALenum error = alGetError ();
			if (error != AL_NO_ERROR)
			{
				DEBUGLOG( "%s\n", alGetString (error));
			}
		}
		else
		{
			m_Context = alcCreateContext(m_Device, NULL);
			ALenum error = alGetError ();
			if (error != AL_NO_ERROR)
			{
				DEBUGLOG( "%s\n", alGetString (error));
			}
			alcMakeContextCurrent(m_Context);
			error = alGetError ();
			if (error != AL_NO_ERROR)
			{
				DEBUGLOG( "%s\n", alGetString (error));
			}
		}
		//if (!alutInit (NULL, NULL))//初始化
		//{
		//	ALenum error = alutGetError ();
		//	DEBUGLOG( "%s\n", alutGetErrorString (error));
		//}
	}
	//-----------------------------------------------------------------------------------
	SoundManage::~SoundManage(void)
	{
		this->CleanWave();//清除全部音乐
		alcDestroyContext(m_Context);
		alcCloseDevice(m_Device);
		//if (!alutExit ())//释放
		//{
		//	ALenum error = alutGetError ();
		//	DEBUGLOG ("%s\n", alutGetErrorString (error));
		//}
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::LoadWAV(const std::string& fileName, SoundData& out)
	{
		//读取声音文件返回内存地址buffer
		struct WAVHead
		{
			char	riff[4];
			int		ChunkDataSize;
			char	wave[4];
			char	fmt[4];
			int		size;
			short	format;
			short	chanel;
			int		ferq;
			int		byteRate;
			short	offset;
			short	bitsPerSample;
		};
		struct WAVData
		{
			char	flag[4];
			int		size;
		};

		ReadUsualFile readfile(fileName);
		WAVHead head;
		WAVData datainfo;
		readfile.OpenFile();
		readfile.ReadFile( &head, sizeof(WAVHead) );
		if( 16 !=  head.size ) //需要跳帧
		{
			short extra = 0;
			readfile.ReadFile(&extra, sizeof(short) );
			if( 0 < extra )
			{
				byte* skip = NEW byte[extra];
				readfile.ReadFile(skip, extra );
				SAFE_DELETE_ARRAY(skip);
			}
			byte fmt;
			bool isContinue = true;
			do 
			{
				readfile.ReadFile(&fmt, sizeof(fmt) );
				if( fmt == 'd' )
				{
					readfile.ReadFile(&fmt, sizeof(fmt) );
					if( fmt == 'a' )
					{
						readfile.ReadFile(&fmt, sizeof(fmt) );
						if( fmt == 't' )
						{
							readfile.ReadFile(&fmt, sizeof(fmt) );
							if( fmt == 'a' )
							{
								isContinue = false;
							}
						}
					}
				}
			} while (isContinue);
			datainfo.flag[0] = 'd';
			datainfo.flag[1] = 'a';
			datainfo.flag[2] = 't';
			datainfo.flag[3] = 'a';
			readfile.ReadFile(&datainfo.size, sizeof(datainfo.size) );
		}
		else
		{
			readfile.ReadFile(&datainfo, sizeof(WAVData) );
			std::string dataflag;
			dataflag.resize(4);
			dataflag[0] = datainfo.flag[0];
			dataflag[1] = datainfo.flag[1];
			dataflag[2] = datainfo.flag[2];
			dataflag[3] = datainfo.flag[3];
			if ( dataflag != "data" )
			{
				ASSERT(false);//未知的文件类型
			}
		}
		byte* data = NEW byte[datainfo.size];
		readfile.ReadFile(data, datainfo.size );
		if( 8 == head.bitsPerSample )
		{
			if (head.chanel == 1)
				out.format = AL_FORMAT_MONO8;
			else
				out.format = AL_FORMAT_STEREO8;
		}
		else
		{
			if (head.chanel == 1)
				out.format = AL_FORMAT_MONO16;
			else
				out.format = AL_FORMAT_STEREO16;
		}
		out.data = data;
		out.ferq = head.ferq;
		out.size = datainfo.size;
		//float duration = float(datainfo.size) / head.byteRate;
		readfile.CloseFile();
		return true;
	}
	//-----------------------------------------------------------------------------------
	size_t ReadHelp(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		IReadFile* reader = static_cast<ReadUsualFile*>( datasource );
		reader->ReadFile( ptr, size * nmemb );
		return nmemb;
	}
	//-----------------------------------------------------------------------------------
	int SeekHelp(void *datasource, ogg_int64_t offset, int whence)
	{
		IReadFile* reader = static_cast<ReadUsualFile*>( datasource );
		return reader->Seek( offset, whence );
	}
	//-----------------------------------------------------------------------------------
	long TellHelp(void *datasource)
	{
		IReadFile* reader = static_cast<ReadUsualFile*>( datasource );
		return reader->Tell();
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::LoadOGG(const std::string& fileName, SoundData& out)
	{
#define BUFFER_SIZE     32768       // 32 KB buffers 
		vorbis_info *pInfo;  
		OggVorbis_File oggFile;  

		ReadUsualFile readfile(fileName);
		readfile.OpenFile();
		ov_callbacks callbacks;
		callbacks.read_func = &ReadHelp;
		callbacks.close_func = NULL;
		callbacks.tell_func = &TellHelp;
		callbacks.seek_func = &SeekHelp;
		//if (ov_open(readfile.BaseFile(), &oggFile, NULL, 0) != 0)
		if (ov_open_callbacks( &readfile, &oggFile, NULL, 0, callbacks ) != 0)
		{
			readfile.CloseFile();
			DEBUGLOG("Error Load sound %s", fileName.c_str());
			return false;
		}
		pInfo = ov_info(&oggFile, -1);
		out.size = ov_pcm_total(&oggFile, -1) * pInfo->channels * sizeof(ogg_int16_t) + 4096;

		if (pInfo->channels == 1)  
			out.format = AL_FORMAT_MONO16;  
		else  
			out.format = AL_FORMAT_STEREO16;  

		out.ferq = pInfo->rate;

		char Local[BUFFER_SIZE];                // Local fixed size array  
		byte* data = NEW byte[out.size];
		memset( data, 0 , out.size );
		out.data = data;
		long bytes;
		int bitStream;
		int loaded = 0;
		do  
		{   
			bytes = ov_read(&oggFile, Local, BUFFER_SIZE, 0, 2, 1, &bitStream);  
			if (bytes < 0)  
			{  
				ov_clear(&oggFile);  
				DEBUGLOG("Error decoding %s \n", fileName.c_str());
				ASSERT(false);
			}  
			if( out.size < loaded + bytes )
			{
				bytes = out.size - loaded;
			}
			memcpy( data, Local, bytes );
			loaded += bytes;
			data += bytes;
		}  
		while (bytes > 0);  

		ov_clear(&oggFile);
		readfile.CloseFile();
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::LoadSound(int Key, const std::string& fileName)
	{

		std::string fullpath = FILEPATH.isExist( fileName ) ? fileName : FILEPATH.SoundFolder() + fileName;
		SoundMap::iterator itm = m_mSounds.find( Key );
		if( itm != m_mSounds.end() )//找到了
		{
			DEBUGLOG( "sound ID %d loaded!!", Key );
			return false;
		}
		SoundData data;
		if( strstr(fullpath.c_str(), ".wav") )
		{
			this->LoadWAV(fullpath, data);
		}
		else if( strstr(fullpath.c_str(), ".ogg") )
		{
			this->LoadOGG(fullpath, data);
		}
		ALuint h = -1;
		alGenBuffers(1, &h);
		DEBUGLOG("Make Buffer %d\n", h);
#ifdef _DEBUG
		ALenum e = alGetError();
		if( AL_NO_ERROR != e )
		{
			DEBUGLOG( "Error loading sound file: '%s'\n",alGetString (e));
		}
#endif
		alBufferData(h, data.format, data.data, data.size, data.ferq);
#ifdef _DEBUG
		e = alGetError();
		if( AL_NO_ERROR != e )
		{
			ASSERT(false);
		}
#endif // _DEBUG
		//将此buffer放到map中
		m_mSounds.insert( SoundPair(Key, h) );
		//handle buffer = (File::AssetFolder::SoundFolder + fileName).c_str()) );
		//if (buffer == AL_NONE)
		//{
		//	ALenum error;
		//	error = alutGetError ();
		//	DEBUGLOG( "Error loading sound file: '%s'\n",alutGetErrorString (error));
		//	return false;
		//}
		////将此buffer放到map中
		//m_mSounds.insert( SoundPair(Key, buffer) );
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::UnloadSound(int key)
	{
		SoundMap::iterator itm = m_mSounds.find( key );
		if( itm == m_mSounds.end() )//没找到
		{
			DEBUGLOG( "sound ID %d don't loaded!!", key );
			return false;
		}
		if ( key == m_hMusicKey )
		{
			this->StopMusic();
		}
		else
		{
			for( EffectList::iterator it = m_lSoundSource.begin() ;
				it != m_lSoundSource.end() ;
				it++ )
			{
				if( key == it->first )
				{
					ALuint source = it->second;
					alSourceStop(source);
					alDeleteSources(1, &source);
					it = m_lSoundSource.erase( it );
					break;
				}
			}
		}
		handle buffer = itm->second;
		alDeleteBuffers(1, &buffer);//清理buffer
		ALenum error;
		error = alGetError ();
		if (error != AL_NO_ERROR)
		{
			DEBUGLOG( "%s\n", alGetString (error));
		}
		m_mSounds.erase( itm );
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::CleanWave()
	{
		//停止全部的sound
		for( EffectList::iterator it = m_lSoundSource.begin() ;
			it != m_lSoundSource.end() ;
			)
		{
			ALuint source = (it->second);
			alSourceStop(source);
			ALenum error;
			error = alGetError ();
			if (error != AL_NO_ERROR)
			{
				DEBUGLOG( "%s\n", alGetString (error));
			}
			alDeleteSources(1, &source);
			error = alGetError ();
			if (error != AL_NO_ERROR)
			{
				DEBUGLOG( "%s\n", alGetString (error));
			}
			it = m_lSoundSource.erase( it );
		}
		StopMusic();//停止音乐

		//删除全部的buffer
		for( SoundMap::iterator it = m_mSounds.begin() ;
			it != m_mSounds.end() ;
			)
		{
			handle buffer = it->second;
			alDeleteBuffers(1, &buffer);//清理buffer
			ALenum error = alGetError ();
			if (error != AL_NO_ERROR)
			{
				DEBUGLOG( "%s\n", alGetString (error));
			}
			SoundMap::iterator temp = it;
			it ++;
			m_mSounds.erase( temp );
		}
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::PlayMusic(int Key)
	{
		SoundMap::iterator it = m_mSounds.find( Key );
		if( it == m_mSounds.end() )//没找到
		{
			DEBUGLOG( "sound ID %d don't loaded!!", Key );
			return false;
		}
		if ( m_isAnyMusicPlaying && Key == m_hMusicKey ) //如果播放的就是当前的音乐就直接返回吧
		{
			return true;
		}
		else if( m_isAnyMusicPlaying )
		{
			this->StopMusic();
		}
		handle buffer = it->second;
		ALuint source;
		alGenSources (1, &source);
		m_hMusicSource = static_cast<handle>(source);
		m_hMusicKey = Key;
		alSourcef(source, AL_GAIN, m_MusicVolume);
		alSourcei (m_hMusicSource, AL_LOOPING, AL_TRUE);
		alSourcei (m_hMusicSource, AL_BUFFER, buffer);
		alSourcePlay (m_hMusicSource);

		//ALenum error = alGetError ();
		//if (error != AL_NO_ERROR)
		//{
		//	DEBUGLOG( "%s\n", alGetString (error));
		//	return false;
		//}
		m_isAnyMusicPlaying = true;
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::StopMusic()
	{
		if( m_isAnyMusicPlaying )
		{
			//ALenum error;
			alSourceStop(m_hMusicSource);
			//error = alGetError ();
			//if (error != AL_NO_ERROR)
			//{
			//	DEBUGLOG( "%s\n", alGetString (error));
			//	return false;
			//}
			alSourceStop(m_hMusicSource);
			//error = alGetError ();
			//if (error != AL_NO_ERROR)
			//{
			//	DEBUGLOG( "%s\n", alGetString (error));
			//	return false;
			//}
			alDeleteSources(1, &m_hMusicSource);
			//error = alGetError ();
			//if (error != AL_NO_ERROR)
			//{
			//	DEBUGLOG( "%s\n", alGetString (error));
			//	return false;
			//}
			m_hMusicSource = -1;
			m_hMusicKey = -1;
		}
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::PlaySoundEffect(int Key)
	{
		SoundMap::iterator it = m_mSounds.find( Key );
		if( it == m_mSounds.end() )//没找到
		{
			DEBUGLOG( "sound ID %d don't loaded!!", Key );
			return false;
		}
		handle buffer = it->second;
		ALuint source;
		alGenSources (1, &source);
		alSourcef(source, AL_GAIN, m_SoundVolume);
		alSourcei (source, AL_BUFFER, buffer);
		alSourcePlay (source);
		//ALenum error = alGetError ();
		//if (error != AL_NO_ERROR)
		//{
		//	DEBUGLOG( "%s\n", alGetString (error));
		//	return false;
		//}
		m_lSoundSource.push_back( std::make_pair( Key, source ) );
		return true;
	}
	//-----------------------------------------------------------------------------------
	void SoundManage::Update()
	{
		for( EffectList::iterator it = m_lSoundSource.begin() ;
			it != m_lSoundSource.end() ;
			)
		{
			ALint status;
			ALuint source = it->second; 
			alGetSourcei ( source , AL_SOURCE_STATE, &status);
			if( AL_STOPPED == status )//播放完毕,清理
			{
				ALenum error;
				alSourceStop(source);
				//error = alGetError ();
				//if (error != AL_NO_ERROR)
				//{
				//	DEBUGLOG( "%s\n", alGetString (error));
				//}
				alDeleteSources(1, &source);
				//error = alGetError ();
				//if (error != AL_NO_ERROR)
				//{
				//	DEBUGLOG( "%s\n", alGetString (error));
				//}
				//DEBUGLOG( "Delete Source %d\n", *it);
				it = m_lSoundSource.erase( it );
			}
			else
			{
				it++;
			}
		}
	}
	//-----------------------------------------------------------------------------------
	void SoundManage::SetMusicVolume( clamp v)
	{
		m_MusicVolume = Math::Clamp0to1(v);
		if( m_isAnyMusicPlaying )
		{
			alSourcef(m_hMusicSource, AL_GAIN, m_MusicVolume);
		}
	}
	//-----------------------------------------------------------------------------------
	void SoundManage::SetSoundVolume( clamp v)
	{
		m_SoundVolume = Math::Clamp0to1(v);
	}
#elif  defined( OPENSL )
	//-----------------------------------------------------------------------------------
	SoundManage::SoundManage(void)
		:m_isAnyMusicPlaying(false),
		m_MusicVolume(0.6),
		m_SoundVolume(0.4)
	{
		SLresult result;

		// create engine
		result = slCreateEngine(&m_pEngineObject, 0, NULL, 0, NULL, NULL);
		ASSERT(SL_RESULT_SUCCESS == result);

		// realize the engine
		result = (*m_pEngineObject)->Realize(m_pEngineObject, SL_BOOLEAN_FALSE);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get the engine interface
		result = (*m_pEngineObject)->GetInterface(m_pEngineObject, SL_IID_ENGINE, &m_pEngine);
		ASSERT(SL_RESULT_SUCCESS == result);

		// create output mix
		const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
		const SLboolean req[1] = {SL_BOOLEAN_FALSE};
		result = (*m_pEngine)->CreateOutputMix(m_pEngine, &m_pMixObject, 1, ids, req);
		ASSERT(SL_RESULT_SUCCESS == result);

		// realize the output mix
		result = (*m_pMixObject)->Realize(m_pMixObject, SL_BOOLEAN_FALSE);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get the environmental reverb interface
		SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
		result = (*m_pMixObject)->GetInterface(m_pMixObject, SL_IID_ENVIRONMENTALREVERB, &m_pEnvironmental);
		if (SL_RESULT_SUCCESS == result) 
		{
			result = (*m_pEnvironmental)->SetEnvironmentalReverbProperties(m_pEnvironmental, &reverbSettings);
		}
	}
	//-----------------------------------------------------------------------------------
	SoundManage::~SoundManage(void)
	{
		this->CleanWave();//清除全部音乐
		if(m_pMixObject)//清除设备
		{
			(*m_pMixObject)->Destroy(m_pMixObject);
			m_pMixObject = NULL;
		}
		if(m_pEngineObject)
		{
			(*m_pEngineObject)->Destroy(m_pEngineObject);
			m_pEngineObject = NULL;
		}
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::LoadSound(int Key, const std::string& fileName)
	{
		SoundMap::iterator it = m_mSounds.find( Key );
		if( it != m_mSounds.end() )//以读取
		{
			DEBUGLOG( "sound ID %d loaded!!", Key );
			return false;
		}
		SLresult result;
		SoundData data;
		// config audio source
		std::string path = FILEPATH.SoundFolder() + fileName;
#ifdef WIN32
		std::string temp = path;
		int t = temp.find_last_of(".");
		path.clear();
		path.insert(0, temp.c_str(), t);
		path += std::string(".wav");
#endif
		char* cp= NEW char[path.size() + 1];
		memcpy( cp, path.c_str(), path.size() + 1);
		SLDataLocator_URI loc_uri = {SL_DATALOCATOR_URI, (SLchar *)cp };
		SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
		SLDataSource audioSrc = {&loc_uri, &format_mime};

		// config audio sink
		SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, m_pMixObject};
		SLDataSink audioSnk = {&loc_outmix, NULL};

		// create audio player
		const SLInterfaceID ids[2] = {SL_IID_SEEK, SL_IID_VOLUME};
		const SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
		result = (*m_pEngine)->CreateAudioPlayer(m_pEngine, &data.m_uriPlayerObject, &audioSrc, &audioSnk, 2, ids, req);
		ASSERT(SL_RESULT_SUCCESS == result);

		// realize the player
		result = (*data.m_uriPlayerObject)->Realize(data.m_uriPlayerObject, SL_BOOLEAN_FALSE);
		if (SL_RESULT_SUCCESS != result) {
			DEBUGLOG("Can't load sound file with name %s, error code %d\n", fileName.c_str(), result);
			(*data.m_uriPlayerObject)->Destroy(data.m_uriPlayerObject);
			data.m_uriPlayerObject = NULL;
			return false;
		}

		// get the play interface
		result = (*data.m_uriPlayerObject)->GetInterface(data.m_uriPlayerObject, SL_IID_PLAY, &data.m_uriPlayerPlay);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get the seek interface
		result = (*data.m_uriPlayerObject)->GetInterface(data.m_uriPlayerObject, SL_IID_SEEK, &data.m_uriPlayerSeek);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get the volume interface
		result = (*data.m_uriPlayerObject)->GetInterface(data.m_uriPlayerObject, SL_IID_VOLUME, &data.m_uriPlayerVolume);
		ASSERT(SL_RESULT_SUCCESS == result);

		data.m_Key = Key;
		m_mSounds.insert( SoundPair(Key, data) );
		SAFE_DELETE_ARRAY(cp);
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::UnloadSound(int key)
	{
		SoundMap::iterator it = m_mSounds.find( key );
		if( it == m_mSounds.end() )//没找到
		{
			DEBUGLOG( "sound ID %d don't loaded!!", key );
			return false;
		}
		if( key == m_hMusicSource.m_Key )
		{
			this->StopMusic();
		}
		SoundData data = it->second;
		(*data.m_uriPlayerPlay)->SetPlayState(data.m_uriPlayerPlay, SL_PLAYSTATE_STOPPED );
		(*data.m_uriPlayerObject)->Destroy(data.m_uriPlayerObject);
		m_mSounds.erase( it );
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::CleanWave()
	{
		if( m_isAnyMusicPlaying )
		{//停止前一个音乐
			(*m_hMusicSource.m_uriPlayerPlay)->SetPlayState(m_hMusicSource.m_uriPlayerPlay, SL_PLAYSTATE_STOPPED );
			m_isAnyMusicPlaying = false;
		}
		for( SoundMap::iterator it = m_mSounds.begin() ;
			it != m_mSounds.end() ;
			)
		{
			SoundData data = it->second;
			(*data.m_uriPlayerPlay)->SetPlayState(data.m_uriPlayerPlay, SL_PLAYSTATE_STOPPED );
			(*data.m_uriPlayerObject)->Destroy(data.m_uriPlayerObject);
			SoundMap::iterator temp = it;
			it++;
			m_mSounds.erase( temp );
		}
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::PlayMusic(int Key)
	{
		SoundMap::iterator it = m_mSounds.find( Key );
		if( it == m_mSounds.end() )//没找到
		{
			DEBUGLOG( "sound ID %d don't loaded!!", Key );
			return false;
		}
		if( m_isAnyMusicPlaying && Key == m_hMusicSource.m_Key ) //播放的就是当前音乐
		{
			return true;
		}
		else if( m_isAnyMusicPlaying )
		{//停止前一个音乐
			(*m_hMusicSource.m_uriPlayerPlay)->SetPlayState(m_hMusicSource.m_uriPlayerPlay, SL_PLAYSTATE_STOPPED );
		}
		m_isAnyMusicPlaying = true;
		SoundData data = it->second;
		m_hMusicSource = data;
		(*data.m_uriPlayerVolume)->SetVolumeLevel(data.m_uriPlayerVolume, GetEngineMusicVolume());
		(*data.m_uriPlayerSeek)->SetLoop(data.m_uriPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
		(*data.m_uriPlayerSeek)->SetPosition(data.m_uriPlayerSeek, 0, SL_SEEKMODE_FAST);
		(*data.m_uriPlayerPlay)->SetPlayState(data.m_uriPlayerPlay, SL_PLAYSTATE_PLAYING );

		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::StopMusic()
	{
		m_isAnyMusicPlaying = false;
		(*m_hMusicSource.m_uriPlayerPlay)->SetPlayState(m_hMusicSource.m_uriPlayerPlay, SL_PLAYSTATE_STOPPED );
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::PlaySoundEffect(int Key)
	{
		SoundMap::iterator it = m_mSounds.find( Key );
		if( it == m_mSounds.end() )//没找到
		{
			DEBUGLOG( "sound ID %d don't loaded!!", Key );
			return false;
		}
		SoundData data = it->second;
		SLuint32 state;
		(*data.m_uriPlayerPlay)->GetPlayState(data.m_uriPlayerPlay, &state);
		SLmillibel level;
		(*data.m_uriPlayerVolume)->GetVolumeLevel(data.m_uriPlayerVolume, &level);
		if( GetEngineSoundVolume() != level )
		{
			(*data.m_uriPlayerVolume)->SetVolumeLevel(data.m_uriPlayerVolume, GetEngineSoundVolume());
		}
		if( state == SL_PLAYSTATE_STOPPED )
		{
			//m_PlayingEffect.push_back(data);
			(*data.m_uriPlayerSeek)->SetLoop(data.m_uriPlayerSeek, SL_BOOLEAN_FALSE, 0, SL_TIME_UNKNOWN);
		}
		(*data.m_uriPlayerPlay)->SetPlayState(data.m_uriPlayerPlay, SL_PLAYSTATE_STOPPED );
		(*data.m_uriPlayerSeek)->SetPosition(data.m_uriPlayerSeek, 0, SL_SEEKMODE_FAST);
		(*data.m_uriPlayerPlay)->SetPlayState(data.m_uriPlayerPlay, SL_PLAYSTATE_PLAYING );
		return true;
	}
	//-----------------------------------------------------------------------------------
	void SoundManage::Update()
	{
		//for( EffectList::iterator it = m_PlayingEffect.begin() ;
		//	it != m_PlayingEffect.end() ;
		//	)
		//{
		//	SLuint32 state;
		//	(*(it->m_uriPlayerPlay))->GetPlayState(it->m_uriPlayerPlay, &state);
		//	if( state == SL_PLAYSTATE_PAUSED )
		//	{
		//		(*(it->m_uriPlayerPlay))->SetPlayState(it->m_uriPlayerPlay, SL_PLAYSTATE_STOPPED );
		//		EffectList::iterator temp = it;
		//		it++;
		//		m_PlayingEffect.erase( temp );
		//	}
		//	else
		//	{
		//		it++;
		//	}
		//}
	}
	//-----------------------------------------------------------------------------------
	void SoundManage::SetMusicVolume( clamp v)
	{
		m_MusicVolume = Math::Clamp0to1(v);
		if( m_isAnyMusicPlaying )
		{
			(*m_hMusicSource.m_uriPlayerVolume)->SetVolumeLevel(m_hMusicSource.m_uriPlayerVolume, GetEngineMusicVolume());
		}
	}
	//-----------------------------------------------------------------------------------
	void SoundManage::SetSoundVolume( clamp v)
	{
		m_SoundVolume = Math::Clamp0to1(v);
	}
#else
	//-----------------------------------------------------------------------------------
	SoundManage::SoundManage( void )
	{

	}
	//-----------------------------------------------------------------------------------
	SoundManage::~SoundManage( void )
	{

	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::LoadSound( int Key, const std::string& fileName )
	{
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::UnloadSound( int key )
	{
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::PlayMusic(int Key)
	{
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::StopMusic()
	{
		return true;
	}
	//-----------------------------------------------------------------------------------
	bool SoundManage::PlaySoundEffect(int Key)
	{
		return true;
	}
	//-----------------------------------------------------------------------------------
	void SoundManage::Update()
	{

	}
	//-----------------------------------------------------------------------------------
	void SoundManage::SetMusicVolume( clamp v)
	{

	}
	//-----------------------------------------------------------------------------------
	void SoundManage::SetSoundVolume( clamp v)
	{

	}
#endif
}