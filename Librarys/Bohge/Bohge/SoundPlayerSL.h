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
#include "SoundPlayer.h"

#ifdef _OPENSL


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
struct SLPlaybackRateItf_;
typedef const struct SLPlaybackRateItf_ * const * SLPlaybackRateItf;
struct SLPitchItf_;
typedef const struct SLPitchItf_ * const * SLPitchItf;
struct SLMuteSoloItf_;
typedef const struct SLMuteSoloItf_ * const * SLMuteSoloItf;


namespace BohgeEngine
{
	class SoundPlayerSL : public SoundPlayer
	{
		friend class SoundManagerSL;
	private:
		bool				m_isPasue;
		bool				m_isPlaying;
		bool				m_isLoop;
		float				m_fVolume;
		SLObjectItf			m_uriPlayerObject;
		SLPlayItf			m_uriPlayerPlay;
		SLSeekItf			m_uriPlayerSeek;
		SLVolumeItf			m_uriPlayerVolume;
		SLMuteSoloItf		m_uriMuteSoloItf;
		SLPitchItf			m_uriPitchItf;
		SLPlaybackRateItf   m_uriPlayRateItf;
	private:
		SoundPlayerSL( int hash, int index, Decoder* res );
		virtual ~SoundPlayerSL(void);
	private:
		virtual void _OnInitialization();
		virtual void _DoSetVolume( float volume );
		virtual void _DoSetPitch(float pitch);
		virtual void _DoSetPaused( bool ispaused );
		virtual void _DoSetLoop( bool isloop );
		virtual void _DoSetPlay( bool isplay );
		virtual void _DoSet3D( bool is3d );
		virtual void _DoSetSoundPosition( const vector3f& pos, const vector3f& forward, const vector3f& up );
		virtual void _DoUpdate();
	public:
		void CreatePlayer( SLEngineItf engine, SLObjectItf mixer );
	};
}

#endif