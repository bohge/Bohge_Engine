//////////////////////////////////////
//			玩家建立的房间		   //
/////////////////////////////////////

#include "stdafx.h"
#include "GameRooms.h"
#include "ClientContext.h"
#include "SQLDataBase.h"
#include <sstream>


using namespace std;


//-------------------------------------------------------------------------------------------------------
GameRooms::GameRooms(void)
{
	// 初始化线程互斥量
	InitializeCriticalSection(&m_csPlayerMap);
	for ( int i = 0 ; i < LOCATION_COUNT ; i ++ )
	{
		InitializeCriticalSection(&m_csRoomMap[i]);
	}
}
//-------------------------------------------------------------------------------------------------------
GameRooms::~GameRooms(void)
{
	// 删除客户端列表的互斥量
	DeleteCriticalSection(&m_csPlayerMap);
	for ( int i = 0 ; i < LOCATION_COUNT ; i ++ )
	{
		DeleteCriticalSection(&m_csRoomMap[i]);
	}
	for ( int i = 0 ; i < LOCATION_COUNT ; i ++ )
	{
		for ( RoomMap::iterator it = m_RoomInfoMap[i].begin();
			it != m_RoomInfoMap[i].end();
			it ++ )
		{
			SAFE_DELETE( it->second );
		}
		m_RoomInfoMap[i].clear();
	}


}
//-------------------------------------------------------------------------------------------------------
const GameRooms::RoomInfo& GameRooms::NewRoom( uint local, const std::wstring& roomname, const std::wstring& password, SmartPtr<ClientContext>& client )
{
	EnterCriticalSection(&m_csRoomMap[Index(local)]);
	RoomInfo* ri = NET_NEW RoomInfo;
	ri->m_ID = m_RoomID.GetID();
	ri->m_Name = roomname;
	ri->m_Password = password;
	ri->m_Players[0].m_Client = client;
	m_RoomInfoMap[Index(local)].insert( std::make_pair( ri->m_ID, ri ) );

	PlayerLocalRoom plm;
	plm.m_local = local;
	plm.m_room = ri->m_ID;
	m_PlayerInfoMap.insert( make_pair( (int)client, plm ) );
	LeaveCriticalSection(&m_csRoomMap[Index(local)]);
	return *ri;
}
//-------------------------------------------------------------------------------------------------------
const GameRooms::RoomInfo* GameRooms::EntryRoom( uint local, uint ID, const std::wstring& password, SmartPtr<ClientContext>& client )
{
	EnterCriticalSection(&m_csRoomMap[Index(local)]);
	RoomMap::iterator it = m_RoomInfoMap[Index(local)].find( ID );
	if ( it == m_RoomInfoMap[Index(local)].end() )//没有找到房间
	{
		LeaveCriticalSection(&m_csRoomMap[Index(local)]);
		return NULL;
	}
	else
	{
		if( ! it->second->isFull()&& it->second->isNotBegin() )
		{
			if( password == it->second->m_Password )
			{
				DEBUGLOG("Password is Currect!\n");
				it->second->m_PlayerCount ++;
				it->second->m_EnterCount = 0;
				( it->second->m_Players[0].m_Client == NULL ? it->second->m_Players[0].m_Client : it->second->m_Players[1].m_Client ) = client;
				PlayerLocalRoom plm;
				plm.m_local = local;
				plm.m_room = ID;
				m_PlayerInfoMap.insert( make_pair( (int)client, plm ) );
				LeaveCriticalSection(&m_csRoomMap[Index(local)]);
				return (it->second);
			}
			else
			{
				DEBUGLOG("Password is Error!\n");
				LeaveCriticalSection(&m_csRoomMap[Index(local)]);
				return NULL;
			}
		}
	}
	LeaveCriticalSection(&m_csRoomMap[Index(local)]);
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
const GameRooms::RoomInfo* GameRooms::FindEntryRoom( uint local, uint ID, SmartPtr<ClientContext>& client, bool& isFind )
{
	EnterCriticalSection(&m_csRoomMap[Index(local)]);
	RoomMap::iterator it = m_RoomInfoMap[Index(local)].find( ID );
	if ( it == m_RoomInfoMap[Index(local)].end() )//没有找到房间
	{
		isFind = false;
		LeaveCriticalSection(&m_csRoomMap[Index(local)]);
		return NULL;
	}
	else
	{
		if( ! it->second->isFull()&& it->second->isNotBegin() )
		{
			isFind = true;
			if( it->second->m_Password.empty() )//如果房间密码是空的就直接进入
			{
				it->second->m_PlayerCount ++;
				it->second->m_EnterCount = 0;
				( it->second->m_Players[0].m_Client == NULL ? it->second->m_Players[0].m_Client : it->second->m_Players[1].m_Client ) = client;
				PlayerLocalRoom plm;
				plm.m_local = local;
				plm.m_room = ID;
				m_PlayerInfoMap.insert( make_pair( (int)client, plm ) );
				LeaveCriticalSection(&m_csRoomMap[Index(local)]);
				return (it->second);
			}
			else//密码不对，但是找到了
			{
				LeaveCriticalSection(&m_csRoomMap[Index(local)]);
				return NULL;
			}
		}
	}
	LeaveCriticalSection(&m_csRoomMap[Index(local)]);
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
const GameRooms::RoomInfo* GameRooms::QuickBegin( uint local, SmartPtr<ClientContext>& client )
{
	EnterCriticalSection(&m_csRoomMap[Index(local)]);
	if( 0 == m_RoomInfoMap[Index(local)].size() )
	{
		LeaveCriticalSection(&m_csRoomMap[Index(local)]);
		return NULL;
	}
	else
	{
		for( RoomMap::reverse_iterator rit = m_RoomInfoMap[Index(local)].rbegin();
			rit != m_RoomInfoMap[Index(local)].rend();
			rit ++ )
		{
			if(		rit->second->m_Password.empty() 
				&&	! rit->second->isFull() //并且没有满
				&&  rit->second->isNotBegin() )
			{
				rit->second->m_PlayerCount ++;
				rit->second->m_EnterCount = 0;
				( rit->second->m_Players[0].m_Client == NULL ? rit->second->m_Players[0].m_Client : rit->second->m_Players[1].m_Client ) = client;
				PlayerLocalRoom plm;
				plm.m_local = local;
				plm.m_room = rit->second->m_ID;
				m_PlayerInfoMap.insert( make_pair( (int)client, plm ) );
				LeaveCriticalSection(&m_csRoomMap[Index(local)]);
				return (rit->second);
			}
		}
	}
	LeaveCriticalSection(&m_csRoomMap[Index(local)]);
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
GameRooms::RoomInfo* GameRooms::FindRoom( uint local, uint ID )
{
	EnterCriticalSection(&m_csRoomMap[Index(local)]);
	RoomMap::iterator it = m_RoomInfoMap[Index(local)].find( ID );
	if ( it == m_RoomInfoMap[Index(local)].end() )//没有找到房间
	{
		LeaveCriticalSection(&m_csRoomMap[Index(local)]);
		return NULL;
	}
	LeaveCriticalSection(&m_csRoomMap[Index(local)]);
	return it->second;
}
//-------------------------------------------------------------------------------------------------------
SmartPtr<ClientContext>* GameRooms::PlayerLeave( uint local, uint ID, SmartPtr<ClientContext>& client )
{
	EnterCriticalSection(&m_csRoomMap[Index(local)]);
	PlayerInfoMap::iterator pim = m_PlayerInfoMap.find( (int)client );
	if( pim != m_PlayerInfoMap.end() ) //去掉玩家信息
	{
		m_PlayerInfoMap.erase( pim );
	}
	RoomMap::iterator it = m_RoomInfoMap[Index(local)].find( ID );
	if ( it == m_RoomInfoMap[Index(local)].end() )//没有找到房间
	{
		LeaveCriticalSection(&m_csRoomMap[Index(local)]);
		return NULL;
	}
	else
	{
		if( it->second->m_Players[0].isPlayer( client ) )
		{
			it->second->m_PlayerCount --;
			it->second->m_EnterCount = 0;
			it->second->m_Players[0].m_PlayerName.clear();
			it->second->m_Players[0].m_Win = 0;
			it->second->m_Players[0].m_Lose = 0;
			it->second->m_Players[0].m_UDID.clear();
			it->second->m_Players[0].m_Client = NULL;
			//it->second->m_Players[0].m_UDPAddress.Clear();
			if( 0 == it->second->m_PlayerCount )
			{
				m_RoomInfoMap[Index(local)].erase( ID );
				DEBUGLOG("Erase Room with ID %d", ID);
				LeaveCriticalSection(&m_csRoomMap[Index(local)]);
				return NULL;
			}
			else
			{
				LeaveCriticalSection(&m_csRoomMap[Index(local)]);
				return &it->second->m_Players[1].m_Client;
			}
		}
		else if ( it->second->m_Players[1].isPlayer( client ) )
		{
			it->second->m_PlayerCount --;
			it->second->m_EnterCount = 0;
			it->second->m_Players[1].m_PlayerName.clear();
			it->second->m_Players[1].m_Win = 0;
			it->second->m_Players[1].m_Lose = 0;
			it->second->m_Players[1].m_UDID.clear();
			it->second->m_Players[1].m_Client = NULL;
			//it->second->m_Players[1].m_UDPAddress.Clear();
			if( 0 == it->second->m_PlayerCount )
			{
				m_RoomInfoMap[Index(local)].erase( ID );
				DEBUGLOG("Erase Room with ID %d", ID);
				LeaveCriticalSection(&m_csRoomMap[Index(local)]);
				return NULL;
			}
			else
			{
				LeaveCriticalSection(&m_csRoomMap[Index(local)]);
				return &it->second->m_Players[0].m_Client;
			}
		}
	}
	LeaveCriticalSection(&m_csRoomMap[Index(local)]);
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
GameRooms::Player* GameRooms::FindPlayer( uint local, uint ID, SmartPtr<ClientContext>& client )
{
	EnterCriticalSection(&m_csRoomMap[Index(local)]);
	RoomMap::iterator it = m_RoomInfoMap[Index(local)].find( ID );
	if ( it == m_RoomInfoMap[Index(local)].end() ) //没有找到房间
	{
		LeaveCriticalSection(&m_csRoomMap[Index(local)]);
		return NULL;
	}
	else
	{
		if( it->second->m_Players[0].isPlayer( client ) )
		{
			LeaveCriticalSection(&m_csRoomMap[Index(local)]);
			return &it->second->m_Players[0];
		}
		else if ( it->second->m_Players[1].isPlayer( client ) )
		{
			LeaveCriticalSection(&m_csRoomMap[Index(local)]);
			return &it->second->m_Players[1];
		}
	}
	LeaveCriticalSection(&m_csRoomMap[Index(local)]);
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
GameRooms::PlayerLocalRoom* GameRooms::FindPlayerLocalRoom( SmartPtr<ClientContext>& client )
{
	EnterCriticalSection(&m_csPlayerMap);
	PlayerInfoMap::iterator pim = m_PlayerInfoMap.find( (int)client );
	if( pim != m_PlayerInfoMap.end() ) //去掉玩家信息
	{
		LeaveCriticalSection(&m_csPlayerMap);
		return &(pim->second);
	}
	LeaveCriticalSection(&m_csPlayerMap);
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
GameRooms::RoomInfo* GameRooms::EntryOne( uint local, uint ID )
{
	EnterCriticalSection(&m_csRoomMap[Index(local)]);
	RoomMap::iterator it = m_RoomInfoMap[Index(local)].find( ID );
	if ( it != m_RoomInfoMap[Index(local)].end() )//找到房间
	{
		it->second->m_EnterCount++;
		if ( PLAYERS == it->second->m_EnterCount)
		{
			DEBUGLOG("Room %d, is all entry", it->second->m_ID );
			LeaveCriticalSection(&m_csRoomMap[Index(local)]);
			return it->second;
		}
	}
	LeaveCriticalSection(&m_csRoomMap[Index(local)]);
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
void GameRooms::DeleteRoom( uint local, uint ID )
{
	EnterCriticalSection(&m_csRoomMap[Index(local)]);
	RoomMap::iterator it = m_RoomInfoMap[Index(local)].find( ID );
	if ( it != m_RoomInfoMap[Index(local)].end() )//找到房间
	{
		SAFE_DELETE( it->second );
		m_RoomInfoMap[Index(local)].erase( it );
	}
	LeaveCriticalSection(&m_csRoomMap[Index(local)]);
}
//-------------------------------------------------------------------------------------------------------
void GameRooms::RoomInfo::OnGameOver( SmartPtr<ClientContext>& client, SQLDataBase& sql )
{
	if( this->m_isGameOver == false && this->m_isNoBegin == false ) //游戏没有结束
	{
		this->m_isGameOver = true;//游戏结束了
		int otherindex = -1;
		if( this->m_Players[0].isPlayer( client ) )
		{
			otherindex = 1;
		}
		else if( this->m_Players[1].isPlayer( client ) )
		{
			otherindex = 0;
		}
		{
			stringstream str;
			str<<"SELECT * FROM Players WHERE ID='"<<client->PlayerIndex()<<"'";
			sql.BeginQuery( str.str() );
			if( sql.hasRecord() )//找到玩家信息了
			{
				uint lose = sql.GetValue("Lose");
				lose ++;
				sql.SetValue( "Lose", lose );
			}
			sql.End();
			if( this->m_Players[otherindex].m_Client != NULL )
			{
				str.str("");
				str<<"SELECT * FROM Players WHERE ID='"<<this->m_Players[otherindex].m_Client->PlayerIndex()<<"'";
				sql.BeginQuery( str.str() );
				if( sql.hasRecord() )//找到玩家信息了
				{
					uint win = sql.GetValue("Win");
					win ++;
					sql.SetValue( "Win", win );
				}
				sql.End();
			}
		}
	}
}
