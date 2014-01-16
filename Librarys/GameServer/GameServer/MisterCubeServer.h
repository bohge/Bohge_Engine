//////////////////////////////////
//			ÍøÂçÏß³Ì		   //
/////////////////////////////////
#pragma once

#include "NetBase.h"
#include "IMessageHandle.h"
#include <list>
#include <map>



class RoomHandle;
class GameHandle;
class LobbyHandle;
class ReceiptHandle;
class MainEntryHandle;
class ClientContext;
class MisterCubeServer
{
private:
	typedef std::list<IMessageHandle*>	HandleList;
private:
	HandleList			m_HandlePtrList;
	RoomHandle*			m_pRoom;
	GameHandle*			m_pGame;
	LobbyHandle*		m_pLobby;
	ReceiptHandle*		m_pReceipt;
	MainEntryHandle*	m_pMainEntry;
private:
	MisterCubeServer(void);
	~MisterCubeServer(void);
public:
	static inline MisterCubeServer& Instance()
	{
		static MisterCubeServer m;
		return m;
	}
public:
	bool DoMessage( SmartPtr<ClientContext>& c, BohgeNet::NetPackage& pak, SQLDataBase& sql );
};
