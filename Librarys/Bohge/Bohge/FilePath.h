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
#pragma once


#include <string>
#include <vector>
#include "Predefine.h"
#include "VariableType.h"





namespace BohgeEngine
{
	class FilePath
	{
	private:
		//引擎默认的几个资源文件夹
		std::string			m_RootPath;
		std::string			m_TextureFolder;
		std::string			m_ShaderFolder;
		std::string			m_SceneFolder;
		std::string			m_ModelFolder;
		std::string			m_SoundFolder;
		std::string			m_FontFolder;
		std::string			m_UIFolder;
		std::string			m_ScriptFolder;
		std::string			m_WriteFolder;
		std::string			m_AddtionPath;
	private:
		FilePath();
		~FilePath();
	public:
		BOHGE_FORCEINLINE static FilePath& Instance()
		{
			static FilePath file;
			return file;
		}
	public:
		void MakeFolder( const std::string& path );
		void SetRootPath(const std::string& path);//设置一个资源目录的根目录
		void SetWritePath(const std::string& path);//设置一个可写的目录
		void SetAddtionPath( const std::string& path );
		bool isExist(const std::string& filename);
		bool DeleteLocalFile(const std::string& strFileName);
		std::vector<std::string> GetFileNames(const std::string& path);//取得文件夹下的所有文件的名称
		std::vector<std::string> GetFileNamesWithExpand(const std::string& path, const std::string& exp);
	public:
		BOHGE_FORCEINLINE const std::string& TextureFolder()
		{
			return m_TextureFolder;
		}
		BOHGE_FORCEINLINE const std::string& ShaderFolder()
		{
			return m_ShaderFolder;
		}
		BOHGE_FORCEINLINE const std::string& SceneFolder()
		{
			return m_SceneFolder;
		}
		BOHGE_FORCEINLINE const std::string& ModelFolder()
		{
			return m_ModelFolder;
		}
		BOHGE_FORCEINLINE const std::string& SoundFolder()
		{
			return m_SoundFolder;
		}
		BOHGE_FORCEINLINE const std::string& FontFolder()
		{
			return m_FontFolder;
		}
		BOHGE_FORCEINLINE const std::string& UIFolder()
		{
			return m_UIFolder;
		}
		BOHGE_FORCEINLINE const std::string& ScriptFolder()
		{
			return m_ScriptFolder;
		}
		BOHGE_FORCEINLINE const std::string& WriteFolder()
		{
			return m_WriteFolder;
		}
		BOHGE_FORCEINLINE const std::string& AddtionFolder()
		{
			return m_AddtionPath;
		}
	};


#define FILEPATH FilePath::Instance()

}