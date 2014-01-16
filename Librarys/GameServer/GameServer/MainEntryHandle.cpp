//////////////////////////////////////
//			处理网络请求		   //
/////////////////////////////////////
#include "stdafx.h"
#include "Base.h"
#include "ServerConfig.h"
#include "MainEntryHandle.h"
#include "NetBase.h"
#include "ClientContext.h"
#include "SQLDataBase.h"
#include "GameSetting.h"
#include "tinyxml.h"

#include <sstream>
#include <string>


using namespace std;
using namespace BohgeNet;


//-------------------------------------------------------------------------------------------------------
MainEntryHandle::MainEntryHandle(void)
{
	TCHAR strFilePath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, strFilePath, MAX_PATH);
	(strrchr(strFilePath, ('\\')))[1] = 0;
	SetCurrentDirectory( strFilePath );
	string path(strFilePath);
	path += "Game.xml";
	TiXmlDocument *myDocument = NET_NEW TiXmlDocument( path.c_str() );
	myDocument->LoadFile();
	TiXmlElement* RootElement = myDocument->RootElement();
	TiXmlElement* AddFreeTime = RootElement->FirstChildElement();
	m_AddTimeDay = atoi( AddFreeTime->Attribute( "Volume" ) );
	SAFE_DELETE( myDocument );
}
//-------------------------------------------------------------------------------------------------------
MainEntryHandle::~MainEntryHandle(void)
{
}
//-------------------------------------------------------------------------------------------------------
bool MainEntryHandle::HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql )
{
	pak.Begin();
	int message;
	pak>>message;
	switch (message)
	{
	case NM_CONNECT_SERVER:
		{
			DEBUGLOG("Receive from %s  NM_CONNECT_SERVER\n", client->BindIPString() );
			int vision;
			pak>>vision;
			if( SERVER_VISION == vision )
			{
				pak.Begin();
				pak<<NM_SERVER_NOTICE<<true<<wstring(L"Test Notice");
			}
			else
			{
				pak.Begin();
				pak<<NM_SERVER_NOTICE<<true<<wstring(L"Need Updata");
			}
			client->Send( pak );
			return true;
		}return true;
	case NM_LOBBY_INFO:
		{
			DEBUGLOG("Receive from %s  NM_LOBBY_INFO\n", client->BindIPString() );
			int vision;
			pak>>vision;
			if( SERVER_VISION == vision )
			{
				string udid;
				pak>>udid;
				stringstream str;
				str<<"SELECT * FROM Players WHERE UDID='"<<udid<<"'";
				sql.BeginQuery( str.str() );
				if( sql.hasRecord() )//找到玩家信息了
				{
					client->PlayerIndex() = sql.GetValue("ID");
					wstring strname = (_bstr_t)sql.GetValue("Player");
					int buytimes = sql.GetValue("BuyTimes");
					int freetimes = sql.GetValue("FreeTimes");
					int Location = sql.GetValue("Location");
					uint win = sql.GetValue("Win");
					uint lose = sql.GetValue("Lose");
					DEBUGLOG("Find Player %s with Freetimes %d, payTime %d\n" , udid.c_str(), freetimes, buytimes);
					if( 0 == freetimes )//如果没有免费时间了,计算是否需要加一次
					{
						int getfree = sql.GetValue("GetFree");
						int now = m_Date.YearMonthDay();
						DEBUGLOG("No Free Times Last Get on %d, Now is %d\n", getfree, now);
						if( ( now - getfree ) >= m_AddTimeDay)
						{
							freetimes = 1;
							sql.SetValue( "FreeTimes", 1);
							sql.SetValue( "GetFree", now);
						}
					}
					pak.Begin();
					if( 0 >= (freetimes+buytimes) )//没有次数了，就不让玩了
					{
						pak<<NM_NO_GAME_TIMES<<m_AddTimeDay;
					}
					else
					{
						pak<<NM_LOBBY_PORT<<true<<strname<<buytimes+freetimes<<Location<<win<<lose;//允许进入
					}
				}
				else//如果没有玩家信息，提示输入新的信息
				{
					pak.Begin();
					pak<<NM_NEW_USER;
				}
				sql.End();
			}
			else
			{
				pak.Begin();
				pak<<NM_LOBBY_PORT<<false;//版本不同，禁止进入
			}
			client->Send( pak );
			return true;
		}return true;
	case NM_USER_DATA:
		{
			string udid;
			wstring name;
			int local;
			pak>>udid>>name>>local;
			stringstream str;
			str<<"SELECT * FROM Players WHERE UDID='"<<udid<<"'";
			sql.BeginQuery( str.str() );
			if( !sql.hasRecord() )//没找到玩家信息了
			{
				if( udid.size() > 64 )
				{
					DEBUGLOG("some one udid big than 64");
					udid.erase(64, udid.size() );
				}
				DEBUGLOG("New Player %s\n" , udid.c_str());
				sql.AddNewRecord();
				sql.InsertValue( "UDID",_variant_t((_bstr_t)udid.c_str()) );
				if( name.size() > 16 )
				{
					name.erase(16, name.size() );
				}
				sql.InsertValue( "Player",_variant_t((_bstr_t)name.c_str()) );
				sql.InsertValue( "BuyTimes",_variant_t(4) );//出来给4次免费机会吧
				sql.InsertValue( "FreeTimes",_variant_t(1) );
				sql.InsertValue( "GetFree",_variant_t( m_Date.YearMonthDay()) );
				sql.InsertValue( "Location",_variant_t(local) );
				sql.InsertValue( "Win",_variant_t(0) );
				sql.InsertValue( "Lose",_variant_t(0) );
			}
			sql.End();
			sql.BeginQuery( str.str() );
			if( sql.hasRecord() )//找到玩家信息了
			{
				client->PlayerIndex() = sql.GetValue("ID");
				int freetimes = sql.GetValue("FreeTimes");
				int buytimes = sql.GetValue("BuyTimes");
				uint win = sql.GetValue("Win");
				uint lose = sql.GetValue("Lose");
				pak.Begin();
				pak<<NM_LOBBY_PORT<<true<<name<<freetimes + buytimes<<local<<win<<lose;//允许进入
			}
			else
			{
				pak.Begin();
				pak<<NM_NEW_USER;
			}
			client->Send( pak );
			sql.End();
			return true;
		}return true;
	case NM_CHANGE_LOCATION:
		{
			DEBUGLOG("Receive from %s  NM_CHANGE_LOCATION\n", client->BindIPString() );
			string udid;
			wstring name;
			int location;
			pak>>udid>>name>>location;
			stringstream str;
			str<<"SELECT * FROM Players WHERE UDID='"<<udid<<"'";
			sql.BeginQuery( str.str() );
			if( sql.hasRecord() )
			{
				sql.SetValue( "Location", (byte)location );
				sql.SetValue( "Player", (_bstr_t)name.c_str() );
				int buytimes = sql.GetValue("BuyTimes");
				int freetimes = sql.GetValue("FreeTimes");
				uint win = sql.GetValue("Win");
				uint lose = sql.GetValue("Lose");
				pak.Begin();
				if( 0 >= (freetimes+buytimes) )//没有次数了，就不让玩了
				{
					pak<<NM_NO_GAME_TIMES<<m_AddTimeDay;
				}
				else
				{
					pak<<NM_LOBBY_PORT<<true<<name<<buytimes+freetimes<<location<<win<<lose;//允许进入
				}
			}
			sql.End();
			client->Send( pak );
			return true;
		}return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------