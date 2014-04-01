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
//		语言设置		//
//////////////////////////

#include "Language.h"
#include "Predefine.h"
#include "IOSystem.h"
#include "EncryptFile.h"
#include "Log.h"
#include "Utility.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>
#include <stdarg.h>
#include <algorithm>

using namespace std;

#define STRING_ROOT_DIR "Root"//读取根目录的

namespace BohgeEngine
{
	LanguageControl* LanguageControl::m_psSelf = NULL;
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	LanguageControl::LanguageControl()
		:m_eLanguage(L_English),
		m_FolderPathName(IOINSTANCE.FontFolder() + "EN" + PLANTFORMDELIMITER)
	{
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	LanguageControl::~LanguageControl()
	{
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void LanguageControl::_LanguageRootPath()
	{
		std::string filename;
		switch( m_eLanguage )
		{
		case L_Chinese: filename = "CN"; break;
		case L_English: filename = "EN"; break;
		default: filename = "EN"; break;
		}
		m_FolderPathName = IOINSTANCE.FontFolder() + filename + PLANTFORMDELIMITER;
		DEBUGLOG("size of wchar_t, %d\n",sizeof(wchar_t));
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	std::wstring LanguageControl::_LoadString( const std::string& path )
	{
		//ReadEncryptFile readfile(path);
		IFile* readfile = FILEFACTORY(path);
		readfile->OpenFile( IFile::AT_READ );
		std::wstring wstr;
#ifdef _DEBUG
		bool isFirst = true;
#endif
		while(true)
		{
			wchar_t wchar = 0;
			if( readfile->ReadFile(&wchar, 2 ) <= 0 ) //读取两个字节，这里不用sizeof(wchar)应为ios android这个值是4
			{
				break;
			}
#ifdef _DEBUG
			if ( isFirst )
			{
				isFirst = false;
				ASSERT( 65279 == wchar );//保证第一个是bom头，文本格式必须是 UCS-2 Little Endian 编码格式
			}
#endif
			if( /*13 != wchar && 10 != wchar &&*/ 0 != wchar && 65279 != wchar )//13,10是回车,0是没有初始化的数据,65279是BOM头，跳过
			{
				wstr.push_back(wchar);
			}
		}
		readfile->CloseFile();
		FILEDESTROY( readfile );
		return wstr;
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void LanguageControl::LoadPackage( const char* dir )
	{
		if ( m_CurrentPackage != dir )
		{
			m_CurrentPackage = dir;
			m_WstringMap.clear();
			string fullpath = m_CurrentPackage == STRING_ROOT_DIR ? m_FolderPathName : m_FolderPathName + m_CurrentPackage;
			vector<string> files = IOINSTANCE.GetFileNamesWithExpand( fullpath, BOHGE_STRING_EXPAND );
			for ( vector<string>::iterator it = files.begin();
				it != files.end();
				it ++ )
			{
				wstring str = _LoadString( fullpath + *it );
				std::string filename = *it;
				filename.erase( filename.find_first_of(".") );
				transform(filename.begin(),filename.end(),filename.begin(), ::tolower);
				m_WstringMap.insert( make_pair( Utility::HashCode( filename ), str ) );
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void LanguageControl::LoadPackage()
	{
		LoadPackage( STRING_ROOT_DIR );
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void LanguageControl::Setlanguage( LanguageType l )
	{
		//if( m_eLanguage != l )
		{
			m_eLanguage = l;
			_LanguageRootPath();
			LoadPackage( m_CurrentPackage.c_str() );
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	std::wstring LanguageControl::GetString( const char* filename )
	{
		std::string name(filename);
		transform(name.begin(),name.end(),name.begin(), ::tolower);
		WstringMap::iterator it = m_WstringMap.find( Utility::HashCode( name ) );
		if ( m_WstringMap.end() != it )
		{
			return it->second;
		}
		return wstring();
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	std::wstring LanguageControl::GetFromatString( const char* filename, ... )
	{
		wstring str = GetString( filename );
		va_list args;
		va_start( args, filename );
		wstringstream reslut;
		for ( int i = 0; i < str.size(); i ++ )
		{
			bool isChar = true;
			if( str[i] == L'%' )
			{
				if ( i+1 < str.size() )
				{
					if( str[i+1] == L's' || str[i+1] == L'd' || str[i+1] == L'f' || str[i+1] == L'c' )
					{
						isChar = false;
						i++;
						switch( str[i] )
						{
						case  L'c':
							{
#ifdef WIN32
								char value = va_arg( args, char );
#else
								char value = va_arg( args, int );//gcc编辑器在...中把char当作int传递
#endif
								reslut<<value;
							}break;
						case  L's':
							{
								char * value = va_arg( args, char * );
								reslut<<value;
							}break;
						case  L'd':
							{
								int value = va_arg( args, int );
								reslut<<value;
							}break;
						case  L'f':
							{
								float value = va_arg( args, double );
								reslut<<value;
							}break;
						}
					}
				}
			}		
			if( isChar )
			{
				reslut<<str[i];
			}
		}
		va_end( args );
		return reslut.str();
	}
}