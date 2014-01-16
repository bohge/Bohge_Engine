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
#include "ClientContext.h"
#include "IOCPModel.h"
#include "IOContext.h"
#include "NetBase.h"
#include <map>


class Listerner  : public ISocketContext//监视连接进入的客户端
{
private:
	std::map<int, IOContext*>		m_IoContextMap;// 客户端网络操作的上下文数据,也就是说对于每一个客户端Socket，是可以在上面同时投递多个IO请求的
	HANDLE							m_hIOCompletionPortCopy;//主框架申请的完成端口句柄的拷贝
	LPFN_ACCEPTEX					m_lpfnAcceptEx;// AcceptEx 和 GetAcceptExSockaddrs 的函数指针，用于调用这两个扩展函数
	LPFN_GETACCEPTEXSOCKADDRS		m_lpfnGetAcceptExSockAddrs; 
public:
	// 初始化
	Listerner()
		:ISocketContext(ISocketContext::ST_LISTRNER),
		m_lpfnAcceptEx(NULL),
		m_lpfnGetAcceptExSockAddrs(NULL)
	{
	}
	// 释放资源
	~Listerner()
	{
		// 释放掉所有的IO上下文数据
		for( std::map<int, IOContext*>::iterator it = m_IoContextMap.begin();
			it != m_IoContextMap.end();
			it ++ )
		{
			SAFE_DELETE( it->second );
		}
		m_IoContextMap.clear();
	}
	// 获取一个新的IoContext
	IOContext* GetNewIoContext()
	{
		IOContext* p = new IOContext;
		m_IoContextMap.insert( std::make_pair( (int)p, p ) );
		return p;
	}
	// 从数组中移除一个指定的IoContext
	void RemoveContext( IOContext* pContext )
	{
		std::map<int, IOContext*>::iterator it = m_IoContextMap.find( (int)pContext );
		if( it != m_IoContextMap.end() )
		{
			SAFE_DELETE_ARRAY( it->second );
			m_IoContextMap.erase(it);
		}
	}
	/////////////////////////////////////////////////////////////////
	// 初始化Socket
	bool InitializeListenSocket( HANDLE& hIOCompletionPort, std::string& ip, ushort potr )
	{
		m_hIOCompletionPortCopy = hIOCompletionPort;
		// AcceptEx 和 GetAcceptExSockaddrs 的GUID，用于导出函数指针
		GUID GuidAcceptEx = WSAID_ACCEPTEX;  
		GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS; 

		// 服务器地址信息，用于绑定Socket
		struct sockaddr_in ServerAddress;

		// 需要使用重叠IO，必须得使用WSASocket来建立Socket，才可以支持重叠IO操作
		m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == m_Socket ) 
		{
			ShowMessage(_T("Initialize Socket fail, error code: %d.\n"), WSAGetLastError());
			return false;
		}
		else
		{
			TRACE("WSASocket() compate.\n");
		}

		//设置tcp为非延迟发送，有消息立即发送
		const char chOpt=1;   
		int   nErr=setsockopt(  m_Socket,   IPPROTO_TCP,   TCP_NODELAY,   &chOpt,   sizeof(char));   
		if(nErr==-1)   
		{   
			ShowMessage(_T("cann't begin no delay TCP,error code: %d.\n"), WSAGetLastError());
			TRACE(_T("setsockopt()   error\n"),WSAGetLastError());   
			return false;   
		} 

		// 将Listen Socket绑定至完成端口中
		//if( NULL== CreateIoCompletionPort( (HANDLE)m_Socket, m_hIOCompletionPortCopy,(DWORD)this, 0))  
		if( NULL== CreateIoCompletionPort( (HANDLE)m_Socket, m_hIOCompletionPortCopy,(DWORD)this->SelfSmartPtr(), 0))  
		{  
			ShowMessage(_T("bind Listen Socket to complation port! error code: %d/n"), WSAGetLastError());  
			RELEASE_SOCKET( m_Socket );
			return false;
		}
		else
		{
			TRACE("Listen Socket bind complation port success complate.\n");
		}

