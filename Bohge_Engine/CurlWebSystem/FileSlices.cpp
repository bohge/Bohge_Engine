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
#include "Framework/Serializer.h"
#include "FileSlices.h"



using namespace eastl;
using namespace BohgeEngine;

namespace CurlWebSystemPlugin
{
	//------------------------------------------------------------------------------
	FileSlices::FileSlices( uint slicesize )
	{
		m_SlicesSize = slicesize;
	}
	//------------------------------------------------------------------------------
	FileSlices::~FileSlices(void)
	{
	}
	//------------------------------------------------------------------------------
	void FileSlices::InitSlices( uint64 size )
	{
		m_FileSize = size;
		m_DispatchedSlicesMap.clear();
		m_DispatchedSlicesMap.insert( make_pair( 0, size ) );
		m_FinishedSlicesMap.clear();
	}
	//------------------------------------------------------------------------------
	bool FileSlices::GetJobSlices( uint64& beginpos, uint64& endpos )
	{
		if (m_DispatchedSlicesMap.empty())
		{
			beginpos = 0;
			endpos = 0;
			return false;
		}
		FileSlicesMap::iterator jobit = m_DispatchedSlicesMap.begin();
		uint64 begin = jobit->first;
		uint64 end = jobit->second;
		m_DispatchedSlicesMap.erase( jobit );//去掉旧段落
		if ( begin + m_SlicesSize < end )//如果还有剩余数据则重新插入
		{
			m_DispatchedSlicesMap.insert( make_pair( begin + m_SlicesSize, end ) );
			endpos = begin + m_SlicesSize - 1;
		}
		else
		{
			endpos = m_FileSize - 1;
		}
		beginpos = begin;
		return true;
	}
	//------------------------------------------------------------------------------
	void FileSlices::_MergeSlices(  )
	{
		for( FileSlicesMap::iterator nextSlice = m_FinishedSlicesMap.begin();
			nextSlice != m_FinishedSlicesMap.end();
			)
		{
			FileSlicesMap::iterator thisSlice = nextSlice;
			++ nextSlice;
			if ( m_FinishedSlicesMap.end() != nextSlice )
			{
				if ( thisSlice->second + 1 == nextSlice->first )
				{
					uint64 be = thisSlice->first;
					uint64 en = nextSlice->second;
					m_FinishedSlicesMap.erase( thisSlice );
					m_FinishedSlicesMap.erase( nextSlice );
					m_FinishedSlicesMap.insert( make_pair( be, en) );
					nextSlice = m_FinishedSlicesMap.find( be );
				}
			}
		}

		printf("finished");
		FOR_EACH( FileSlicesMap, it, m_FinishedSlicesMap )
		{
			printf("     %d-%d", (uint)it->first, (uint)it->second );
		}
		printf("\n");
	}
	//------------------------------------------------------------------------------
	void FileSlices::JobSlicesFinish( uint64 begin, uint64 end )
	{
		m_FinishedSlicesMap.insert( make_pair( begin, end ) );
		_MergeSlices( );
	}
	//------------------------------------------------------------------------------
	void FileSlices::Save( Serializer& writer )
	{
		writer<<m_FileSize
			<<m_FinishedSlicesMap.size();
		FOR_EACH( FileSlicesMap, it , m_FinishedSlicesMap )
		{
			writer<<it->first
				<<it->second;
		}
	}
	//------------------------------------------------------------------------------
	void FileSlices::Load( Serializer& reader )
	{
		uint size;
		reader>>m_FileSize
			>>size;
		uint64 be = 0;
		uint64 en = 0;
		for ( int i = 0; i < size; ++ i )
		{
			reader>>be>>en;
			m_FinishedSlicesMap.insert( make_pair( be, en ) );
		}
		//计算需要分发的数组
		FileSlicesMap::iterator first = m_FinishedSlicesMap.begin();
		if ( m_FinishedSlicesMap.end() != first 
			&& 0 != first->first )
		{
			m_DispatchedSlicesMap.insert( make_pair(0,first->first-1) );
		}
		for( FileSlicesMap::iterator nextSlice = m_FinishedSlicesMap.begin();
			nextSlice != m_FinishedSlicesMap.end();
			)
		{
			FileSlicesMap::iterator thisSlice = nextSlice;
			++ nextSlice;
			if ( m_FinishedSlicesMap.end() != nextSlice )
			{
				m_DispatchedSlicesMap.insert( make_pair(thisSlice->second+1, nextSlice->first-1) );
			}
			else//最后一个
			{
				if ( m_FileSize != thisSlice->second )
				{
					m_DispatchedSlicesMap.insert( make_pair( thisSlice->second + 1, m_FileSize) );
				}
			}
		}
	}

}