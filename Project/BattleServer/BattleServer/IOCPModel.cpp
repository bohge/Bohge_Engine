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



#include "StdAfx.h"
#include "IOCPModel.h"
#include "MainDlg.h"
#include "ListenerContext.h"
#include "ClientContext.h"
#include "IOContext.h"
#include "SQLDataBase.h"
#include "ServerConfig.h"

using namespace std;

extern void OnClientQuit( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );



CIOCPModel::CIOCPModel(void):
m_nThreads(0),
m_hShutdownEvent(NULL),
m_hIOCompletionPort(NULL),
m_phWorkerThreads(NULL),
m_strIP(DEFAULT_IP),
m_pListenContext( NULL ),
m_nPort(DEFAULT_PORT)
{
}


CIOCPModel::~CIOCPModel(void)
{
	// 确保资源彻底释放
	this->Stop();
}




///////////////////////////////////////////////////////////////////
// 工作者线程:   为IOCP请求服务的工作者线程
//         也就是每当完成端口上出现了完成数据包,就将之取出来进行处理的线程
///////////////////////////////////////////////////////////////////

DWORD WINAPI CIOCPModel::_WorkerThread(LPVOID lpParam)
{    
	THREADPARAMS_WORKER* pParam = (THREADPARAMS_WORKER*)lpParam;
	CIOCPModel* pIOCPModel = (CIOCPModel*)pParam->pIOCPModel;
	int nThreadNo = (int)pParam->nThreadNo;
	
	SQLDataBase::COMInitializeEx();//初始化多线程调用COM
	SQLDataBase sql( ServerConfig::Instance().ConnectString() );

	bool isAdd = false;

	BohgeNet::NetPackage pak;

	ShowMessage(_T("Working thread is begin,ID: %d."),nThreadNo);

	OVERLAPPED*						pOverlapped = NULL;
	SmartPtr<ISocketContext>*		pSmartPtrContext = NULL;
	DWORD							dwBytesTransfered = 0;

	// 循环处理请求,知道接收到Shutdown信息为止
	while (WAIT_OBJECT_0 != WaitForSingleObject(pIOCPModel->m_hShutdownEvent, 0))
	{
		if ( pIOCPModel->GetReleaser().isNeedRelease() )
		{
			pIOCPModel->GetReleaser().DoRelease();
		}
		BOOL bReturn = GetQueuedCompletionStatus(
			pIOCPModel->m_hIOCompletionPort,// 这个就是我们建立的那个唯一的完成端口  
			&dwBytesTransfered,//这个是操作完成后返回的字节数  
			(PULONG_PTR)&pSmartPtrContext,// 这个是我们建立完成端口的时候绑定的那个自定义结构体参数  
			&pOverlapped,// 这个是我们在连入Socket的时候一起建立的那个重叠结构
			INFINITE);// 等待完成端口的超时时间,如果线程不需要做其他的事情,那就INFINITE就行了
		// 如果收到的是退出标志,则直接退出
		if ( EXIT_CODE==(DWORD)pSmartPtrContext )
		{
			break;
		}
		// 判断是否出现了错误
		if( !bReturn )  
		{  
			pIOCPModel->MutexClinetSmartPtrMap();
			if( pIOCPModel->_isInContextList( pSmartPtrContext ) )
			{
				DWORD dwErr = GetLastError();
				// 显示一下提示信息
				OnClientQuit( check_pointer_cast<ClientContext>(*pSmartPtrContext), pak, sql );
				if( !pIOCPModel->HandleError( pSmartPtrContext, dwErr ) )
				{
					break;//这里是完成端口出问题,直接推出线程了
				}
			}
			pIOCPModel->UnmutexClinetSmartPtrMap();
			continue;  
		}  
		else  
		{  	
			// 读取传入的参数
			IOContext* pIoContext = CONTAINING_RECORD(pOverlapped, IOContext, OverLopped());  
			// 判断是否有客户端断开了
			if((0 == dwBytesTransfered) && ( RECV_POSTED==pIoContext->m_OpType || SEND_POSTED==pIoContext->m_OpType))  
			{  
				pIOCPModel->MutexClinetSmartPtrMap();
				if( pIOCPModel->_isInContextList( pSmartPtrContext ) )
				{
					DEBUGLOG( _T("Client %s:%d disconnect."), (*pSmartPtrContext)->BindIPString(), ntohs((*pSmartPtrContext)->BindPort()) );
					OnClientQuit( check_pointer_cast<ClientContext>(*pSmartPtrContext), pak, sql );
					// 释放掉对应的资源
					pIOCPModel->_RemoveContext( pSmartPtrContext );
				}
				pIOCPModel->UnmutexClinetSmartPtrMap();
				continue;  
			}  
			else
			{
				if( (*pSmartPtrContext)->isConnect() )
				{
					(*pSmartPtrContext)->Actived();//这个时间轮内被驱动了
					SmartPtr<ISocketContext>* temp = (*pSmartPtrContext)->DoWork(pIoContext, pak, sql, isAdd);//如果有返回,只可能是listener返回的
					if ( NULL != temp )
					{
						if( isAdd )
						{
							pIOCPModel->_AddToContextList( temp );
						}
						else
						{
							pIOCPModel->MutexClinetSmartPtrMap();
							DEBUGLOG( _T("client error on clinetContext %s:%d disconnet."), (*pSmartPtrContext)->BindIPString(), ntohs((*pSmartPtrContext)->BindPort()) );
							OnClientQuit(check_pointer_cast<ClientContext>(*temp), pak, sql );
							pIOCPModel->_RemoveContext( temp );
							pIOCPModel->UnmutexClinetSmartPtrMap();
						}
					}
				}
			}//if
		}//if

	}//while

	TRACE(_T("Work thread %d exit.\n"),nThreadNo);
	SQLDataBase::COMUninitialize();//初始化多线程调用COM
	// 释放线程参数
	RELEASE(lpParam);	

	return 0;
}