		// 填充地址信息
		ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
		ServerAddress.sin_family = AF_INET;
		// 这里可以绑定任何可用的IP地址，或者绑定一个指定的IP地址 
		//ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);                      
		ServerAddress.sin_addr.s_addr = inet_addr(ip.c_str());         
		ServerAddress.sin_port = htons(potr);                          

		// 绑定地址和端口
		if (SOCKET_ERROR == bind(m_Socket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress))) 
		{
			ShowMessage("bind() function fail.\n");
			return false;
		}
		else
		{
			TRACE("bind() Success.\n");
		}

		// 开始进行监听
		if (SOCKET_ERROR == listen(m_Socket,SOMAXCONN))
		{
			ShowMessage("Listen() funcion error.\n");
			return false;
		}
		else
		{
			TRACE("Listen() success.\n");
		}

		// 使用AcceptEx函数，因为这个是属于WinSock2规范之外的微软另外提供的扩展函数
		// 所以需要额外获取一下函数的指针，
		// 获取AcceptEx函数指针
		DWORD dwBytes = 0;  
		if(SOCKET_ERROR == WSAIoctl(
			m_Socket, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&GuidAcceptEx, 
			sizeof(GuidAcceptEx), 
			&m_lpfnAcceptEx, 
			sizeof(m_lpfnAcceptEx), 
			&dwBytes, 
			NULL, 
			NULL))  
		{  
			ShowMessage("WSAIoctl cann't get AcceptEx founction ptr. error code: %d\n", WSAGetLastError()); 
			return false;  
		}  

		// 获取GetAcceptExSockAddrs函数指针，也是同理
		if(SOCKET_ERROR == WSAIoctl(
			m_Socket, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&GuidGetAcceptExSockAddrs,
			sizeof(GuidGetAcceptExSockAddrs), 
			&m_lpfnGetAcceptExSockAddrs, 
			sizeof(m_lpfnGetAcceptExSockAddrs),   
			&dwBytes, 
			NULL, 
			NULL))  
		{  
			ShowMessage("WSAIoctl cann't get GuidGetAcceptExSockAddrs founction ptr. error code: %d\n", WSAGetLastError());  
			return false; 
		}  
		// 为AcceptEx 准备参数，然后投递AcceptEx I/O请求
		for( int i=0;i<MAX_POST_ACCEPT;i++ )
		{
			// 新建一个IO_CONTEXT
			IOContext* pAcceptIoContext = this->GetNewIoContext();

			if( false==this->PostAccept( pAcceptIoContext ) )
			{
				this->RemoveContext(pAcceptIoContext);
				return false;
			}
		}
		ShowMessage( _T("post %d AcceptEx complate"),MAX_POST_ACCEPT );
		return true;
	}
