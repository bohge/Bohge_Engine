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
#include "Framework/IndicesBufferEntity.h"
#include "Framework/IGraphicSystem.h"
#include "RenderObjectResourceGL.h"
#include <GLEW/glew.h>


using namespace BohgeEngine;
namespace OpenGLGraphicSystemPlugin
{
	//------------------------------------------------------------------------------------------------------
	RenderObjectResourceGL::RenderObjectResourceGL(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	RenderObjectResourceGL::~RenderObjectResourceGL(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResourceGL::_DoDrawElements( RenderObjectProperty::RenderMode rm, BufferObjectProperty::IndicesType it, uint count )
	{
		GLenum type = 0;
		switch( it )
		{
		case BufferObjectProperty::IT_UINT16: type = GL_UNSIGNED_SHORT; break;
		case BufferObjectProperty::IT_UINT32: type = GL_UNSIGNED_INT; break;
		default:ASSERT(false);
		}
		glDrawElements(GL_POINTS + static_cast<uint>(rm), count, type, NULL);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderObjectResourceGL::_DoDrawArrays( RenderObjectProperty::RenderMode rm, uint count )
	{
		glDrawArrays(GL_POINTS + static_cast<uint>(rm), 0, count );
	}

}