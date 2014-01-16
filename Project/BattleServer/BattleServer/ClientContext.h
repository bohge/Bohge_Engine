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



//====================================================================================
//
//				单句柄数据结构体定义(用于每一个完成端口，也就是每一个Socket的参数)
//
//====================================================================================
#pragma once
#include "ISocketContext.h"
#include "IOContext.h"
#include "NetBase.h"
#include "Packer.h"
#include <map>



class ClientContext;
extern void OnGetMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );//外部函数，在通讯框架之外的，处理接收到的消息的函数

class ClientContext : public ISocketContext
{ 
	friend class SmartPtr<ClientContext>;
	friend class CIOCPModel;
	friend class Listerner;
	// 客户端网络操作的上下文数据,也就是说对于每一个客户端Socket，是可以在上面同时投递多个IO请求的
private:
	bool					m_isCopying;//发送的信息是否在拷贝中
	bool					m_isActive;//在work线程中被调用的，停止响应客户端的DoSend，应为，他自己会响应
	bool					m_isSending;//发送中
	IOContext				m_IoContext;
	BohgeNet::Packer		m_Packer;//这个类是收发安全的，不同线程可以同时调用收发
	uint					m_PlayerIndex;//玩家在数据库中的索引
	SmartPtr<ClientContext>	m_CopyBasePtr;//这个是拷贝的他的基类的那个智能指针，这里为了快速传递给OnGetMessage
private:
	// 初始化
	ClientContext()
		:ISocketContext(ISocketContext::ST_CLIENT),
		m_isSending(false),
		m_isActive(false),
		m_PlayerIndex(-1),
		m_isCopying(false)
	{ 
		m_CopyBasePtr.Copy( check_pointer_cast<ClientContext>( (*this->SelfSmartPtr()) ) );//这里是拷贝，不增加引用计数
	}
	// 释放资源
	~ClientContext()
	{
	}
	//-------------------------------------------------------------------------------------------------------
	inline bool Receive( BohgeNet::NetPackage& pak )
	{
		return m_Packer.GetPackage(pak); //如果有数据就返回true，并且不发送和接受消息
	}
public:
	//-------------------------------------------------------------------------------------------------------
	inline uint& PlayerIndex()
	{
		return m_PlayerIndex;
	}
	inline void Send( BohgeNet::NetPackage& pak )
	{
		while(true)
		{
			if( m_isCopying == false )//防止多线程同时写入
			{
				break;
			}
			//Sleep( 1 );
		}
		m_isCopying = true;
		m_Packer.CopyPackage( pak );
		m_isCopying = false;
	}
	//-------------------------------------------------------------------------------------------------------
	inline void SendImmediately() //实际的发送出去,这个函数不能重复调用，一个流程只能调用一次
	{
		if( ! m_isActive )//这个函数最好少掉，目前有问题，等着改吧，大致就是目前的client是活动状态，再调用他会有问题吧
		{
			this->DoSend();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	inline bool isClient ( const SmartPtr<ClientContext>& client ) const
	{
		return (this->GetIndex() == client->GetIndex()) && (this->m_PlayerIndex == client->m_PlayerIndex ) ;
	}
private:
	void MakeIOContext()
	{
		m_IoContext.Socket() = m_Socket;
	}
	//-------------------------------------------------------------------------------------------------------
	// 投递接收数据请求
	bool PostRecv( )
	{
		// 初始化变量
		DWORD dwFlags = 0;
		DWORD dwBytes = 0;
		m_IoContext.m_OpType = RECV_POSTED;
		m_IoContext.Buffer().len = TCPBUFFER_SIZE;

		// 初始化完成后，，投递WSARecv请求
		int nBytesRecv = WSARecv(	m_IoContext.Socket(),
									&m_IoContext.Buffer(),
									1, &dwBytes, &dwFlags,
									&m_IoContext.OverLopped(),
									NULL );

		// 如果返回值错误，并且错误的代码并非是Pending的话，那就说明这个重叠请求失败了
		if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError()))
		{
			ShowMessage("post WSARecv fail ! error code: %d!\n", WSAGetLastError());
			return false;
		}
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	bool PostSend( )
	{
		// 初始化变量
		DWORD dwFlags = 0;
		DWORD dwBytes = 0;
		m_IoContext.m_OpType = SEND_POSTED;

		DEBUGLOG("Send Message to %s : %d", this->BindIPString(), ntohs(this->BindPort()));
		// 初始化完成后，，投递WSASend请求
		int nBytesSend = WSASend(	m_IoContext.Socket(),
									&m_IoContext.Buffer(),
									1, &dwBytes, dwFlags,
									&m_IoContext.OverLopped(), 
									NULL );
		//如果返回值错误，并且错误的代码并非是Pending的话，那就说明这个重叠请求失败了
		if ((SOCKET_ERROR == nBytesSend) && (WSA_IO_PENDING != WSAGetLastError()))
		{
			ShowMessage("Post SASend fail, error code: %d!\n", WSAGetLastError());
			return false;
		}
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	// 在有接收的数据到达的时候，进行处理
	inline bool DoRecv( BohgeNet::NetPackage& pak, SQLDataBase& sql )
	{
		if( BohgeNet::Packer::isRecevSignal( (byte*)m_IoContext.Buffer().buf ) )//如果是对方接收到的消息
		{
			m_isSending = false;
			return DoSend();//继续尝试发送
		}
		else//如果不是这个消息
		{
			if( m_isSending ) //如果当前是发送状态就接收到不是继续发送的请求信号，则放弃发送，处理新消息
			{
				m_isSending = false;
				m_isActive = false;
				m_Packer.Reset();
			}
			switch( m_Packer.RecevData( (byte*)m_IoContext.Buffer().buf ) )
			{
			case BohgeNet::Packer::PS_BADPACKAGE :
				{
					DEBUGLOG("%s:%d recev a incorrcet package", this->BindIPString(), this->BindPort() );
					return PostRecv();//无效数据包裹，继续接收客户端信息
				}break;
			case BohgeNet::Packer::PS_FRAGMENT :
				{
					m_isActive = true;
					BohgeNet::Packer::GetRecevSignal( (byte*)m_IoContext.Buffer().buf, (int&)m_IoContext.Buffer().len );//得到接收到信号
					return PostSend();//把这个信号发送出去
				}break;
			case BohgeNet::Packer::PS_ALLDONE :
				{
					DEBUGLOG("%s:% dRecev a package", this->BindIPString(), this->BindPort() );
					m_isActive = true;
					while(1)
					{
						if ( m_Packer.GetPackage( pak ) )
						{
							OnGetMessage( m_CopyBasePtr, pak, sql );
						}
						else
						{
							break;
						}
					}
					return DoSend();//否则直接检查是否有需要发送的数据
				}break;
			}
			return PostRecv();//不会执行到这里
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool DoSend( )	//当需要发送数据的时候，进行处理
	{
		if( ! m_isSending  //如果当前是非发送状态
			&& m_isConnect )//而且要是连接状态
		{
			if( m_Packer.SendData( (byte*)m_IoContext.Buffer().buf, (int&)m_IoContext.Buffer().len ) )//如果不是多播，这里就把imulitsend状态清零了
			{
				if( m_Packer.isMulitSend() )//只有是多播才需要设置sending，等待对方接收确认消息
				{
					m_isSending = true;
				}
				// 然后开始投递下一个WSARecv请求
				return this->PostSend( );
			}
		}
		m_isActive = false; //发送完毕了
		return PostRecv( );//否则切换到监听状态，等待回执
	}
	//-------------------------------------------------------------------------------------------------------
	virtual SmartPtr<ISocketContext>* DoWork( IOContext* io,  BohgeNet::NetPackage& pak, SQLDataBase& sql, bool& isAdd )
	{
		//ASSERT( &m_IoContext == io );
		bool Success = false;
		switch( io->m_OpType )  
		{ 
			// RECV
		case RECV_POSTED:
			{
				// 为了增加代码可读性，这里用专门的_DoRecv函数进行处理接收请求
				Success = DoRecv( pak, sql );
			}
			break;
			// SEND
		case SEND_POSTED:
			{
				//当消息发送后需要置换为准备接受的状态
				Success = DoSend( );
			}
			break;
		default:
			// 不应该执行到这里
			ASSERT(false);
			break;
		}
		if( !Success ) //如果执行失败了，就返回自己，然后删除自己
		{
			isAdd = false;
			return this->SelfSmartPtr();
		}
		return NULL;
	}
};