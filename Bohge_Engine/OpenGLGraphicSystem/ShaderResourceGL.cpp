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
#include "Framework/VertexStreamLayout.h"
#include "Framework/VertexBufferEntity.h"
#include "Framework/IGraphicSystem.h"
#include "Framework/TextureEntity.h"
#include "Framework/Log.h"

#include "OpenGLGraphicSystem.h"
#include "ShaderResourceGL.h"



#include <GLEW/glew.h>
#include <EASTL/set.h>


using namespace eastl;
using namespace BohgeEngine;



namespace OpenGLGraphicSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	ShaderResourceGL::ShaderResourceGL(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderResourceGL::~ShaderResourceGL(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_OnShaderBegin()
	{
		static_cast<OpenGLGraphicSystem*>( IGraphicSystem::Instance() )->CleanTextureReferencre();
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoBindProgram( handle h )
	{
		glUseProgram( h );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoDeleteProgram( handle h )
	{
		glDeleteProgram( h );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	bool ShaderResourceGL::_ComplieShaderCode( const eastl::string& code, handle hShaderHandle, bool isvs )
	{
		const char* ptr = code.c_str();
		glShaderSource( hShaderHandle, 1, &(ptr), NULL );
		glCompileShader( hShaderHandle );
		//Check for compile success
		GLint nCompileResult = 0;
		glGetShaderiv( hShaderHandle, GL_COMPILE_STATUS, &nCompileResult );
		if( 0 == nCompileResult )
		{
			char strInfoLog[1024];
			sint nLength;
			glGetShaderInfoLog( hShaderHandle, 1024, &nLength, strInfoLog );
			DEBUGLOG( "Error %s\n", strInfoLog );
			return false;
		}
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	handle ShaderResourceGL::_DoCreateProgram( const Attributes& attr, const eastl::string& vscode, const eastl::string& pscode )
	{
		//创建shader句柄
		handle hVertexShader = glCreateShader( GL_VERTEX_SHADER );
		handle hFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		//读取文件
		if( !_ComplieShaderCode( vscode, hVertexShader, true ) )
		{
			glDeleteShader( hVertexShader );
			glDeleteShader( hFragmentShader );
			ASSERT(false);
			return -1;
		}
		if( !_ComplieShaderCode( pscode, hFragmentShader, false ) )
		{
			glDeleteShader( hVertexShader );
			glDeleteShader( hFragmentShader );
			ASSERT(false);
			return -1;
		}

		//绑定shader
		handle hShaderProgram = glCreateProgram();
		glAttachShader( hShaderProgram, hVertexShader );
		glAttachShader( hShaderProgram, hFragmentShader );

		//绑定shader的Attributes
		const Attributes::AttributeList& attlist = attr.GetAttributeList();
		FOR_EACH_CONST( Attributes::AttributeList, att, attlist )
		{
			glBindAttribLocation( hShaderProgram, att->nLocation, att->strName.c_str() );
			CHECK_GRAPHIC_SYSTEM_ERROR;
		}
		//if( 0 != attr.Size() )
		//{
		//	for( uint i=0; i<attr.Size(); i++ )
		//	{
		//		if ( -1 != attr.GetLocation(i) )
		//		{
		//			glBindAttribLocation(	hShaderProgram,	attr.GetLocation(i),
		//				attr.StringAttribute(i).c_str() );
		//		}
		//	}
		//}
		//连接shader
		glLinkProgram( hShaderProgram );
		//Check for link success
		GLint LinkStatus;
		glGetProgramiv( hShaderProgram, GL_LINK_STATUS, &LinkStatus );
		if( 0 == LinkStatus )
		{
			char  strInfoLog[1024];
			int nLength;
			glGetProgramInfoLog( hShaderProgram, 1024, &nLength, strInfoLog );
			DEBUGLOG( "Error %s\n", strInfoLog );
			glDeleteProgram( hShaderProgram );
			ASSERT(false);
			return -1;
		}

		//释放
		glDeleteShader( hVertexShader );
		glDeleteShader( hFragmentShader );
		return hShaderProgram;
	}
	//-------------------------------------------------------------------------------------------------------
	handle ShaderResourceGL::_DoGetParameterHandle( handle hPragma, const eastl::string& name )
	{
		return glGetUniformLocation( hPragma, name.c_str() );
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter(handle h, const BohgeEngine::vector2arrayf& input)
	{
		glUniform2fv(h, input.Size(), input.Data() );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter(handle h, const BohgeEngine::vector3arrayf& input)
	{
		glUniform3fv(h, input.Size(), input.Data() );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter(handle h, const BohgeEngine::vector4arrayf& input)
	{
		glUniform4fv(h, input.Size(), input.Data() );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter( handle h, const int input )
	{
		glUniform1i(h, input);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter( handle h, const uint input )
	{
		glUniform1i(h, input);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter( handle h, const float input )
	{
		glUniform1f(h, input);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter( handle h, const eastl::vector<float>& input )
	{
		glUniform1fv(h, input.size(), input.data());
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter( handle h, const vector2f& input )
	{
		glUniform2f(h, input.m_x, input.m_y);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter( handle h, const vector3f& input )
	{
		glUniform3f(h, input.m_x, input.m_y, input.m_z);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter( handle h, const vector4f& input )
	{
		glUniform4f(h, input.m_x, input.m_y, input.m_z, input.m_w);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter( handle h, const Matrix33f& input )
	{
		glUniformMatrix3fv(h, 1, false, input.GetMatrixArray() );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter( handle h, const Matrix44f& input )
	{
		glUniformMatrix4fv(h, 1, false, input.GetMatrixArray() );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderResourceGL::_DoSetParameter( handle h, const TextureEntity& input )
	{
		//input.ActiveTexture();shader绑定之时激活纹理
		glUniform1i( h, input.GetHandle() );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
}