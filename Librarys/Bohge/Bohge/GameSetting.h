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

#pragma once
#include <string>
#include "3DMath.h"
#include "VariableType.h"


namespace BohgeEngine
{
	class GameSetting
	{
		std::string		m_FileName;	//保存文件的名字
		bool			m_isBloom;
		bool			m_isVolumeLight;
		bool			m_isVolumeLightHighEffect;//sobel过滤
		bool			m_isFirstTime;//第一次进入游戏
		float			m_fMusicVolume;
		float			m_fSoundVolume;
		//地形相关参数
		vector4f		m_TurningPoint;//地形开始变化的距离百分比
		vector4f		m_PlantVisableDistance;//植被的可见精度细节
	private:
		GameSetting();
	public:
		void SaveData();
		void LoadData();
		bool isSettingExist();
	public:
		static GameSetting& Instance()
		{
			static GameSetting setting;
			return setting;
		}
		BOHGE_FORCEINLINE bool isFirstTime()
		{
			return m_isFirstTime;
		}
		BOHGE_FORCEINLINE bool isBloom()
		{
			return m_isBloom;
		}
		BOHGE_FORCEINLINE bool isVolumeLight()
		{
			return m_isVolumeLight;
		}
		BOHGE_FORCEINLINE void SetBloom(bool isActive)
		{
			m_isBloom = isActive;
		}
		BOHGE_FORCEINLINE void SetVolumeLight(bool isActive)
		{
			m_isVolumeLight = isActive;
		}
		BOHGE_FORCEINLINE bool isVolumeLightHeigh()
		{
			return m_isVolumeLightHighEffect;
		}
		BOHGE_FORCEINLINE void SetVolumeLightHeigh(bool isActive)
		{
			m_isVolumeLightHighEffect = isActive;
		}
		BOHGE_FORCEINLINE void SetMusicVolume(float v)
		{
			m_fMusicVolume = v;
		}
		BOHGE_FORCEINLINE void SetSoundVolume(float v)
		{
			m_fSoundVolume = v;
		}
		BOHGE_FORCEINLINE vector4f& TurningPoint()
		{
			return m_TurningPoint;
		}
		BOHGE_FORCEINLINE vector4f& PlantVisableDistance()
		{
			return m_PlantVisableDistance;
		}
	};

}