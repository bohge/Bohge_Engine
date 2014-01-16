//////////////////////////////////////
//			´¦ÀíÍøÂçÇëÇó		   //
/////////////////////////////////////

#pragma once
#include "IMessageHandle.h"
#include <vector>


class GameHandle : public IMessageHandle
{
private:
	std::vector<int>	m_GameRule;
public:
	GameHandle(void);
	~GameHandle(void);
	virtual bool HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );
};
