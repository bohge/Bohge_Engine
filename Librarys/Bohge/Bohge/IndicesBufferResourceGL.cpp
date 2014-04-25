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
#include "IDevice.h"
#include "IndicesBufferResourceGL.h"


#include <GLEW/glew.h>

namespace BohgeEngine
{
	//---------------------------------------------------------------------------------------------------------
	IndicesBufferResourceGL::IndicesBufferResourceGL(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IndicesBufferResourceGL::~IndicesBufferResourceGL(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void IndicesBufferResourceGL::_DoDeleteBuffer( handle& h )
	{
		glDeleteBuffers( 1, &h);
		CHECK_ERROR_RENDER;
		h = -1;
	}
	//---------------------------------------------------------------------------------------------------------
	void IndicesBufferResourceGL::_DoBindBuffer( handle h )
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, h);
		CHECK_ERROR_RENDER;
	}
	//---------------------------------------------------------------------------------------------------------
	handle IndicesBufferResourceGL::_DoCreateBuffer( uint datasize, uint typesize, const void* dataptr, BufferObjectStatus::MemoryUseage use )
	{
		handle h;
		glGenBuffers(1, &h);
		CHECK_ERROR_RENDER;
		BindBuffer();
		switch( use )
		{
		case BufferObjectStatus::MU_STATIC:glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize*typesize, dataptr, GL_STATIC_DRAW);break;
		case BufferObjectStatus::MU_DYNAMIC:glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize*typesize, dataptr, GL_DYNAMIC_DRAW);break;
		}
		CHECK_ERROR_RENDER;
		return h;
	}
	//---------------------------------------------------------------------------------------------------------
	void IndicesBufferResourceGL::_DoChangeData( handle h, uint offset, uint datasize, uint typesize, const void* dataptr )
	{
		BindBuffer();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, datasize*typesize, dataptr);
		CHECK_ERROR_RENDER;
	}

}