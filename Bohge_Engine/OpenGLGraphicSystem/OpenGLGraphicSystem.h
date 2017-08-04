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
#include "Framework/IGraphicSystem.h"

#include <GLEW/glew.h>
#include <GLEW/wglew.h>


struct glslopt_ctx;
namespace OpenGLGraphicSystemPlugin
{
	class OpenGLGraphicSystem : public BohgeEngine::IGraphicSystem
	{
	private:
		HGLRC						m_hGLRC;
		HDC							m_hHDC;
		uint						m_uTextrueReferenceCount;
		glslopt_ctx*				m_pGlslopt_ctx;
	public:
		OpenGLGraphicSystem(void);
		virtual ~OpenGLGraphicSystem(void);
	private:
		virtual void _CreateContext( void* hWindows );//创建设备
		virtual void _SwapBuffer();
	public:
		virtual void CheckError();
		virtual BohgeEngine::ISharedResource* CreateResource( BohgeEngine::IResourceProperty::SharedResourceType type );
		virtual void ProcessShader( BohgeEngine::ShaderProperty::ShaderCode sc, eastl::string& code );
		virtual uint GetMaxUniformArraySize();
	public:
		virtual void RegistEvent( BohgeEngine::Framework& framework ); //注册事件
		void OnFrameworkEntry( float time, BohgeEngine::Framework& framework );
		void OnFrameworkExit( float time, BohgeEngine::Framework& framework );
		void OnSwapBuffer( BohgeEngine::Framework& framework );
	public:
		BOHGE_FORCEINLINE void IncreaseTextureReferencre()
		{
			++ m_uTextrueReferenceCount;
		}
		BOHGE_FORCEINLINE void CleanTextureReferencre()
		{
			m_uTextrueReferenceCount = 0;
		}
		BOHGE_FORCEINLINE uint GetTextureReferencre()
		{
			return m_uTextrueReferenceCount;
		}
	};
}
