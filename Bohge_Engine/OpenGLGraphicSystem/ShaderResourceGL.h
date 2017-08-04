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
#include "Framework/ShaderResource.h"



namespace OpenGLGraphicSystemPlugin
{
	class ShaderResourceGL : public BohgeEngine::ShaderResource
	{
	public:
		ShaderResourceGL(void);
		virtual ~ShaderResourceGL(void);
	private:
		virtual void _OnShaderBegin();
		virtual void _DoBindProgram(handle h);
		virtual void _DoDeleteProgram(handle h);
		virtual handle _DoCreateProgram( const BohgeEngine::Attributes& attr, const eastl::string& vscode, const eastl::string& pscode );
		virtual handle _DoGetParameterHandle(handle hPragma, const eastl::string& name);
		virtual void _DoSetParameter(handle h, const BohgeEngine::vector2arrayf& input);
		virtual void _DoSetParameter(handle h, const BohgeEngine::vector3arrayf& input);
		virtual void _DoSetParameter(handle h, const BohgeEngine::vector4arrayf& input);
		virtual void _DoSetParameter(handle h, const int input);
		virtual void _DoSetParameter(handle h, const uint input);
		virtual void _DoSetParameter(handle h, const float input);
		virtual void _DoSetParameter(handle h, const eastl::vector<float>& input);
		virtual void _DoSetParameter(handle h, const BohgeEngine::vector2f& input);
		virtual void _DoSetParameter(handle h, const BohgeEngine::vector3f& input);
		virtual void _DoSetParameter(handle h, const BohgeEngine::vector4f& input);
		virtual void _DoSetParameter(handle h, const BohgeEngine::Matrix33f& input);
		virtual void _DoSetParameter(handle h, const BohgeEngine::Matrix44f& input);
		virtual void _DoSetParameter(handle h, const BohgeEngine::TextureEntity& input);
	private:
		bool _ComplieShaderCode( const eastl::string& code, handle hShaderHandle, bool isvs );
	};
}

