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



/////////////////////////////////////////
//			State Manage class		   //
/////////////////////////////////////////

#pragma once
#ifndef __cplusplus
#error( "just using in cpp file!" )
#endif

#ifndef  ASSERT
#ifdef _DEBUG
#include <assert.h>
#define ASSERT(f) assert(f)
#else
#define ASSERT(f)
#endif
#endif

#ifdef WIN32
//#define TRACKMEMORY
#endif

#ifdef TRACKMEMORY
#include "Track.h"
#define NEW new(__FILE__, __LINE__) 
#else 
#define NEW new 
#endif

#ifdef _WIN32
#define BOHGE_FORCEINLINE __forceinline
#else
#define BOHGE_FORCEINLINE __attribute__((always_inline))
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


#ifdef MEMORYPOOL

extern "C"
{
#include "MemoryPool.h"
};

#ifdef TRACKMEMORY
inline void * 
#ifdef WIN32
__cdecl
#endif
operator new(unsigned int size, const char *file, int line) 
#else
inline void *
#ifdef WIN32
__cdecl
#endif
operator new(unsigned int size)
#endif
{ 
	void *ptr = CreateMemory(size); 
#ifdef TRACKMEMORY
	AddTrack((int)ptr, size, file, line); 
#endif
	return(ptr); 
}; 
inline void 
#ifdef WIN32
__cdecl
#endif
operator delete(void *p) 
{
#ifdef TRACKMEMORY
	RemoveTrack((int)p); 
#endif
	ReleaseMemory(p); 
}; 

#ifdef TRACKMEMORY
inline void *
#ifdef WIN32
__cdecl
#endif
operator new[](unsigned int size, const char *file, int line) 
#else
inline void *
#ifdef WIN32
__cdecl
#endif
operator new[](unsigned int size)
#endif
{ 
	void *ptr = CreateMemory(size); 
#ifdef TRACKMEMORY
	AddTrack((int)ptr, size, file, line); 
#endif
	return(ptr); 
}; 
inline void
#ifdef WIN32
__cdecl
#endif
operator delete[](void *p) 
{
#ifdef TRACKMEMORY
	RemoveTrack((int)p);
#endif
	ReleaseMemory(p); 
};

#endif




//游戏中的时间体系是毫秒
#define TO_SEC(x) ( x / 1000.0f )
#define TO_MIN(x) ( x / 1000.0f / 60.0f )
#define SEC(x)	( x * 1000.0f )
#define MIN(x)	( x * 60.0f * 1000.0f )

//#ifdef WIN32
//#define PLANTFORMDELIMITER  "\\" //放弃使用这个反正windws平台能看懂 "/"
//#else
#define PLANTFORMDELIMITER  "/"
//#endif



#define FOR_EACH( type, var, in ) for( type::iterator var = in.begin(); var != in.end(); ++ var )
#define FOR_EACH_CONST( type, var, in ) for( type::const_iterator var = in.begin(); var != in.end(); ++ var )