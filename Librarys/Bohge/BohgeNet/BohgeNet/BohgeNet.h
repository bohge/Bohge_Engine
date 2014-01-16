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



/////////////////////////////
//		变量类型		   //
/////////////////////////////
#pragma once


#define SERVER_VISION	1//服务器版本号
//缓冲区长度 (1024*8)
//之所以为什么设置8K，也是一个江湖上的经验值
//如果确实客户端发来的每组数据都比较少，那么就设置得小一些，省内存
//548是一些网络端口MTU的上限，这里就用512了
#define BUFFER_SIZE		512//实际在我们NetPackage中的数据大小，与下列在发送时候的大小没有关系
#define MAIN_ENTER_PORT	56423//主要的进入端口
#define UDP_ENTRT_PORT	56420//打通用的


typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned char	byte;
typedef unsigned int	bitmask;
typedef int				sint;
typedef float			clamp;
typedef unsigned int	handle;



//TCP的数据包头，为了解决我们服务器在一个pass中发送多个数据包的问题，
//这里我们使用这种方法把全部数据打包到一个数据包中，
//然后接收端收到之后将数据拆分开来
struct TCPHead
{
	enum
	{
		Max_Package_Count = 16,	//每个包裹包含最大的数据包个数不能超过14个
	};
	ushort			m_FragmentCount;//告知对方需要接受多少个片段，才能组成一个完整的命令流
	ushort			m_PackageCount;//这个buffer包含的包裹数量
	ushort			m_LengthEachPackage[Max_Package_Count];//每个包裹的长度
	TCPHead()
	{
		this->Reset();
	}
	void Reset()
	{
		m_FragmentCount = 0;
		m_PackageCount = 0;
		for ( int i = 0 ; i < Max_Package_Count ; i ++ )
		{
			m_LengthEachPackage[i] = 0;
		}
	}
	ushort Size()
	{
		ushort r = 0;
		for ( int i = 0 ; i < m_PackageCount ; i ++ )
		{
			r += m_LengthEachPackage[i];
		}
		return r;
	}
};


#define TCPBUFFER_SIZE (BUFFER_SIZE + sizeof(TCPHead) )//每一次发送的包含TCP包头
#define UDPBUFFER_SIZE (BUFFER_SIZE + sizeof(PROTOCOL_ID) + sizeof(uint) )//最后一个是包裹的ID号码


#define PROTOCOL_ID			0x19851022//通讯的识别码
const static byte	ID[sizeof(PROTOCOL_ID)] =
										{
											(byte)(PROTOCOL_ID >> 24),
											(byte)(PROTOCOL_ID >> 16),
											(byte)(PROTOCOL_ID >> 8),
											(byte)PROTOCOL_ID
										};


#define PLATFORM_WINDOWS	1
#define PLATFORM_IOS		2
#define PLATFORM_ANDROID	3


#ifdef _WIN32
#define PLATFORM PLATFORM_WINDOWS
#elif _WIN64
#define PLATFORM PLATFORM_WINDOWS
#elif IOS
#define PLATFORM PLATFORM_IOS
#elif ANDROID
#define PLATFORM PLATFORM_ANDROID
#else
#error unknown platform!
#endif



#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>
#include <windows.h>

#pragma comment( lib, "wsock32.lib" )

#elif PLATFORM == PLATFORM_IOS || PLATFORM == PLATFORM_ANDROID
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>

#else

#error unknown platform!

#endif




//输出
#ifdef APP
#include "Log.h"
#define NETDEBUGLOG(x,...) Tracker::PrintDebugInfo("",__VA_ARGS__)
#else
#ifdef ANDROID
#include <android/log.h>
#define TODO(x)
#define LOG_TAG    "Bohge"
#define NETDEBUGLOG(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);
#endif
#ifdef IOS
#include <stdio.h>
#define NETDEBUGLOG(x,...)  printf(x,__VA_ARGS__)
#endif
#ifdef WIN32
#include <stdio.h>
#define NETDEBUGLOG(x,...)  printf(x,__VA_ARGS__)
#endif
#endif

#ifndef  ASSERT
#ifdef _DEBUG
#include <assert.h>
#define ASSERT(f) assert(f);
#else
#define ASSERT(f) ;
#endif
#endif


#ifdef _DEBUG
//#define NET_TRACK
#endif

#ifdef NET_TRACK
#define NET_NEW new(__FILE__, __LINE__) 
#else 
#define NET_NEW new 
#endif

#ifdef WIN32
#define NULL 0
#endif


#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if(p){delete p; p = NULL;}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) if(p){delete[] p; p = NULL;}
#endif


namespace BohgeNet
{
	class NetPackage;
	class NetHost;
	class TCPCorrespondent;
	class UDPCorrespondent;
}


#ifndef BOHGE_FORCEINLINE
#ifdef WIN32
#define BOHGE_FORCEINLINE __forceinline
#else
#define BOHGE_FORCEINLINE __attribute__((always_inline))
#endif
#endif