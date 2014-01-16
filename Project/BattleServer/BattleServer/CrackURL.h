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
#include "HttpBase.h"
#include <string>

// CrackedUrl
class CrackedUrl 
{
	int m_scheme;
	std::wstring m_host;
	int m_port;
	std::wstring m_path;
public:
	CrackedUrl( std::wstring& url )
	{
		URL_COMPONENTS uc = { 0};
		uc.dwStructSize = sizeof(uc);

		const DWORD BUF_LEN = 256;

		WCHAR host[BUF_LEN];
		uc.lpszHostName = host;
		uc.dwHostNameLength = BUF_LEN;

		WCHAR path[BUF_LEN];
		uc.lpszUrlPath = path;
		uc.dwUrlPathLength = BUF_LEN;

		WCHAR extra[BUF_LEN];
		uc.lpszExtraInfo = extra;
		uc.dwExtraInfoLength = BUF_LEN;

#ifdef USE_WINHTTP
		if (!WinHttpCrackUrl(url.c_str(), 0, ICU_ESCAPE, &uc)) {
			printf("Error:InternetCrackUrl failed!\n");
		}

#else
		if (!InternetCrackUrl(url.c_str(), 0, ICU_ESCAPE, &uc)) {
			printf("Error:InternetCrackUrl failed!\n");
		}
#endif
		m_scheme = uc.nScheme;
		m_host = host;
		m_port = uc.nPort;
		m_path = path;
	}

	int GetScheme() const
	{
		return m_scheme;
	}

	LPCWSTR GetHostName() const
	{
		return m_host.c_str();
	}

	int GetPort() const
	{
		return m_port;
	}

	LPCWSTR GetPath() const
	{
		return m_path.c_str();
	}

	static std::string UrlEncode(const char* p)
	{
		if (p == 0) {
			return std::string();
		}

		std::string buf;

		for (;;) {
			int ch = (BYTE) (*(p++));
			if (ch == '\0') {
				break;
			}

			if (isalnum(ch) || ch == '_' || ch == '-' || ch == '.') {
				buf += (char)ch;
			}
			else if (ch == ' ') {
				buf += '+';
			}
			else {
				char c[16];
				wsprintfA(c, "%%%02X", ch);
				buf += c;
			}
		}

		return buf;
	}
};
