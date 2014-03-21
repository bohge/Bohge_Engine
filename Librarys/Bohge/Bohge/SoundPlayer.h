#pragma once
#include "3DMath.h"
#include "SoundManager.h"



namespace BohgeEngine
{
	class SoundResource;
	class SoundPlayer
	{

		friend class SoundManager;
	private:
		enum SoundSourceFlag
		{
			SSF_PAUSED			=	1UL << 0,
			SSF_LOOP			=	1UL << 1,
			SSF_PLAYING			=	1UL << 2,
			SSF_IS3D			=	1UL << 3,
		};
	private:
		int				m_nIndex;
		uint			m_nHashCode;
		float			m_fVolume;
		float			m_fPitch;
		int				m_nFlag;
		SoundResource*	m_pResource;
	protected:
		SoundPlayer( uint hash, int index, SoundResource* res );
		virtual ~SoundPlayer(void);
	private:
		virtual void _DoSetVolume( float volume ) = 0;
		virtual void _DoSetPitch(float pitch) = 0;
		virtual void _DoSetPaused( bool ispaused ) = 0;
		virtual void _DoSetLoop( bool isloop ) = 0;
		virtual void _DoSetPlay( bool isplay ) = 0;
		virtual void _DoSet3D( bool is3d ) = 0;
		virtual void _DoSetSoundPosition( const vector3f& pos, const vector3f& forward, const vector3f& up ) = 0;
	public:
		virtual void Update() = 0;
	private:
		inline void _SetFlag( bool isflag, SoundSourceFlag flag )
		{
			isflag ? m_nFlag |= flag : m_nFlag &= ~flag;
		}
		inline bool _GetFlag( SoundSourceFlag flag ) const
		{
			return 0 != ( m_nFlag & flag );
		}
	public:
		inline int GetIndex() const
		{
			return m_nIndex;
		}
		inline uint GetHashCode() const
		{
			return m_nHashCode;
		}
		inline void Release( )
		{
			SoundManager::Instance()->ReleaseSound( this );
		}
		inline void SetVolume( float volume )
		{
			m_fVolume = volume;
			_DoSetVolume( volume * SoundManager::Instance()->GetGlobalVolume() );
		}
		inline float GetVolume() const
		{
			return m_fVolume;
		}
		inline void SetPitch( float pitch )
		{
			m_fPitch = pitch;
			_DoSetPitch( pitch * SoundManager::Instance()->GetGlobalPitch() );
		}
		inline float GetPitch() const
		{
			return m_fPitch;
		}
		inline void SetPaused( bool ispaused )
		{
			_SetFlag( ispaused, SSF_PAUSED );
			_DoSetPaused( ispaused );
		}
		inline bool isPaused( ) const
		{
			return _GetFlag( SSF_PAUSED );
		}
		inline void SetLoop( bool isloop )
		{
			_SetFlag( isloop, SSF_LOOP );
			_DoSetLoop( isloop );
		}
		inline bool isLoop( ) const
		{
			return _GetFlag( SSF_LOOP );
		}
		inline void Paly( )
		{
			_SetFlag( true, SSF_PLAYING );
			SetPaused( false );
			_DoSetPlay( true );
		}
		inline void Stop()
		{
			_SetFlag( false, SSF_PLAYING );
			_DoSetPlay( false );
		}
		inline bool isPlaying( ) const
		{
			return _GetFlag( SSF_PLAYING ) & !_GetFlag( SSF_PAUSED );
		}
		inline bool Set3D( bool is3D )
		{
			_SetFlag( is3D, SSF_IS3D );
			_DoSet3D( is3D );
		}
		inline bool is3D( ) const
		{
			_GetFlag( SSF_IS3D );
		}
	};

}