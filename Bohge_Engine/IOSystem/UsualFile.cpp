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
//			最基本的文件	   //
/////////////////////////////////
#include "Framework/IIOSystem.h"
#include "Framework/Log.h"
#include "UsualFile.h"

#ifdef _PLATFORM_WINDOWS
#include <direct.h>
#include <io.h>
#else
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif


using namespace eastl;

namespace IOSystemPlugin
{
	//////////////////////
	//		写文件	   //
	/////////////////////
	//--------------------------------------------------------------------------------------------------------------
	UsualFile::UsualFile( const eastl::string& url )
		:BaseFile( url ),
		m_pFile(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	UsualFile::~UsualFile(void)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	bool UsualFile::_DoOpenFile( BohgeEngine::IFile::ActionType at )
	{
		switch( at )
		{
		case IFile::AT_WRITE:
			{
				BohgeEngine::IIOSystem::Instance()->MakeFolder( GetFilePath() );
				fopen_s( &m_pFile, GetFilePath().c_str(), "wb" );
				DEBUGLOG("Write %s\n", GetFilePath().c_str() );
			}break;
		case IFile::AT_READ:
			{
				fopen_s( &m_pFile, GetFilePath().c_str(), "rb" );
				DEBUGLOG("Read %s\n", GetFilePath().c_str() );
			}break;
		default: ASSERT(false);
		}
		//m_pFile = fopen( GetFilePath().c_str(), "rb" );
		if( NULL == m_pFile )
		{
			DEBUGLOG("File %s Can not Open! \n", GetFilePath().c_str() );
			//ASSERT(false);
		}
		else
		{
			uint nCurrentPos = ftell( m_pFile );//读文件大小
			fseek( m_pFile, 0, SEEK_END );
			uint size = ftell( m_pFile ) + nCurrentPos;
			fseek( m_pFile, nCurrentPos, SEEK_SET );
			_SetFileSize( size );
		}
		return m_pFile != NULL ? true : false;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool UsualFile::_DoCloseFile()
	{
		return fclose( m_pFile ) == -1 ? false : true;
	}
	//--------------------------------------------------------------------------------------------------------------
	int UsualFile::_DoReadFile( void* data, uint bitesize )
	{
		if ( fread( data, bitesize, 1, m_pFile ) == 1 )
		{
			return bitesize;
		}
		return 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	int UsualFile::_DoSeekFile( int to, int whence )
	{
		return fseek( m_pFile, to, whence );
	}
	//--------------------------------------------------------------------------------------------------------------
	int UsualFile::_DoTell()
	{
		return ftell( m_pFile );
	}
	//--------------------------------------------------------------------------------------------------------------
	int UsualFile::_DoWriteFile( const void* data, uint bitesize )
	{
		if ( fwrite( data, bitesize, 1, m_pFile ) == 1 )
		{
			return bitesize;
		}
		return 0;
	}

}