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
#pragma once

#include "Predefine.h"
#include "VariableType.h"


#include <map>
#include <list>
#include <string>

#ifdef WIN32
//#define OPENSL
#define OPENAL
#endif

#ifdef ANDROID
#define OPENSL
#endif

#ifdef IOS
#define OPENAL
#endif






#ifdef OPENSL //opensl实现
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
#endif

#ifdef OPENAL
typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;
#endif


namespace BohgeEngine
{
	class SoundManage
	{
	private:
		clamp		m_MusicVolume;
		clamp		m_SoundVolume;
#ifdef OPENAL	//openal 实现
	private:
		struct SoundData
		{
			int		format;
			byte*	data;
			int		size;
			int		ferq;
			~SoundData()
			{
				SAFE_DELETE_ARRAY(data);
			}
		};
	private:
		typedef std::pair<int, handle> SoundPair;
		typedef std::map<int,handle> SoundMap;//第一个是int是声音枚举的索引，handle是声音句柄
		typedef std::list<std::pair<int, handle> > EffectList;//保存要播放的音效
	private:
		ALCdevice*		m_Device;//设备
		ALCcontext*		m_Context;//上下文
		SoundMap		m_mSounds;	//已经读入的音乐的buffer表
		bool			m_isAnyMusicPlaying;
		int				m_hMusicKey;//音乐播key
		handle			m_hMusicSource;//音乐播放源
		EffectList		m_lSoundSource;//同时可以存在多个音效
	private:
		bool LoadWAV(const std::string& fileName, SoundData& out);
		bool LoadOGG(const std::string& fileName, SoundData& out);
#endif

#ifdef OPENSL //opensl实现
	private:
		struct SoundData //保存可播放的音乐结构体
		{
			int				m_Key;
			SLObjectItf		m_uriPlayerObject;
			SLPlayItf		m_uriPlayerPlay;
			SLSeekItf		m_uriPlayerSeek;
			SLVolumeItf		m_uriPlayerVolume;
		};
		typedef std::pair<int, SoundData> SoundPair;
		typedef std::map<int, SoundData> SoundMap;//全部可以播放的音乐列表，第一个是声音的枚举索引
		//typedef std::list<SoundData> EffectList;//正在播放的soundeffect
	private:
		bool							m_isAnyMusicPlaying;
		SLObjectItf						m_pEngineObject;
		SLEngineItf						m_pEngine;
		SLObjectItf						m_pMixObject;
		SLEnvironmentalReverbItf		m_pEnvironmental;

		SoundMap						m_mSounds;	//已经读入的音乐的buffer表
		SoundData						m_hMusicSource;//音乐播放源
		//EffectList						m_PlayingEffect;
		BOHGE_FORCEINLINE int GetEngineSoundVolume()
		{
			return  -2000 * (1.0-m_SoundVolume);
		}
		BOHGE_FORCEINLINE int GetEngineMusicVolume()
		{
			return  -2000 * (1.0-m_MusicVolume);
		}
#endif
	private:
		/*释放声音资源*/
		bool CleanWave();
	public:
		SoundManage(void);
		~SoundManage(void);
		/*读取一个wav声音到内存中*/
		bool LoadSound(int Key, const std::string& fileName);
		bool UnloadSound(int key);
		/*反复的播放一个背景音乐*/
		bool PlayMusic(int Key);
		bool StopMusic();
		/*只播放一次的*/
		bool PlaySoundEffect(int Key);
		/*检测播放状态将播放完毕的删除*/
		void Update();
		void SetMusicVolume( clamp v);
		void SetSoundVolume( clamp v);
	public:
		BOHGE_FORCEINLINE clamp GetMusicVolume()
		{
			return m_MusicVolume;
		}
		BOHGE_FORCEINLINE clamp GetSoundVolume()
		{
			return m_SoundVolume;
		}
	};
}