void CIOCPModel::_DisconnectDeadClient()
{
	EnterCriticalSection( &m_csContextList );
	int disconnectCount = 0;
	for( ClinetSmartPtrMap::iterator it = m_ContextPtrMap.begin() ;
		it != m_ContextPtrMap.end() ;
		 )
	{
		if(		(*it->second)->GetType() == ISocketContext::ST_CLIENT
			&&	(*it->second)->isActived() == false )
		{
			disconnectCount ++;
			DEBUGLOG( _T("Client %s:%d has been kick."), (*it->second)->BindIPString(), ntohs((*it->second)->BindPort()) );
			OnClientQuit( check_pointer_cast<ClientContext>((*it->second)), m_Package, *m_pSql );
			(*it->second)->Disconnect();
			m_Releaser.AddToReleaseList( it->second );
			it = m_ContextPtrMap.erase( it );
		}
		else
		{
			it++;
		}
	}
	if( disconnectCount != 0 )
	{
		ClientDisonnect(disconnectCount);
	}
	LeaveCriticalSection( &m_csContextList );
}


//====================================================================================
//
//				    系统初始化和终止
//
//====================================================================================




////////////////////////////////////////////////////////////////////
// 初始化WinSock 2.2
bool CIOCPModel::LoadSocketLib()
{    
	WSADATA wsaData;
	int nResult;
	nResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	// 错误(一般都不可能出现)
	if (NO_ERROR != nResult)
	{
		ShowMessage(_T(" Initialize WinSock 2.2 fail !\n"));
		return false; 
	}

	return true;
}

//////////////////////////////////////////////////////////////////
//	启动服务器
bool CIOCPModel::Start()
{
	m_isBegin = true;

	ShowMessage( _T("SQL connect string is %s"), ServerConfig::Instance().ConnectString().c_str() );
	ShowMessage( _T("Verify domain is %s"), ServerConfig::Instance().VerifyString().c_str() );

	// 初始化线程互斥量
	InitializeCriticalSection(&m_csContextList);

	// 建立系统退出的事件通知
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// 初始化IOCP
	if (false == _InitializeIOCP())
	{
		ShowMessage(_T("Initialize IOCP fail !\n"));
		return false;
	}
	else
	{
		ShowMessage(_T("\nIOCP Initialize success\n."));
	}

	// 初始化Socket
	// 生成用于监听的Socket的信息
	m_pListenContext = new Listerner;
	this->_AddToContextList( m_pListenContext->SelfSmartPtr() );
	if( false == m_pListenContext->InitializeListenSocket( m_hIOCompletionPort, m_strIP, m_nPort ) )
	{
		ShowMessage(_T("Listen Socket Initialize fail !\n"));
		this->_DeInitialize();
		return false;
	}
	else
	{
		ShowMessage(_T("Listen Socket Initialize success."));
	}

	SQLDataBase::COMInitializeEx();//初始化多线程调用COM
	m_pSql = NET_NEW SQLDataBase( ServerConfig::Instance().ConnectString() );

	ShowMessage(_T("all Initialize done, witing connect....\n"));

	return true;
}


