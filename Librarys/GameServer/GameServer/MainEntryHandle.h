//////////////////////////////////////
//			处理网络请求		   //
/////////////////////////////////////

#pragma once
#include "IMessageHandle.h"
#include "Utility.h"

class MainEntryHandle : public IMessageHandle
{
private:
	int				m_AddTimeDay;
	YYYYMMDD		m_Date;//日期
public:
	MainEntryHandle(void);
	~MainEntryHandle(void);
	virtual bool HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );
};
