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
#include "3DMath.h"
#include "SmartPtr.hpp"


#include <map>
#include <string>



#ifdef WIN32
//#define _OPENSL
#define _OPENAL
#endif

#ifdef ANDROID
#define _OPENSL
#endif

#ifdef IOS
#define _OPENAL
#endif


namespace BohgeEngine
{
	class Decoder;
	class SoundPlayer;
	class SoundResource;
	class SoundManager
	{
	private:
		enum Constant
		{
			SMC_MAX_REPEAT_SOUND_PLAYER		= 3,//最大255
		};
	private:
		struct SharedSoundPlayer
		{
			SoundPlayer*		m_pSoundPlayer;//声音资源
			uint				m_ReferenceCounting;//引用计数
		};
		struct SoundPlayerMapManager
		{
		public:
			typedef std::map< uint, SharedSoundPlayer* >	SoundPlayerMap;
		public:
			SoundPlayerMap::iterator	m_bCurrentIndex;//当前分配到的资源索引
			byte						m_bCreatedPlayer;//已经建立的资源索引
			SoundPlayerMap				m_SoundPlayerMap;
		public:
			SoundPlayerMapManager():m_bCreatedPlayer(0){};
		};
	private:
		typedef std::map< uint, SoundPlayerMapManager* >	SoundPlayerManagerMap;
	protected:
		static SoundManager*		m_pInstance;
	private:
		SoundPlayerManagerMap	m_SoundMapMap;
		float					m_fGlobalVolume;
		float					m_fGlobalPitch;
		bool					m_isGlobalMute;
		uint					m_nPlayerIndex;
	protected:
		SoundManager(void);
		virtual ~SoundManager(void);
	public:
		static SoundManager* Create();
		static void Destroy();
	private:
		virtual void _OnCreate( ) = 0;
		virtual void _OnDestroy( ) = 0;
	public:
		virtual SoundPlayer* CreatePlayer( int hash, int index, SmartPtr<Decoder>& res ) = 0;
	public:
		SoundPlayer* LoadSound( const std::string& path );
		void ReleaseSound( SoundPlayer** sound );
		void Update();
	public:
		BOHGE_FORCEINLINE void SetGlobalVolume( float volume )
		{
			m_fGlobalVolume = Math::Clamp0to1( volume );//0-1
		}
		BOHGE_FORCEINLINE float GetGlobalVolume() const
		{
			return m_fGlobalVolume;
		}
		BOHGE_FORCEINLINE void SetGlobalPitch( float pitch )
		{
			m_fGlobalPitch = pitch;//0-1
		}
		BOHGE_FORCEINLINE float GetGlobalPitch() const
		{
			return m_fGlobalPitch;
		}
		BOHGE_FORCEINLINE bool SetGLobalMute( bool mute )
		{
			m_isGlobalMute = mute;
		}
		BOHGE_FORCEINLINE bool isGlobalMute() const
		{
			return m_isGlobalMute;
		}
	};
}