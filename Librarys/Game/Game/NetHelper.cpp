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



//////////////////////////////////
//			网络组件		   //
/////////////////////////////////
#include "Log.h"
#include "NetHelper.h"
#include "BohgeNet.h"
#include "NetMessage.h"
#include "NetBase.h"
#include "NetHost.h"
#include "Engine.h"
#include "NetHost.h"

using namespace std;
using namespace BohgeNet;
using namespace BohgeEngine;


namespace BohgeGame
{
	//-------------------------------------------------------------------------------------------------------
	NetHelper::NetHelper(void)
		:m_RoomID(0),
		m_Location(-1),
		m_isClickBattel(false)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	NetHelper::~NetHelper(void)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void NetHelper::ConnectServer()
	{
		m_isConnect = true;
		//engine::Instance().GetNet()->ConnectTCP( Address(23,21,89,16,MAIN_ENTER_PORT) );
		Engine::Instance().GetNet()->ConnectTCP( Domain( "www.bohge.info", MAIN_ENTER_PORT ) );
		//engine::Instance().GetNet()->ConnectTCP( Address(192,168,1,99,MAIN_ENTER_PORT) );
		//engine::Instance().GetNet()->ConnectTCP( Address(172,17,72,77,MAIN_ENTER_PORT) );
	}
	//-------------------------------------------------------------------------------------------------------
	//void NetHelper::ConnectToPlayer( const Address& add)
	//{
	//	engine::Instance().GetNet()->ConnectUDP( add );
	//}
	//-------------------------------------------------------------------------------------------------------
	void NetHelper::DisconnectServer()
	{
		m_isConnect = false;
		Engine::Instance().GetNet()->DisConnectTCP();
		Engine::Instance().GetNet()->DisConnectUDP();
	}
	//-------------------------------------------------------------------------------------------------------
	void NetHelper::CheckReconnect()
	{//提供当服务器从后台切到前台时候使用
		if( m_isConnect == true )
		{
			this->ConnectServer();
		}
	}
}