//////////////////////////////////
//			网络线程		   //
/////////////////////////////////

#include "Base.h"
#include "MisterCubeServer.h"
#include "ClientContext.h"
#include "MainEntryHandle.h"
#include "LobbyHandle.h"
#include "RoomHandle.h"
#include "ReceiptHandle.h"
#include "GameRooms.h"
#include "GameHandle.h"


#include <string>


using namespace std;
using namespace BohgeNet;

extern void OnGetMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql )//外部链接的时候连接到此
{
	MisterCubeServer::Instance().DoMessage( client, pak, sql );
}


extern void OnClientQuit( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql )//客户端退出时
{
	if ( client != NULL )
	{
		GameRooms::PlayerLocalRoom* plr = GameRooms::Instance().FindPlayerLocalRoom( client );
		if( NULL != plr )
		{
			uint local = plr->m_local;
			uint ID = plr->m_room;
			GameRooms::RoomInfo* room = GameRooms::Instance().FindRoom(local,ID);
			if( NULL != room )
			{
				room->OnGameOver(client, sql);
			}
			GameRooms::Player* player = GameRooms::Instance().FindPlayer( local, ID, client );
			if( NULL != player )
			{
				wstring name = player->m_PlayerName;
				pak.Begin();
				pak<<NM_LEAVE<<name;
				SmartPtr<ClientContext>* others = GameRooms::Instance().PlayerLeave( local, ID, client );
				if( NULL != others )
				{
					//通知另一个玩家，对方离开了
					(*others)->Send( pak );
					(*others)->SendImmediately();
				}
			}
		}
	}
}



//-------------------------------------------------------------------------------------------------------
MisterCubeServer::MisterCubeServer(void)
{
	m_pRoom = NET_NEW RoomHandle;
	m_pGame = NET_NEW GameHandle;
	m_pLobby = NET_NEW LobbyHandle;
	m_pReceipt = NET_NEW ReceiptHandle;
	m_pMainEntry = NET_NEW MainEntryHandle;
	m_HandlePtrList.push_back( m_pRoom );
	m_HandlePtrList.push_back( m_pGame );
	m_HandlePtrList.push_back( m_pLobby );
	m_HandlePtrList.push_back( m_pReceipt );
	m_HandlePtrList.push_back( m_pMainEntry );
}
//-------------------------------------------------------------------------------------------------------
MisterCubeServer::~MisterCubeServer(void)
{
	SAFE_DELETE( m_pRoom );
	SAFE_DELETE( m_pGame );
	SAFE_DELETE( m_pLobby );
	SAFE_DELETE( m_pReceipt );
	SAFE_DELETE( m_pMainEntry );
	m_HandlePtrList.clear();
}
//-------------------------------------------------------------------------------------------------------
bool MisterCubeServer::DoMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql )
{
	for ( HandleList::iterator it = m_HandlePtrList.begin();
		it != m_HandlePtrList.end();
		it++ )
	{
		if( (*it)->HanldeMessage( client, pak, sql ) )
		{
			break;
		}
	}
	return true;
}