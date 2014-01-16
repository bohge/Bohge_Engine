//////////////////////////////////////
//			处理网络请求		   //
/////////////////////////////////////
#include "stdafx.h"
#include "BohgeNet.h"
#include "GameHandle.h"
#include "Base.h"
#include "NetBase.h"
#include "ClientContext.h"
#include "GameRooms.h"
#include "tinyxml.h"
#include <string>




using namespace std;
using namespace BohgeNet;

//-------------------------------------------------------------------------------------------------------
GameHandle::GameHandle(void)
{
	TCHAR strFilePath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, strFilePath, MAX_PATH);
	(strrchr(strFilePath, ('\\')))[1] = 0;
	SetCurrentDirectory( strFilePath );
	string path(strFilePath);
	path += "Game.xml";
	TiXmlDocument *myDocument = new TiXmlDocument( path.c_str() );
	myDocument->LoadFile();
	TiXmlElement* RootElement = myDocument->RootElement();

	TiXmlElement* LiveCount = RootElement->FirstChildElement()->NextSiblingElement();

	TiXmlElement* LiveN1 = LiveCount->NextSiblingElement();
	TiXmlElement* LiveN2 = LiveN1->NextSiblingElement();
	TiXmlElement* LiveC2 = LiveN2->NextSiblingElement();
	TiXmlElement* LiveN3 = LiveC2->NextSiblingElement();
	TiXmlElement* LiveC3 = LiveN3->NextSiblingElement();
	TiXmlElement* LiveN4 = LiveC3->NextSiblingElement();
	TiXmlElement* LiveC4 = LiveN4->NextSiblingElement();
	TiXmlElement* LiveC5 = LiveC4->NextSiblingElement();

	TiXmlElement* IceN1 = LiveC5->NextSiblingElement();
	TiXmlElement* IceN2 = IceN1->NextSiblingElement();
	TiXmlElement* IceC2 = IceN2->NextSiblingElement();
	TiXmlElement* IceN3 = IceC2->NextSiblingElement();
	TiXmlElement* IceC3 = IceN3->NextSiblingElement();
	TiXmlElement* IceN4 = IceC3->NextSiblingElement();
	TiXmlElement* IceC4 = IceN4->NextSiblingElement();
	TiXmlElement* IceC5 = IceC4->NextSiblingElement();

	TiXmlElement* LavaN1 = IceC5->NextSiblingElement();
	TiXmlElement* LavaN2 = LavaN1->NextSiblingElement();
	TiXmlElement* LavaC2 = LavaN2->NextSiblingElement();
	TiXmlElement* LavaN3 = LavaC2->NextSiblingElement();
	TiXmlElement* LavaC3 = LavaN3->NextSiblingElement();
	TiXmlElement* LavaN4 = LavaC3->NextSiblingElement();
	TiXmlElement* LavaC4 = LavaN4->NextSiblingElement();
	TiXmlElement* LavaC5 = LavaC4->NextSiblingElement();

	m_GameRule.push_back( atoi( LiveCount->Attribute( "Volume" ) ) );

	m_GameRule.push_back( atoi( LiveN1->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LiveN2->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LiveC2->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LiveN3->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LiveC3->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LiveN4->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LiveC4->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LiveC5->Attribute( "Volume" ) ) );

	m_GameRule.push_back( atoi( IceN1->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( IceN2->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( IceC2->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( IceN3->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( IceC3->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( IceN4->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( IceC4->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( IceC5->Attribute( "Volume" ) ) );

	m_GameRule.push_back( atoi( LavaN1->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LavaN2->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LavaC2->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LavaN3->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LavaC3->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LavaN4->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LavaC4->Attribute( "Volume" ) ) );
	m_GameRule.push_back( atoi( LavaC5->Attribute( "Volume" ) ) );

	delete(myDocument);
}
//-------------------------------------------------------------------------------------------------------
GameHandle::~GameHandle(void)
{
}
//-------------------------------------------------------------------------------------------------------
bool GameHandle::HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql )
{
	pak.Begin();
	int message;
	pak>>message;
	switch (message)
	{
	case NM_GAEM_IM_READY:
		{
			int local,id;
			pak>>local>>id;
			GameRooms::RoomInfo* room = GameRooms::Instance().EntryOne(local,id);
			if( NULL != room )
			{
				if( !room->m_isGameOver )
				{
					pak.Begin();
					pak<<NM_GAEM_IM_READY;
					for ( int i = 0 ; i < m_GameRule.size() ; i ++)
					{
						pak<<m_GameRule[i];
					}
					room->m_Players[0].m_Client->Send( pak );
					room->m_Players[1].m_Client->Send( pak );
					room->m_Players[0].m_Client->SendImmediately();
					room->m_Players[1].m_Client->SendImmediately();
				}
			}
		}return true;
	case NM_GAME_SKILL_DATA:
		{
			int local,id;
			pak>>local>>id;
			GameRooms::RoomInfo* room = GameRooms::Instance().FindRoom(local,id);
			if( NULL != room )
			{
				if( !room->m_isGameOver )
				{
					if( room->isFull() )
					{
						if( room->m_Players[0].isPlayer( client ) )
						{
							DEBUGLOG( "Send NM_GAME_SKILL_DATA to %s:%d", room->m_Players[1].m_Client->BindIPString(), room->m_Players[1].m_Client->BindPort() );
							room->m_Players[1].m_Client->Send( pak );
							room->m_Players[1].m_Client->SendImmediately();
						}
						else if( room->m_Players[1].isPlayer( client ) )
						{
							DEBUGLOG( "Send NM_GAME_SKILL_DATA to %s:%d", room->m_Players[0].m_Client->BindIPString(), room->m_Players[1].m_Client->BindPort() );
							room->m_Players[0].m_Client->Send( pak );
							room->m_Players[0].m_Client->SendImmediately();
						}
					}
				}
				else
				{
					pak.Begin();
					pak<<NM_GAME_DEAD<<local<<id;
					client->Send( pak );
				}
			}
		}return true;
	case NM_GAME_DEAD:
		{
			int local,id;
			pak>>local>>id;
			GameRooms::RoomInfo* room = GameRooms::Instance().FindRoom(local,id);
			if( NULL != room )
			{
				room->OnGameOver(client, sql);//游戏结束了
				if( room->m_Players[0].isPlayer( client ) )
				{
					DEBUGLOG( "Send NM_GAME_DEAD to %s:%d", room->m_Players[1].m_Client->BindIPString(), room->m_Players[1].m_Client->BindPort() );
					room->m_Players[1].m_Client->Send( pak );
					room->m_Players[1].m_Client->SendImmediately();
				}
				else if( room->m_Players[1].isPlayer( client ) )
				{
					DEBUGLOG( "Send NM_GAME_DEAD to %s:%d", room->m_Players[0].m_Client->BindIPString(), room->m_Players[1].m_Client->BindPort() );
					room->m_Players[0].m_Client->Send( pak );
					room->m_Players[0].m_Client->SendImmediately();
				}
			}
		}return true;
	case NM_GAME_LEAVE: //转发这个消息
		{
			int local,id;
			pak>>local>>id;
			GameRooms::RoomInfo* room = GameRooms::Instance().FindRoom(local,id);
			if( NULL != room )
			{
				room->OnGameOver(client, sql);
				SmartPtr<ClientContext>* others = GameRooms::Instance().PlayerLeave( local, id, client );
				if( NULL != others )
				{
					//通知另一个玩家，对方离开了
					(*others)->Send( pak );
					(*others)->SendImmediately();
				}
			}
		}return true;

	}
	return false;
}
