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

#include "Plugindefine.h"
#include "Allocator.h"
#include "Track.h"

#include <EABase/eabase.h>

#ifdef _DEBUG
	//#define _TRACK_MEMORY
#endif


#ifndef  ASSERT
	#ifdef _DEBUG
		#include <assert.h>
		#define ASSERT(f) assert(f)
	#else
		#define ASSERT(f)
	#endif
#endif


#ifdef _PLATFORM_WINDOWS
	#define BOHGE_FORCEINLINE __forceinline
#else
	#define BOHGE_FORCEINLINE __attribute__((always_inline))
#endif

#ifdef _PLATFORM_WINDOWS
	#define NULL 0
#endif


#ifdef _TRACK_MEMORY
	#define NEW new(__FILE__, __LINE__)
	inline void * 
	#ifdef _PLATFORM_WINDOWS
		__cdecl
	#endif
		operator new(unsigned int size, const char *file, int line) 
	{ 
		void* ptr = BohgeEngine::Allocator::s_pAlloc( size ); 
		BohgeEngine::MemoryTrack::AddTrack( (int)ptr, size, file, line );
		return ptr;
	};


	inline void *
	#ifdef _PLATFORM_WINDOWS
		__cdecl
	#endif
		operator new[](unsigned int size, const char *file, int line) 
	{ 
		void* ptr = BohgeEngine::Allocator::s_pAlloc( size ); 
		BohgeEngine::MemoryTrack::AddTrack( (int)ptr, size, file, line );
		return ptr;
	}; 


	inline void 
	#ifdef _PLATFORM_WINDOWS
		__cdecl
	#endif
		operator delete(void *p) 
	{
		BohgeEngine::MemoryTrack::RemoveTrack((int)p);
		BohgeEngine::Allocator::s_pRelease(p); 
	}; 

	inline void
	#ifdef _PLATFORM_WINDOWS
		__cdecl
	#endif
		operator delete[](void *p) 
	{
		BohgeEngine::MemoryTrack::RemoveTrack((int)p);
		BohgeEngine::Allocator::s_pRelease(p); 
	};
#else
	#define NEW new
#endif

#define SAFE_DELETE(p) if(p){delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){delete[] p; p = NULL;}




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



#define FOR_EACH( type, var, in ) for( type::iterator var = (in).begin(); var != (in).end(); ++ (var) )
#define FOR_EACH_REVERSE( type, var, in ) for( type::reverse_iterator var = (in).rbegin(); var != (in).rend(); ++ (var) )
#define FOR_EACH_CONST( type, var, in ) for( type::const_iterator var = (in).begin(); var != (in).end(); ++ (var) )
#define FOR_EACH_CONST_REVERSE( type, var, in ) for( type::const_reverse_iterator var = (in).rbegin(); var != (in).rend(); ++ (var) )


