//////////////////////////////////////
//			处理网络请求		   //
/////////////////////////////////////
#include "stdafx.h"
#include "Base.h"
#include "LobbyHandle.h"
#include "NetBase.h"
#include "ClientContext.h"
#include "SQLDataBase.h"
#include "GameRooms.h"
#include "GameSetting.h"

#include <sstream>
#include <string>


using namespace std;
using namespace BohgeNet;

int Index(uint l)
{
	switch(l)
	{
	case LOCATION_ASIA: return 0;
	case LOCATION_EURO: return 1;
	case LOCATION_USA:  return 2;
	}
	return 0;
}


//-------------------------------------------------------------------------------------------------------
LobbyHandle::LobbyHandle(void)
{
	//GameRooms::Instance().NewRoom(0, L"Bohge", L"1", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(0, L"是天才", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(0, L"Penn", L"3", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(0, L"是蛤蟆皮", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(0, L"啦啦啦", L"4", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(0, L"气死你", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"7", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"8", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"9", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"10", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"11", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"12", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"13", L"1", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"14", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"15", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"16", L"3", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"17", L"4", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"18", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"19", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"20", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"21", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"22", L"", SmartPtr<ClientContext>() );
}
//-------------------------------------------------------------------------------------------------------
LobbyHandle::~LobbyHandle(void)
{
}
//-------------------------------------------------------------------------------------------------------
bool LobbyHandle::HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql )
{
	pak.Begin();
	int message;
	pak>>message;
	switch (message)
	{
	case NM_LOBBY_ENTER: //发送当前房间信息
	case NM_LOBBY_ROOM_REFESH: //发送当前房间信息
		{
			uint location;
			pak>>location;
			DEBUGLOG("Receive from %s  NM_LOBBY_ENTER  NM_LOBBY_ROOM_REFESH\n", client->BindIPString() );
			int i = 0;
			GameRooms::Instance().RoomMapBegin( location );
			for( GameRooms::RoomMap::iterator it = GameRooms::Instance().GetRoomMap(location).begin();
				it != GameRooms::Instance().GetRoomMap(location).end();
				it++ )
			{
				if( i >= 20 )//之发送前20个房间，刷新需要客户手动刷新
				{
					break;
				}
				if( !it->second->isFull() && it->second->isNotBegin() )
				{
					i++;
					pak.Begin();
					pak<<NM_LOBBY_ROOM_DATA<<it->second->m_ID<<it->second->m_Name<<(!it->second->m_Password.empty());
					client->Send( pak );
				}
			}
			GameRooms::Instance().RoomMapEnd( location );
			return true;
		}return true;
	case NM_LOBBY_ROOM_NEW://转向至房间服务端
		{
			DEBUGLOG("Receive from %s NM_LOBBY_ROOM_NEW\n", client->BindIPString() );
			uint local;
			wstring n,p;
			pak >> local;
			pak >> n;
			pak >> p;
			pak.Begin();
			pak<<NM_ROOM_PORT<<GameRooms::Instance().NewRoom(local, n, p, client ).m_ID;//添加房间，通知客户端转换端口和所持有的ID
			client->Send( pak );
			//传递给room线程新建立房间信息
			return true;
		}return true;
	case NM_LOBBY_ROOM_ENTER:
		{
			DEBUGLOG("Receive from %s NM_LOBBY_ROOM_ENTER\n", client->BindIPString() );
			bool isInputPassword;
			uint local;
			uint roomID = 0;
			wstring password;
			pak>>local>>roomID>>isInputPassword;
			NETDEBUGLOG("Room ID's %d\n", roomID);
			if( isInputPassword )
			{
				pak>>password;
			}
			const GameRooms::RoomInfo* proom = GameRooms::Instance().EntryRoom( local, roomID, password, client );
			if ( NULL == proom )//没有找到房间
			{
				pak.Begin();
				pak<<NM_LOBBY_ROOM_ENTER_REFUES;//拒绝进入
				client->Send( pak );
			}
			else
			{
				DEBUGLOG("Password is Currect!\n");
				pak.Begin();
				pak<<NM_LOBBY_ROOM_ENTER_PASS
					<<roomID;
				client->Send( pak );
			}
		}return true;
	case NM_LOBBY_ROOM_FIND:
		{
			DEBUGLOG("Receive from %s NM_LOBBY_ROOM_FIND\n", client->BindIPString() );
			uint local, roomID = 0;
			pak>>local>>roomID;
			bool isFind = false;
			const GameRooms::RoomInfo* ri = GameRooms::Instance().FindEntryRoom(local, roomID, client, isFind );
			if ( false == isFind )//没有找到房间
			{
				pak.Begin();
				pak<<NM_LOBBY_ROOM_ENTER_REFUES;//拒绝进入
				client->Send( pak );
			}
			else
			{
				if( NULL != ri )//如果房间密码是空的就直接进入
				{
					pak.Begin();
					pak<<NM_LOBBY_ROOM_ENTER_PASS
						<<roomID;
					client->Send( pak );
					//通知room有人进来了
				}
				else
				{
					NETDEBUGLOG("Need the Password!\n");
					pak.Begin();
					pak<<NM_LOBBY_ROOM_PASSWORD;//向客户端请求密码
					client->Send( pak );
				}
			}
		}return true;
	case NM_LOBBY_ROOM_QUICK:
		{
			DEBUGLOG("Receive from %s NM_LOBBY_ROOM_QUICK\n", client->BindIPString() );
			uint local;
			pak>>local;
			const GameRooms::RoomInfo* ri = GameRooms::Instance().QuickBegin( local, client );
			if( NULL == ri )
			{
				pak.Begin();
				pak<<NM_LOBBY_ROOM_ENTER_REFUES;//拒绝进入
				client->Send( pak );
			}
			else
			{
				pak.Begin();
				pak<<NM_LOBBY_ROOM_ENTER_PASS
					<<ri->m_ID;
				client->Send( pak );
			}
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------