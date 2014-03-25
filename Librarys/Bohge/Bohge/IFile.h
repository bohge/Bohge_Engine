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
#include "Predefine.h"
#include "VariableType.h"
#include <string>
#include <vector>


namespace BohgeEngine
{
	//基本类型
	class IFile
	{
	public:
		enum ActionType
		{
			FILE_READ,
			FILE_WRITE,
		};
	private:
		ActionType			m_ActionType;
		std::string			m_FileUrl;
	protected:
		bool				m_isOpen;
		uint				m_FileSize;
	public:
		IFile( const std::string& url, ActionType action )
			:m_FileUrl( url ),
			m_ActionType(action),
			m_isOpen(false),
			m_FileSize(0)
		{
		}
		virtual ~IFile(void)
		{
			if (m_isOpen)
			{
				CloseFile();
			}
		}
	protected:
		virtual bool _DoOpenFile() = 0;
		virtual bool _DoCloseFile() = 0;
		virtual int _DoSeekFile( uint to, int whence ) = 0;
		virtual int _DoTell() = 0;
	public:
		virtual int DoWork( void* ptr, uint bitesize ) = 0;//读取或者写入
	public:
		BOHGE_FORCEINLINE bool OpenFile()
		{
			ASSERT( m_isOpen == false );
			m_isOpen = _DoOpenFile();
			return m_isOpen;
		}
		BOHGE_FORCEINLINE bool CloseFile()
		{
			ASSERT( m_isOpen == true );
			m_isOpen = false;
			_DoCloseFile();
			return !m_isOpen;
		}
		BOHGE_FORCEINLINE int Seek( uint to, int whence )
		{
			ASSERT( m_isOpen == true );
			return _DoSeekFile( to, whence );
		}
		BOHGE_FORCEINLINE int Tell()
		{
			return _DoTell();
		}
		BOHGE_FORCEINLINE const std::string& GetFilePath()
		{
			return m_FileUrl;
		}
		BOHGE_FORCEINLINE uint GetSize()
		{
			ASSERT( m_isOpen == true );
			return m_FileSize;
		}
		BOHGE_FORCEINLINE void NewFile( const std::string& path )
		{
			ASSERT( m_isOpen == false );
			m_FileUrl = path;
		}
	};




	//读取文件基本类型
	class IReadFile : public IFile
	{
	public:
		IReadFile( const std::string& url )
			:IFile( url, IFile::FILE_READ )
		{
		}
		virtual ~IReadFile(void){}
	protected:
		virtual int _DoReadFile( void* data, uint bitesize ) = 0; //实际的读取函数
	public:
		virtual int DoWork( void* ptr, uint bitesize )
		{
			return ReadFile( ptr, bitesize );
		}
	public:
		BOHGE_FORCEINLINE int ReadFile( void* ptr, uint size )
		{
			ASSERT( m_isOpen == true );
			return _DoReadFile( ptr, size );
		}
	};


	//写入文件
	class IWriteFile : public IFile
	{
	private:
		enum Constant
		{
			WC_FILEBUFFER = 32*1024,//缓存的buffer
		};
	private:
		std::vector<byte>		m_Datas;
		uint					m_PosIndex;
	public:
		IWriteFile( const std::string& url )
			:IFile( url, IFile::FILE_READ ),
			m_PosIndex(0)
		{
			m_Datas.resize( WC_FILEBUFFER );
		}
		virtual ~IWriteFile(void)
		{
			ASSERT( 0 == m_PosIndex );
		}
	protected:
		virtual int _DoWriteFile( const void* data, uint bitesize ) = 0; //实际的写入函数
		virtual bool _DoCloseFile()
		{
			if ( m_PosIndex != 0 )
			{
				_DoWriteFile( &m_Datas[0], m_PosIndex );
			}
			m_PosIndex = 0;
			return true;
		}
	public:
		virtual int DoWork( void* ptr, uint bitesize )
		{
			return WriteFile( ptr, bitesize );
		}
	public:
		BOHGE_FORCEINLINE int WriteFile( const void* data, uint bitesize )
		{
			ASSERT( m_isOpen == true );
			if ( bitesize > WC_FILEBUFFER || bitesize > ( WC_FILEBUFFER - m_PosIndex ) )
			{
				if ( 0 != m_PosIndex )
				{
					_DoWriteFile( &m_Datas[0], m_PosIndex );
				}
				_DoWriteFile( data, bitesize );
				m_PosIndex = 0;
			}
			else
			{
				memcpy( &m_Datas[m_PosIndex], data, bitesize );
				m_PosIndex += bitesize;
			}
			m_FileSize += bitesize;
			return bitesize;
		}
	};

}