#include "AndroidIOSystem.h"
#ifdef _PLATFORM_ANDROID
#include "Framework/Predefine.h"
#include "Framework/Log.h"
#include "ZipFile.h"
#include "UsualFile.h"
#include "EncryptFile.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>


using namespace eastl;

namespace IOSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	AndroidIOSystem::AndroidIOSystem(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	AndroidIOSystem::~AndroidIOSystem(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void AndroidIOSystem::MakeFolder( const eastl::string& path )
	{
		if( access(path.c_str(),0) == -1 ) //如果文件夹不存在
		{
			DEBUGLOG("make folder %s\n", path.c_str());
			mkdir(path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool AndroidIOSystem::isExist( const eastl::string& filename )
	{
		if( 0 == access(filename.c_str(), 0) )//如果直接存在
		{
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	bool AndroidIOSystem::DeleteLocalFile( const eastl::string& strFileName )
	{
		if( isExist(strFileName) )
		{
			remove( strFileName.c_str() );
		}
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::vector<eastl::string> AndroidIOSystem::GetFileNamesWithExpand( const eastl::string& path, const eastl::string& exp )
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
	//-------------------------------------------------------------------------------------------------------
	BohgeEngine::IFile* AndroidIOSystem::FileFactory( const eastl::string& path )
	{
		if ( string::npos != path.find(".enc") )
		{
			return NEW EncryptFile( path );
		}
		else if ( string::npos != path.find(".zip") )
		{
			return NEW ZipFile( path );
		}
		else
		{
			return NEW UsualFile( path );
		}
		ASSERT(false);
		return NULL;
	}

}
#endif