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
#include "Framework/VariableType.h"


#include <EASTL/map.h>

namespace BohgeEngine{ class Serializer; }

namespace CurlWebSystemPlugin
{
	class FileSlices
	{
	private:
		typedef eastl::map< uint64, uint64 > FileSlicesMap;
	private:
		FileSlicesMap			m_DispatchedSlicesMap;//记录分发下载段落
		FileSlicesMap			m_FinishedSlicesMap;//下载完的段落
		uint					m_SlicesSize;
		uint64					m_FileSize;
	public:
		FileSlices( uint slicesize );
		~FileSlices(void);
	private:
		void _MergeSlices( );
	public:
		void InitSlices( uint64 size );//初始化，设置文件大小，片段大小
		bool GetJobSlices( uint64& beginpos, uint64& endpos );//返回一个需要下载的段落
		void JobSlicesFinish( uint64 begin, uint64 end );//完成了一段任务
		void Save( BohgeEngine::Serializer& writer );
		void Load( BohgeEngine::Serializer& reader );
	public:
		inline bool isAnyJob()//是否还有下载任务
		{
			return !m_DispatchedSlicesMap.empty();
		}
		inline bool isFinish()
		{
			return 1 == m_FinishedSlicesMap.size() 
				&& 0 == m_FinishedSlicesMap.begin()->first
				&& m_FileSize == m_FinishedSlicesMap.begin()->second + 1;
		}
	};
}

