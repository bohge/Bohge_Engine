#pragma once
#include "VariableType.h"
#include "Predefine.h"



#include <map>
#include <string>

namespace BohgeEngine
{
	class SoundPlayer;
	class ISoundPlayerFactory;
	class SoundManager
	{
	public:
		enum ServerType
		{
			ST_OPENAL	= 0,
			ST_OPENSL	= 1,
			ST_FMOD		= 2,
		};
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
			SoundPlayerMapManager():m_bCurrentIndex(m_SoundPlayerMap.begin()),m_bCreatedPlayer(0){};
		};
	private:
		typedef std::map< uint, SoundPlayerMapManager* >	SoundPlayerManagerMap;
	protected:
		static SoundManager*		m_pInstance;
	private:
		SoundPlayerManagerMap	m_SoundMapMap;
		ISoundPlayerFactory*	m_SoundFactory;
		float					m_fGlobalVolume;
		float					m_fGlobalPitch;
		bool					m_isGlobalMute;
		uint					m_nPlayerIndex;
	protected:
		SoundManager(void);
		virtual ~SoundManager(void);
	public:
		static BOHGE_FORCEINLINE SoundManager* Instance() { return m_pInstance; };
		static void Create( ServerType st );
		static void Destroy();
	private:
		virtual void _OnCreate( ) = 0;
		virtual void _OnDestroy( ) = 0;
	public:
		SoundPlayer* LoadSound( const std::string& path );
		void ReleaseSound( SoundPlayer* sound );
		void Update();
	public:
		BOHGE_FORCEINLINE void SetGlobalVolume( float volume )
		{
			m_fGlobalVolume = volume;//0-1
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