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
#include "BohgeNet.h"
#include "NetHost.h"
#include "NetBase.h"
#include "Correspondent.h"



using namespace std;


namespace BohgeNet
{
	//-------------------------------------------------------------------------------------------------------
	NetHost::NetHost(void)
		:m_pTcp(NULL),
		m_pUdp(NULL),
		m_isTCPConnecting(false),
		m_isConnectingTcp(false)
	{
		m_pLocalPackage = NET_NEW NetPackage;
		m_pUdp = NET_NEW UDPCorrespondent();
		m_pTcp = NET_NEW TCPCorrespondent();
	}
	//-------------------------------------------------------------------------------------------------------
	NetHost::~NetHost(void)
	{
		SAFE_DELETE(m_pLocalPackage);
		SAFE_DELETE(m_pTcp);
		SAFE_DELETE(m_pUdp);
	}
	//-------------------------------------------------------------------------------------------------------
	void NetHost::ConnectTCP( const Address& server_address)
	{
		m_isConnectingTcp = true;
		if( false == m_isTCPConnecting 
			&& ( m_pTcp->ServerAddress() != server_address //这里如果是一个地址就不重复发送了
			|| !m_pTcp->isReady() 
			|| !m_pTcp->isConnect()
			)
			)
		{
			m_isTCPConnecting = true;
			TCPConnect* tcp = NET_NEW TCPConnect;
			tcp->m_isDomain = false;
			tcp->m_Tcp = m_pTcp;
			tcp->m_isProcess = &m_isTCPConnecting;
			tcp->m_Addres = server_address;
			pthread_create(&m_TcpConnectThread,NULL, NetHost::TCPConncetThread, tcp);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void NetHost::ConnectTCP( const Domain& domain )
	{
		m_isConnectingTcp = true;
		if( false == m_isTCPConnecting 
			&&	( m_pTcp->ServerDomain() != domain//这里暂时不做判断了
			|| !m_pTcp->isReady() 
			|| !m_pTcp->isConnect()
			)
			)
		{
			m_isTCPConnecting = true;
			TCPConnect* tcp = NET_NEW TCPConnect;
			tcp->m_isDomain= true;
			tcp->m_Tcp = m_pTcp;
			tcp->m_isProcess = &m_isTCPConnecting;
			tcp->m_Domain = domain;
			pthread_create(&m_TcpConnectThread,NULL, NetHost::TCPConncetThread, tcp);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool NetHost::ConnectUDP( const Address& server_address)
	{
		return m_pUdp->ConnectWithAddress( server_address );
	}
	//-------------------------------------------------------------------------------------------------------
	bool BohgeNet::NetHost::ConnectUDP( const Domain& domain )
	{
		return m_pUdp->ConnectWithDomain( domain );
	}
	//-------------------------------------------------------------------------------------------------------

	void NetHost::Initialization()
	{
#if PLATFORM == PLATFORM_WINDOWS
		WSADATA WsaData;
		m_isInitialized = WSAStartup( MAKEWORD(2,2), &WsaData ) == NO_ERROR;
#else
		signal(SIGPIPE, SIG_IGN); //lan jie SIGPIPE
		m_isInitialized = true;
#endif
	}
	//-------------------------------------------------------------------------------------------------------
	void NetHost::Destry()
	{
		m_pTcp->Close();
		m_pUdp->Close();
#if PLATFORM == PLATFORM_WINDOWS
		WSACleanup();
#endif
	}
	//-------------------------------------------------------------------------------------------------------
	void NetHost::OnFarmeBegin()
	{
		if( m_pTcp->isReady() )
		{
			while( m_pTcp->Receive( *m_pLocalPackage ) )
			{
				m_TcpEvent.Multicast( *m_pTcp, *m_pLocalPackage);
			}
		}
		if( m_pUdp->isReady() )
		{
			while( m_pUdp->Receive( *m_pLocalPackage ) )
			{
				m_UdpEvent.Multicast( *m_pUdp, *m_pLocalPackage);
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void NetHost::OnFarmeEnd()
	{
		if( m_pTcp->isReady() 
			&& !m_isTCPConnecting )
		{
			m_pTcp->OnFrameEnd();
		}
		if( m_pUdp->isReady() )
		{
			m_pUdp->OnFrameEnd();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void* BohgeNet::NetHost::TCPConncetThread( void* prama )
	{
		MilisecondTimer timer;
		TCPConnect* Tcp = static_cast<TCPConnect*>(prama);
		if( Tcp->m_isDomain )
		{
			Tcp->m_Tcp->ConnectWithDomain( Tcp->m_Domain );
		}
		else
		{
			Tcp->m_Tcp->ConnectWithAddress( Tcp->m_Addres );
		}
		int time = timer.GetMilisecondTimeSpan();
		time = max( 1000 - time, 0);
#ifdef WIN32
		Sleep(time);
#else
		usleep( time * 1000 );//延迟1秒，服务器响应
#endif
		(*Tcp->m_isProcess) = false;
		SAFE_DELETE( Tcp );
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	void BohgeNet::NetHost::DisConnectTCP()
	{
		if( m_pTcp->isReady() )
		{
			m_pTcp->Close();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void BohgeNet::NetHost::DisConnectUDP()
	{
		if( m_pUdp->isReady() )
		{
			m_pUdp->Close();
		}
	}
}