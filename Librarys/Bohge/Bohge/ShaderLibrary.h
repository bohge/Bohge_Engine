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



/////////////////////////////////////
//		新的shader段的管理库		   //
/////////////////////////////////////
#pragma once
#include "ScriptBase.h"


#include <string>
#include <map>



namespace BohgeEngine
{
	class ShaderFile;
	class ShaderSegment;
	class ShaderLibrary : public ScriptBase
	{
	private:
		typedef std::map< uint, ShaderFile* >			ShaderFileMap;
		typedef std::map< uint, ShaderSegment* >		ShaderSegmentMap;//shader代码段
	private:
		ShaderFileMap			m_IncludedShaderFiles;
		ShaderSegmentMap		m_CodeSegmentMap;
	public:
		ShaderLibrary(void);
		~ShaderLibrary(void);
	private:
		virtual void _OnScriptLoaded();//当脚本加载后
		virtual void _SetGlobalClassesIndex();
	public:
		virtual void LoadScript( const std::string& filename );
		virtual void DoScript( const std::string& buffer );
		virtual void UnLoadScript();
	public:
		void Initialization();
		void ReleaseResource();//释放全部以加载的shader段
		void HandleShaderSegment( const std::string& filename );//处理输入的shader，分拆为代码段
		ShaderSegment* GetCodeSegment( const std::string& name );//得到代码段,在代码段编译的时候可能会修改内部的参数名字
	public:
		BOHGE_FORCEINLINE void Being()
		{
			m_CodeSegmentMap.clear();
		}
	};
}
