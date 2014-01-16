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



//////////////////////////////
//	  将资源解压到根目		//
//////////////////////////////

#include "Unpack.h"
#include "Log.h"
#include "VariableType.h"
#include "Predefine.h"
#include "FilePath.h"


#ifdef ANDROID

#include <zip.h>
#include <zipint.h>

Unpack::Unpack()
:m_pAPKArchive(NULL)
{

}
//-------------------------------------------------------------------------------------------------------
Unpack::~Unpack()
{

}
//-------------------------------------------------------------------------------------------------------
void Unpack::Initialization(const std::string& apkPath)
{
	DEBUGLOG("Loading APK %s", apkPath.c_str());
	m_pAPKArchive = zip_open(apkPath.c_str(), 0, NULL);
	if (m_pAPKArchive == NULL) 
	{
		DEBUGLOG("Error loading APK");
	}
}
//-------------------------------------------------------------------------------------------------------
void Unpack::Decompress(const std::string& from, const std::string& to)
{
	struct zip_stat zs;
	zip_stat(m_pAPKArchive, from.c_str(), ZIP_FL_UNCHANGED , &zs);//读取zip属性
	DEBUGLOG("%s size is %d ", from.c_str(), zs.size);

	byte* data = NEW byte[zs.size];

	zip_file* zipfile;
	zipfile = zip_fopen(m_pAPKArchive, from.c_str(), 0); //打开文件流
	zip_fread(zipfile, data, zs.size); //读取文件

	DEBUGLOG("save to %s", to.c_str());

	FilePtr writeto;
	File::Instance().OpenFile(to.c_str(), File::FILE_WRITE, writeto);
	File::Instance().WriteFile(data, zs.size, 1, writeto);
	File::Instance().CloseFile(writeto);

	zip_fclose(zipfile);//关闭文件
}
//-------------------------------------------------------------------------------------------------------


#else

//window,ios是空的
Unpack::Unpack()
:m_pAPKArchive(NULL)
{

}
//-------------------------------------------------------------------------------------------------------
Unpack::~Unpack()
{

}
//-------------------------------------------------------------------------------------------------------
void Unpack::Initialization(const std::string& apkPath)
{
	DEBUGLOG("Loading APK %s\n", apkPath.c_str());
}
//-------------------------------------------------------------------------------------------------------
void Unpack::Decompress(const std::string& from, const std::string& to)
{
	DEBUGLOG("%s to %s\n", from.c_str(), to.c_str());
}
//-------------------------------------------------------------------------------------------------------
#endif








//-------------------------------------------------------------------------------------------------------
Unpack& Unpack::Instance()
{
	static Unpack inst;
	return inst;
}
