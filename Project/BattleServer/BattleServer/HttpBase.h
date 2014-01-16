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



#pragma once
#define USE_WINHTTP    //Comment this line to user wininet.
#ifdef USE_WINHTTP
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#else
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#endif



HINTERNET OpenSession(LPCWSTR userAgent = 0)
{
#ifdef USE_WINHTTP
	return WinHttpOpen(userAgent, NULL, NULL, NULL, NULL);;
#else
	return InternetOpen(userAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
#endif
}

HINTERNET Connect(HINTERNET hSession, LPCWSTR serverAddr, int portNo)
{
#ifdef USE_WINHTTP
	return WinHttpConnect(hSession, serverAddr, (INTERNET_PORT) portNo, 0);
#else
	return InternetConnect(hSession, serverAddr, portNo, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
#endif
}

HINTERNET OpenRequest(HINTERNET hConnect, LPCWSTR verb, LPCWSTR objectName, int scheme)
{
	DWORD flags = 0;
#ifdef USE_WINHTTP
	if (scheme == INTERNET_SCHEME_HTTPS) {
		flags |= WINHTTP_FLAG_SECURE;
	}

	return WinHttpOpenRequest(hConnect, verb, objectName, NULL, NULL, NULL, flags);

#else
	if (scheme == INTERNET_SCHEME_HTTPS) {
		flags |= INTERNET_FLAG_SECURE;
	}

	return HttpOpenRequest(hConnect, verb, objectName, NULL, NULL, NULL, flags, 0);
#endif
}

BOOL AddRequestHeaders(HINTERNET hRequest, LPCWSTR header)
{
	SIZE_T len = lstrlenW(header);
#ifdef USE_WINHTTP
	return WinHttpAddRequestHeaders(hRequest, header, DWORD(len), WINHTTP_ADDREQ_FLAG_ADD);
#else
	return HttpAddRequestHeaders(hRequest, header, DWORD(len), HTTP_ADDREQ_FLAG_ADD);
#endif
}

BOOL SendRequest(HINTERNET hRequest, const void* body, DWORD size)
{
#ifdef USE_WINHTTP
	return WinHttpSendRequest(hRequest, 0, 0, const_cast<void*>(body), size, size, 0);
#else
	return HttpSendRequest(hRequest, 0, 0, const_cast<void*>(body), size);
#endif
}

BOOL EndRequest(HINTERNET hRequest)
{
#ifdef USE_WINHTTP
	return WinHttpReceiveResponse(hRequest, 0);
#else
	// if you use HttpSendRequestEx to send request then use HttpEndRequest in here!
	return TRUE;
#endif
}

BOOL QueryInfo(HINTERNET hRequest, int queryId, char* szBuf, DWORD* pdwSize)
{
#ifdef USE_WINHTTP
	return WinHttpQueryHeaders(hRequest, (DWORD) queryId, 0, szBuf, pdwSize, 0);
#else
	return HttpQueryInfo(hRequest, queryId, szBuf, pdwSize, 0);
#endif
}

BOOL ReadData(HINTERNET hRequest, void* buffer, DWORD length, DWORD* cbRead)
{
#ifdef USE_WINHTTP
	return WinHttpReadData(hRequest, buffer, length, cbRead);
#else
	return InternetReadFile(hRequest, buffer, length, cbRead);
#endif
}

void CloseInternetHandle(HINTERNET hInternet)
{
	if (hInternet)
	{
#ifdef USE_WINHTTP
		WinHttpCloseHandle(hInternet);
#else
		InternetCloseHandle(hInternet);
#endif
	}
}