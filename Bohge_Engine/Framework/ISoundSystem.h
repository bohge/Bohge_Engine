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
#include "SystemSingleton.h"
#include "IPlugin.h"
#include "3DMath.h"




namespace BohgeEngine
{
	class ISoundComponent;
	class BOHGE_FRAMEWORK ISoundSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR( ISoundSystem );
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( ISoundSystem, IPlugin );
	private:
		float					m_fGlobalVolume;
		float					m_fGlobalPitch;
		bool					m_isGlobalMute;
	public:
		ISoundSystem(void)
			:m_fGlobalVolume(0.8f),
			m_fGlobalPitch(1.0f),
			m_isGlobalMute(false)
		{SYSTEM_SINGLETON_INITIALIZE}
		virtual ~ISoundSystem(void){SYSTEM_SINGLETON_DESTROY;}
	public:
		virtual ISoundComponent* LoadSound( const eastl::string& path ) = 0;
		virtual void UnloadSound( ISoundComponent* player ) = 0;
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
		BOHGE_FORCEINLINE void SetGLobalMute( bool mute )
		{
			m_isGlobalMute = mute;
		}
		BOHGE_FORCEINLINE bool isGlobalMute() const
		{
			return m_isGlobalMute;
		}
	};
}