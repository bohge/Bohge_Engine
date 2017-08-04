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



#include "Track.h"
#include "Log.h"

#include <map>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
CRITICAL_SECTION cs;
using namespace std;
namespace BohgeEngine
{

	void MemoryTrack::Initialize()
	{
		InitializeCriticalSection( &cs );
	}

	typedef map< int, ALLOC_INFO* > AllocMap; 

	static AllocMap g_allocMap; 

	void MemoryTrack::AddTrack(int addr, int asize, const char *fname, int lnum) 
	{ 
		ALLOC_INFO *info; 

		info = (ALLOC_INFO*)malloc( sizeof(ALLOC_INFO) );
		info->address = addr; 
		strncpy(info->file, fname, 511);
		info->line = lnum; 
		info->size = asize;
		EnterCriticalSection( &cs );
		g_allocMap.insert( make_pair( addr, info) );
		LeaveCriticalSection( &cs );
	}; 

	void MemoryTrack::RemoveTrack(int addr) 
	{ 
		EnterCriticalSection( &cs );
		AllocMap::iterator it = g_allocMap.find( addr );
		if ( it != g_allocMap.end() )
		{
			free( it->second );
			g_allocMap.erase( it );
		}
		LeaveCriticalSection( &cs );
	};

	void MemoryTrack::DumpUnfreed() 
	{		
		int totalSize = 0; 
		char buf[2048];
		if ( 0 != g_allocMap.size() )
		{
			ofstream out;
			out.open( "d:\\bohge.txt", std::ios::out | std::ios::binary );
			for( AllocMap::iterator it = g_allocMap.begin();
				it != g_allocMap.end();
				++ it ) 
			{ 
				sprintf(buf, "%-50s: LINE %d, ADDRESS %d %d unfreed\n",it->second->file, it->second->line, it->second->address, it->second->size); 
				DEBUGLOG(buf);
				out<<buf;
				totalSize += it->second->size; 
				delete it->second;
			} 
			sprintf(buf, "\n----------------------------------------------------------\n"); 
			DEBUGLOG(buf); 
			sprintf(buf, "Total Unfreed: %d bytes ", totalSize); 
			DEBUGLOG(buf);
			out.close();
		}
		DeleteCriticalSection( &cs );
	};

}

#endif