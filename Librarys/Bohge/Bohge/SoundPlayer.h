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
#include "Engine.h"
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
		SoundPlayer( uint hash, int index, SmartPtr<Decoder>& res );
		virtual ~SoundPlayer(void);
	private:
		virtual void _OnInitialization() = 0;
		virtual void _DoSetVolume( float volume ) = 0;
		virtual void _DoSetPitch(float pitch) = 0;
		virtual void _DoSetPaused( bool ispaused ) = 0;
		virtual void _DoSetLoop( bool isloop ) = 0;
		virtual void _DoSetPlay( bool isplay ) = 0;
		virtual void _DoSet3D( bool is3d ) = 0;
		virtual void _DoSetSoundPosition( const vector3f& pos, const vector3f& forward, const vector3f& up ) = 0;
		virtual void _DoUpdate() = 0;
	public:
		void Update();
	private:
		BOHGE_FORCEINLINE void _SetFlag( bool isflag, SoundSourceFlag flag )
		{
			isflag ? m_nFlag |= flag : m_nFlag &= ~flag;
		}
		BOHGE_FORCEINLINE bool _GetFlag( SoundSourceFlag flag ) const
		{
			return 0 != ( m_nFlag & flag );
		}
	protected:
		BOHGE_FORCEINLINE SoundResource* _GetSoundResource()
		{
			return m_pResource;
		}
		BOHGE_FORCEINLINE void _OnPlayDone()//Íê³É²¥·Å
		{
			if ( !_GetFlag(SSF_LOOP) )
			{
				Stop();
			}
		}
	public:
		BOHGE_FORCEINLINE void Initialization()
		{
			_OnInitialization();
		}
		BOHGE_FORCEINLINE int GetIndex() const
		{
			return m_nIndex;
		}
		BOHGE_FORCEINLINE uint GetHashCode() const
		{
			return m_nHashCode;
		}
		BOHGE_FORCEINLINE void SetVolume( float volume )
		{
			m_fVolume = Math::Clamp0to1( volume );
			_DoSetVolume( volume * Engine::Instance().GetSoundManager()->GetGlobalVolume() );
		}
		BOHGE_FORCEINLINE float GetVolume() const
		{
			return m_fVolume;
		}
		BOHGE_FORCEINLINE void SetPitch( float pitch )
		{
			m_fPitch = pitch;
			_DoSetPitch( pitch * Engine::Instance().GetSoundManager()->GetGlobalPitch() );
		}
		BOHGE_FORCEINLINE float GetPitch() const
		{
			return m_fPitch;
		}
		BOHGE_FORCEINLINE void SetPaused( bool ispaused )
		{
			_SetFlag( ispaused, SSF_PAUSED );
			_DoSetPaused( ispaused );
		}
		BOHGE_FORCEINLINE bool isPaused( ) const
		{
			return _GetFlag( SSF_PAUSED );
		}
		BOHGE_FORCEINLINE void SetLoop( bool isloop )
		{
			_SetFlag( isloop, SSF_LOOP );
			_DoSetLoop( isloop );
		}
		BOHGE_FORCEINLINE bool isLoop( ) const
		{
			return _GetFlag( SSF_LOOP );
		}
		BOHGE_FORCEINLINE void Paly( )
		{
			_SetFlag( true, SSF_PLAYING );
			SetPaused( false );
			_DoSetPlay( true );
		}
		BOHGE_FORCEINLINE void Stop()
		{
			_SetFlag( false, SSF_PLAYING );
			_DoSetPlay( false );
		}
		BOHGE_FORCEINLINE bool isPlaying( ) const
		{
			return _GetFlag( SSF_PLAYING ) & !_GetFlag( SSF_PAUSED );
		}
		BOHGE_FORCEINLINE bool Set3D( bool is3D )
		{
			_SetFlag( is3D, SSF_IS3D );
			_DoSet3D( is3D );
		}
		BOHGE_FORCEINLINE bool is3D( ) const
		{
			_GetFlag( SSF_IS3D );
		}
	};

}