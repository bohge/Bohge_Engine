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



/////////////////////////
//		实用工具	   //
/////////////////////////
#include "NetUtility.h"

#ifdef IOS
#include <sys/time.h>
#endif
#include <time.h>

//-------------------------------------------------------------------------------------------------------
MilisecondTimer::MilisecondTimer(void)
{
	//得到时间
#if PLATFORM == PLATFORM_WINDOWS
	m_uBeginTime = clock();
#elif PLATFORM == PLATFORM_ANDROID
	timespec time1;
	clock_gettime(CLOCK_MONOTONIC, &time1);
	m_uBeginTime = time1.tv_sec * 1000 + time1.tv_nsec / 1000000;
#elif PLATFORM == PLATFORM_IOS
	struct timeval tv;
	gettimeofday(&tv, NULL);
	m_uBeginTime = 1000* tv.tv_sec + tv.tv_usec / 1000;
#endif
	NETDEBUGLOG("begin time is %d\n", m_uBeginTime);
	m_uLastTime = 0;
}
//-------------------------------------------------------------------------------------------------------
uint MilisecondTimer::GetMilisecondTimeSpan()
{
	ulong CurrentTime;
#if PLATFORM == PLATFORM_WINDOWS
	CurrentTime = clock();
#elif PLATFORM == PLATFORM_ANDROID
	timespec time1;
	clock_gettime(CLOCK_MONOTONIC, &time1);
	CurrentTime = time1.tv_sec * 1000 + time1.tv_nsec / 1000000;
#elif PLATFORM == PLATFORM_IOS
	struct timeval tv;
	gettimeofday(&tv, NULL);
	CurrentTime = 1000* tv.tv_sec + tv.tv_usec / 1000;
#endif
	CurrentTime -= m_uBeginTime;
	if( CurrentTime - m_uLastTime <= 0)
	{
		return 0;
	}
	ulong timespan = CurrentTime - m_uLastTime;
	m_uLastTime = CurrentTime;
	return timespan;
}
//-------------------------------------------------------------------------------------------------------
uint MilisecondTimer::NowTimeAsSecond()
{
	ulong CurrentTime;
#if PLATFORM == PLATFORM_WINDOWS
	CurrentTime = clock();
#elif PLATFORM == PLATFORM_ANDROID
	timespec time1;
	clock_gettime(CLOCK_MONOTONIC, &time1);
	CurrentTime = time1.tv_sec * 1000 + time1.tv_nsec / 1000000;
#elif PLATFORM == PLATFORM_IOS
	struct timeval tv;
	gettimeofday(&tv, NULL);
	CurrentTime = 1000* tv.tv_sec + tv.tv_usec / 1000;
#endif
	CurrentTime -= m_uBeginTime;
	return CurrentTime / 1000;
}




#if PLATFORM == PLATFORM_WINDOWS
uint YYYYMMDD::YearMonthDay()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	uint r = 0;
	r = st.wYear * 10000 + st.wMonth * 100 + st.wDay;
	return r;
}
#endif