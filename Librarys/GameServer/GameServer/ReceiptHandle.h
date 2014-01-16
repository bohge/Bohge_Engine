//////////////////////////////////////
//			处理网络请求		   //
/////////////////////////////////////

#pragma once
#include "IMessageHandle.h"
#include "Utility.h"
#include "json.h"


#include <string>
#include <map>
#include <list>


class CrackedUrl;
class ReceiptHandle : public IMessageHandle
{
private:
	struct ReslutData
	{
		SmartPtr<ClientContext>		m_Client;
		std::string			m_UDID;
		bool				m_isSucess;//服务器是否返回数据
		bool				m_isRealReceipt;//是否是正确的付款的订单
		Json::Value			m_ReslutJson;
	};
	struct ReceiptFragment//内部数据
	{
		ushort							m_PacketCount;//分包总数
		std::map<ushort, std::string>	m_Fragment;//分包
	};
	struct ReceiptVerifyData
	{
		CrackedUrl*		m_pURL;
		std::string		m_Receipt;
		ReslutData*		m_pReslut;
	};
private:
	typedef std::map<std::string, ReceiptFragment> ReceiptMap;
	typedef std::list< ReslutData* > Reslutlist;
private:
	CrackedUrl*			m_pUrlResolve;
	ReceiptMap			Receipts;
	Reslutlist			m_Reslutlist;
	CRITICAL_SECTION	m_csReslutlist;// 用于Worker线程同步的互斥量
	HANDLE				m_ProcessEvent;//处理返回数据的事件
	HANDLE				m_ReslutThreads;
public:
	ReceiptHandle(void);
	~ReceiptHandle(void);
	virtual bool HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );
	static DWORD WINAPI HttpReceiptVerifyData(LPVOID lpParam);
	static DWORD WINAPI PorcessReslut(LPVOID lpParam);
	void InsertReslut( ReslutData* data );
	ReslutData* GetReslutData();
	inline HANDLE GetEvent()
	{
		return m_ProcessEvent;
	}
	inline CRITICAL_SECTION GetMutex()
	{
		return m_csReslutlist;
	}
};
