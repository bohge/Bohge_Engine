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
#include "GPUProgramRespurceGL.h"
#include "IDevice.h"


#include <GLEW/glew.h>


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	GPUProgramRespurceGL::GPUProgramRespurceGL(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	GPUProgramRespurceGL::~GPUProgramRespurceGL(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoBindProgram( handle h )
	{
		glUseProgram( h );
		CHECK_ERROR_RENDER;
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoDeleteProgram( handle h )
	{
		glDeleteProgram( h );
		CHECK_ERROR_RENDER;
	}
	//-------------------------------------------------------------------------------------------------------
	handle GPUProgramRespurceGL::_DoCreateProgram( const Attributes& attr, const std::string& vscode, const std::string& pscode )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	handle GPUProgramRespurceGL::_DoGetParameterHandle( handle hPragma, const std::string& name )
	{
		return glGetUniformLocation( hPragma, name.c_str() );
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoSetParameter( handle h, const std::vector<Quaternionf>& input )
	{
		glUniform4fv(h, input.size(), &(input[0].data[0]));
		CHECK_ERROR_RENDER;
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoSetParameter( handle h, const int input )
	{
		glUniform1i(h, input);
		CHECK_ERROR_RENDER;
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoSetParameter( handle h, const float input )
	{
		glUniform1f(h, input);
		CHECK_ERROR_RENDER;
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoSetParameter( handle h, const std::vector<float>& input )
	{
		glUniform1fv(h, input.size(), &input[0]);
		CHECK_ERROR_RENDER;
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoSetParameter( handle h, const vector2f& input )
	{
		glUniform2f(h, input.m_x, input.m_y);
		CHECK_ERROR_RENDER;
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoSetParameter( handle h, const vector3f& input )
	{
		glUniform3f(h, input.m_x, input.m_y, input.m_z);
		CHECK_ERROR_RENDER;
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoSetParameter( handle h, const vector4f& input )
	{
		glUniform4f(h, input.m_x, input.m_y, input.m_z, input.m_w);
		CHECK_ERROR_RENDER;
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoSetParameter( handle h, const Matrix33f& input )
	{
		glUniformMatrix3fv(h, 1, false, input.GetMatrixArray() );
		CHECK_ERROR_RENDER;
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoSetParameter( handle h, const Matrix44f& input )
	{
		glUniformMatrix4fv(h, 1, false, input.GetMatrixArray() );
		CHECK_ERROR_RENDER;
	}
	//-------------------------------------------------------------------------------------------------------
	void GPUProgramRespurceGL::_DoSetParameter( handle h, const TextureTemp& input )
	{
		//ASSERT( m_uTextrueReferenceCount < 32 );
		////glActiveTexture( GL_TEXTURE0 + m_uTextrueReferenceCount );
		//this->SetTexture(input);
		//glUniform1i( h, m_uTextrueReferenceCount );
		//CHECK_ERROR_GLES;
		//m_uTextrueReferenceCount ++;
	}

}