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

//#ifdef _DEBUG
#define  PRINTLOG
//#endif

#ifdef PRINTLOG

#ifdef WIN32
#include "Predefine.h"
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>



class BOHGE_FRAMEWORK Tracker
{
private:
	class BOHGE_FRAMEWORK Console
	{
	private:
		FILE* outPutFile;
	public:
	Console();
	~Console();
	void Console::Printf( const char * sander, va_list args);
	};
private:
	static Console info;
public:
	static inline void PrintDebugInfo(const char * sander, ... )
	{
		va_list args;
		va_start( args, sander );
		info.Printf( sander, args);
		va_end( args );
	}
};

#define DEBUGLOG(...) Tracker::PrintDebugInfo("",__VA_ARGS__);

#define TODO(x) message(x);


#endif

#ifdef ANDROID
#include <android/log.h>
#define TODO(x)
#define LOG_TAG    "Bohge"
#define DEBUGLOG(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);
#endif

#ifdef IOS
#include <stdio.h>
#define DEBUGLOG(x,...)  printf(x,__VA_ARGS__)
#endif
#else
#define DEBUGLOG(...)
#endif // PRINTLOG