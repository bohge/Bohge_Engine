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



/////////////////////////////////////////
//			enter point of game		   //
/////////////////////////////////////////

#pragma once
#include "3DMath.h"
#include "Predefine.h"
#include "StateManager.h"
#include "Environment.h"
#include "Device.h"
#include "Engine.h"
#include "Bfont.h"
#include "Log.h"
#include "ShaderManager.h"
#include "ActionManager.h"
#include "Camera.h"
#include "Pipeline.h"
#include "EngineResourceList.h"
#include "AdHelper.h"
#include "MessageBox.h"
#include "Language.h"
#include "UIManager.h"
#include "GameResourceList.h"
#include "SceneMain.h"
#include "SceneManager.h"
#include "ScriptManager.h"
#include "AES.h"


#include "NetHost.h"
#include "NetHelper.h"
#include "ShopHelper.h"

#include <string>
#include <sstream>


using namespace BohgeEngine;

namespace BohgeGame
{
	class Game
	{
	private:
		bool					m_isRunning;
		bool					m_bPasue;
		Engine*					m_pEngine;	//驱动
		GameMessageBox*			m_pMessageBox;

		//动作输入相关
		bool					m_isAnyAction;//是否有动作
		vector3f				m_vLastRayOrig;//上一个触摸的射线
		vector2f				m_vLastPoint;//前一个点

