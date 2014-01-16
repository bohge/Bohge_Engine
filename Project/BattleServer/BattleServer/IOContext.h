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
//				单IO数据结构体定义(用于每一个重叠操作的参数)
//
//====================================================================================
#pragma once
#include <winsock2.h>
#include <MSWSock.h>
#include "BohgeNet.h"




//////////////////////////////////////////////////////////////////
// 在完成端口上投递的I/O操作的类型
typedef enum _OPERATION_TYPE  
{  
	ACCEPT_POSTED,                     // 标志投递的Accept操作
	SEND_POSTED,                       // 标志投递的是发送操作
	RECV_POSTED,                       // 标志投递的是接收操作
	NULL_POSTED                        // 用于初始化，无意义

}OPERATION_TYPE;


class IOContext
{
private:
	OVERLAPPED     m_Overlapped;                               // 每一个重叠网络操作的重叠结构(针对每一个Socket的每一个操作，都要有一个)              
	SOCKET         m_sockAccept;                               // 这个网络操作所使用的Socket
	WSABUF         m_wsaBuf;                                   // WSA类型的缓冲区，用于给重叠操作传参数的
public:
	char*          m_szBuffer;									// 这个是WSABUF里具体存字符的缓冲区
	OPERATION_TYPE m_OpType;                                   // 标识网络操作的类型(对应上面的枚举)
public:
	IOContext(void)
	{
		ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
		m_szBuffer = NET_NEW char[ TCPBUFFER_SIZE ];
		ZeroMemory( m_szBuffer,TCPBUFFER_SIZE );
		m_sockAccept = INVALID_SOCKET;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = TCPBUFFER_SIZE;
		m_OpType     = NULL_POSTED;
	}
	~IOContext(void)
	{
		SAFE_DELETE_ARRAY( m_szBuffer );
	}
	inline void ResetBuffer()
	{
		ZeroMemory( m_szBuffer,TCPBUFFER_SIZE );
	}
	inline OVERLAPPED& OverLopped()
	{
		return m_Overlapped;
	}
	inline SOCKET& Socket()
	{
		return m_sockAccept;
	}
	inline WSABUF& Buffer()
	{
		return m_wsaBuf;
	}
};