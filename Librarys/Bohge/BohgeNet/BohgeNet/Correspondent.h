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
//	 服务器/客户端   //
/////////////////////////
#pragma once
#include "BohgeNet.h"
#include "NetBase.h"
#include "NetUtility.h"
#include "Packer.h"
#include <vector>
#include <list>
#include <map>


namespace BohgeNet
{
	//////////////////////
	//		基础数据	//
	//////////////////////
	class ICorrespondent
	{
	protected:
		Address			m_Server;
		Domain			m_Domain;
		bool			m_isReady;
	protected:
		ICorrespondent();
		virtual ~ICorrespondent();
	protected:
		bool ConnectWithDomain( const Domain& domain );
		virtual bool ConnectWithAddress( const Address& server ) = 0;
		virtual void Close() = 0;
		virtual bool Receive( NetPackage& pak ) = 0; //接受讯息
		virtual void OnFrameEnd(  ) = 0;//当每帧结束的时候，发送本地数据
	public:
		virtual bool Send( NetPackage& pak ) = 0;//发送讯息
	public:
		BOHGE_FORCEINLINE bool isReady()
		{
			return m_isReady;
		}
	public:
		BOHGE_FORCEINLINE const Address& ServerAddress()
		{
			return m_Server;
		}
		BOHGE_FORCEINLINE const Domain& ServerDomain()
		{
			return m_Domain;
		}
	};

	//////////////////////
	//		UDP数据		//
	//////////////////////
	class UDPCorrespondent : public ICorrespondent // udp上层连接着
	{
		friend class NetHost;
	protected:
		std::vector<byte>	m_vBuffer;//保存的提取的数据
		uint				m_SendID;//发送数据ID
		uint				m_ReciveID;//接受的ID
		SocketUDP*			m_pSocket;
	protected:
		UDPCorrespondent( ); //绑定随机端口
		UDPCorrespondent( ushort port );//绑定指定端口
		virtual ~UDPCorrespondent();
	protected:
		virtual bool ConnectWithAddress( const Address& server );
		virtual void Close();
		virtual bool Receive( NetPackage& pak ); //发送讯息
		virtual void OnFrameEnd( ){ }//udp实时发送，这里就是空的
	public:
		virtual bool Send( NetPackage& pak );//接受讯息
	};

	//////////////////////
	//	UDP Server		//
	//////////////////////
	class UDPServer
	{
	private:
		std::vector<byte>	m_vBuffer;//保存的提取的数据
		const uint			m_SendID;//发送数据ID
		SocketUDP*			m_pSocket;
	public:
		UDPServer( ushort port );
		~UDPServer();
	public:
		bool RecevieFrom( Address& ad, NetPackage& pak);
		bool SendTo( Address& ad, NetPackage& pak );
	};


	//////////////////////
	//		TCP数据		//
	//////////////////////
	class TCPCorrespondent : public ICorrespondent // TCP上层连接着
	{
		friend class NetHost;
	private:
		bool				m_isGeting;//接收完毕可以取得数据了
		bool				m_isMulitSending;//多次分包发送，等待返回接收确认中
		uint				m_RecevedCount;//收到数据包片段的个数
		SocketTCP*			m_pSocket;
		Packer				m_Packer;
		Packer::Buffer		m_vSendRecev;//构造好的发送，接受buffer
	protected:
		TCPCorrespondent( );
		~TCPCorrespondent();
	protected:
		virtual bool ConnectWithAddress( const Address& server );
		virtual void Close();
		virtual bool Receive( NetPackage& pak ); //发送讯息
		virtual void OnFrameEnd();
	public:
		virtual bool Send( NetPackage& pak );//接受讯息
        BOHGE_FORCEINLINE bool isConnect()
        {
            return m_pSocket->isConnect();
        }
	protected:
		BOHGE_FORCEINLINE bool DoSend()//实际发送数据的代码
		{
			if ( !m_isMulitSending )
			{
				int size = 0;
				if( m_Packer.SendData( &m_vSendRecev[0], size ) )
				{
					if( m_Packer.isMulitSend() ) //只有当是多次发送的时候
					{
						m_isMulitSending = true;
					}
					return m_pSocket->Send( &m_vSendRecev[0], size );
				}
			}
			return true;
		}
	};


}
