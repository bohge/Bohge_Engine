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



#include "Game.h"
#include "NetHost.h"
#include "NetHelper.h"

using namespace BohgeEngine;


namespace BohgeGame
{
	const float spanH = 5.5;
	const float spanV = 4;
	const float downspan = 0.015f;


	void Game::BecomeBackground()//«–µΩ±≥æ∞µƒ ±∫Úµ˜”√
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void Game::BecomeFrontground()
	{
		NetHelper::Instance().CheckReconnect();
	}
	//-------------------------------------------------------------------------------------------------------
	bool Game::OnTouchEvent( ActionManager& sender )
	{
		m_isAnyAction = false;
		m_vLastRayOrig = sender.TouchRay().GetRayOrigin();
		m_vLastPoint = sender.TouchPoint();
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	bool Game::OnMoveEvent( ActionManager& sender )
	{
		if( NULL != sender.TouchObject() )//处理移动
		{
			//DEBUGLOG("Ray on X %d, Y %d\n", (int)sender.CurrentRay().m_x, (int)sender.CurrentRay().m_y);
			if( (sender.TouchRay().GetRayOrigin().m_x - m_vLastRayOrig.m_x) < -spanH )//左移
			{
				sender.TouchObject()->Response(ActionManager::ACTION_LEFT);
				m_vLastRayOrig.m_x = sender.TouchRay().GetRayOrigin().m_x;
				m_isAnyAction = true;
				//DEBUGLOG("MoveObject left\n");
			}
			else if( (sender.TouchRay().GetRayOrigin().m_x - m_vLastRayOrig.m_x) > spanH )//右移
			{
				sender.TouchObject()->Response(ActionManager::ACTION_RIGHT);
				m_vLastRayOrig.m_x = sender.TouchRay().GetRayOrigin().m_x;
				m_isAnyAction = true;
				//DEBUGLOG("MoveObject right\n");
			}
			else if( (sender.TouchRay().GetRayOrigin().m_z - m_vLastRayOrig.m_z) < -spanV )//上移
			{
				sender.TouchObject()->Response(ActionManager::ACTION_UP);
				m_vLastRayOrig.m_z = sender.TouchRay().GetRayOrigin().m_z;
				m_isAnyAction = true;
				//DEBUGLOG("MoveObject up\n");
			}
			else if( (sender.TouchRay().GetRayOrigin().m_z - m_vLastRayOrig.m_z) > spanV )//下移
			{
				sender.TouchObject()->Response(ActionManager::ACTION_DOWN);
				m_vLastRayOrig.m_z = sender.TouchRay().GetRayOrigin().m_z;
				m_isAnyAction = true;
				//DEBUGLOG("MoveObject down\n");
			}
		}
		else//处理下落
		{
			if( (m_vLastPoint.m_y - sender.TouchPoint().m_y) > downspan )
			{
				if( NULL != sender.FirstObject() && Math::Abs( sender.TouchPoint().m_x )> 0.5 )
				{
					sender.FirstObject()->Response(ActionManager::ACTION_FALL);
				}
				m_vLastPoint = sender.TouchPoint();
				m_isAnyAction = true;
			}	
		}
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	bool Game::OnReleaseEvent( ActionManager& sender )
	{
		if( false == m_isAnyAction )
		{
			if( NULL != sender.TouchObject() )
			{
				sender.TouchObject()->Response(ActionManager::ACTION_SWITH);
			}
		}
		m_isAnyAction = false;
		return true;
	}
}