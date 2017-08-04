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
#pragma once
#include "Framework/ISoundSystem.h"
#include "Framework/IIOSystem.h"

#include <EASTL/map.h>


struct SLObjectItf_;
typedef const struct SLObjectItf_ * const * SLObjectItf;
struct SLEngineItf_;
typedef const struct SLEngineItf_ * const * SLEngineItf;
struct SLEnvironmentalReverbItf_;
typedef const struct SLEnvironmentalReverbItf_ * const * SLEnvironmentalReverbItf;

namespace OpenSLESSoundSystemPlugin
{
	class SoundSystemSLES : public BohgeEngine::ISoundSystem
	{
	private:
		enum Constant
		{
			SMC_MAX_REPEAT_SOUND_PLAYER		= 3,//最大255
		};
	private:
		struct SharedSoundPlayer
		{
			BohgeEngine::ISoundComponent*		m_pSoundPlayer;//声音资源
			uint							m_ReferenceCounting;//引用计数
		};
		struct SoundPlayerMapManager
		{
		public:
			typedef eastl::map< intptr_t, SharedSoundPlayer* >	SoundPlayerMap;
		public:
			SoundPlayerMap::iterator	m_bCurrentIndex;//当前分配到的资源索引
			byte						m_bCreatedPlayer;//已经建立的资源索引
			SoundPlayerMap				m_SoundPlayerMap;
		public:
			SoundPlayerMapManager():m_bCreatedPlayer(0){};
		};
	private:
		typedef eastl::map< uint, SoundPlayerMapManager* >	SoundPlayerManagerMap;
	private:
		SoundPlayerManagerMap			m_SoundMapMap;
	private:
		SLObjectItf						m_pEngineObject;
		SLEngineItf						m_pEngine;
		SLObjectItf						m_pMixObject;
		SLEnvironmentalReverbItf		m_pEnvironmental;
	public:
		SoundSystemSLES(void);
		~SoundSystemSLES(void);
	public:
		virtual void RegistEvent( BohgeEngine::Framework& framework ); //创建系统
	public:
		void OnFrameworkEntry( float time, BohgeEngine::Framework& framework );
		void OnFrameworkExit( float time, BohgeEngine::Framework& framework );
		void OnUpdate( float time, BohgeEngine::Framework& framework );
	public:
		virtual BohgeEngine::ISoundComponent* LoadSound( const eastl::string& path );
		virtual void UnloadSound( BohgeEngine::ISoundComponent* player );
	};
}