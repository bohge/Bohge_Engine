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
#include "Framework/VariableType.h"
#include "Framework/Predefine.h"
#include "ScriptParser.h"

#include <EASTL/string.h>
#include <EASTL/map.h>



namespace LuaMaterialSystemPlugin
{
	class ShaderFile;
	class ScriptParser;
	class ShaderSegment;
	class ShaderLibrary
	{
	private:
		typedef eastl::map< uint, ShaderFile* >			ShaderFileMap;
		typedef eastl::map< uint, ShaderSegment* >		ShaderSegmentMap;//shader代码段
	private:
		ShaderFileMap			m_IncludedShaderFiles;
		ShaderSegmentMap		m_CodeSegmentMap;
		ScriptParser*			m_pScriptParser;
	public:
		ShaderLibrary(void);
		~ShaderLibrary(void);
	public:
		void Initialize();
		void ReleaseResource();//释放全部以加载的shader段
		void HandleShaderSegment( const eastl::string& filename );//处理输入的shader，分拆为代码段
		ShaderSegment* GetCodeSegment( const eastl::string& name );//得到代码段,在代码段编译的时候可能会修改内部的参数名字
	public:
		BOHGE_FORCEINLINE void Being()
		{
			m_CodeSegmentMap.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void DoScript( const eastl::string& buffer )
		{
			m_pScriptParser->DoScript( buffer );
		}
		//-------------------------------------------------------------------------------------------------------
		template <typename T>
		BOHGE_FORCEINLINE void SetGlobal(const char* name, T object)
		{
			m_pScriptParser->SetGlobal( name, object );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T GetGlobal(const char* name) const
		{
			return m_pScriptParser->GetGlobal<T>( name );
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void ParserCall(const char* func)
		{
			m_pScriptParser->ParserCall( func );
		}
	};
}
