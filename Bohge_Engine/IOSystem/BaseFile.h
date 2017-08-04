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



/////////////////////////////////
//			文件系统		   //
/////////////////////////////////
#pragma once
#include "Framework/IFile.h"


#include <EASTL/string.h>
#include <EASTL/vector.h>


namespace IOSystemPlugin
{
	//基本类型
	class BaseFile : public BohgeEngine::IFile
	{
	private:
		enum Constant
		{
			FC_FILEBUFFER = 1024*32,//32kb缓存的buffer
		};
	private:
		eastl::string				m_FileUrl;
		ActionType				m_ActionType;
		byte*					m_pDatas;
		int						m_BufferSize;//缓存的大小，当文件小于FC_FILEBUFFER就需要计算缓存大小
		int						m_FileSize;
		int						m_PosIndex;
		int						m_ReadedBufferSize;//已经加载的buffer大小
		bool					m_isOpen;
	protected:
		BaseFile( const eastl::string& url );
		virtual ~BaseFile(void);
	protected:
		virtual bool _DoOpenFile( BohgeEngine::IFile::ActionType at ) = 0;
		virtual bool _DoCloseFile() = 0;
		virtual int _DoSeekFile( int to, int whence ) = 0;
		virtual int _DoTell() = 0;
		virtual int _DoReadFile( void* data, uint bitesize ) = 0; //实际的读取函数
		virtual int _DoWriteFile( const void* data, uint bitesize ) = 0; //实际的写入函数
	public:
		virtual bool OpenFile( BohgeEngine::IFile::ActionType at );
		virtual bool isOpen();
		virtual bool CloseFile();
		virtual int Seek( int to, int whence );
		virtual int Tell();
		virtual int ReadFile( void* data, int bitesize );
		virtual int WriteFile( const void* data, int bitesize );
		virtual const eastl::string& GetFilePath();
		virtual uint GetSize();
	protected:
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint _GetFileSize( )
		{
			if ( !m_isOpen )//当没有打开文件就读取大小肯定是读文件
			{
				OpenFile( AT_READ );
			}
			return m_FileSize;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void _SetFileSize( uint size )
		{
			m_FileSize = size;
		}
	public:
		//-------------------------------------------------------------------------------------------------------
		template<typename T> BOHGE_FORCEINLINE int ReadFile( T* ptr )
		{
			ReadFile( ptr, sizeof(T) );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> BOHGE_FORCEINLINE int WriteFile( T* ptr )
		{
			WriteFile( ptr, sizeof(T) );
		}
	};
}