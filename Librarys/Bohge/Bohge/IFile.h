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
		friend class IOSystemBase;
	public:
		enum ActionType
		{
			AT_NONE = 0, //没有初始化的
			AT_READ,
			AT_WRITE,
		};
	private:
		enum Constant
		{
			FC_FILEBUFFER = 1024*64,//64kb缓存的buffer
		};
	private:
		std::string				m_FileUrl;
		ActionType				m_ActionType;
		byte*					m_pDatas;
		uint					m_BufferSize;//缓存的大小，当文件小于FC_FILEBUFFER就需要计算缓存大小
		uint					m_FileSize;
		uint					m_PosIndex;
		bool					m_isOpen;
	protected:
		IFile( const std::string& url )
			:m_FileUrl( url ),
			m_ActionType(AT_NONE),
			m_isOpen(false),
			m_FileSize(0),
			m_PosIndex(0),
			m_BufferSize(0),
			m_pDatas(NULL)
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
		virtual bool _DoOpenFile( ActionType at ) = 0;
		virtual bool _DoCloseFile() = 0;
		virtual int _DoSeekFile( uint to, int whence ) = 0;
		virtual int _DoTell() = 0;
		virtual int _DoReadFile( void* data, uint bitesize ) = 0; //实际的读取函数
		virtual int _DoWriteFile( const void* data, uint bitesize ) = 0; //实际的写入函数
	protected:
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint _GetFileSize( )
		{
			return m_FileSize;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void _SetFileSize( uint size )
		{
			m_FileSize = size;
		}
	public:
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const std::string& GetFilePath()
		{
			return m_FileUrl;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool OpenFile( ActionType at )
		{
			ASSERT( m_isOpen == false );
			m_isOpen = _DoOpenFile( at );
			if ( m_isOpen )
			{
				switch( at )
				{
				case AT_READ:
					{
						m_BufferSize = m_FileSize > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize;
						m_pDatas = NEW byte[m_BufferSize];
						_DoReadFile( m_pDatas, m_BufferSize );//重新填充
					}break;
				case AT_WRITE:
					{
						m_BufferSize = FC_FILEBUFFER;
						m_pDatas = NEW byte[m_BufferSize];
					}break;
				default:ASSERT(false);
				}
			}
			m_ActionType = m_isOpen ? at : AT_NONE;
			return m_isOpen;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE bool CloseFile()
		{
			ASSERT( m_isOpen == true );
			if ( m_PosIndex != 0 && AT_WRITE == m_ActionType )
			{
				_DoWriteFile( m_pDatas, m_PosIndex );
			}
			m_isOpen = _DoCloseFile();
			m_PosIndex = 0;
			SAFE_DELETE( m_pDatas );
			return !m_isOpen;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE int Seek( uint to, int whence )
		{
			int result = 0;
			ASSERT( m_isOpen == true );
			switch( m_ActionType )
			{
			case AT_READ:
				{
					result = _DoSeekFile( to, whence );
					int pos = _DoTell();
					m_BufferSize = m_FileSize - pos > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize - pos;
					_DoReadFile( m_pDatas, m_BufferSize );//重新加载
					m_PosIndex = 0;
				}break;
			case AT_WRITE:
				{
					if ( 0 != m_PosIndex )
					{
						_DoWriteFile( m_pDatas, m_PosIndex );//把已有数据写入
						m_PosIndex = 0;
					}
					result = _DoSeekFile( to, whence );
				}break;
			default:
				{
					ASSERT( false );
				}break;
			}
			return result;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE int Tell()
		{
			ASSERT( m_isOpen == true );
			return _DoTell() - m_BufferSize + m_PosIndex;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetSize()
		{
			return m_FileSize;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> BOHGE_FORCEINLINE int ReadFile( T* ptr )
		{
			ReadFile( ptr, sizeof(T) );
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE int ReadFile( void* data, uint bitesize )
		{
			if ( !m_isOpen )
			{
				OpenFile( AT_READ );
			}
			ASSERT( AT_READ == m_ActionType );
			if ( bitesize > m_BufferSize || bitesize > ( m_BufferSize - m_PosIndex ) )
			{
				uint readsize = m_BufferSize - m_PosIndex;//buffer中的剩余数据加载出来先
				memcpy( data, m_pDatas + m_PosIndex, readsize );
				int pos = _DoTell();
				m_BufferSize = m_FileSize - pos > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize - pos;
				if ( 0 == _DoReadFile( m_pDatas, m_BufferSize ) )//重新填充,0为没有数据
				{
					return 0;
				}
				uint remainsize = bitesize - readsize;//剩余需要读取的
				memcpy( ((byte*)data)+readsize, m_pDatas, remainsize );
				m_PosIndex = remainsize;
			}
			else
			{
				memcpy( data, m_pDatas + m_PosIndex, bitesize );
				m_PosIndex += bitesize;
			}
			return bitesize;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> BOHGE_FORCEINLINE int WriteFile( T* ptr )
		{
			WriteFile( ptr, sizeof(T) );
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE int WriteFile( const void* data, uint bitesize )
		{
			if ( !m_isOpen )
			{
				OpenFile( AT_WRITE );
			}
			ASSERT( AT_WRITE == m_ActionType );
			if ( bitesize > m_BufferSize || bitesize > ( m_BufferSize - m_PosIndex ) )
			{
				if ( 0 != m_PosIndex )
				{
					_DoWriteFile( m_pDatas, m_PosIndex );
				}
				_DoWriteFile( data, bitesize );
				m_PosIndex = 0;
			}
			else
			{
				memcpy( m_pDatas + m_PosIndex, data, bitesize );
				m_PosIndex += bitesize;
			}
			m_FileSize += bitesize;
			return bitesize;
		}
	};
}