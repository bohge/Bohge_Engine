//////////////////////////////////////
//			处理网络请求		   //
/////////////////////////////////////

#pragma once
#include "IMessageHandle.h"
#include "Utility.h"

class RoomHandle : public IMessageHandle
{
private:
	//HANDLE				m_hUDPThreads;
public:
	RoomHandle(void);
	~RoomHandle(void);
	virtual bool HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );

	// 线程函数，为IOCP请求服务的工作者线程
	//static DWORD WINAPI UDPThread(LPVOID lpParam);
};
