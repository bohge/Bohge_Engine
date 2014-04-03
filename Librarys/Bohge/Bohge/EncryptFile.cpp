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



#include "EncryptFile.h"
#include "AES.h"

using namespace  std;

namespace BohgeEngine
{
	//////////////////////
	//		¶ÁÎÄ¼þ	   //
	/////////////////////
	EncryptFile::EncryptFile(  const std::string& url  )
		:UsualFile( url ),
		m_PosIndex(0),
		m_pDatas(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	EncryptFile::~EncryptFile(void)
	{
		SAFE_DELETE( m_pDatas );
	}
	//--------------------------------------------------------------------------------------------------------------
	bool EncryptFile::_DoOpenFile( ActionType at )
	{
		bool isopen = UsualFile::_DoOpenFile( at );
		if ( isopen )
		{
			byte* raw = NEW byte[GetSize()];;
			m_pDatas = NEW byte[GetSize()];
			UsualFile::_DoReadFile( raw, GetSize() );
			AES::Instance().Decrypt( (char*)raw, (char*)m_pDatas, GetSize() );
		}
		return isopen;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool EncryptFile::_DoCloseFile()
	{
		return UsualFile::_DoCloseFile();
	}
	//--------------------------------------------------------------------------------------------------------------
	int EncryptFile::_DoReadFile( void* data, uint bitesize )
	{
		if ( GetSize() < m_PosIndex + bitesize )
		{
			return 0;
		}
		memcpy( data, &m_pDatas[m_PosIndex], bitesize );
		m_PosIndex += bitesize;
		return bitesize;
	}
	//--------------------------------------------------------------------------------------------------------------
	int EncryptFile::_DoSeekFile( int to, int whence )
	{
		switch( whence )
		{
		case SEEK_SET: m_PosIndex = 0; return 0;
		case SEEK_CUR: m_PosIndex += to; return 0;
		case SEEK_END: m_PosIndex = _GetFileSize() - to; return 0;
		}
		return -1;
	}
	//--------------------------------------------------------------------------------------------------------------
	int EncryptFile::_DoTell()
	{
		return m_PosIndex;
	}
	//--------------------------------------------------------------------------------------------------------------
	int EncryptFile::_DoWriteFile( const void* data, uint bitesize )
	{
		ASSERT(false);
		return 0;
	}

}