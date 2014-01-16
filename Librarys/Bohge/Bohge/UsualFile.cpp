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
#include "Log.h"
#include "FilePath.h"
#include "UsualFile.h"


#ifdef WIN32
#include <direct.h>
#include <io.h>
#else
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif


using namespace std;

namespace BohgeEngine
{
	//////////////////////
	//		写文件	   //
	/////////////////////
	//--------------------------------------------------------------------------------------------------------------
	ReadUsualFile::ReadUsualFile( const std::string& url )
		:IReadFile( url ),
		m_pFile(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	ReadUsualFile::~ReadUsualFile(void)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	bool ReadUsualFile::_DoOpenFile()
	{
		DEBUGLOG("Read %s\n", GetFilePath().c_str() );
		m_pFile = fopen( GetFilePath().c_str(), "rb" );
		if( NULL == m_pFile )
		{
			DEBUGLOG("File %s Can not Open! \n", GetFilePath().c_str() );
			ASSERT(false);
		}
		else
		{
			uint nCurrentPos = ftell( m_pFile );//读文件大小
			fseek( m_pFile, 0, SEEK_END );
			m_FileSize = ftell( m_pFile ) + nCurrentPos;
			fseek( m_pFile, nCurrentPos, SEEK_SET );
		}
		return m_pFile != NULL ? true : false;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool ReadUsualFile::_DoCloseFile()
	{
		return fclose( m_pFile ) == -1 ? false : true;
	}
	//--------------------------------------------------------------------------------------------------------------
	int ReadUsualFile::_DoReadFile( void* data, uint bitesize )
	{
		if ( fread( data, bitesize, 1, m_pFile ) == 1 )
		{
			return bitesize;
		}
		return 0;
	}





	//////////////////////
	//		写文件	   //
	/////////////////////
	WriteUsualFile::WriteUsualFile( const std::string& url )
		:IWriteFile( url ),
		m_pFile(NULL)
	{

	}
	//--------------------------------------------------------------------------------------------------------------
	WriteUsualFile::~WriteUsualFile()
	{

	}
	//--------------------------------------------------------------------------------------------------------------
	bool WriteUsualFile::_DoOpenFile()
	{
		DEBUGLOG("Wirer into %s\n", GetFilePath().c_str() );
		for ( size_t found = 0 ; ;)
		{
			found = GetFilePath().find_first_of("/\\",found+1);
			if ( found < GetFilePath().size() )
			{
				string dir = GetFilePath().substr(0, found);
				FILEPATH.MakeFolder(dir);
			}
			else
			{
				break;
			}
		}
		m_pFile = fopen( GetFilePath().c_str(), "wb" );
		if( NULL == m_pFile )
		{
			DEBUGLOG("File %s Can not Open! \n", GetFilePath().c_str() );
			ASSERT(false);
		}
		return m_pFile != NULL ? true : false;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool WriteUsualFile::_DoCloseFile()
	{
		IWriteFile::_DoCloseFile();
		return fclose( m_pFile ) == -1 ? false : true;
	}
	//--------------------------------------------------------------------------------------------------------------
	int WriteUsualFile::_DoWriteFile( const void* data, uint bitesize )
	{
		if ( fwrite( data, bitesize, 1, m_pFile ) == 1 )
		{
			return bitesize;
		}
		return 0;
	}
	//--------------------------------------------------------------------------------------------------------------
}