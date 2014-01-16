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
//		通讯员		   //
/////////////////////////
#include "Correspondent.h"
#include "NetBase.h"


namespace BohgeNet
{



	//-------------------------------------------------------------------------------------------------------
	ICorrespondent::ICorrespondent()
		:m_isReady(false)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	ICorrespondent::~ICorrespondent()
	{

	}
	//-------------------------------------------------------------------------------------------------------
	bool ICorrespondent::ConnectWithDomain( const Domain& domain )
	{
		m_Domain = domain;
		return this->ConnectWithAddress( m_Domain.ToAddress() );
	}




	//////////////////////
	//		UDP数据		//
	//////////////////////
	UDPCorrespondent::UDPCorrespondent( ushort port )
		:m_SendID(0),
		m_ReciveID(0)
	{
		m_pSocket = NET_NEW SocketUDP( port );
		m_vBuffer.resize( UDPBUFFER_SIZE );
	}
	//-------------------------------------------------------------------------------------------------------
	UDPCorrespondent::UDPCorrespondent( )
		:m_SendID(0),
		m_ReciveID(0)
	{
		m_pSocket = NET_NEW SocketUDP( 0 );
		m_vBuffer.resize( UDPBUFFER_SIZE );
	}
	//-------------------------------------------------------------------------------------------------------
	UDPCorrespondent::~UDPCorrespondent(void)
	{
		SAFE_DELETE( m_pSocket );
	}
	//-------------------------------------------------------------------------------------------------------
	bool UDPCorrespondent::ConnectWithAddress( const Address& server )
	{
		m_SendID = 0;//转向要充值这个值
		m_ReciveID = 0;
		if( !m_pSocket->isConnect( ) )
		{
			m_pSocket->Connect( false );
			m_isReady = m_pSocket->isConnect();
		}
		m_Server = server;
		return m_isReady;
	}
	//-------------------------------------------------------------------------------------------------------
	void UDPCorrespondent::Close()
	{
		m_Server.Clear();
		m_isReady = false;
		m_pSocket->Close();
	}
	//-------------------------------------------------------------------------------------------------------
	bool UDPCorrespondent::Receive( NetPackage& pak )
	{
		uint RecevID(0);
		Address address;
		int recsize = m_pSocket->Receive( address, &m_vBuffer[0], m_vBuffer.size() );
		if( 0 == recsize )
		{
			return 0;
		}
		else // 查询通讯码
		{
			if( address != m_Server )//不是对方发来的
			{
				return 0;
			}
			else
			{
				int protoclID = m_vBuffer[0] << 24 | m_vBuffer[1] << 16 | m_vBuffer[2] << 8 | m_vBuffer[3];
				if( protoclID != PROTOCOL_ID ) //如果协议号码不一样
				{
					return 0;
				}
				else
				{
					RecevID = m_vBuffer[4] << 24 | m_vBuffer[5] << 16 | m_vBuffer[6] << 8 | m_vBuffer[7];
					if( RecevID < m_ReciveID //如果接受到的是过期的数据包裹
						&& RecevID != uint(-1) ) //uint(-1)是服务器发来的
					{
						return 0;
					}
				}
			}
		}
		m_ReciveID = RecevID;//改变到最新的ID
		recsize -= ( sizeof(ID) + sizeof(RecevID) ); //去除识别码和包裹ID的实际数据
		ASSERT( BUFFER_SIZE >= recsize );
		memcpy( pak.GetBuffer(),  &m_vBuffer[sizeof(ID) + sizeof(RecevID)], recsize);
		pak.Begin();
		return recsize > 0;
	}
	//-------------------------------------------------------------------------------------------------------
	bool UDPCorrespondent::Send( NetPackage& pak )
	{
		m_SendID++;
		const byte Pid[sizeof(uint)] =
		{
			(byte)(m_SendID >> 24),
			(byte)(m_SendID >> 16),
			(byte)(m_SendID >> 8),
			(byte)m_SendID
		};
		memcpy( &m_vBuffer[0], ID, sizeof(ID) );//拷贝识别号
		memcpy( &m_vBuffer[sizeof(ID)], Pid, sizeof(Pid) );//拷贝包裹号码
		memcpy( &m_vBuffer[sizeof(ID)+sizeof(Pid)], pak.GetBuffer(), pak.Size() );
		int SizeSended = ( pak.Size()+sizeof(ID)+sizeof(Pid) );
		return m_pSocket->Send( m_Server, &m_vBuffer[0], SizeSended );
	}