////////////////////////////////////////////////////////////////////
//	开始发送系统退出消息,退出完成端口和线程资源
void CIOCPModel::Stop()
{
	if( m_isBegin )
	{
		m_isBegin = false;
		if( m_pListenContext!=NULL && m_pListenContext->Socket()!=INVALID_SOCKET )
		{
			// 激活关闭消息通知
			SetEvent(m_hShutdownEvent);

			for (int i = 0; i < m_nThreads; i++)
			{
				// 通知所有的完成端口操作退出
				PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
			}

			// 等待所有的客户端资源退出
			WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

			// 清除客户端列表信息
			this->_ClearContextList();

			// 释放其他资源
			this->_DeInitialize();

			SAFE_DELETE( m_pSql );
			SQLDataBase::COMUninitialize();//初始化多线程调用COM

			ShowMessage(_T("Stoped!\n"));
		}	
	}
}


////////////////////////////////
// 初始化完成端口
bool CIOCPModel::_InitializeIOCP()
{
	// 建立第一个完成端口
	m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );

	if ( NULL == m_hIOCompletionPort)
	{
		ShowMessage(_T("creat complattion port fail ! error code: %d!\n"), WSAGetLastError());
		return false;
	}

	// 根据本机中的处理器数量,建立对应的线程数
	m_nThreads = WORKER_THREADS_PER_PROCESSOR * _GetNoOfProcessors();

	// 为工作者线程初始化句柄
	m_phWorkerThreads = new HANDLE[m_nThreads];

	// 根据计算出来的数量建立工作者线程
	DWORD nThreadID;
	for (int i = 0; i < m_nThreads; i++)
	{
		THREADPARAMS_WORKER* pThreadParams = new THREADPARAMS_WORKER;
		pThreadParams->pIOCPModel = this;
		pThreadParams->nThreadNo  = i+1;
		m_phWorkerThreads[i] = ::CreateThread(0, 0, _WorkerThread, (void *)pThreadParams, 0, &nThreadID);
	}

	TRACE(" Create _WorkerThread %d .\n", m_nThreads );

	return true;
}


////////////////////////////////////////////////////////////
//	最后释放掉所有资源
void CIOCPModel::_DeInitialize()
{
	// 删除客户端列表的互斥量
	DeleteCriticalSection(&m_csContextList);

	// 关闭系统退出事件句柄
	RELEASE_HANDLE(m_hShutdownEvent);

	// 释放工作者线程句柄指针
	for( int i=0;i<m_nThreads;i++ )
	{
		RELEASE_HANDLE(m_phWorkerThreads[i]);
	}

	RELEASE(m_phWorkerThreads);

	// 关闭IOCP句柄
	RELEASE_HANDLE(m_hIOCompletionPort);

	// 关闭监听Socket
	//RELEASE(m_pListenContext);//由于他把自身加入了List,这里就不需要再次释放了

	ShowMessage("All resource is released.\n");
}

//====================================================================================
//
//				    ContextList 相关操作
//
//====================================================================================