private:
	/////////////////////////////////////////////////////
	// 将句柄(Socket)绑定到完成端口中
	inline bool AssociateWithIOCP( ClientContext *pContext )
	{
		// 将用于和客户端通信的SOCKET绑定到完成端口中
		//HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, m_hIOCompletionPortCopy, (DWORD)pContext, 0);
		HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, m_hIOCompletionPortCopy, (DWORD)pContext->SelfSmartPtr(), 0);
		if (NULL == hTemp)
		{
			ShowMessage(_T("run CreateIoCompletionPort() fail . error code: %d"),GetLastError());
			return false;
		}
		return true;
	}
	//////////////////////////////////////////////////////////////////
	// 投递Accept请求
	inline bool PostAccept( IOContext* io )
	{
		ASSERT( INVALID_SOCKET!=m_Socket );

		// 准备参数
		DWORD dwBytes = 0;  
		io->m_OpType = ACCEPT_POSTED;  
		WSABUF *p_wbuf   = &io->Buffer();
		OVERLAPPED *p_ol = &io->OverLopped();

		// 为以后新连入的客户端先准备好Socket( 这个是与传统accept最大的区别 ) 
		io->Socket()  = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);  
		if( INVALID_SOCKET==io->Socket() )  
		{  
			ShowMessage(_T("create Accept 's Socket fial! error code: %d"), WSAGetLastError()); 
			return false;  
		} 

		const char chOpt=1;   
		int   nErr=setsockopt(  io->Socket(),   IPPROTO_TCP,   TCP_NODELAY,   &chOpt,   sizeof(char));   
		if(nErr==-1)   
		{   
			TRACE(_T("setsockopt()   error\n"),WSAGetLastError());   
			return false;   
		} 

		// 投递AcceptEx
		if(FALSE == m_lpfnAcceptEx( this->m_Socket, io->Socket(), p_wbuf->buf,
			p_wbuf->len - ((sizeof(SOCKADDR_IN)+16)*2),   //如果这一个参数设置为0则客户端connect的时候立即accpet
			sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, &dwBytes, p_ol))  
		{  
			if(WSA_IO_PENDING != WSAGetLastError())  
			{  
				ShowMessage(_T("post AcceptEx fail, error code: %d"), WSAGetLastError());  
				return false;  
			}  
		} 
		return true;
	}
	inline SmartPtr<ISocketContext>* DoAccpet( IOContext* io )
	{
		SOCKADDR_IN* ClientAddr = NULL;
		SOCKADDR_IN* LocalAddr = NULL;  
		int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);  

		///////////////////////////////////////////////////////////////////////////
		// 1. 首先取得连入客户端的地址信息
		// 这个 m_lpfnGetAcceptExSockAddrs 不得了啊~~~~~~
		// 不但可以取得客户端和本地端的地址信息，还能顺便取出客户端发来的第一组数据，老强大了...
		m_lpfnGetAcceptExSockAddrs(io->Buffer().buf, io->Buffer().len - ((sizeof(SOCKADDR_IN)+16)*2),  
			sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, (LPSOCKADDR*)&LocalAddr, &localLen, (LPSOCKADDR*)&ClientAddr, &remoteLen);  

		int protoclID = ((byte)io->Buffer().buf[0]) << 24 | ((byte)io->Buffer().buf[1]) << 16 | ((byte)io->Buffer().buf[2]) << 8 | ((byte)io->Buffer().buf[3]);
		DEBUGLOG( _T("client %s:%d connect. ID message: %d."),inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port), protoclID );
		if( protoclID != PROTOCOL_ID )
		{
			ShowMessage( _T(" ID message: %d is not match %d"), protoclID, PROTOCOL_ID );
			return NULL;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// 2. 这里需要注意，这里传入的这个是ListenSocket上的Context，这个Context我们还需要用于监听下一个连接
		// 所以我还得要将ListenSocket上的Context复制出来一份为新连入的Socket新建一个SocketContext
		ClientContext* pNewSocketContext = NET_NEW ClientContext;
		pNewSocketContext->m_Socket           = io->Socket();
		memcpy(&(pNewSocketContext->m_BindAddr), ClientAddr, sizeof(SOCKADDR_IN));

		// 参数设置完毕，将这个Socket和完成端口绑定(这也是一个关键步骤)
		if( false==this->AssociateWithIOCP( pNewSocketContext ) )
		{
			ShowMessage( _T("can not bind complation port") );
			SAFE_DELETE( pNewSocketContext );
			return NULL;
		} 

		///////////////////////////////////////////////////////////////////////////////////////////////////
		// 3. 继续，建立其下的IoContext，用于在这个Socket上投递第一个Recv数据请求
		pNewSocketContext->MakeIOContext();
		// 绑定完毕之后，就可以开始在这个Socket上投递完成请求了
		if( false==pNewSocketContext->PostRecv( ) )
		{
			ShowMessage( _T("can not recev message on this port") );
			SAFE_DELETE( pNewSocketContext );
			return NULL;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		// 4. 使用完毕之后，把Listen Socket的那个IoContext重置，然后准备投递新的AcceptEx
		io->ResetBuffer();
		this->PostAccept( io );		

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// 5. 如果成功，那么就把这个有效的客户端信息，加入到ContextList中去(需要统一管理，方便释放资源)
		pNewSocketContext->Actived();
		ClientConnect();
		return pNewSocketContext->SelfSmartPtr();
	}
public:
	virtual SmartPtr<ISocketContext>* DoWork( IOContext* io, BohgeNet::NetPackage& pak, SQLDataBase& sql, bool& isAdd )
	{
		if( io->m_OpType ==  ACCEPT_POSTED )
		{
			isAdd = true;
			return this->DoAccpet(io);
		}
		else
		{
			ASSERT(false);
		}
		return NULL;
	}
};