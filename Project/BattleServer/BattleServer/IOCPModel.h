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




/*
==========================================================================

Purpose:

* 这个类CIOCPModel是本代码的核心类，用于说明WinSock服务器端编程模型中的
完成端口(IOCP)的使用方法，并使用MFC对话框程序来调用这个类实现了基本的
服务器网络通信的功能。

* 其中的PER_IO_DATA结构体是封装了用于每一个重叠操作的参数
PER_HANDLE_DATA 是封装了用于每一个Socket的参数，也就是用于每一个完成端口的参数

* 详细的文档说明请参考 http://blog.csdn.net/PiggyXP

Notes:

* 具体讲明了服务器端建立完成端口、建立工作者线程、投递Recv请求、投递Accept请求的方法，
所有的客户端连入的Socket都需要绑定到IOCP上，所有从客户端发来的数据，都会实时显示到
主界面中去。

Author:

* Bohge

Date:

* 20012/10/22

==========================================================================
*/

#pragma once

// winsock 2 的头文件和库
#include <winsock2.h>
#include <MSWSock.h>
#include <map>
#include <list>
#include <string>

#include "AutoIndex.h"
#include "SmartPtr.hpp"
#include "Releaser.h"

#pragma comment(lib,"ws2_32.lib")




//====================================================================================
//
//				CIOCPModel类定义
//
//====================================================================================

// 工作者线程的线程参数
class CIOCPModel;
typedef struct _tagThreadParams_WORKER
{
	CIOCPModel* pIOCPModel;                                   // 类指针，用于调用类中的函数
	int         nThreadNo;                                    // 线程编号

} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER; 


class SQLDataBase;
class IOContext;
class Listerner;
class ISocketContext;
class ClientContext;
class CIOCPModel// CIOCPModel类
{
public:
	CIOCPModel(void);
	~CIOCPModel(void);

public:

	// 启动服务器
	bool Start();

	//	停止服务器
	void Stop();

	void OnTimer();

	// 加载Socket库
	bool LoadSocketLib();

	// 卸载Socket库，彻底完事
	void UnloadSocketLib() { WSACleanup(); }

	// 获得本机的IP地址
	std::string GetLocalIP();

private:

	// 初始化IOCP
	bool _InitializeIOCP();

	// 最后释放资源
	void _DeInitialize();

	// 将客户端的相关信息存储到数组中
	void _AddToContextList( SmartPtr<ISocketContext> *pSocketContextPtr );

	//客户端是否在buff中
	bool _isInContextList( SmartPtr<ISocketContext> *pSocketContextPtr );

	// 将客户端的信息从数组中移除
	void _RemoveContext( SmartPtr<ISocketContext> *pSocketContextPtr );

	// 清空客户端信息
	void _ClearContextList();

	// 处理完成端口上的错误
	bool HandleError( SmartPtr<ISocketContext> *pSocketContextPtr,const DWORD& dwErr );

	// 线程函数，为IOCP请求服务的工作者线程
	static DWORD WINAPI _WorkerThread(LPVOID lpParam);

	//服务器主动断开连接
	void _DisconnectDeadClient();

	// 获得本机的处理器数量
	int _GetNoOfProcessors();

	// 判断客户端Socket是否已经断开
	bool _IsSocketAlive(SOCKET s);

	void inline MutexClinetSmartPtrMap()
	{
		EnterCriticalSection( &m_csContextList );
	}

	void inline UnmutexClinetSmartPtrMap()
	{
		LeaveCriticalSection( &m_csContextList );
	}

	inline Releaser& GetReleaser()
	{
		return m_Releaser;
	}

private:
	typedef std::list<int>	FreeIDList;
	//typedef std::map<int,ISocketContext*> ClinetMap;
	typedef std::map<int,SmartPtr<ISocketContext>*> ClinetSmartPtrMap;
public:
	bool						 m_isBegin;

	HANDLE                       m_hShutdownEvent;              // 用来通知线程系统退出的事件，为了能够更好的退出线程

	HANDLE                       m_hIOCompletionPort;           // 完成端口的句柄

	HANDLE*                      m_phWorkerThreads;             // 工作者线程的句柄指针

	int		                     m_nThreads;                    // 生成的线程数量

	std::string                  m_strIP;                       // 服务器端的IP地址

	unsigned short				 m_nPort;						// 服务器端的监听端口

	CRITICAL_SECTION             m_csContextList;               // 用于Worker线程同步的互斥量
     
	ClinetSmartPtrMap			 m_ContextPtrMap;				// 客户端Socket的Context信息     

	Releaser					 m_Releaser;					//全部的断开连接的客户端不立即删除，放到这个链表中，待删除

	Listerner*					 m_pListenContext;              // 用于监听的Socket的Context信息

	SQLDataBase*				 m_pSql;

	BohgeNet::NetPackage		 m_Package;
};

