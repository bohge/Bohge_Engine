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



#include "stdafx.h"
#include "ServerConfig.h"
#include "tinyxml.h"

using namespace std;

std::wstring Ansi2WChar( const char* pszSrc, int nLen)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if(nSize <= 0) return NULL;
	WCHAR *pwszDst = new WCHAR[nSize+1];
	if( NULL == pwszDst) return NULL;
	MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;
	if( pwszDst[0] == 0xFEFF) // skip Oxfeff
		for(int i = 0; i < nSize; i ++) 
			pwszDst[i] = pwszDst[i+1]; 
	wstring wcharString(pwszDst);
	delete pwszDst;
	return wcharString;
}

ServerConfig::ServerConfig()
:m_isShowLog(false)
{
	TCHAR strFilePath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, strFilePath, MAX_PATH);
	(strrchr(strFilePath, ('\\')))[1] = 0;
	SetCurrentDirectory( strFilePath );
	string path(strFilePath);
	path += "Config.xml";
	TiXmlDocument *myDocument = new TiXmlDocument( path.c_str() );
	myDocument->LoadFile();
	//获得根元素
	TiXmlElement* RootElement = myDocument->RootElement();
	TiXmlElement* timerlong = RootElement->FirstChildElement();
	TiXmlElement* islog = timerlong->NextSiblingElement();
	TiXmlElement* sql = islog->NextSiblingElement();
	TiXmlElement* verify = sql->NextSiblingElement();
	m_Timer = atoi( timerlong->Attribute( "Content" ) );
	string isshow = islog->Attribute( "Content" );
	if( isshow == string("true") )
	{
		m_isShowLog = true;
	}
	m_ConnectString = sql->Attribute( "Content" );
	m_VerifyString = verify->Attribute( "Content" );
	m_ReceiptVerify = Ansi2WChar( m_VerifyString.c_str() , m_VerifyString.size() );
	delete(myDocument);
}