		//相机
		ICamera*			m_pCamera;
	private:
		Game()
			:m_bPasue(false),
			m_isRunning(false),
			m_isAnyAction(false),
			m_pMessageBox(NULL),
			m_pCamera(NULL)
		{
			AES::Instance().MakeKey("bohge&penngames!", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16, 16);
		}
	public:
		static BOHGE_FORCEINLINE Game& Instance()
		{
			static Game game;
			return game;
		}
	public:
		void BecomeBackground();//切到背景的时候调用
		void BecomeFrontground();//切换到前台
		bool OnTouchEvent( ActionManager& sender );
		bool OnMoveEvent( ActionManager& sender );
		bool OnReleaseEvent( ActionManager& sender );
		bool OnCameraUpdate( ICamera& camera )
		{
			vector3f pos = camera.GetPosition();
			float nearondiYr = 0;
			vector3f downDir;
			vector3f roaxis;
			roaxis = camera.GetForward().Cross( camera.GetUp() );
			roaxis.NormalizeSelf();
			Quaternionf rota;
			rota.RotateAxis( roaxis, -camera.Fov() / camera.Aspect() * 0.8 );
			downDir = camera.GetForward() * rota;
			downDir.NormalizeSelf();
			if ( downDir.Dot( vector3f( 0,1,0 ) ) < 0.0 )
			{
				nearondiYr = -( downDir * camera.GetNear() ).m_y;
			}
			float wh_nearondiYr =m_pEngine->GetSceneManager()->GetWorld().GetHeight( pos.m_x, pos.m_z ) + nearondiYr;
			pos.m_y = wh_nearondiYr;
			camera.SetPosition( pos );
			return true;
		}
	public:
		BOHGE_FORCEINLINE void Initialization(const vector2d& view, const vector2d& resolution, Engine::DeviceLevel l, bool isForceAD, const std::string& root, const std::string& wirte, const std::string& udid)
		{
			DEBUGLOG("Game Initialization!\n");
			m_pEngine = Engine::CreateEngine( view, resolution, l, root, wirte );
			//m_pengine->GetCamera()->CreatePerspectiveProjection(3.1415926f / 4.0f, w/h, 10.0f , 1000.0f );


			//m_pCamera = NEW TrackballCamera();
			m_pCamera = NEW FPSCamera();
			m_pCamera->AttachCamera();
			m_pCamera->ConectOnUpdateCallBack( make_bind( &SceneManager::OnCameraUpdate, m_pEngine->GetSceneManager() ) );
			//m_pCamera->ConectOnUpdateCallBack( MEMBER_FUNC_PTR( &Game::OnCameraUpdate ) );
			m_pEngine->GetCamera()->SetMoveRotateZoom( 50.0, PI/2, 10.0 );
			m_pEngine->GetDevice()->DeviceCaps();
			//加载字体放在了Logo面去做了
			//engine::Instance().GetStringManager()->LoadFont(FontName);//加载字体库
			//GetAdmobID(AdHelper::PF_IPAD);
			if( false == isForceAD )
			{
				GetAdmobID( AdHelper::PF_ANDROID );
				ADConnect();
			}
			//设置设备UDID
			NetHelper::Instance().UDID() = udid;
			m_pEngine->GetNet()->AddTCPResponse( make_bind( &ShopHelper::ListenReceipt, &ShopHelper::Instance() ) );//讲订单的回调添加到响应中
			m_pEngine->GetActionManager()->OnTouchEvent( MEMBER_FUNC_PTR( &Game::OnTouchEvent ) );
			m_pEngine->GetActionManager()->OnMoveEvent( MEMBER_FUNC_PTR( &Game::OnMoveEvent ) );
			m_pEngine->GetActionManager()->OnReleaseEvent( MEMBER_FUNC_PTR( &Game::OnReleaseEvent ) );
			m_isRunning = true;//完成初始化，开始游戏逻辑
		}
		BOHGE_FORCEINLINE void QuitGame()
		{
			m_isRunning = false;
		}
		BOHGE_FORCEINLINE bool Update()
		{
			if( !m_isRunning )
			{
				return false;
			}
			else if( NULL != m_pMessageBox 
				&& m_pEngine->GetActionManager()->isAction(ActionManager::ACTION_EXIT)
				&& StateManager::State_Main_Menu == StateManager::Instance().CurrextState() )
			{
				if( !m_pMessageBox->isShowing() )
				{
					m_bPasue= true;
					m_pEngine->GetUIManager()->PauseResponseAll();
					m_pMessageBox->Popup();
				}
				else //如果在按下退出对话框的时候再次退出则退出游戏
				{
					m_isRunning = false;
				}
			}
			m_pEngine->OnFramebegin();
			if( !m_bPasue )
			{
				StateManager::Instance().Update(*m_pEngine, m_pEngine->GetTimeSpan());	//更新state
			}
			m_pEngine->Update();	//更新engine中的各种manage
			return true;
		}
		BOHGE_FORCEINLINE void Paint()
		{
			m_pEngine->Render();	//引擎绘制管线
		}
		BOHGE_FORCEINLINE void Exit()
		{
			//m_pEngine->GetSoundManage()->UnloadSound( sou_Button );
			//m_pEngine->GetSoundManage()->UnloadSound( sou_Slider );
			SAFE_DELETE(m_pMessageBox);
			SAFE_DELETE(m_pCamera);
			StateManager::Instance().Exit(*m_pEngine);//通知当前state退出，释放资源
			//最后删除engine
			Engine::DropEngine( &m_pEngine );
		}
		void InitializationMessageBox()
		{
			if( NULL == m_pMessageBox )
			{
				m_pMessageBox = NEW GameMessageBox(2);
				m_pMessageBox->SetResponse( MEMBER_FUNC_PTR(&Game::OnButtonOkClick), MEMBER_FUNC_PTR(&Game::OnButtonCancleClick));
			}
			m_pMessageBox->SetContext( STRING.GetString( "Ok" ),
				STRING.GetString( "Cancel" ),
				STRING.GetString( "ExitOrNot" ));
		}
		bool OnButtonOkClick(UIBase& sender)
		{
			m_isRunning = false;
			m_bPasue = true;
			return true;
		}
		bool OnButtonCancleClick(UIBase& sender)
		{
			m_pEngine->GetUIManager()->ResumeResponseAll();
			m_bPasue = false;
			return true;
		}
		BOHGE_FORCEINLINE void SetKeyDown(ActionManager::Key_Down k)
		{
			if(m_isRunning) m_pEngine->GetActionManager()->KeyDown( k );
		}
		BOHGE_FORCEINLINE void SetActionState(ActionManager::Action action)
		{
			if(m_isRunning) m_pEngine->GetActionManager()->SetAction(action);
		}
		BOHGE_FORCEINLINE void TouchPoint(const ActionManager::InputAction& input)
		{
			if(m_isRunning) m_pEngine->GetActionManager()->TouchAction( input );
		}
		BOHGE_FORCEINLINE bool isShowAd()
		{
			return AdHelper::Instance().isShowAd();
		}
		BOHGE_FORCEINLINE void ADConnect()	//广告已经连接了
		{
			AdHelper::Instance().Connected();
			//DEBUGLOG("Reserver AD call back invoke",1);
		}
		std::string GetAdmobID( AdHelper::Platfrom p )
		{
			return AdHelper::Instance().GetAdmobID(p);
		}
		BOHGE_FORCEINLINE void CheckIapStatus()
		{
			ShopHelper::Instance().Request();
			if( ShopHelper::Instance().isUnprocessReceipt() )
			{
				NetHelper::Instance().ConnectServer();
				ShopHelper::Instance().ShopServerStatus( *m_pEngine );//连接服务器，尝试解决订单的问题
			}
		}
	};
}