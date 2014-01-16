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

#include "FilePath.h"
#include "Predefine.h"
#include "Log.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <direct.h>
#include <io.h>
#else
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#if IOS
int access(const char* path, int mode)
{
	struct stat st;
	return stat(path, &st);
}
#endif


using namespace std;



namespace BohgeEngine
{
	//-------------------------------------------------------------------
	void FilePath::MakeFolder( const std::string& path )
	{
		if( access(path.c_str(),0) == -1 ) //如果文件夹不存在
		{
			DEBUGLOG("make folder %s\n", path.c_str());
#ifdef WIN32
			mkdir(path.c_str());
#else
			mkdir(path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
		}
	}
	//-------------------------------------------------------------------
	FilePath::FilePath()
	{
	}
	//-------------------------------------------------------------------
	FilePath::~FilePath()
	{
	}
	//-------------------------------------------------------------------
	void FilePath::SetRootPath(const string& path)
	{
		DEBUGLOG("set root path %s\n", path.c_str());
		m_RootPath = path;
		m_TextureFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Texture") + PLANTFORMDELIMITER; //do not change / becouse app need!!!
		m_ShaderFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Shaders") + PLANTFORMDELIMITER;
		m_SceneFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Scene") + PLANTFORMDELIMITER;
		m_ModelFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Model") + PLANTFORMDELIMITER;
		m_SoundFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Sound") + PLANTFORMDELIMITER;
		m_FontFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Font") + PLANTFORMDELIMITER;
		m_UIFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("UI") + PLANTFORMDELIMITER;
		m_ScriptFolder = path + std::string("AppAsset") + PLANTFORMDELIMITER + std::string("Script") + PLANTFORMDELIMITER;
	}
	//-------------------------------------------------------------------
	bool FilePath::isExist(const string& filename)
	{
		if( 0 == access(filename.c_str(), 0) )//如果直接存在
		{
			return true;
		}
		return false;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FilePath::DeleteLocalFile(const string& strFileName)
	{
		if( isExist(strFileName) )
		{
			remove( strFileName.c_str() );
		}
		return true;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	std::vector<std::string> FilePath::GetFileNames(const string& path)
	{
		return GetFileNamesWithExpand(path, "" );
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	std::vector<std::string> FilePath::GetFileNamesWithExpand( const std::string& path, const std::string& exp )
	{
		vector<string> files;
		string fullpath = path;
#if WIN32
		struct _finddata_t c_file; 
		intptr_t   hFile; 
		if(_chdir(fullpath.c_str())) 
		{
			DEBUGLOG("Cant' open direction, %s\n", fullpath.c_str() );
		} 
		else 
		{
			hFile = _findfirst("*.*", &c_file); 
			if( 0 < c_file.size )
			{
				DEBUGLOG("include file, %s\n", c_file.name );
				string filename = c_file.name;
				if ( exp.empty() || string::npos != filename.find(exp) )
				{
					files.push_back( filename.substr(filename.find_last_of("/\\")+1) );
				}
			}
		}
		while(_findnext(hFile, &c_file) == 0) 
		{ 
			if( 0 < c_file.size )
			{
				DEBUGLOG("include file, %s\n", c_file.name );
				string filename = c_file.name;
				if ( exp.empty() || string::npos != filename.find(exp) )
				{
					files.push_back( filename.substr(filename.find_last_of("/\\")+1) );
				}
			}
		} 
		_findclose(hFile); 
#else
		struct dirent* ent = NULL;
		DIR* pDir;
		pDir = opendir(path.c_str());
		while(NULL != (ent = readdir(pDir)))
		{
			string fullpath = path + PLANTFORMDELIMITER + ent->d_name;
			struct stat st;
			int ret = stat(fullpath.c_str(), &st);
			if( ret>=0 && S_ISREG(st.st_mode) )
			{
				DEBUGLOG("include file, %s\n", ent->d_name );
				string filename = ent->d_name;
				if ( exp.empty() || string::npos != filename.find(exp) )
				{
					files.push_back( filename.substr(filename.find_last_of("/\\")+1) );
				}
			}
		}
		closedir(pDir);
#endif
		return files;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void FilePath::SetWritePath(const std::string& path)
	{
		ASSERT( 0 == m_WriteFolder.size() );
		m_WriteFolder = path;
		MakeFolder(m_WriteFolder);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void FilePath::SetAddtionPath( const std::string& path )
	{
		ASSERT( 0 == m_AddtionPath.size() );
		m_AddtionPath = path;
		MakeFolder(m_AddtionPath);
	}

}