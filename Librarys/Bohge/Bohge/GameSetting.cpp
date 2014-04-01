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



//////////////////////////////
//		游戏的设置			//
//////////////////////////////

#include "GameSetting.h"
#include "IOSystem.h"
#include "Serializer.h"
#include "Language.h"
#include "Engine.h"
#include "Device.h"
#include "SoundManager.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	GameSetting::GameSetting()
		:m_isBloom(true),
		m_isVolumeLight(true),
		m_isVolumeLightHighEffect(true),
		m_isFirstTime(false),
		m_fMusicVolume(0.6),
		m_fSoundVolume(0.4),
		//m_TurningPoint( 1.0f, 0.6f, 0.35f, 0.15f ),
		m_PlantVisableDistance( 1.0f, 0.5f, 0.2f, 0.05f ),
		m_TurningPoint( 1.0f, 0.5f, 0.25f, 0.1f )
		//m_PlantVisableDistance( 1.0f, 0.4f, 0.15f, 0.05f )
		//m_TurningPoint( 1.0f, 0.9f, 0.8f, 0.7f ),
		//m_PlantVisableDistance( 1.0f, 0.9f, 0.8f, 0.7f )
	{
		m_FileName = "setting.bin";
	}
	//-------------------------------------------------------------------------------------------------------
	void GameSetting::LoadData()
	{
		if(  IOINSTANCE.isExist( IOINSTANCE.WriteFolder()+m_FileName ) )
		{
			Serializer file( IOINSTANCE.WriteFolder()+m_FileName );
			int language = 0;
			file>>m_isBloom>>m_isVolumeLight>>language>>m_isVolumeLightHighEffect>>m_fMusicVolume>>m_fSoundVolume;
			STRING.Setlanguage( static_cast<LanguageControl::LanguageType>(language) );
			//Engine::Instance().GetSoundManager()->SetMusicVolume( m_fMusicVolume );
			//Engine::Instance().GetSoundManager()->SetSoundVolume( m_fSoundVolume );
			//Engine::Instance().GetSoundManager()->SetGlobalVolume( m_fSoundVolume );
			if ( !Engine::Instance().GetDevice()->isPackedDepthStencil() )
			{
				m_isBloom = false;
				m_isVolumeLight = false;
				m_isVolumeLightHighEffect = false;
			}
		}
		else
		{
			//初次进入游戏，设置特效等级
			if( Engine::Instance().GetDevice()->isPackedDepthStencil() )
			{
				if( Engine::LEL_LOW == Engine::Instance().GetDeviceLevel() )
				{
					m_isBloom = false;
					m_isVolumeLight =false;
					m_isVolumeLightHighEffect =false;
				}
				else if( Engine::LEL_BASE == Engine::Instance().GetDeviceLevel() )
				{
					m_isBloom = false;
					m_isVolumeLight =false;
					m_isVolumeLightHighEffect =false;
				}
				else if( Engine::LEL_MEDIUM == Engine::Instance().GetDeviceLevel() )
				{
					m_isBloom = true;
					m_isVolumeLight =false;
					m_isVolumeLightHighEffect =false;
				}
				else if( Engine::LEL_HIGH == Engine::Instance().GetDeviceLevel() )
				{
					m_isBloom = true;
					m_isVolumeLight =true;
					m_isVolumeLightHighEffect =false;
				}
				else if( Engine::LEL_ULTRA == Engine::Instance().GetDeviceLevel() )
				{
					m_isBloom = true;
					m_isVolumeLight =true;
					m_isVolumeLightHighEffect =true;
				}
			}
			else
			{
				m_isBloom = false;
				m_isVolumeLight =false;
				m_isVolumeLightHighEffect =false;
			}
			m_isFirstTime = true;
			this->SaveData();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void GameSetting::SaveData()
	{
		Serializer file( IOINSTANCE.WriteFolder() + m_FileName );
		file<<m_isBloom<<m_isVolumeLight<<static_cast<int>(STRING.GetLanguage())<<m_isVolumeLightHighEffect<<m_fMusicVolume<<m_fSoundVolume;
	}
	//-------------------------------------------------------------------------------------------------------
	bool GameSetting::isSettingExist()
	{
		return IOINSTANCE.isExist( IOINSTANCE.WriteFolder()+m_FileName );
	}
	//-------------------------------------------------------------------------------------------------------

}