#include "AndroidIOSystem.h"
#ifdef ANDROID
#include "Predefine.h"
#include "Log.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>


using namespace std;

namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	AndroidIOSystem::AndroidIOSystem(void)
	{
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	AndroidIOSystem::~AndroidIOSystem(void)
	{
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void AndroidIOSystem::MakeFolder( const std::string& path )
	{
		if( access(path.c_str(),0) == -1 ) //如果文件夹不存在
		{
			DEBUGLOG("make folder %s\n", path.c_str());
			mkdir(path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	bool AndroidIOSystem::isExist( const std::string& filename )
	{
		if( 0 == access(filename.c_str(), 0) )//如果直接存在
		{
			return true;
		}
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	bool AndroidIOSystem::DeleteLocalFile( const std::string& strFileName )
	{
		if( isExist(strFileName) )
		{
			remove( strFileName.c_str() );
		}
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	std::vector<std::string> AndroidIOSystem::GetFileNamesWithExpand( const std::string& path, const std::string& exp )
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
	//--------------------------------------------------------------------------------------------------------------------------------------
	IFile* AndroidIOSystem::FileFactory( const std::string& path )
	{

	}

}
#endif