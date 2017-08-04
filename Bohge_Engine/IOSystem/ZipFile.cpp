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



//////////////////////////
//		压缩的文件	   //
/////////////////////////
#include "ZipFile.h"
#include "Framework/Log.h"


#include "zip.h"
#include "zipint.h"




namespace IOSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	ZipFile::ZipFile( const eastl::string& url )
		:BaseFile( url ),
		m_pZipArchive( NULL )
	{
		uint pos = GetFilePath().find_first_of('.');
		uint posend = GetFilePath().find_first_of( "/\\", pos );
		m_ZipPath = GetFilePath().substr( 0, posend );
		m_FilePathInZip = GetFilePath().substr( posend + 1, GetFilePath().size() - posend );

		pos = 0;
		while(true)
		{
			pos = m_FilePathInZip.find_first_of( '\\', pos );
			if ( -1 == pos )
			{
				break;
			}
			m_FilePathInZip[pos] = '/';
		}
		m_pZipStat = NEW struct zip_stat();
	}
	//-------------------------------------------------------------------------------------------------------
	ZipFile::~ZipFile()
	{
		SAFE_DELETE( m_pZipStat );
	}
	//-------------------------------------------------------------------------------------------------------
	bool ZipFile::_DoOpenFile( BohgeEngine::IFile::ActionType at )
	{
		m_pZipArchive = zip_open(m_ZipPath.c_str(), 0, NULL);
		if (m_pZipArchive == NULL) 
		{
			DEBUGLOG("Error loading Zip",1);
		}
		zip_stat(m_pZipArchive, m_FilePathInZip.c_str(), ZIP_FL_UNCHANGED , m_pZipStat);//读取zip属性
		_SetFileSize( m_pZipStat->size );
		m_pZipfile = zip_fopen(m_pZipArchive, m_FilePathInZip.c_str(), 0); //打开文件流
		if ( NULL != m_pZipfile )
		{
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	int ZipFile::_DoReadFile( void* data, uint bitesize )
	{
		return zip_fread(m_pZipfile, data, bitesize); //读取文件
	}
	//-------------------------------------------------------------------------------------------------------
	int ZipFile::_DoWriteFile( const void* data, uint bitesize )
	{
		ASSERT(false);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	bool ZipFile::_DoCloseFile()
	{
		return zip_fclose(m_pZipfile);
	}
	//-------------------------------------------------------------------------------------------------------
	int ZipFile::_DoSeekFile( int to, int whence )
	{
		ASSERT( false );
		return -1;
	}
	//-------------------------------------------------------------------------------------------------------
	int ZipFile::_DoTell()
	{
		ASSERT( false );
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
}