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



// TestClinet.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "Correspondent.h"
#include "NetHost.h"
#include "NetBase.h"
#include <conio.h>
#include <string>

using namespace std;
using namespace BohgeNet;

#define TEST_THREAD 300


class IDriver
{
public:
	NetHost*		m_pNet;
	IDriver()
	{
		m_pNet = new NetHost;
		m_pNet->Initialization();
	}
	~IDriver()
	{
		m_pNet->Destry();
		delete m_pNet;
	}
	bool isReady()
	{
		return m_pNet->m_pTcp->isReady();
	}
	void begin()
	{
		m_pNet->OnFarmeBegin();
	}
	uint GetCurrectTime(void)
	{
		return 0;
	}
	void End()
	{
		m_pNet->OnFarmeEnd();
	}
};

class TCPRecever
{
public:
	bool OnTCPMessage(TCPCorrespondent& sender, BohgeNet::NetPackage& pak)
	{
		pak.Begin();
		pak<<BohgeNet::NM_CONNECT_SERVER
			<<SERVER_VISION;
		sender.Send( pak );
		return true;
	}
};

DWORD WINAPI _WorkerThread(LPVOID lpParam);
int _tmain(int argc, _TCHAR* argv[])
{
	DWORD nThreadID;
	for (int i = 0; i < TEST_THREAD; i++)
	{
		::CreateThread(0, 0, _WorkerThread, NULL, 0, &nThreadID);
		Sleep(10);
	}
	getch();
}

DWORD WINAPI _WorkerThread(LPVOID lpParam)
{
	//Address ad(192,168,1,99,56423);
	//Address ad(172,17,72,77,56423);
	Address ad(23,21,89,16,56423);
	IDriver d;
	TCPRecever tcp;
	d.m_pNet->ConnectTCP(ad);
	d.m_pNet->AddTCPResponse( make_bind( &TCPRecever::OnTCPMessage, &tcp ) );
	d.begin();
	d.m_pNet->Package().Begin();
	d.m_pNet->Package()
		<<BohgeNet::NM_CONNECT_SERVER
		<<SERVER_VISION;
	d.m_pNet->TCPSend( d.m_pNet->Package() );
	while ( 1 )
	{
		d.begin();
		d.End();
		Sleep(30);
	}
	return 0;
}
