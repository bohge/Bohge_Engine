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
#pragma once
#include "SystemSingleton.h"
#include "IPlugin.h"

#include <EASTL/string.h>


namespace BohgeEngine
{
	class IFile;
	class BOHGE_FRAMEWORK IIOSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IIOSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IIOSystem, IPlugin )
	public:
		enum Constant
		{
			IO_IDENTIFIERS_LENGTH = 4,
			IO_SUBPATH_POS = 5,
			IO_MAX_LITE_PATH_SIZE = 1024,
		};
	private:
		//引擎默认的几个资源文件夹
		eastl::string*			m_pAssetRoot;
		eastl::string*			m_pDocPath;
		eastl::string*			m_pDefaultAssetPath;
		eastl::string*			m_pLitePath;
	protected:
		IIOSystem(void);
		virtual ~IIOSystem(void);
	private:
		virtual void _DoMakeFolder( const eastl::string& path ) = 0;
		virtual bool _DoIsFileExist(const eastl::string& filename) = 0;
		virtual bool _DoDeleteFile(const eastl::string& strFileName) = 0;
		virtual IFile* _DoFileFactory( const eastl::string& path ) = 0;
	public:
		virtual void RecycleBin( IFile* file ) = 0;
	public:
		virtual void RegistEvent( Framework& framework ); //创建系统
	public:
		void OnFrameworkEntry( float time, Framework& framework );
		void OnFrameworkExit( float time, Framework& framework );
	public:
		void SetRootPath(const eastl::string& path);//设置软件根目录（win系统为exe位置，android为app压缩包，ios为app资源目录）
		void SetResourcePath(const eastl::string& path);//设置一个可写的目录（win系统为exe位置，android为app可写目录或者sd卡，ios为app可写目录）
		void SetLiteappPath(const eastl::string& path);//设置Liteapp的路径
		void MakeFolder(const eastl::string& path);
		bool isFileExist(const eastl::string& path);
		bool DeleteFile(const eastl::string& path);
		eastl::string PathAssembly( const eastl::string& path );//路径组装
		BohgeEngine::IFile* FileFactory( const eastl::string& path );
	public:
		BOHGE_FORCEINLINE const eastl::string& GetRootPath()//app根目录
		{
			return *m_pAssetRoot;
		}
		BOHGE_FORCEINLINE const eastl::string& GetDocumentPath()//可写的目录
		{
			return *m_pDocPath;
		}
		BOHGE_FORCEINLINE const eastl::string& GetDefaultAssetPath()//默认资源目录，可能不可写
		{
			return *m_pDefaultAssetPath;
		}
		BOHGE_FORCEINLINE const eastl::string& GetLiteappPath()//当前应用程序目录
		{
			return *m_pLitePath;
		}
	};
}
