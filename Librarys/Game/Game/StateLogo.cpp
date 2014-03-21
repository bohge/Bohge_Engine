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



#include "StateLogo.h"
#include "StateManager.h"
#include "UIManager.h"
#include "Device.h"
#include "GameResourceList.h"
#include "Device.h"
#include "Bfont.h"
#include "EngineResourceList.h"
#include "Camera.h"
#include "AdHelper.h"
#include "GameSetting.h"
#include "UIAnimation.h"
#include "Language.h"
#include "UI.h"

using namespace BohgeEngine;

namespace BohgeGame
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	StateLogo::StateLogo()
		:m_TimeSpan(0),
		m_isLogoDone(false),
		m_isBeginProcessBar(false)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void StateLogo::LoadResource(Engine& engine)
	{
		STRING.LoadPackage( );

		engine.GetDevice()->SetClearScreenColor(1.0, 1.0, 1.0, 1.0);
		engine.GetCamera()->CreatePerspectiveProjection(10.0f , 450.0f );
		engine.GetCamera()->LookAt( vector3f(0,0,0),vector3f(0,0,1),vector3f(0,1,0) );

		//UIAnimation anim1(1000);
		//anim1.ColorAnimation( Color(1,1,1,1), Color(1,1,1,0) );
		//anim1.SetDelay( 4000 );
		//m_pStudioLogo = engine.GetUIManager()->CreateUIGroup(UIFileName[ui_StudioLogo]);
		//m_pStudioLogo->SetUIAnimation(anim1, MEMBER_FUNC_PTR( &StateLogo::OnLogoOutDone ) );

		UIAnimation anim2(1000);
		anim2.ColorAnimation( Color(1,1,1,0), Color(1,1,1,1) );
		m_pEngineLogo = engine.GetUIManager()->CreateUIGroup(UIFileName[ui_EngineLogo]);
		m_pEngineLogo->SetUIAnimation(anim2, MEMBER_FUNC_PTR( &StateLogo::OnEngineInDone ) );
		//m_pEngineLogo->SetShow(false);

		//ReleaseResource::Instance().CreateReleaseTread();//开启一个释放线程
		AdHelper::Instance().Where() = AdHelper::OS_TOPCENTER;
		AdHelper::Instance().setShowAd(true);
	}
	//-------------------------------------------------------------------------------------------------------
	void StateLogo::ReleaseResource(Engine& engine)
	{
		//engine.GetUIManager()->DeleteUIGroup(m_pStudioLogo);
		engine.GetUIManager()->DeleteUIGroup(m_pEngineLogo);
		//engine.GetSoundManage()->LoadSound( sou_Button, SoundFileName[sou_Button] );
		//engine.GetSoundManage()->LoadSound( sou_Slider, SoundFileName[sou_Slider] );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void StateLogo::OnEnter(Engine& engine, StateManager::GameState Previous)
	{
		GameSetting::Instance().LoadData();//读取设置
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void StateLogo::OnLeave(Engine& engine)
	{
		engine.GetStringManager()->LoadFont(FontName);//加载字体库
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void StateLogo::Update(StateManager& state, Engine& engine, uint millisecond)
	{
		//state.ChangeState(StateManage::State_Main_Menu, true, false, true);GameSetting::Instance().LoadData();


		if( m_isLogoDone )
		{
			//if( ReleaseResource::Instance().isFinished() )
			{
				if( GameSetting::Instance().isFirstTime() )
				{
					STRING.Setlanguage(LanguageControl::L_English);//设置默认语言进入
					StateManager::Instance().ChangeState(StateManager::State_Main_Menu, true, false, false);
				}
				else
				{
					StateManager::Instance().ChangeState(StateManager::State_Main_Menu, true, false, true);
				}
			}
			//else
			//{
			//	if( m_isBeginProcessBar )
			//	{
			//		m_TimeSpan += millisecond;
			//		if(  m_TimeSpan > 2100 )
			//		{
			//			m_TimeSpan = 0;
			//		}
			//		m_pProcess->SetPercentage( m_TimeSpan / static_cast<float>(2000) );
			//	}
			//	else
			//	{
			//		m_isBeginProcessBar = true;
			//		m_pEngineLogo->SetShow(false);
			//		m_pChenckData->SetShow(true);
			//	}
			//}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	bool StateLogo::OnLogoOutDone(UIGroup& sender)
	{
		m_pEngineLogo->SetShow(true);
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	bool StateLogo::OnEngineInDone(UIGroup& sender)
	{
		UIAnimation anim(1000);
		anim.ColorAnimation( Color(1,1,1,1), Color(1,1,1,0) );
		anim.SetDelay( 3000 );
		m_pEngineLogo->SetUIAnimation(anim, MEMBER_FUNC_PTR( &StateLogo::OnEnginOutDone ) );
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	bool StateLogo::OnEnginOutDone(UIGroup& sender)
	{
		m_isLogoDone = true;
		return true;
	}
}