//////////////////////////////////////////////////////////////
// 将客户端的相关信息存储到数组中
void CIOCPModel::_AddToContextList( SmartPtr<ISocketContext> *pSocketContextPtr )
{
	EnterCriticalSection(&m_csContextList);
	m_ContextPtrMap.insert( make_pair( (int)pSocketContextPtr, pSocketContextPtr) );
	LeaveCriticalSection(&m_csContextList);
}
////////////////////////////////////////////////////////////////
//客户端是否在buff中
bool CIOCPModel::_isInContextList( SmartPtr<ISocketContext> *pSocketContextPtr )
{
	//EnterCriticalSection(&m_csContextList);//在外部调用了
	ClinetSmartPtrMap::iterator it = m_ContextPtrMap.find( (int)pSocketContextPtr );
	//LeaveCriticalSection(&m_csContextList);
	return it != m_ContextPtrMap.end();
}
////////////////////////////////////////////////////////////////
//	移除某个特定的Context
void CIOCPModel::_RemoveContext( SmartPtr<ISocketContext> *pSocketContextPtr )
{
	//EnterCriticalSection(&m_csContextList);//在外部调用了
	ClinetSmartPtrMap::iterator it = m_ContextPtrMap.find( (int)pSocketContextPtr );
	if( it != m_ContextPtrMap.end() )
	{
		(*it->second)->Disconnect();
		//SAFE_DELETE( it->second );//不懂了就跟一下这里,它对应的ISocketContext的析构不能Delete这个指针-----第二次修改,这里不直接删除,交给releaser管理
		m_Releaser.AddToReleaseList( it->second );
		m_ContextPtrMap.erase( it );
		ClientDisonnect(1);
	}
	//LeaveCriticalSection(&m_csContextList);
}

////////////////////////////////////////////////////////////////
// 清空客户端信息
void CIOCPModel::_ClearContextList()
{
	EnterCriticalSection(&m_csContextList);
	for( ClinetSmartPtrMap::iterator it = m_ContextPtrMap.begin() ;
		it != m_ContextPtrMap.end() ;
		it++ )
	{
		SAFE_DELETE( it->second );
		ClientDisonnect(1);
	}
	m_ContextPtrMap.clear();
	LeaveCriticalSection(&m_csContextList);
}



//====================================================================================
//
//				       其他辅助函数定义
//
//====================================================================================



////////////////////////////////////////////////////////////////////
// 获得本机的IP地址
std::string CIOCPModel::GetLocalIP()
{
	// 获得本机主机名
	char hostname[MAX_PATH] = {0};
	gethostname(hostname,MAX_PATH);                
	struct hostent FAR* lpHostEnt = gethostbyname(hostname);
	if(lpHostEnt == NULL)
	{
		m_strIP = DEFAULT_IP;
		return DEFAULT_IP;
	}

	// 取得IP地址列表中的第一个为返回的IP(因为一台主机可能会绑定多个IP)
	LPSTR lpAddr = lpHostEnt->h_addr_list[0];      

	// 将IP地址转化成字符串形式
	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);
	m_strIP = std::string( inet_ntoa(inAddr) );        
	return m_strIP;
}

///////////////////////////////////////////////////////////////////
// 获得本机中处理器的数量
int CIOCPModel::_GetNoOfProcessors()
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}



/////////////////////////////////////////////////////////////////////
// 判断客户端Socket是否已经断开,否则在一个无效的Socket上投递WSARecv操作会出现异常
// 使用的方法是尝试向这个socket发送数据,判断这个socket调用的返回值
// 因为如果客户端网络异常断开(例如客户端崩溃或者拔掉网线等)的时候,服务器端是无法收到客户端断开的通知的

bool CIOCPModel::_IsSocketAlive(SOCKET s)
{
	int nByteSent=send(s,"",0,0);
	if (-1 == nByteSent) return false;
	return true;
}

///////////////////////////////////////////////////////////////////
// 显示并处理完成端口上的错误
bool CIOCPModel::HandleError( SmartPtr<ISocketContext> *pSocketContextPtr,const DWORD& dwErr )
{
	// 如果是超时了,就再继续等吧  
	if(WAIT_TIMEOUT == dwErr)  
	{  	
		// 确认客户端是否还活着...
		if( !_IsSocketAlive( (*pSocketContextPtr)->Socket()) )
		{
			ShowMessage( _T("_IsSocketAlive deteced client unhandle exit") );
			this->_RemoveContext( pSocketContextPtr );
			return true;
		}
		else
		{
			ShowMessage( _T("net work over time!retry...") );
			return true;
		}
	} 
	// 可能是客户端异常退出了
	else if( ERROR_NETNAME_DELETED==dwErr )
	{
		DEBUGLOG( _T("ERROR_NETNAME_DELETED  deteced client unhandle exit!") );
		this->_RemoveContext( pSocketContextPtr );
		return true;
	}

	else
	{
		ShowMessage( _T("opretor on complation port is fail,thread exit, error code: %d"),dwErr );
		return false;
	}
	return true;
}

void CIOCPModel::OnTimer()
{
	this->_DisconnectDeadClient();
}
