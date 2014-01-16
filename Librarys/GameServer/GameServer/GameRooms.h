//////////////////////////////////////
//			玩家建立的房间		   //
/////////////////////////////////////

#pragma once
#include "ServerConfig.h"
#include "BohgeNet.h"
#include "AutoIndex.h"
#include "ClientContext.h"
#include "NetBase.h"
#include "GameSetting.h"
#include <string>
#include <map>



class SQLDataBase;
class GameRooms
{
public:
	enum
	{
		PLAYERS = 2,
	};
public:
	struct PlayerLocalRoom	//当玩家非正常退出的时候，需要这个来检索信息判断是否需要对房间操作
	{
		uint	m_local;
		uint	m_room;
	};
	struct Player
	{
		std::string				m_UDID;
		uint					m_Win;
		uint					m_Lose;
		std::wstring			m_PlayerName;
		SmartPtr<ClientContext>	m_Client;
		//BohgeNet::Address		m_UDPAddress;//udp的端口
		Player()
			:m_Client(NULL)
		{
		}
		inline bool isPlayer( SmartPtr<ClientContext>& client ) const
		{
			if( m_Client == NULL )
			{
				return false;
			}
			return m_Client->isClient( client );
		}
	};
	struct RoomInfo
	{
	private:
		bool				m_isNoBegin;//房间游戏是否开始
	public:
		bool				m_isGameOver;
		bool				m_isReady;
		uint				m_ID; //房间ID
		uint				m_PlayerCount;
		uint				m_EnterCount;//进入游戏人数记录
		std::wstring		m_Name; //房间民称
		std::wstring		m_Password;
		Player				m_Players[PLAYERS];
		RoomInfo()
			:m_ID(0),
			m_PlayerCount(1),
			m_isReady(false),
			m_EnterCount(0),
			m_isNoBegin(true),
			m_isGameOver(false)
		{
		}
		bool isFull() const
		{
			return m_PlayerCount >= PLAYERS;
		}
		void RoomStart()
		{
			m_isNoBegin = false;
		}
		bool isNotBegin() const
		{
			return m_isNoBegin;
		}
		void OnGameOver( SmartPtr<ClientContext>& client, SQLDataBase& sql );
	};
public:
	typedef std::map<uint,RoomInfo*>		RoomMap;
private:
	typedef std::map<int, PlayerLocalRoom>		PlayerInfoMap;
private:
	AutoIndex			m_RoomID;//每个房间的ID
	RoomMap				m_RoomInfoMap[LOCATION_COUNT];//准备N个服务器地区的room
	PlayerInfoMap		m_PlayerInfoMap;
	CRITICAL_SECTION	m_csRoomMap[LOCATION_COUNT];// 用于Worker线程同步的互斥量
	CRITICAL_SECTION	m_csPlayerMap;// 用于Worker线程同步的互斥量
public:
	GameRooms(void);
	~GameRooms(void);
public:
	static inline GameRooms& Instance()
	{
		static GameRooms gr;
		return gr;
	}
public:
	const RoomInfo& NewRoom( uint local, const std::wstring& roomname, const std::wstring& password, SmartPtr<ClientContext>& client );
	const RoomInfo* EntryRoom( uint local, uint ID, const std::wstring& password, SmartPtr<ClientContext>& client );
	const RoomInfo* FindEntryRoom( uint local, uint ID, SmartPtr<ClientContext>& client, bool& isFind );//找到而且进入
	const RoomInfo* QuickBegin( uint local, SmartPtr<ClientContext>& client );
	RoomInfo* FindRoom( uint local, uint ID );//只找到，不做别的处理
	Player* FindPlayer( uint local, uint ID, SmartPtr<ClientContext>& client );
	SmartPtr<ClientContext>* PlayerLeave( uint local, uint ID, SmartPtr<ClientContext>& client );//处理玩家离开，返回没有离开的玩家
	PlayerLocalRoom* FindPlayerLocalRoom( SmartPtr<ClientContext>& client );
	RoomInfo* EntryOne( uint local, uint ID );//有一个进入游戏了
	void DeleteRoom( uint local, uint ID );
private:
	inline int Index( uint local )
	{
		switch(local)
		{
		case LOCATION_ASIA: return 0;
		case LOCATION_EURO: return 1;
		case LOCATION_USA:  return 2;
		}
		return 0;
	}
public:
	inline void RoomMapBegin( uint local )
	{
		EnterCriticalSection(&m_csRoomMap[Index(local)]);
	}
	inline RoomMap& GetRoomMap( uint local ) //仅为刷新房间使用，其他时候不要调用
	{
		return m_RoomInfoMap[ Index(local) ];
	}
	inline void RoomMapEnd( uint local )
	{
		LeaveCriticalSection(&m_csRoomMap[Index(local)]);
	}
};
