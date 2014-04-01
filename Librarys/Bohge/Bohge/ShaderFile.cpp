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
//		新的shader文件管理		   //
/////////////////////////////////////
#include "ShaderFile.h"
#include "IFile.h"
#include "IOSystem.h"
#include "Utility.h"
#include "ShaderSegment.h"



using namespace std;


static string SHADER_DEFINE("#SHADER_DEFINE");
static string END_DEFINE("#END_DEFINE");


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	ShaderFile::ShaderFile(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderFile::~ShaderFile(void)
	{
		FOR_EACH( ShaderSegmentMap, it, m_CodeSegmentMap )
		{
			SAFE_DELETE( it->second );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderFile::LoadShaderFile( const std::string& file )
	{
		m_FileName = file;
		IFile* readfile = FILEFACTORY( IOINSTANCE.ShaderFolder() + file );
		readfile->OpenFile( IFile::AT_READ );
		char* datas = NEW char[readfile->GetSize()+1];
		readfile->ReadFile( datas, readfile->GetSize() );
		datas[readfile->GetSize()] = 0;
		readfile->CloseFile();
		FILEDESTROY( readfile );
		string source = datas;
		SAFE_DELETE_ARRAY(datas);
		Utility::RemoveComment( "//", source );
		//拆分全部的函数段
		vector<string> shaders;
		Utility::SeparateAllString( SHADER_DEFINE, END_DEFINE, shaders, source );//将材质分拆开来
		for( int i = 0; i < shaders.size() ; i ++ )
		{
			string funcname;
			Utility::MacroName( SHADER_DEFINE, shaders[i], funcname );
			ShaderSegment* seg = NEW ShaderSegment( funcname, shaders[i] );
			m_CodeSegmentMap.insert( make_pair( Utility::HashCode(funcname), seg ) );
		}
	}

}