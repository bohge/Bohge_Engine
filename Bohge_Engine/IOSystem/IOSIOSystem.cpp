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
#include "IOSIOSystem.h"
#ifdef _PLATFORM_IOS
#include "Framework/Predefine.h"
#include "Framework/Log.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>


using namespace eastl;

namespace IOSystemPlugin
{
	//-------------------------------------------------------------------
	IOSIOSystem::IOSIOSystem(void)
	{
	}
	//-------------------------------------------------------------------
	IOSIOSystem::~IOSIOSystem(void)
	{
	}
	//-------------------------------------------------------------------
	void IOSIOSystem::MakeFolder( const eastl::string& path )
	{
		if( access(path.c_str(),0) == -1 ) //如果文件夹不存在
		{
			DEBUGLOG("make folder %s\n", path.c_str());
			mkdir(path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
	}
	//-------------------------------------------------------------------
	bool IOSIOSystem::isExist( const eastl::string& filename )
	{
		if( 0 == access(filename.c_str(), 0) )//如果直接存在
		{
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------
	bool IOSIOSystem::DeleteLocalFile( const eastl::string& strFileName )
	{
		if( isExist(strFileName) )
		{
			remove( strFileName.c_str() );
		}
		return true;
	}
	//-------------------------------------------------------------------
	eastl::vector<eastl::string> IOSIOSystem::GetFileNamesWithExpand( const eastl::string& path, const eastl::string& exp )
	{
		vector<string> files;
		string fullpath = path;
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
		return files;
	}
	//-------------------------------------------------------------------
	int IOSIOSystem::Ios_access( const char* path, int mode )
	{
		struct stat st;
		return stat(path, &st);
	}
	//-------------------------------------------------------------------
	BohgeEngine::IFile* IOSIOSystem::FileFactory( const eastl::string& path )
	{

	}

}
#endif
