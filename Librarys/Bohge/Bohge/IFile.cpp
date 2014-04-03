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
#include "IFile.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	IFile::IFile( const std::string& url )
		:m_FileUrl( url ),
		m_ActionType(AT_NONE),
		m_isOpen(false),
		m_FileSize(0),
		m_PosIndex(0),
		m_BufferSize(0),
		m_ReadedBufferSize(0),
		m_pDatas(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	IFile::~IFile(void)
	{
		ASSERT( false == m_isOpen );
	}
	//-------------------------------------------------------------------------------------------------------
	bool IFile::OpenFile( ActionType at )
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
					m_ReadedBufferSize += m_BufferSize;
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
	bool IFile::CloseFile()
	{
		ASSERT( m_isOpen == true );
		if ( m_PosIndex != 0 && AT_WRITE == m_ActionType )
		{
			_DoWriteFile( m_pDatas, m_PosIndex );
		}
		m_isOpen = !_DoCloseFile();
		m_PosIndex = 0;
		SAFE_DELETE( m_pDatas );
		return !m_isOpen;
	}
	//-------------------------------------------------------------------------------------------------------
	int IFile::Seek( int to, int whence )
	{
		int result = 0;
		ASSERT( m_isOpen == true );
		switch( m_ActionType )
		{
		case AT_READ:
			{
				switch( whence )
				{
				case SEEK_SET:
					{
						result = _DoSeekFile( to, whence );
						m_ReadedBufferSize = to;
						m_BufferSize = m_FileSize - m_ReadedBufferSize > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize - m_ReadedBufferSize;
					}break;
				case SEEK_CUR:
					{
						int realto = to - m_BufferSize + m_PosIndex;
						m_ReadedBufferSize += realto;
						result = _DoSeekFile( realto, whence );
						m_BufferSize = m_FileSize - m_ReadedBufferSize > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize - m_ReadedBufferSize;
					}break;
				case SEEK_END:
					{
						result = _DoSeekFile( to, whence );
						m_ReadedBufferSize = m_FileSize - to;
						m_BufferSize = to > FC_FILEBUFFER ? FC_FILEBUFFER : to;
					}break;
				default: ASSERT(false);
				}
				m_PosIndex = m_BufferSize;//制空当先buffer数据堆
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
	int IFile::Tell()
	{
		ASSERT( m_isOpen == true );
		switch( m_ActionType )
		{
		case AT_READ: return _DoTell() - m_BufferSize + m_PosIndex;
		case AT_WRITE:return _DoTell() + m_PosIndex;
		default: ASSERT( false ); break;
		}
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	int IFile::ReadFile( void* data, uint bitesize )
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
			uint remainsize = bitesize - readsize;//剩余需要读取的
			if ( remainsize > FC_FILEBUFFER )//当剩余的需要读取的大于buffer
			{
				m_ReadedBufferSize += remainsize;
				if ( 0 == _DoReadFile( ((byte*)data)+readsize,  remainsize ) )//重新填充,0为没有数据
				{
					return 0;
				}
				m_BufferSize = m_FileSize - m_ReadedBufferSize > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize - m_ReadedBufferSize;
				m_ReadedBufferSize += m_BufferSize;
				m_PosIndex = 0;
				if ( 0 == _DoReadFile( m_pDatas,  m_BufferSize ) )//重新填充,0为没有数据
				{
					return 0;
				}
			}
			else
			{
				m_BufferSize = m_FileSize - m_ReadedBufferSize > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize - m_ReadedBufferSize;
				m_ReadedBufferSize += m_BufferSize;
				if ( 0 == _DoReadFile( m_pDatas,  m_BufferSize ) )//重新填充,0为没有数据
				{
					return 0;
				}
				uint remainsize = bitesize - readsize;//剩余需要读取的
				memcpy( ((byte*)data)+readsize, m_pDatas, remainsize );
				m_PosIndex = remainsize;
			}
		}
		else
		{
			memcpy( data, m_pDatas + m_PosIndex, bitesize );
			m_PosIndex += bitesize;
		}
		return bitesize;
	}
	//-------------------------------------------------------------------------------------------------------
	int IFile::WriteFile( const void* data, uint bitesize )
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
}