//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



/////////////////////////
//		网络宿主	   //
/////////////////////////
#pragma once
#include "Correspondent.h"
#include "Event.hpp"
#include <string>
#include <list>
#include <string>
#include <pthread.h>


//宿主类，初始化通讯，提供通讯管道
namespace BohgeNet
{
	enum ICorrespondentType
	{
		CT_UDP,
		CT_TCP,
	};
	class NetHost //这个类整个都为客户端考虑的，服务器就不用了
	{
	private:
		struct TCPConnect
		{
			bool*				m_isProcess;
			bool				m_isDomain;
			TCPCorrespondent*	m_Tcp;
			Address				m_Addres;
			Domain				m_Domain;
		};
	private:
		bool											m_isInitialized;
		NetPackage*										m_pLocalPackage;
		bool											m_isConnectingTcp;//是否调用过TCP连接，没掉过就不能发送这个Debug用的
		bool											m_isTCPConnecting;//tcp的链接是异步，一个时间只能连接一个，第二个直接放弃了，暂时不搞那么麻烦
		pthread_t										m_TcpConnectThread;
		TCPCorrespondent*								m_pTcp;//客户端打开一个tcp
		Event2<bool, TCPCorrespondent&, NetPackage&>	m_TcpEvent;
		UDPCorrespondent*								m_pUdp;//客户端打开一个udp
		Event2<bool, UDPCorrespondent&, NetPackage&>	m_UdpEvent;
	public:
		NetHost(void);
		~NetHost(void);
		void Initialization();//每一个线程都需要初始化一次，所以提出该函数
		void Destry();//对应初始化
		void OnFarmeBegin();//让Driver调用,收取消息
		void OnFarmeEnd();//让Driver调用,发送消息
		static void* TCPConncetThread(void* prama);
	public:
		void ConnectTCP( const Address& server_address );//连接服务器
		bool ConnectUDP( const Address& server_address);//连接服务器
		void ConnectTCP( const Domain& domain );//连接服务器
		bool ConnectUDP( const Domain& domain );//连接服务器
		void DisConnectTCP();//断开连接
		void DisConnectUDP();
	public:
		BOHGE_FORCEINLINE FunctionPtr AddTCPResponse(IFunction2<bool, TCPCorrespondent&, NetPackage&>* const func)//添加接收到消息响应事件
		{
			return m_TcpEvent.Connect( func );
		}
		BOHGE_FORCEINLINE FunctionPtr AddUDPResponse(IFunction2<bool, UDPCorrespondent&, NetPackage&>* const func)
		{
			return m_UdpEvent.Connect( func );
		}
		BOHGE_FORCEINLINE void RemoveTCPResponse(FunctionPtr ptr)//清除
		{
			m_TcpEvent.Remove(ptr);
		}
		BOHGE_FORCEINLINE void RemoveUDPResponse(FunctionPtr ptr)
		{
			m_UdpEvent.Remove(ptr);
		}
		BOHGE_FORCEINLINE bool TCPSend( NetPackage& pak )
		{
			return m_pTcp->Send( pak );
		}
		BOHGE_FORCEINLINE bool UDPSend( NetPackage& pak )
		{
			if( m_pUdp->isReady() )
			{
				return m_pUdp->Send( pak );
			}
			return false;
		}
		BOHGE_FORCEINLINE NetPackage& Package()
		{
			return *m_pLocalPackage;
		}
		BOHGE_FORCEINLINE bool isTCPReady()
		{
			return m_pTcp->isReady();
		}
		BOHGE_FORCEINLINE bool isUDPReady()
		{
			return m_pUdp->isReady();
		}
	};
}
