//////////////////////////////////////
//			´¦ÀíÍøÂçÇëÇó		   //
/////////////////////////////////////

#pragma once
#include "ServerConfig.h"
#include "IMessageHandle.h"
#include "Utility.h"
#include <map>


class LobbyHandle : public IMessageHandle
{
public:
	LobbyHandle(void);
	~LobbyHandle(void);
	virtual bool HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );
public:
};
