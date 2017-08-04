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
#include "ShaderParser.h"
#include "ShaderSegment.h"
#include "ShaderLibrary.h"
#include "ShaderAssembler.h"


using namespace eastl;
using namespace BohgeEngine;

namespace LuaMaterialSystemPlugin
{
	SINGLETON_IMPLEMENT(ShaderParser)
	//------------------------------------------------------------------------------------------------------
	ShaderParser::ShaderParser(void)
	{
		m_pShaderLibrary = NEW ShaderLibrary();
		m_pShaderAssembler = NEW ShaderAssembler();
	}
	//------------------------------------------------------------------------------------------------------
	ShaderParser::~ShaderParser(void)
	{
		SAFE_DELETE( m_pShaderLibrary );
		SAFE_DELETE( m_pShaderAssembler );
	}
	//------------------------------------------------------------------------------------------------------
	void ShaderParser::_OnCreate()
	{
		m_pShaderLibrary->Initialize();
	}
	//------------------------------------------------------------------------------------------------------
	void ShaderParser::_OnDestroy()
	{
		m_pShaderLibrary->ReleaseResource();
	}
	//------------------------------------------------------------------------------------------------------
	void ShaderParser::Parser( const eastl::string& root, const StringList& includes, const StringList& vs, const StringList& ps, ShaderSource& shader )
	{
		Segments VertexSegment;
		Segments FargmentSegment;
		m_pShaderLibrary->Being();
		FOR_EACH_CONST( StringList, str, includes )//处理include
		{
			m_pShaderLibrary->HandleShaderSegment( root + *str );
		}
		{//处理vs
			FOR_EACH_CONST( StringList, str, vs )//处理include
			{
				ShaderSegment* seg = NEW ShaderSegment( *m_pShaderLibrary->GetCodeSegment( *str ) );
				VertexSegment.push_back( seg );
			}
		}

		{//处理ps
			FOR_EACH_CONST( StringList, str, ps )//处理include
			{
				ShaderSegment* seg = NEW ShaderSegment( *m_pShaderLibrary->GetCodeSegment( *str ) );
				FargmentSegment.push_back( seg );
			}
		}
		m_pShaderAssembler->CompileShader( VertexSegment, FargmentSegment, shader );
		FOR_EACH( Segments, it, VertexSegment )
		{
			SAFE_DELETE(*it);
		}
		FOR_EACH( Segments, it, FargmentSegment )
		{
			SAFE_DELETE(*it);
		}
	}

}