	//////////////////////
	//	UDP Server		//
	//////////////////////
	UDPServer::UDPServer( ushort port )
		:m_SendID( uint(-1) )
	{
		m_pSocket = NET_NEW SocketUDP( port );
		m_pSocket->Connect( true );
		m_vBuffer.resize( UDPBUFFER_SIZE );
	}
	//-------------------------------------------------------------------------------------------------------
	UDPServer::~UDPServer()
	{
		SAFE_DELETE( m_pSocket );
	}
	//-------------------------------------------------------------------------------------------------------
	bool UDPServer::RecevieFrom( Address& ad, NetPackage& pak)
	{
		int recsize = m_pSocket->Receive( ad, &m_vBuffer[0], m_vBuffer.size() );
		if( 0 == recsize )
		{
			return 0;
		}
		else // 查询通讯码
		{
			int protoclID = m_vBuffer[0] << 24 | m_vBuffer[1] << 16 | m_vBuffer[2] << 8 | m_vBuffer[3];
			if( protoclID != PROTOCOL_ID ) //如果协议号码不一样
			{
				return 0;
			}
		}
		recsize -= ( sizeof(ID) + sizeof(m_SendID) ); //去除识别码和包裹ID的实际数据
		ASSERT( BUFFER_SIZE >= recsize );
		memcpy( pak.GetBuffer(),  &m_vBuffer[sizeof(ID) + sizeof(m_SendID)], recsize);
		pak.Begin();
		return recsize > 0;
	}
	//-------------------------------------------------------------------------------------------------------
	bool UDPServer::SendTo( Address& ad, NetPackage& pak )
	{
		const byte Pid[sizeof(uint)] =
		{
			(byte)(m_SendID >> 24),
			(byte)(m_SendID >> 16),
			(byte)(m_SendID >> 8),
			(byte)m_SendID
		};
		memcpy( &m_vBuffer[0], ID, sizeof(ID) );//拷贝识别号
		memcpy( &m_vBuffer[sizeof(ID)], Pid, sizeof(Pid) );//拷贝包裹号码
		memcpy( &m_vBuffer[sizeof(ID)+sizeof(Pid)], pak.GetBuffer(), pak.Size() );
		int SizeSended = ( pak.Size()+sizeof(ID)+sizeof(Pid) );
		return m_pSocket->Send( ad, &m_vBuffer[0], SizeSended );
	}




	//////////////////////
	//		TCP数据		//
	//////////////////////
	//-------------------------------------------------------------------------------------------------------
	TCPCorrespondent::TCPCorrespondent( )
		:m_isGeting(false),
		m_RecevedCount(0),
		m_isMulitSending(false)
	{
		m_vSendRecev.resize( TCPBUFFER_SIZE );//每次发送的数据长度不允许超过这个长度
		m_pSocket = NET_NEW SocketTCP( );
	}
	//-------------------------------------------------------------------------------------------------------
	TCPCorrespondent::~TCPCorrespondent(void)
	{
		//this->DoSend();//退出前，先把数据发送了
		SAFE_DELETE( m_pSocket );
	}
	//-------------------------------------------------------------------------------------------------------
	bool TCPCorrespondent::ConnectWithAddress( const Address& server )
	{
		if( false == m_pSocket->isConnect() //当没有链接，或者目标地址与现在的不相同再转向
			|| server != m_Server )
		{
			if( m_pSocket->isConnect() )
			{
				m_pSocket->Close();
			}
			m_pSocket->Connect( server );
			m_isReady = m_pSocket->isConnect();
			if( m_isReady )
			{
				m_Server = server;//保存对方网址
				m_pSocket->Send( ID, sizeof(ID) );//发送识别码，建立连接
			}
		}
		return m_isReady;
	}
	//-------------------------------------------------------------------------------------------------------
	void TCPCorrespondent::Close()
	{
		m_Server.Clear();
		m_isReady = false;
		m_pSocket->Close();
	}
	//-------------------------------------------------------------------------------------------------------
	bool TCPCorrespondent::Receive( NetPackage& pak ) //发送讯息
	{
		if( m_pSocket->Receive( &m_vSendRecev[0], TCPBUFFER_SIZE ) > 0 )
		{
			if( m_Packer.isRecevSignal( &m_vSendRecev[0] ) )//如果是对方接收到的消息
			{
				if( m_isMulitSending )
				{
					m_isMulitSending = false;
					this->DoSend();//继续尝试发送
					return false;
				}
			}
			else//如果不是这个消息
			{
				if( m_isMulitSending ) //如果当前是发送状态就接收到不是继续发送的请求信号，则放弃发送，处理新消息
				{
					m_isMulitSending = false;
					m_Packer.Reset();
				}
				switch( m_Packer.RecevData( &m_vSendRecev[0] ) )
				{
				case BohgeNet::Packer::PS_BADPACKAGE : break;//无效数据包裹，继续接收客户端信息
				case BohgeNet::Packer::PS_ALLDONE : m_isGeting = true; break;//接收完毕了，开始处理消息
				case BohgeNet::Packer::PS_FRAGMENT :
					{
						int size = 0;//如果没有完成，继续发送本机接受了，要求发送下一个
						m_Packer.GetRecevSignal( &m_vSendRecev[0], size );
						m_pSocket->Send( &m_vSendRecev[0], size );
						return false;
					}break;
				}
				return false;//不会执行到这里
			}
		}
		if( m_isGeting )
		{
			if ( m_Packer.GetPackage(pak) )
			{
				return true;
			}
			else
			{
				m_isGeting = false;
				return false;
			}
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	bool TCPCorrespondent::Send( NetPackage& pak )//接受讯息
	{
		m_Packer.CopyPackage( pak );
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	void TCPCorrespondent::OnFrameEnd()
	{
		if( false == this->DoSend() )
		{
			this->Close();//close socket and connect;
		}
	}
}